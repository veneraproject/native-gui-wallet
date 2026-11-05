                                       
                                              
  
                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

 
#include "pending_tx_validation.h"

 
#include "misc_log_ex.h"
#include "common/apply_permutation.h"
#include "crypto/crypto.h"
#include "cryptonote_basic/account.h"
#include "cryptonote_basic/cryptonote_basic_impl.h"
#include "cryptonote_basic/cryptonote_format_utils.h"
#include "cryptonote_basic/subaddress_index.h"
#include "cryptonote_core/cryptonote_tx_utils.h"
#include "cryptonote_config.h"
#include "device/device_default.hpp"
#include "ringct/rctOps.h"
#include "ringct/rctTypes.h"
#include "string_tools.h"
#include "wallet2.h"

 
#include <boost/algorithm/string/join.hpp>
#include <boost/multiprecision/cpp_int.hpp>

 
#include <algorithm>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#undef MONERO_DEFAULT_LOG_CATEGORY
#define MONERO_DEFAULT_LOG_CATEGORY "wallet.tx_builder"

namespace tools
{
namespace wallet
{
 
static const std::vector<cryptonote::tx_destination_entry>& get_tx_destinations(const wallet2::tx_construction_data &tx)
{
    return tx.splitted_dsts;
}
 
static const std::vector<cryptonote::tx_destination_entry>& get_tx_destinations(const wallet2::pending_tx &tx)
{
    return tx.construction_data.splitted_dsts;
}
 
static const std::vector<cryptonote::tx_source_entry>& get_tx_sources(const wallet2::tx_construction_data &tx)
{
    return tx.sources;
}
 
static const std::vector<cryptonote::tx_source_entry>& get_tx_sources(const wallet2::pending_tx &tx)
{
    return tx.construction_data.sources;
}
 
template<typename T>
static bool check_consistent_ins_outs_impl(const std::vector<T> &txes)
{
    std::unordered_set<rct::key> seen_ins;
    std::unordered_map<cryptonote::account_public_address, bool> destination_types{};
    boost::multiprecision::uint128_t total_input_amount = 0;
    for (const auto &tx: txes)
    {
         
        for (const auto &src: get_tx_sources(tx))
        {
            CHECK_AND_ASSERT_THROW_MES(src.real_output < src.outputs.size(),
                "check_consistent_ins_outs: ring sig index " << src.real_output << " out of input set size "
                << src.outputs.size());
            const auto &dest = src.outputs[src.real_output].second.dest;
            const auto result = seen_ins.emplace(dest);
            CHECK_AND_ASSERT_THROW_MES(result.second,
                "check_consistent_ins_outs: duplicate input pubkey");
            total_input_amount += src.amount;
        }

         
         
         
         
        for (const auto &dest: get_tx_destinations(tx))
        {
            const auto result = destination_types.emplace(dest.addr, dest.is_subaddress);
            CHECK_AND_ASSERT_THROW_MES(result.second || result.first->second == dest.is_subaddress,
                "check_consistent_ins_outs: duplicate destinations do not have matching subaddress designations");
        }
    }
    CHECK_AND_ASSERT_THROW_MES(total_input_amount <= UINT64_MAX,
        "check_consistent_ins_outs: tx set input amount > 2^64 - 1");
    return true;
}
 
static void sanity_check_unsigned_tx(const wallet2::tx_construction_data &tx,
    const cryptonote::account_keys &account_keys,
    const std::unordered_map<crypto::public_key, cryptonote::subaddress_index> &subaddresses)
{
    sanity_check_tx_construction_data(tx);

    const auto &change = tx.change_dts;
    const bool pays_change = std::any_of(tx.splitted_dsts.begin(), tx.splitted_dsts.end(),
        [&change](const cryptonote::tx_destination_entry &dest) {
            return dest.amount > 0 && dest.addr == change.addr;
        });

     
    if (change.amount == 0 && !pays_change)
        return;

    if (cryptonote::sanity_check_change_address(change.addr, subaddresses, account_keys))
        return;

    const auto expected_change = account_keys.get_device().get_subaddress(account_keys, {tx.subaddr_account, 0});
    CHECK_AND_ASSERT_THROW_MES(change.addr == expected_change,
        "sanity_check_unsigned_tx: change address does not belong to the sender account");
}
 
static void validate_tx_outs(
    const wallet2::pending_tx &ptx,
    const std::vector<cryptonote::txout_to_tagged_key> &ext_outputs,
    const crypto::secret_key &k_view,
    const cryptonote::tx_destination_entry &change,
    const std::vector<cryptonote::tx_destination_entry> &dests,
    const crypto::secret_key &tx_key,
    const std::vector<crypto::secret_key> &additional_tx_keys,
    crypto::public_key &tx_pubkey_out,
    std::vector<crypto::public_key> &tx_additional_pubkeys_out)
{
    tx_pubkey_out = crypto::public_key{};
    tx_additional_pubkeys_out.clear();
    CHECK_AND_ASSERT_THROW_MES(ptx.tx.vout.size() == dests.size(),
        "validate_tx_outs: dests size mismatch");
    CHECK_AND_ASSERT_THROW_MES(ext_outputs.size() == dests.size(),
        "validate_tx_outs: ext_outputs size mismatch");
    CHECK_AND_ASSERT_THROW_MES(ptx.tx.version == 1 || ptx.tx.rct_signatures.ecdhInfo.size() == dests.size(),
        "validate_tx_outs: ecdhInfo size mismatch");
    CHECK_AND_ASSERT_THROW_MES(ptx.tx.version == 1 || ptx.tx.rct_signatures.outPk.size() == dests.size(),
        "validate_tx_outs: outPk size mismatch");

     
     
    size_t num_stdaddresses = 0;
    size_t num_subaddresses = 0;
    cryptonote::account_public_address single_dest_subaddress;
    cryptonote::classify_addresses(dests, change.addr, num_stdaddresses, num_subaddresses, single_dest_subaddress);
    const bool need_additional_txkeys = num_subaddresses > 0 && (num_stdaddresses > 0 || num_subaddresses > 1);

    const size_t expect_num_additional = need_additional_txkeys ? dests.size() : 0;
    CHECK_AND_ASSERT_THROW_MES(additional_tx_keys.size() == expect_num_additional,
        "validate_tx_outs: additional tx keys invalid size");

     
    if (num_stdaddresses == 0 && num_subaddresses == 1)
    {
        tx_pubkey_out = rct::rct2pk(rct::scalarmultKey(
                rct::pk2rct(single_dest_subaddress.m_spend_public_key),
                rct::sk2rct(tx_key)
            ));
    }
    else
    {
        tx_pubkey_out = rct::rct2pk(rct::scalarmultBase(rct::sk2rct(tx_key)));
    }

     
    cryptonote::account_keys sender_keys;
    sender_keys.m_view_secret_key = k_view;

    std::vector<rct::key> amount_keys;
    for (size_t i = 0; i < dests.size(); ++i)
    {
        const auto &dest = dests.at(i);

        crypto::public_key repro_onetime_addr{};
        crypto::view_tag repro_view_tag{};
        const bool r = hw::core::device_default().generate_output_ephemeral_keys(
                ptx.tx.version,
                sender_keys,
                tx_pubkey_out,
                tx_key,
                dest,
                change.addr,
                i,
                need_additional_txkeys,
                additional_tx_keys,
                tx_additional_pubkeys_out,
                amount_keys,
                repro_onetime_addr,
                ptx.construction_data.use_view_tags,
                repro_view_tag
            );
        CHECK_AND_ASSERT_THROW_MES(r, "validate_tx_outs: failed to generate output ephemeral key");

         
        CHECK_AND_ASSERT_THROW_MES(repro_onetime_addr == ext_outputs.at(i).key,
            "validate_tx_outs: failed reproducing onetime address");

         
        CHECK_AND_ASSERT_THROW_MES(!ptx.construction_data.use_view_tags || repro_view_tag == ext_outputs.at(i).view_tag,
            "validate_tx_outs: failed reproducing view tag");

        if (ptx.tx.version == 1)
        {
             
            CHECK_AND_ASSERT_THROW_MES(ptx.tx.vout.at(i).amount == dest.amount,
                "validate_tx_outs: v1 tx output amount doesn't match destination amount");

            continue;
        }

         
        rct::ecdhTuple amnt_data{};
        amnt_data.amount = rct::d2h(dest.amount);
        rct::ecdhDecode(amnt_data, amount_keys.at(i), true);  
        CHECK_AND_ASSERT_THROW_MES(ptx.tx.rct_signatures.ecdhInfo.at(i).amount == amnt_data.amount,
            "validate_tx_outs: failed reproducing encoded amount");

         
        const rct::key repro_C = rct::commit(dest.amount, amnt_data.mask);
        CHECK_AND_ASSERT_THROW_MES(ptx.tx.rct_signatures.outPk.at(i).mask == repro_C,
            "validate_tx_outs: failed reproducing amount commitment");
    }
}
 
static void reconstruct_payment_id(const std::string &extracted_payment_id,
    const boost::optional<crypto::hash8> &parsed_to_check,
     
    const cryptonote::account_public_address &change_addr,
    const std::vector<cryptonote::tx_destination_entry> &dests,
     
    const boost::optional<crypto::secret_key> tx_key,
    const std::vector<cryptonote::tx_extra_field> &tx_extra_fields)
{
     
     
    const crypto::public_key view_key_pub = cryptonote::get_destination_view_key_pub(dests, change_addr);
    CHECK_AND_ASSERT_THROW_MES(view_key_pub != crypto::null_pkey,
        "reconstruct_payment_id: encrypted payment IDs must only be in txs with one destination");

     
    cryptonote::tx_extra_nonce extra_nonce;
    crypto::hash8 encrypted_payment_id8 = crypto::null_hash8;
    CHECK_AND_ASSERT_THROW_MES(cryptonote::find_tx_extra_field_by_type(tx_extra_fields, extra_nonce),
        "reconstruct_payment_id: expected payment id is missing");
    CHECK_AND_ASSERT_THROW_MES(cryptonote::get_encrypted_payment_id_from_tx_extra_nonce(
            extra_nonce.nonce,
            encrypted_payment_id8
        ),
        "reconstruct_payment_id: expected payment id is missing");

     
    crypto::hash8 to_encrypt_payment_id;
    CHECK_AND_ASSERT_THROW_MES(tools::wallet2::parse_short_payment_id(extracted_payment_id, to_encrypt_payment_id),
        "reconstruct_payment_id: failed parsing short payment id from integrated address");
    if (parsed_to_check)
    {
        CHECK_AND_ASSERT_THROW_MES(*parsed_to_check == to_encrypt_payment_id,
            "reconstruct_payment_id: did not extract the same parsed payment id");
    }
     
     
     
     
     

    if (tx_key)
    {
         
        CHECK_AND_ASSERT_THROW_MES(hw::core::device_default().encrypt_payment_id(to_encrypt_payment_id,
                view_key_pub, *tx_key),
            "reconstruct_payment_id: failed encrypting payment id");

         
        CHECK_AND_ASSERT_THROW_MES(to_encrypt_payment_id == encrypted_payment_id8,
            "reconstruct_payment_id: failed encrypting payment id");
    }
}
 
void sanity_check_pending_tx(const wallet2::pending_tx &ptx,
    const cryptonote::network_type nettype,
    const cryptonote::account_keys &account_keys,
    const std::unordered_map<crypto::public_key, cryptonote::subaddress_index> &subaddresses,
    const std::vector<wallet2::transfer_details> &transfers,
    const bool redacted,
    const boost::optional<std::function<const crypto::key_image(const size_t)>> &transfer_ki_resolver,
    const bool allow_read_only)
{
    const auto &construct = ptx.construction_data;
    const bool transfer_ki_accessible = bool(transfer_ki_resolver);

     
    std::vector<cryptonote::txout_to_tagged_key> ext_outputs;
    for (const auto &out : ptx.tx.vout)
    {
        cryptonote::txout_to_tagged_key extracted{};
        CHECK_AND_ASSERT_THROW_MES(cryptonote::get_output_public_key(out, extracted.key),
            "sanity_check_pending_tx: unsupported output type");
        const auto tag = cryptonote::get_output_view_tag(out);
        CHECK_AND_ASSERT_THROW_MES(bool(tag) == construct.use_view_tags,
            "sanity_check_pending_tx: output view tag mode mismatch");
        if (tag) extracted.view_tag = *tag;
        ext_outputs.push_back(extracted);
    }
    CHECK_AND_ASSERT_THROW_MES(construct.splitted_dsts.size() == ptx.tx.vout.size(),
        "sanity_check_pending_tx: destination vecs are inconsistent");
    CHECK_AND_ASSERT_THROW_MES(ptx.tx.unlock_time == construct.unlock_time,
        "sanity_check_pending_tx: unlock_time mismatch");

     
    std::vector<cryptonote::tx_extra_field> tx_extra_fields;
    CHECK_AND_ASSERT_THROW_MES(cryptonote::parse_tx_extra(ptx.tx.extra, tx_extra_fields),
        "sanity_check_pending_tx: ptx.tx.extra extraction failure");
     
     
     
     
     
     
     
     
     
     
     
     
     
     

     
     

    if (!redacted)
    {
        crypto::public_key reconstruct_pubkey;
        std::vector<crypto::public_key> reconstruct_additional_pubkeys;

        validate_tx_outs(
            ptx,
            ext_outputs,
            account_keys.m_view_secret_key,
            construct.change_dts,
            construct.splitted_dsts,
            ptx.tx_key,
            ptx.additional_tx_keys,
            reconstruct_pubkey,
            reconstruct_additional_pubkeys
        );

        cryptonote::tx_extra_pub_key tx_extra_pub_key;
        CHECK_AND_ASSERT_THROW_MES(cryptonote::find_tx_extra_field_by_type(tx_extra_fields, tx_extra_pub_key),
            "sanity_check_pending_tx: tx_extra missing tx pub key");
        CHECK_AND_ASSERT_THROW_MES(tx_extra_pub_key.pub_key == reconstruct_pubkey,
            "sanity_check_pending_tx: tx_extra unable to reproduce tx pubkey");
        if (reconstruct_additional_pubkeys.size())
        {
            cryptonote::tx_extra_additional_pub_keys tx_extra_additional_pub_keys;
            CHECK_AND_ASSERT_THROW_MES(cryptonote::find_tx_extra_field_by_type(tx_extra_fields, tx_extra_additional_pub_keys),
                "sanity_check_pending_tx: tx_extra missing extra tx pub keys");
            CHECK_AND_ASSERT_THROW_MES(tx_extra_additional_pub_keys.data.size() == reconstruct_additional_pubkeys.size(),
                "sanity_check_pending_tx: tx_extra extra tx pub keys size mismatch");
            for (size_t p = 0; p < reconstruct_additional_pubkeys.size(); ++p)
            {
                const auto &extra_pk = tx_extra_additional_pub_keys.data.at(p);
                const auto &re_pk = reconstruct_additional_pubkeys.at(p);
                CHECK_AND_ASSERT_THROW_MES(extra_pk == re_pk,
                    "sanity_check_pending_tx: tx_extra unable to reproduce extra tx pubkey");
            }
        }
    }

     
    std::vector<crypto::key_image> ext_key_images;
    std::vector<rct::xmr_amount> ext_input_amounts;
    std::vector<std::vector<uint64_t>> ext_key_offsets;
    const bool all_are_txin_to_key = std::all_of(ptx.tx.vin.begin(), ptx.tx.vin.end(),
        [&](const cryptonote::txin_v& s_e) -> bool
        {
            CHECKED_GET_SPECIFIC_VARIANT(s_e, const cryptonote::txin_to_key, in, false);
            ext_key_images.push_back(in.k_image);
            ext_input_amounts.push_back(in.amount);
            ext_key_offsets.push_back(in.key_offsets);
            return true;
        }
    );
    std::string ext_key_images_str;
    for (const auto &ki : ext_key_images)
    {
        ext_key_images_str += boost::to_string(ki) + " ";
    }
    CHECK_AND_ASSERT_THROW_MES(all_are_txin_to_key,
        "sanity_check_pending_tx: all inputs are not txin_to_key");
    CHECK_AND_ASSERT_THROW_MES(ptx.key_images == ext_key_images_str,
        "sanity_check_pending_tx: failed reconstructing key_images field");

    const std::unordered_set<crypto::key_image> ext_key_images_set(ext_key_images.begin(), ext_key_images.end());
    CHECK_AND_ASSERT_THROW_MES(ext_key_images.size() == ext_key_images_set.size(),
        "sanity_check_pending_tx: duplicate key images");

     
     
    CHECK_AND_ASSERT_THROW_MES(construct.sources.size() > 0,
        "sanity_check_pending_tx: tx has no inputs");
    CHECK_AND_ASSERT_THROW_MES(construct.sources.size() == ptx.tx.vin.size(),
        "sanity_check_pending_tx: source/vin size mismatch");
     
     
     
     
     
     
     
     
     
     
     
     
    CHECK_AND_ASSERT_THROW_MES(construct.sources.size() == ptx.selected_transfers.size(),
        "sanity_check_pending_tx: selected_transfers invalid size");
    CHECK_AND_ASSERT_THROW_MES(construct.selected_transfers == ptx.selected_transfers,
        "sanity_check_pending_tx: selected_transfers inconsistent");
     
    auto sources_ordered = construct.sources;
    std::vector<size_t> ins_order;
    std::unordered_set<crypto::public_key> seen_ins;
    for (const size_t selected_transfer : ptx.selected_transfers)
    {
        CHECK_AND_ASSERT_THROW_MES(selected_transfer < transfers.size(),
            "sanity_check_pending_tx: invalid transfers index");
        const auto &transfer = transfers.at(selected_transfer);
        const auto transfer_pkey = transfer.get_public_key();
        for (size_t i = 0; i < sources_ordered.size(); ++i)
        {
            const auto &src = sources_ordered.at(i);
            CHECK_AND_ASSERT_THROW_MES(src.real_output < src.outputs.size(),
                "sanity_check_pending_tx: ring sig index " << src.real_output << " out of input set size "
                << src.outputs.size());
             
             
            if (src.outputs[src.real_output].first != transfer.m_global_output_index
                || src.amount != transfer.m_amount
                || memcmp(src.outputs[src.real_output].second.dest.bytes, rct::pk2rct(transfer_pkey).bytes, sizeof(rct::key)) != 0)
                continue;
            ins_order.push_back(i);
            break;
        }
         
         
        CHECK_AND_ASSERT_THROW_MES(seen_ins.count(transfer_pkey) == 0,
            "sanity_check_pending_tx: duplicate input pubkey");
        seen_ins.insert(transfer_pkey);
    }
    CHECK_AND_ASSERT_THROW_MES(ins_order.size() == sources_ordered.size(),
        "sanity_check_pending_tx: global index mismatch between sources and tx");
    tools::apply_permutation(ins_order, sources_ordered);

     
     
     
    std::vector<size_t> inp_order(ptx.selected_transfers.size());
    size_t found_kis_count = 0;
    CHECK_AND_ASSERT_THROW_MES(ext_key_images.size() == ptx.selected_transfers.size(),
        "sanity_check_pending_tx: extracted key images size mismatch to selected transfers");
    for (size_t i = 0; i < ext_key_images.size() && transfer_ki_accessible; ++i)
    {
        const crypto::key_image &ki = ext_key_images[i];
        for (size_t j = 0; j < ptx.selected_transfers.size(); ++j)
        {
            const auto &selected_transfer = ptx.selected_transfers.at(j);
            CHECK_AND_ASSERT_THROW_MES(selected_transfer < transfers.size(),
                "sanity_check_pending_tx: invalid transfers index");
            if ((*transfer_ki_resolver)(selected_transfer) != ki)
                continue;
            inp_order.at(j) = i;
            ++found_kis_count;
            break;
        }
    }
    CHECK_AND_ASSERT_THROW_MES(!transfer_ki_accessible || found_kis_count == ptx.selected_transfers.size(),
        "sanity_check_pending_tx: ki mismatch between selected transfers and tx");

    boost::multiprecision::uint128_t input_amnt = 0;
    for (size_t i = 0; i < ptx.selected_transfers.size(); ++i)
    {
        const auto &selected_transfer = ptx.selected_transfers.at(i);
        CHECK_AND_ASSERT_THROW_MES(selected_transfer < transfers.size(),
            "sanity_check_pending_tx: invalid transfers index");

        const auto &src = sources_ordered.at(i);
        const auto &transfer = transfers.at(selected_transfer);

        if (ptx.tx.version == 1)
        {
            CHECK_AND_ASSERT_THROW_MES(!src.rct,
                "sanity_check_pending_tx: rct sources not allowed for v1 txs");
        }
        CHECK_AND_ASSERT_THROW_MES(src.amount == transfer.m_amount,
            "sanity_check_pending_tx: transfer amount mismatch");
        CHECK_AND_ASSERT_THROW_MES(src.real_output < src.outputs.size(),
            "sanity_check_pending_tx: ring sig index out of input set size");
         
        const rct::key commitment = rct::commit(src.amount, ptx.tx.version > 1 ? src.mask : rct::identity());
         
        if (ptx.tx.version > 1 && !src.rct)
        {
            CHECK_AND_ASSERT_THROW_MES(src.mask == rct::identity(),
                "sanity_check_pending_tx: pre-rct input in > v1 tx has non-identity mask recorded");
        }
        CHECK_AND_ASSERT_THROW_MES(src.outputs[src.real_output].second.mask == commitment,
            "sanity_check_pending_tx: failed reproducing real input's amount commitment");

         
         
         
         
         
         
         
         
         
         
         
         

        if (!allow_read_only)
        {
            CHECK_AND_ASSERT_THROW_MES(!transfer.m_spent,
                "sanity_check_pending_tx: non-read-only transfer - is marked as spent");
            CHECK_AND_ASSERT_THROW_MES(!transfer.m_frozen,
                "sanity_check_pending_tx: non-read-only transfer - is marked as frozen");
        }
        CHECK_AND_ASSERT_THROW_MES(src.rct == transfer.m_rct,
            "sanity_check_pending_tx: transfer - 'is rct' mismatch");
        CHECK_AND_ASSERT_THROW_MES(src.real_output_in_tx_index == transfer.m_internal_output_index,
            "sanity_check_pending_tx: transfer - input's output-set index mismatch");
        CHECK_AND_ASSERT_THROW_MES(src.outputs[src.real_output].second.dest == rct::pk2rct(transfer.get_public_key()),
            "sanity_check_pending_tx: transfer - onetime addr mismatch");

        if (memcmp(transfer.m_mask.bytes, rct::identity().bytes, sizeof(rct::key)) != 0)
        {
             
             
             
             
            CHECK_AND_ASSERT_THROW_MES(src.mask == transfer.m_mask,
                "sanity_check_pending_tx: transfer - mask mismatch");
        }

        input_amnt += src.amount;
         
        if (!transfer_ki_accessible)
            continue;

        const uint64_t ext_input_amount = ext_input_amounts.at(inp_order.at(i));
        const std::vector<uint64_t> &ext_relative_key_offsets = ext_key_offsets.at(inp_order.at(i));

         
         
        CHECK_AND_ASSERT_THROW_MES(src.rct
            ? ext_input_amount == 0
            : ext_input_amount == src.amount,
            "sanity_check_pending_tx: amount extracted from inputs is wrong");

         
        std::vector<uint64_t> src_absolute_key_offsets;
        for (const auto &src_out : src.outputs)
            src_absolute_key_offsets.push_back(src_out.first);
        const std::vector<uint64_t> ext_absolute_key_offsets = cryptonote::relative_output_offsets_to_absolute(
            ext_relative_key_offsets
        );
        CHECK_AND_ASSERT_THROW_MES(src_absolute_key_offsets == ext_absolute_key_offsets,
            "sanity_check_pending_tx: key offsets mismatch");
    }

    CHECK_AND_ASSERT_THROW_MES(input_amnt <= UINT64_MAX,
        "sanity_check_pending_tx: input amount > 2^64 - 1");

     
    boost::optional<cryptonote::subaddress_index> recognized_change_index;
    recognized_change_index = cryptonote::sanity_check_change_address(ptx.change_dts.addr, subaddresses, account_keys);
    for (const auto &dst_entr : construct.splitted_dsts)
    {
        CHECK_AND_ASSERT_THROW_MES(dst_entr.amount > 0 || ptx.tx.version > 1,
            "sanity_check_pending_tx: Destination with wrong amount: " << dst_entr.amount);
        const bool matches_change_addr = dst_entr.addr == ptx.change_dts.addr;
        const bool is_bad_change_dst = matches_change_addr && dst_entr.amount > 0 && !recognized_change_index;
        CHECK_AND_ASSERT_THROW_MES(!is_bad_change_dst,
            "sanity_check_pending_tx: Non-zero amount change address is not recognized as belonging to the sender account");
    }

     
    CHECK_AND_ASSERT_THROW_MES(construct.change_dts == ptx.change_dts,
        "sanity_check_pending_tx: change_dts inconsistent");
    CHECK_AND_ASSERT_THROW_MES(construct.dests == ptx.dests,
        "sanity_check_pending_tx: destination vecs are inconsistent");
    std::vector<cryptonote::tx_destination_entry> splitted_dsts_repro;

    if (ptx.tx.version == 1)
    {
        splitted_dsts_repro = construct.splitted_dsts;

         
         
         
         

         
        std::unordered_map<cryptonote::account_public_address, boost::multiprecision::uint128_t> acc_split;
        for (const auto &split : construct.splitted_dsts)
        {
            auto maybe_found = acc_split.find(split.addr);
            if (maybe_found != acc_split.cend())
            {
                maybe_found->second += split.amount;
            }
            else
            {
                acc_split.insert({split.addr, split.amount});
            }
        }

         
        std::unordered_map<cryptonote::account_public_address, boost::multiprecision::uint128_t> acc_dest;
        if (ptx.change_dts.amount > 0)
            acc_dest.emplace(ptx.change_dts.addr, ptx.change_dts.amount);

        for (const auto &dest : ptx.dests)
        {
            auto maybe_found = acc_dest.find(dest.addr);
            if (maybe_found != acc_dest.cend())
            {
                maybe_found->second += dest.amount;
            }
            else
            {
                acc_dest.insert({dest.addr, dest.amount});
            }
        }

         
        CHECK_AND_ASSERT_THROW_MES(acc_split.size() == acc_dest.size(),
            "sanity_check_pending_tx: v1 dest inconsistency (size)");
        for (const auto &split : acc_split)
        {
            const auto &dest_res = acc_dest.find(split.first);
            CHECK_AND_ASSERT_THROW_MES(dest_res != acc_dest.cend(),
                "sanity_check_pending_tx: v1 dest inconsistency (addr matching)");
            CHECK_AND_ASSERT_THROW_MES(split.second == dest_res->second,
                "sanity_check_pending_tx: v1 dest inconsistency (amount matching)");
            CHECK_AND_ASSERT_THROW_MES(split.second <= UINT64_MAX,
                "sanity_check_pending_tx: v1 dest error (amount out of bounds)");
        }
    }
    else  
    {
        splitted_dsts_repro = ptx.dests;

        if (ptx.change_dts.amount > 0 || ptx.dests.size() <= 1)
        {
            splitted_dsts_repro.push_back(ptx.change_dts);
        }
        CHECK_AND_ASSERT_THROW_MES(construct.splitted_dsts.size() == splitted_dsts_repro.size(),
            "sanity_check_pending_tx: failed checking splitted_dsts size");
        CHECK_AND_ASSERT_THROW_MES(ptx.tx.rct_signatures.ecdhInfo.size() == splitted_dsts_repro.size(),
            "sanity_check_pending_tx: ecdhInfo size mismatch");
        CHECK_AND_ASSERT_THROW_MES(ptx.tx.rct_signatures.outPk.size() == splitted_dsts_repro.size(),
            "sanity_check_pending_tx: outPk size mismatch");
    }

    boost::multiprecision::uint128_t output_amnt = 0;
    size_t integrated_count = 0;
    std::unordered_map<crypto::public_key, cryptonote::tx_destination_entry> dest_duplicates{};

    for (size_t i = 0; i < construct.splitted_dsts.size(); ++i)
    {
        const auto &dest = construct.splitted_dsts.at(i);

         
        CHECK_AND_ASSERT_THROW_MES(!dest.is_integrated || dest.original.size() > 0,
            "sanity_check_pending_tx: integrated destination missing original address string");

         
        if (dest.original.size() > 0)
        {
            std::string address;
            std::string extracted_payment_id;
            uint64_t _amount;
            std::string _tx_description;
            std::string _recipient_name;
            std::vector<std::string> _unknown_parameters;
            std::string _error;
            const bool parsed_uri = wallet2::parse_uri_impl(dest.original,
                nettype,
                address,
                extracted_payment_id,
                _amount,
                _tx_description,
                _recipient_name,
                _unknown_parameters,
                _error);

            if (!parsed_uri)
            {
                 
                address = dest.original;
                extracted_payment_id = "";
            }

             
            cryptonote::address_parse_info parse_info;
            const bool parsed_info = cryptonote::get_account_address_from_str(parse_info, nettype, address);
            if (parsed_info)
            {
                CHECK_AND_ASSERT_THROW_MES(parse_info.is_subaddress == dest.is_subaddress,
                    "sanity_check_pending_tx: destination addr string mismatch - is_subaddress");
                CHECK_AND_ASSERT_THROW_MES(parse_info.has_payment_id == dest.is_integrated,
                    "sanity_check_pending_tx: destination addr string mismatch - is_integrated");
                CHECK_AND_ASSERT_THROW_MES(parse_info.address == dest.addr,
                    "sanity_check_pending_tx: destination addr string mismatch - address");

                if (parse_info.has_payment_id && !extracted_payment_id.empty())
                {
                    CHECK_AND_ASSERT_THROW_MES(extracted_payment_id == epee::string_tools::pod_to_hex(parse_info.payment_id),
                        "sanity_check_pending_tx: destination addr string mismatch - internal payment id inconsistency");
                }

                 
                 
                if (parse_info.has_payment_id && extracted_payment_id.empty())
                    extracted_payment_id = epee::string_tools::pod_to_hex(parse_info.payment_id);
            }

            if (dest.is_integrated)
            {
                ++integrated_count;
                CHECK_AND_ASSERT_THROW_MES(integrated_count == 1,
                    "sanity_check_pending_tx: more than one integrated address detected");

                 
                 
                 
                 
                 
                 
                 
                 
                 

                 
                 
                 
                 
                 
                 
                 
                 
                 
                 

                if (!extracted_payment_id.empty())
                {
                    reconstruct_payment_id(extracted_payment_id,
                        parsed_info ? boost::optional<crypto::hash8>{parse_info.payment_id} : boost::optional<crypto::hash8>(),
                         
                        ptx.change_dts.addr,
                        construct.splitted_dsts,
                        redacted ? boost::optional<crypto::secret_key>() : boost::optional<crypto::secret_key>{ptx.tx_key},
                        tx_extra_fields);
                }
            }
        }

         
        const auto &it = std::find_if(splitted_dsts_repro.cbegin(), splitted_dsts_repro.cend(),
            [&dest](const auto &a) {
                return a == dest;
            }
        );
        CHECK_AND_ASSERT_THROW_MES(it != splitted_dsts_repro.cend(),
            "sanity_check_pending_tx: failed checking splitted_dsts consistency");

         
         
        CHECK_AND_ASSERT_THROW_MES(cryptonote::check_address(dest.addr),
            "sanity_check_pending_tx: invalid destination address keys");

         
        const auto &dup = dest_duplicates.find(dest.addr.m_spend_public_key);
        if (dup != dest_duplicates.cend())
        {
            CHECK_AND_ASSERT_THROW_MES(dup->second.addr == dest.addr,
                "sanity_check_pending_tx: destinations have duplicate spend keys but not duplicate view keys");
            CHECK_AND_ASSERT_THROW_MES(dup->second.is_subaddress == dest.is_subaddress,
                "sanity_check_pending_tx: duplicate destinations do not have matching subaddress designations");
             
             
        }
        else
        {
            dest_duplicates[dest.addr.m_spend_public_key] = dest;
        }

         
        splitted_dsts_repro.erase(it);

         
        if (ptx.tx.version == 1)
        {
            CHECK_AND_ASSERT_THROW_MES(ptx.tx.vout.at(i).amount == dest.amount,
                "sanity_check_pending_tx: v1 tx output amount doesn't match destination amount");
        }

        output_amnt += dest.amount;
    }
    CHECK_AND_ASSERT_THROW_MES(splitted_dsts_repro.size() == 0,
        "sanity_check_pending_tx: failed checking splitted_dsts consistency");

     
    CHECK_AND_ASSERT_THROW_MES(cryptonote::check_tx_burn_rules(ptx.tx) &&
        !ptx.tx.is_coinbase(), "sanity_check_pending_tx: invalid Venera transaction metadata");
    const uint64_t burn = cryptonote::get_tx_burn_amount(ptx.tx);
    const boost::multiprecision::uint128_t fee_side =
        boost::multiprecision::uint128_t(ptx.fee) + burn;
    CHECK_AND_ASSERT_THROW_MES(fee_side <= UINT64_MAX,
        "sanity_check_pending_tx: fee plus burn overflow");
    output_amnt += fee_side;

    if (ptx.tx.version > 1)
    {
        CHECK_AND_ASSERT_THROW_MES(ptx.tx.rct_signatures.txnFee == fee_side,
                "sanity_check_pending_tx: inconsistent fee for tx.v > 1 transaction");
    }
    if (ptx.dust_added_to_fee)
    {
        CHECK_AND_ASSERT_THROW_MES(ptx.dust <= ptx.fee,
            "sanity_check_pending_tx: invalid dust amount when dust added to fee");
    }
    CHECK_AND_ASSERT_THROW_MES(output_amnt <= UINT64_MAX,
            "sanity_check_pending_tx: output amount > 2^64 - 1");
    CHECK_AND_ASSERT_THROW_MES(output_amnt == input_amnt,
            "sanity_check_pending_tx: output amount != input amount");
}
 
void check_consistent_ins_outs(const std::vector<wallet2::tx_construction_data> &txes)
{
    check_consistent_ins_outs_impl(txes);
}
 
void check_consistent_ins_outs(const std::vector<wallet2::pending_tx> &txes)
{
    check_consistent_ins_outs_impl(txes);
}
 
void sanity_check_tx_construction_data(const wallet2::tx_construction_data &tx)
{
    boost::multiprecision::uint128_t input_amount = 0;
    boost::multiprecision::uint128_t output_amount = 0;
    boost::multiprecision::uint128_t amount_to_change_address = 0;
    for (const auto &src: tx.sources)
        input_amount += src.amount;
    for (const auto &dest: tx.splitted_dsts)
    {
        output_amount += dest.amount;
        if (dest.addr == tx.change_dts.addr)
            amount_to_change_address += dest.amount;
    }
    CHECK_AND_ASSERT_THROW_MES(output_amount <= input_amount,
        "sanity_check_tx_construction_data: output amount exceeds input amount");
    CHECK_AND_ASSERT_THROW_MES(tx.change_dts.amount <= amount_to_change_address,
        "sanity_check_tx_construction_data: change exceeds payment to change address");
}
 
void sanity_check_unsigned_tx_set(const std::vector<wallet2::tx_construction_data> &txes,
    const cryptonote::account_keys &account_keys,
    const std::unordered_map<crypto::public_key, cryptonote::subaddress_index> &subaddresses)
{
    check_consistent_ins_outs(txes);

    for (const auto &tx: txes)
        sanity_check_unsigned_tx(tx, account_keys, subaddresses);
}
 
void sanity_check_pending_tx_set(const std::vector<wallet2::pending_tx> &ptxs,
    const cryptonote::network_type nettype,
    const cryptonote::account_keys &account_keys,
    const std::unordered_map<crypto::public_key, cryptonote::subaddress_index> &subaddresses,
    const std::vector<wallet2::transfer_details> &transfers,
    const bool redacted,
    const boost::optional<std::function<const crypto::key_image(const size_t)>> &transfer_ki_resolver,
    const bool allow_read_only)
{
    check_consistent_ins_outs(ptxs);

    for (const auto &ptx: ptxs)
    {
        sanity_check_pending_tx(ptx, nettype, account_keys, subaddresses, transfers,
            redacted, transfer_ki_resolver, allow_read_only);
    }
}
 
}  
}  

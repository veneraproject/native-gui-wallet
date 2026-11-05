#include "block_approval.h"
#include "cryptonote_format_utils.h"
#include "string_tools.h"
#include "ringct/rctOps.h"

namespace cryptonote {
bool needs_block_approval(const block& b)
{
  return b.miner_tx.vin.size() == 1 &&
      b.miner_tx.vin[0].type() == typeid(txin_gen) &&
      boost::get<txin_gen>(b.miner_tx.vin[0]).height >= BLOCK_APPROVAL_HEIGHT;
}
bool valid_block_approval_public_key(const crypto::public_key& key)
{
  const auto point = rct::pk2rct(key);
  return crypto::check_key(key) && !(point == rct::identity()) && rct::isInMainSubgroup(point);
}
bool get_block_approval_public_key(crypto::public_key& key)
{
  return epee::string_tools::hex_to_pod(BLOCK_APPROVAL_PUBLIC_KEY, key) && valid_block_approval_public_key(key);
}
crypto::hash block_approval_message(const block& b, network_type nettype)
{
   
   
  std::string message = "Venera block approval v1";
  const auto& network = get_config(nettype).NETWORK_ID;
  message.append(reinterpret_cast<const char*>(network.data), network.size());
  message += get_block_hashing_blob(b);
  crypto::hash hash;
  crypto::cn_fast_hash(message.data(), message.size(), hash);
  return hash;
}
bool check_block_approval(const block& b, network_type nettype)
{
  if (!needs_block_approval(b)) return true;
  crypto::public_key key;
  return get_block_approval_public_key(key) &&
      crypto::check_signature(block_approval_message(b, nettype), key, b.approval_signature);
}
}

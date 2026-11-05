                                       
                                              

  
                       
  
                                                                                       
                                                            
  
                                                                                         
                                              
  
                                                                                         
                                                                                  
                                               
  
                                                                                       
                                                                                     
                               
  
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

#include "message_store.h"
#include <boost/archive/portable_binary_iarchive.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/system/error_code.hpp>
#include <boost/filesystem.hpp>
#include <fstream>
#include <sstream>
#include "file_io_utils.h"
#include "storages/http_abstract_invoke.h"
#include "wallet_errors.h"
#include "serialization/binary_utils.h"
#include "common/base58.h"
#include "common/util.h"
#include "common/utf8.h"
#include "string_tools.h"


#undef MONERO_DEFAULT_LOG_CATEGORY
#define MONERO_DEFAULT_LOG_CATEGORY "wallet.mms"

namespace mms
{

message_store::message_store(std::unique_ptr<epee::net_utils::http::abstract_http_client> http_client) : m_transporter(std::move(http_client))
{
  m_active = false;
  m_auto_send = false;
  m_next_message_id = 1;
  m_num_authorized_signers = 0;
  m_num_required_signers = 0;
  m_nettype = cryptonote::network_type::UNDEFINED;
  m_run = true;
}

namespace
{
   
   
   
  struct options
  {
    const command_line::arg_descriptor<std::string> bitmessage_address = {"bitmessage-address", mms::message_store::tr("Use PyBitmessage instance at URL <arg>"), "http://localhost:8442/"};
    const command_line::arg_descriptor<std::string> bitmessage_login = {"bitmessage-login", mms::message_store::tr("Specify <arg> as username:password for PyBitmessage API"), "username:password"};
  };
}

void message_store::init_options(boost::program_options::options_description& desc_params)
{
  const options opts{};
  command_line::add_arg(desc_params, opts.bitmessage_address);
  command_line::add_arg(desc_params, opts.bitmessage_login);
}

void message_store::init(const multisig_wallet_state &state, const std::string &own_label,
                         const std::string &own_transport_address, uint32_t num_authorized_signers, uint32_t num_required_signers)
{
  m_num_authorized_signers = num_authorized_signers;
  m_num_required_signers = num_required_signers;
  m_signers.clear();
  m_messages.clear();
  m_next_message_id = 1;

   
   
   
  authorized_signer signer;
  for (uint32_t i = 0; i < m_num_authorized_signers; ++i)
  {
    signer.me = signer.index == 0;     
    m_signers.push_back(signer);
    signer.index++;
  }

  set_signer(state, 0, own_label, own_transport_address, state.address);

  m_nettype = state.nettype;
  set_active(true);
  m_filename = state.mms_file;
  save(state);
}

void message_store::set_options(const boost::program_options::variables_map& vm)
{
  const options opts{};
  std::string bitmessage_address = command_line::get_arg(vm, opts.bitmessage_address);
  epee::wipeable_string bitmessage_login = command_line::get_arg(vm, opts.bitmessage_login);
  set_options(bitmessage_address, bitmessage_login);
}

void message_store::set_options(const std::string &bitmessage_address, const epee::wipeable_string &bitmessage_login)
{
  m_transporter.set_options(bitmessage_address, bitmessage_login);
}

void message_store::set_signer(const multisig_wallet_state &state,
                               uint32_t index,
                               const boost::optional<std::string> &label,
                               const boost::optional<std::string> &transport_address,
                               const boost::optional<cryptonote::account_public_address> monero_address)
{
  THROW_WALLET_EXCEPTION_IF(index >= m_num_authorized_signers, tools::error::wallet_internal_error, "Invalid signer index " + std::to_string(index));
  authorized_signer &m = m_signers[index];
  if (label)
  {
    m.label = get_sanitized_text(label.get(), 50);
  }
  if (transport_address)
  {
    m.transport_address = get_sanitized_text(transport_address.get(), 200);
  }
  if (monero_address)
  {
    m.monero_address_known = true;
    m.monero_address = monero_address.get();
  }
   
  save(state);
}

const authorized_signer &message_store::get_signer(uint32_t index) const
{
  THROW_WALLET_EXCEPTION_IF(index >= m_num_authorized_signers, tools::error::wallet_internal_error, "Invalid signer index " + std::to_string(index));
  return m_signers[index];
}

bool message_store::signer_config_complete() const
{
  for (uint32_t i = 0; i < m_num_authorized_signers; ++i)
  {
    const authorized_signer &m = m_signers[i];
    if (m.label.empty() || m.transport_address.empty() || !m.monero_address_known)
    {
      return false;
    }
  }
  return true;
}

 
 
bool message_store::signer_labels_complete() const
{
  for (uint32_t i = 0; i < m_num_authorized_signers; ++i)
  {
    const authorized_signer &m = m_signers[i];
    if (m.label.empty())
    {
      return false;
    }
  }
  return true;
}

void message_store::get_signer_config(std::string &signer_config)
{
  std::stringstream oss;
  binary_archive<true> ar(oss);
  THROW_WALLET_EXCEPTION_IF(!::serialization::serialize(ar, m_signers), tools::error::wallet_internal_error, "Failed to serialize signer config");
  signer_config = oss.str();
}

void message_store::unpack_signer_config(const multisig_wallet_state &state, const std::string &signer_config,
                                         std::vector<authorized_signer> &signers)
{
  try
  {
    binary_archive<false> ar{epee::strspan<std::uint8_t>(signer_config)};
    THROW_WALLET_EXCEPTION_IF(!::serialization::serialize(ar, signers), tools::error::wallet_internal_error, "Failed to serialize signer config");
  }
  catch (...)
  {
    THROW_WALLET_EXCEPTION_IF(true, tools::error::wallet_internal_error, "Invalid structure of signer config");
  }
  uint32_t num_signers = (uint32_t)signers.size();
  THROW_WALLET_EXCEPTION_IF(num_signers != m_num_authorized_signers, tools::error::wallet_internal_error, "Wrong number of signers in config: " + std::to_string(num_signers));
  for (uint32_t i = 0; i < num_signers; ++i)
  {
    authorized_signer &m = signers[i];
    m.label = get_sanitized_text(m.label, 50);
    m.transport_address = get_sanitized_text(m.transport_address, 200);
    m.auto_config_token = get_sanitized_text(m.auto_config_token, 20);
  }
}

void message_store::process_signer_config(const multisig_wallet_state &state, const std::string &signer_config)
{
   
   
   
   
   
   
   
   
   
   
  std::vector<authorized_signer> signers;
  unpack_signer_config(state, signer_config, signers);
  
  uint32_t new_index = 1;
  for (uint32_t i = 0; i < m_num_authorized_signers; ++i)
  {
    const authorized_signer &m = signers[i];
    uint32_t index;
    uint32_t take_index;
    bool found = get_signer_index_by_monero_address(m.monero_address, index);
    if (found)
    {
       
      take_index = index;
    }
    else
    {
       
       
      take_index = new_index;
      if ((new_index + 1) < m_num_authorized_signers)
      {
        new_index++;
      }
    }
    authorized_signer &modify = m_signers[take_index];
    modify.label = get_sanitized_text(m.label, 50);   
    if (!modify.me)
    {
      modify.transport_address = get_sanitized_text(m.transport_address, 200);
      modify.monero_address_known = m.monero_address_known;
      if (m.monero_address_known)
      {
        modify.monero_address = m.monero_address;
      }
    }
  }
  save(state);
}

void message_store::start_auto_config(const multisig_wallet_state &state)
{
  for (uint32_t i = 0; i < m_num_authorized_signers; ++i)
  {
    authorized_signer &m = m_signers[i];
    if (!m.me)
    {
      setup_signer_for_auto_config(i, create_auto_config_token(), true);
    }
    m.auto_config_running = true;
  }
  save(state);
}

 
 
 
bool message_store::check_auto_config_token(const std::string &raw_token,
                                            std::string &adjusted_token) const
{
  std::string prefix(AUTO_CONFIG_TOKEN_PREFIX);
  uint32_t num_hex_digits = (AUTO_CONFIG_TOKEN_BYTES + 1) * 2;
  uint32_t full_length = num_hex_digits + prefix.length();
  uint32_t raw_length = raw_token.length();
  std::string hex_digits;

  if (raw_length == full_length)
  {
     
    std::string raw_prefix(raw_token.substr(0, 3));
    boost::algorithm::to_lower(raw_prefix);
    if (raw_prefix != prefix)
    {
      return false;
    }
    hex_digits = raw_token.substr(3);
  }
  else if (raw_length == num_hex_digits)
  {
     
    hex_digits = raw_token;
  }
  else
  {
    return false;
  }

   
  boost::algorithm::to_lower(hex_digits);
  std::replace(hex_digits.begin(), hex_digits.end(), 'o', '0');
  std::replace(hex_digits.begin(), hex_digits.end(), 'i', '1');
  std::replace(hex_digits.begin(), hex_digits.end(), 'l', '1');

   
  std::string token_bytes;
  if (!epee::string_tools::parse_hexstr_to_binbuff(hex_digits, token_bytes))
  {
    return false;
  }
  const crypto::hash &hash = crypto::cn_fast_hash(token_bytes.data(), token_bytes.size() - 1);
  if (token_bytes[AUTO_CONFIG_TOKEN_BYTES] != hash.data[0])
  {
    return false;
  }
  adjusted_token = prefix + hex_digits;
  return true;
}

 
std::string message_store::create_auto_config_token()
{
  unsigned char random[AUTO_CONFIG_TOKEN_BYTES];
  crypto::rand(AUTO_CONFIG_TOKEN_BYTES, random);
  std::string token_bytes;
  token_bytes.append((char *)random, AUTO_CONFIG_TOKEN_BYTES);

   
   
  const crypto::hash &hash = crypto::cn_fast_hash(token_bytes.data(), token_bytes.size());
  token_bytes += hash.data[0];
  std::string prefix(AUTO_CONFIG_TOKEN_PREFIX);
  return prefix + epee::string_tools::buff_to_hex_nodelimer(token_bytes);
}

 
 
size_t message_store::add_auto_config_data_message(const multisig_wallet_state &state,
                                                   const std::string &auto_config_token)
{
  authorized_signer &me = m_signers[0];
  me.auto_config_token = auto_config_token;
  setup_signer_for_auto_config(0, auto_config_token, false);
  me.auto_config_running = true;

  auto_config_data data;
  data.label = me.label;
  data.transport_address = me.transport_address;
  data.monero_address = me.monero_address;

  std::stringstream oss;
  binary_archive<true> ar(oss);
  THROW_WALLET_EXCEPTION_IF(!::serialization::serialize(ar, data), tools::error::wallet_internal_error, "Failed to serialize auto config data");

  return add_message(state, 0, message_type::auto_config_data, message_direction::out, oss.str());
}

 
void message_store::process_auto_config_data_message(uint32_t id)
{
   
   
   

  const message &m = get_message_ref_by_id(id);

  auto_config_data data;
  try
  {
    binary_archive<false> ar{epee::strspan<std::uint8_t>(m.content)};
    THROW_WALLET_EXCEPTION_IF(!::serialization::serialize(ar, data), tools::error::wallet_internal_error, "Failed to serialize auto config data");
  }
  catch (...)
  {
    THROW_WALLET_EXCEPTION_IF(true, tools::error::wallet_internal_error, "Invalid structure of auto config data");
  }

  authorized_signer &signer = m_signers[m.signer_index];
   
  signer.transport_address = data.transport_address;
  signer.monero_address_known = true;
  signer.monero_address = data.monero_address;
  signer.auto_config_running = false;
}

void add_hash(crypto::hash &sum, const crypto::hash &summand)
{
  for (uint32_t i = 0; i < crypto::HASH_SIZE; ++i)
  {
    uint32_t x = (uint32_t)sum.data[i];
    uint32_t y = (uint32_t)summand.data[i];
    sum.data[i] = (char)((x + y) % 256);
  }
}

 
 
 
 
std::string message_store::get_config_checksum() const
{
  crypto::hash sum = crypto::null_hash;
  uint32_t num = SWAP32LE(m_num_authorized_signers);
  add_hash(sum, crypto::cn_fast_hash(&num, sizeof(num)));
  num = SWAP32LE(m_num_required_signers);
  add_hash(sum, crypto::cn_fast_hash(&num, sizeof(num)));
  for (uint32_t i = 0; i < m_num_authorized_signers; ++i)
  {
    const authorized_signer &m = m_signers[i];
    add_hash(sum, crypto::cn_fast_hash(m.transport_address.data(), m.transport_address.size()));
    if (m.monero_address_known)
    {
      add_hash(sum, crypto::cn_fast_hash(&m.monero_address.m_spend_public_key, sizeof(m.monero_address.m_spend_public_key)));
      add_hash(sum, crypto::cn_fast_hash(&m.monero_address.m_view_public_key, sizeof(m.monero_address.m_view_public_key)));
    }
  }
  std::string checksum_bytes;
  checksum_bytes += sum.data[0];
  checksum_bytes += sum.data[1];
  checksum_bytes += sum.data[2];
  checksum_bytes += sum.data[3];
  return epee::string_tools::buff_to_hex_nodelimer(checksum_bytes);
}

void message_store::stop_auto_config()
{
  for (uint32_t i = 0; i < m_num_authorized_signers; ++i)
  {
    authorized_signer &m = m_signers[i];
    if (!m.auto_config_transport_address.empty())
    {
       
      m_transporter.delete_transport_address(m.auto_config_transport_address);
    }
    m.auto_config_token.clear();
    m.auto_config_public_key = crypto::null_pkey;
    m.auto_config_secret_key = crypto::null_skey;
    m.auto_config_transport_address.clear();
    m.auto_config_running = false;
  }  
}

void message_store::setup_signer_for_auto_config(uint32_t index, const std::string token, bool receiving)
{
   
   
   
   
   
   
   
   
   
   
   
  THROW_WALLET_EXCEPTION_IF(index >= m_num_authorized_signers, tools::error::wallet_internal_error, "Invalid signer index " + std::to_string(index));
  authorized_signer &m = m_signers[index];
  m.auto_config_token = token;
  crypto::hash_to_scalar(token.data(), token.size(), m.auto_config_secret_key);
  crypto::secret_key_to_public_key(m.auto_config_secret_key, m.auto_config_public_key);
  m.auto_config_transport_address = m_transporter.derive_transport_address(m.auto_config_token);
}

bool message_store::get_signer_index_by_monero_address(const cryptonote::account_public_address &monero_address, uint32_t &index) const
{
  for (uint32_t i = 0; i < m_num_authorized_signers; ++i)
  {
    const authorized_signer &m = m_signers[i];
    if (m.monero_address == monero_address)
    {
      index = m.index;
      return true;
    }
  }
  MWARNING("No authorized signer with Venera address " << account_address_to_string(monero_address));
  return false;
}

bool message_store::get_signer_index_by_label(const std::string label, uint32_t &index) const
{
  for (uint32_t i = 0; i < m_num_authorized_signers; ++i)
  {
    const authorized_signer &m = m_signers[i];
    if (m.label == label)
    {
      index = m.index;
      return true;
    }
  }
  MWARNING("No authorized signer with label " << label);
  return false;
}

void message_store::process_wallet_created_data(const multisig_wallet_state &state, message_type type, const std::string &content)
{
  switch(type)
  {
  case message_type::key_set:
     
     
  case message_type::additional_key_set:
     
     
  case message_type::multisig_sync_data:
     
     
    for (uint32_t i = 1; i < m_num_authorized_signers; ++i)
    {
      add_message(state, i, type, message_direction::out, content);
    }
    break;

  case message_type::partially_signed_tx:
     
     
     
    if (m_num_required_signers == 1)
    {
       
       
      type = message_type::fully_signed_tx;
    }
    add_message(state, 0, type, message_direction::in, content);
    break;

  case message_type::fully_signed_tx:
    add_message(state, 0, type, message_direction::in, content);
    break;

  default:
    THROW_WALLET_EXCEPTION(tools::error::wallet_internal_error, "Illegal message type " + std::to_string((uint32_t)type));
    break;
  }
}

size_t message_store::add_message(const multisig_wallet_state &state,
                                  uint32_t signer_index, message_type type, message_direction direction,
                                  const std::string &content)
{
  message m;
  m.id = m_next_message_id++;
  m.type = type;
  m.direction = direction;
  m.content = content;
  m.created = (uint64_t)time(NULL);
  m.modified = m.created;
  m.sent = 0;
  m.signer_index = signer_index;
  if (direction == message_direction::out)
  {
    m.state = message_state::ready_to_send;
  }
  else
  {
    m.state = message_state::waiting;
  };
  m.wallet_height = (uint32_t)state.num_transfer_details;
  if (m.type == message_type::additional_key_set)
  {
    m.round = state.multisig_rounds_passed;
  }
  else
  {
    m.round = 0;
  }
  m.signature_count = 0;   
  m.hash = crypto::null_hash;
  m_messages.push_back(m);

   
  save(state);

  MINFO(boost::format("Added %s message %s for signer %s of type %s")
          % message_direction_to_string(direction) % m.id % signer_index % message_type_to_string(type));
  return m_messages.size() - 1;
}

 
bool message_store::get_message_index_by_id(uint32_t id, size_t &index) const
{
  for (size_t i = 0; i < m_messages.size(); ++i)
  {
    if (m_messages[i].id == id)
    {
      index = i;
      return true;
    }
  }
  MWARNING("No message found with an id of " << id);
  return false;
}

 
size_t message_store::get_message_index_by_id(uint32_t id) const
{
  size_t index;
  bool found = get_message_index_by_id(id, index);
  THROW_WALLET_EXCEPTION_IF(!found, tools::error::wallet_internal_error, "Invalid message id " + std::to_string(id));
  return index;
}

 
message& message_store::get_message_ref_by_id(uint32_t id)
{
  return m_messages[get_message_index_by_id(id)];
}

 
 
bool message_store::get_message_by_id(uint32_t id, message &m) const
{
  size_t index;
  bool found = get_message_index_by_id(id, index);
  if (found)
  {
    m = m_messages[index];
  }
  return found;
}

 
message message_store::get_message_by_id(uint32_t id) const
{
  message m;
  bool found = get_message_by_id(id, m);
  THROW_WALLET_EXCEPTION_IF(!found, tools::error::wallet_internal_error, "Invalid message id " + std::to_string(id));
  return m;
}

bool message_store::any_message_of_type(message_type type, message_direction direction) const
{
  for (size_t i = 0; i < m_messages.size(); ++i)
  {
    if ((m_messages[i].type == type) && (m_messages[i].direction == direction))
    {
      return true;
    }
  }
  return false;
}

bool message_store::any_message_with_hash(const crypto::hash &hash) const
{
  for (size_t i = 0; i < m_messages.size(); ++i)
  {
    if (m_messages[i].hash == hash)
    {
      return true;
    }
  }
  return false;
}

 
 
 
 
 
size_t message_store::get_other_signers_id_count(const std::vector<uint32_t> &ids) const
{
  size_t count = 0;
  for (size_t i = 1  ; i < ids.size(); ++i)
  {
    if (ids[i] != 0)
    {
      count++;
    }
  }
  return count;
}

 
bool message_store::message_ids_complete(const std::vector<uint32_t> &ids) const
{
  return get_other_signers_id_count(ids) == (ids.size() - 1);
}

void message_store::delete_message(uint32_t id)
{
  delete_transport_message(id);
  size_t index = get_message_index_by_id(id);
  m_messages.erase(m_messages.begin() + index);
}

void message_store::delete_all_messages()
{
  for (size_t i = 0; i < m_messages.size(); ++i)
  {
    delete_transport_message(m_messages[i].id);
  }
  m_messages.clear();
}

 
 
std::string message_store::get_sanitized_text(const std::string &text, size_t max_length)
{
   
  size_t length = std::min(text.length(), max_length);
  std::string sanitized_text = text.substr(0, length);

  try
  {
    sanitized_text = tools::utf8canonical(sanitized_text, [](wint_t c)
    {
      if ((c < 0x20) || (c == 0x7f) || (c >= 0x80 && c <= 0x9f))
      {
         
         
        c = '?';
      }
      else if ((c == '<') || (c == '>'))
      {
         
         
        c = '?';
      }
      return c;
    });
  }
  catch (const std::exception &e)
  {
    sanitized_text = "(Illegal UTF-8 string)";
  }
  return sanitized_text;
}

void message_store::write_to_file(const multisig_wallet_state &state, const std::string &filename)
{
  std::stringstream oss;
  binary_archive<true> ar(oss);
  THROW_WALLET_EXCEPTION_IF(!::serialization::serialize(ar, *this), tools::error::wallet_internal_error, "Failed to serialize MMS state");
  std::string buf = oss.str();

  crypto::chacha_key key;
  crypto::generate_chacha_key(&state.view_secret_key, sizeof(crypto::secret_key), key, 1);

  file_data write_file_data = {};
  write_file_data.magic_string = "MMS";
  write_file_data.file_version = 0;
  write_file_data.iv = crypto::rand<crypto::chacha_iv>();
  std::string encrypted_data;
  encrypted_data.resize(buf.size());
  crypto::chacha20(buf.data(), buf.size(), key, write_file_data.iv, &encrypted_data[0]);
  write_file_data.encrypted_data = encrypted_data;

  std::stringstream file_oss;
  binary_archive<true> file_ar(file_oss);
  THROW_WALLET_EXCEPTION_IF(!::serialization::serialize(file_ar, write_file_data), tools::error::wallet_internal_error, "Failed to serialize MMS state");

  bool success = epee::file_io_utils::save_string_to_file(filename, file_oss.str());
  THROW_WALLET_EXCEPTION_IF(!success, tools::error::file_save_error, filename);
}

void message_store::read_from_file(const multisig_wallet_state &state, const std::string &filename, bool load_deprecated_formats)
{
  boost::system::error_code ignored_ec;
  bool file_exists = boost::filesystem::exists(filename, ignored_ec);
  if (!file_exists)
  {
     
     
    MINFO("No message store file found: " << filename);
    return;
  }

  std::string buf;
  bool success = epee::file_io_utils::load_file_to_string(filename, buf);
  THROW_WALLET_EXCEPTION_IF(!success, tools::error::file_read_error, filename);

  bool loaded = false;
  file_data read_file_data;
  try
  {
    binary_archive<false> ar{epee::strspan<std::uint8_t>(buf)};
    if (::serialization::serialize(ar, read_file_data))
      if (::serialization::check_stream_state(ar))
        loaded = true;
  }
  catch (...) {}
  if (!loaded && load_deprecated_formats)
  {
    try
    {
      std::stringstream iss;
      iss << buf;
      boost::archive::portable_binary_iarchive ar(iss);
      ar >> read_file_data;
      loaded = true;
    }
    catch (const std::exception &e)
    {
      MERROR("MMS file " << filename << " has bad structure <iv,encrypted_data>: " << e.what());
      THROW_WALLET_EXCEPTION_IF(true, tools::error::file_read_error, filename);
    }
  }
  if (!loaded)
  {
    MERROR("MMS file " << filename << " has bad structure <iv,encrypted_data>");
    THROW_WALLET_EXCEPTION_IF(true, tools::error::file_read_error, filename);
  }

  crypto::chacha_key key;
  crypto::generate_chacha_key(&state.view_secret_key, sizeof(crypto::secret_key), key, 1);
  std::string decrypted_data;
  decrypted_data.resize(read_file_data.encrypted_data.size());
  crypto::chacha20(read_file_data.encrypted_data.data(), read_file_data.encrypted_data.size(), key, read_file_data.iv, &decrypted_data[0]);

  loaded = false;
  try
  {
    binary_archive<false> ar{epee::strspan<std::uint8_t>(decrypted_data)};
    if (::serialization::serialize(ar, *this))
      if (::serialization::check_stream_state(ar))
        loaded = true;
  }
  catch(...) {}
  if (!loaded && load_deprecated_formats)
  {
    try
    {
      std::stringstream iss;
      iss << decrypted_data;
      boost::archive::portable_binary_iarchive ar(iss);
      ar >> *this;
      loaded = true;
    }
    catch (const std::exception &e)
    {
      MERROR("MMS file " << filename << " has bad structure: " << e.what());
      THROW_WALLET_EXCEPTION_IF(true, tools::error::file_read_error, filename);
    }
  }
  if (!loaded)
  {
    MERROR("MMS file " << filename << " has bad structure");
    THROW_WALLET_EXCEPTION_IF(true, tools::error::file_read_error, filename);
  }

  m_filename = filename;
}

 
 
 
void message_store::save(const multisig_wallet_state &state)
{
  if (!m_filename.empty())
  {
    write_to_file(state, m_filename);
  }
}

bool message_store::get_processable_messages(const multisig_wallet_state &state,
                                             bool force_sync, std::vector<processing_data> &data_list, std::string &wait_reason)
{
  uint32_t wallet_height = (uint32_t)state.num_transfer_details;
  data_list.clear();
  wait_reason.clear();
   
   
   
   

  std::vector<uint32_t> auto_config_messages(m_num_authorized_signers, 0);
  bool any_auto_config = false;

  for (size_t i = 0; i < m_messages.size(); ++i)
  {
    message &m = m_messages[i];
    if ((m.type == message_type::auto_config_data) && (m.state == message_state::waiting))
    {
      if (auto_config_messages[m.signer_index] == 0)
      {
        auto_config_messages[m.signer_index] = m.id;
        any_auto_config = true;
      }
       
    }
  }

  if (any_auto_config)
  {
    bool auto_config_complete = message_ids_complete(auto_config_messages);
    if (auto_config_complete)
    {
      processing_data data;
      data.processing = message_processing::process_auto_config_data;
      data.message_ids = auto_config_messages;
      data.message_ids.erase(data.message_ids.begin());
      data_list.push_back(data);
      return true;
    }
    else
    {
      wait_reason = tr("Auto-config cannot proceed because auto config data from other signers is not complete");
      return false;
       
       
       
    }
  }

   
  for (size_t i = 0; i < m_messages.size(); ++i)
  {
    message &m = m_messages[i];
    if ((m.type == message_type::signer_config) && (m.state == message_state::waiting))
    {
      processing_data data;
      data.processing = message_processing::process_signer_config;
      data.message_ids.push_back(m.id);
      data_list.push_back(data);
      return true;
    }
  }

   
  if (!signer_config_complete())
  {
    wait_reason = tr("The signer config is not complete.");
    return false;
  }

  if (!state.multisig)
  {
    
    if (!any_message_of_type(message_type::key_set, message_direction::out))
    {
       
       
       
      processing_data data;
      data.processing = message_processing::prepare_multisig;
      data_list.push_back(data);
      return true;
    }

     
     
     
     
     
    std::vector<uint32_t> key_set_messages(m_num_authorized_signers, 0);

    for (size_t i = 0; i < m_messages.size(); ++i)
    {
      message &m = m_messages[i];
      if ((m.type == message_type::key_set) && (m.state == message_state::waiting)
          && (m.round == 0))
      {
        if (key_set_messages[m.signer_index] == 0)
        {
          key_set_messages[m.signer_index] = m.id;
        }
         
      }
    }

    bool key_sets_complete = message_ids_complete(key_set_messages);
    if (key_sets_complete)
    {
       
      processing_data data;
      data.processing = message_processing::make_multisig;
      data.message_ids = key_set_messages;
      data.message_ids.erase(data.message_ids.begin());
      data_list.push_back(data);
      return true;
    }
    else
    {
      wait_reason = tr("Wallet can't go multisig because key sets from other signers are missing or not complete.");
      return false;
    }
  }

  if (state.multisig && !state.multisig_is_ready)
  {
     
     
     
     
     
     
     
    std::vector<uint32_t> additional_key_set_messages(m_num_authorized_signers, 0);

    for (size_t i = 0; i < m_messages.size(); ++i)
    {
      message &m = m_messages[i];
      if ((m.type == message_type::additional_key_set) && (m.state == message_state::waiting)
         && (m.round == state.multisig_rounds_passed))
      {
        if (additional_key_set_messages[m.signer_index] == 0)
        {
          additional_key_set_messages[m.signer_index] = m.id;
        }
         
      }
    }

    bool key_sets_complete = message_ids_complete(additional_key_set_messages);
    if (key_sets_complete)
    {
      processing_data data;
      data.processing = message_processing::exchange_multisig_keys;
      data.message_ids = additional_key_set_messages;
      data.message_ids.erase(data.message_ids.begin());
      data_list.push_back(data);
      return true;
    }
    else
    {
      wait_reason = tr("Wallet can't start another key exchange round because key sets from other signers are missing or not complete.");
      return false;
    }
  }

   
   
   
   
   
   
   
   
   
   
   
   
  if (state.has_multisig_partial_key_images || force_sync)
  {
     
     
     
     
    bool own_sync_data_created = false;
    std::vector<uint32_t> sync_messages(m_num_authorized_signers, 0);
    for (size_t i = 0; i < m_messages.size(); ++i)
    {
      message &m = m_messages[i];
      if ((m.type == message_type::multisig_sync_data) && (force_sync || (m.wallet_height == wallet_height)))
       
       
      {
        if (m.direction == message_direction::out)
        {
          own_sync_data_created = true;
           
        }
        else if ((m.direction == message_direction::in) && (m.state == message_state::waiting))
        {
          if (sync_messages[m.signer_index] == 0)
          {
            sync_messages[m.signer_index] = m.id;
          }
           
        }
      }
    }
    if (!own_sync_data_created)
    {
       
       
      processing_data data;
      data.processing = message_processing::create_sync_data;
      data_list.push_back(data);
      return true;
    }
    uint32_t id_count = (uint32_t)get_other_signers_id_count(sync_messages);
     
    bool all_sync_data = id_count == (m_num_authorized_signers - 1);
     
     
     
    bool enough_sync_data = id_count >= (m_num_required_signers - 1);
    bool sync = false;
    wait_reason = tr("Syncing not done because multisig sync data from other signers are missing or not complete.");
    if (all_sync_data)
    {
      sync = true;
    }
    else if (enough_sync_data)
    {
      if (force_sync)
      {
        sync = true;
      }
      else
      {
         
        wait_reason += (boost::format("\nUse \"mms next sync\" if you want to sync with just %s out of %s authorized signers and transact just with them")
                                     % (m_num_required_signers - 1) % (m_num_authorized_signers - 1)).str();
      }
    }
    if (sync)
    {
      processing_data data;
      data.processing = message_processing::process_sync_data;
      for (size_t i = 0; i < sync_messages.size(); ++i)
      {
        uint32_t id = sync_messages[i];
        if (id != 0)
        {
          data.message_ids.push_back(id);
        }
      }
      data_list.push_back(data);
      return true;
    }
    else
    {
       
      return false;
    }
  }

  bool waiting_found = false;
  bool note_found = false;
  bool sync_data_found = false;
  for (size_t i = 0; i < m_messages.size(); ++i)
  {
    message &m = m_messages[i];
    if (m.state == message_state::waiting)
    {
      waiting_found = true;
      switch (m.type)
      {
      case message_type::fully_signed_tx:
      {
         
        processing_data data;
        data.processing = message_processing::submit_tx;
        data.message_ids.push_back(m.id);
        data_list.push_back(data);

        data.processing = message_processing::send_tx;
        for (uint32_t j = 1; j < m_num_authorized_signers; ++j)
        {
          data.receiving_signer_index = j;
          data_list.push_back(data);
        }
        return true;
      }

      case message_type::partially_signed_tx:
      {
        if (m.signer_index == 0)
        {
           
           
           
           
           
          processing_data data;
          data.processing = message_processing::send_tx;
          data.message_ids.push_back(m.id);
          for (uint32_t j = 1; j < m_num_authorized_signers; ++j)
          {
            data.receiving_signer_index = j;
            data_list.push_back(data);
          }
          return true;
        }
        else
        {
           
           
          processing_data data;
          data.processing = message_processing::sign_tx;
          data.message_ids.push_back(m.id);
          data_list.push_back(data);
          return true;
        }
      }

      case message_type::note:
        note_found = true;
        break;

      case message_type::multisig_sync_data:
        sync_data_found = true;
        break;

      default:
        break;
      }
    }
  }
  if (waiting_found)
  {
    wait_reason = tr("There are waiting messages, but nothing is ready to process under normal circumstances");
    if (sync_data_found)
    {
      wait_reason += tr("\nUse \"mms next sync\" if you want to force processing of the waiting sync data");
    }
    if (note_found)
    {
      wait_reason += tr("\nUse \"mms note\" to display the waiting notes");
    }
  }
  else
  {
    wait_reason = tr("There are no messages waiting to be processed.");
  }

  return false;
}

void message_store::set_messages_processed(const processing_data &data)
{
  for (size_t i = 0; i < data.message_ids.size(); ++i)
  {
    set_message_processed_or_sent(data.message_ids[i]);
  }
}

void message_store::set_message_processed_or_sent(uint32_t id)
{
  message &m = get_message_ref_by_id(id);
  if (m.state == message_state::waiting)
  {
     
     
     
    delete_transport_message(id);
    m.state = message_state::processed;
  }
  else if (m.state == message_state::ready_to_send)
  {
    m.state = message_state::sent;
  }
  m.modified = (uint64_t)time(NULL);
}

void message_store::encrypt(crypto::public_key public_key, const std::string &plaintext,
                            std::string &ciphertext, crypto::public_key &encryption_public_key, crypto::chacha_iv &iv)
{
  crypto::secret_key encryption_secret_key;
  crypto::generate_keys(encryption_public_key, encryption_secret_key);

  crypto::key_derivation derivation;
  bool success = crypto::generate_key_derivation(public_key, encryption_secret_key, derivation);
  THROW_WALLET_EXCEPTION_IF(!success, tools::error::wallet_internal_error, "Failed to generate key derivation for message encryption");

  crypto::chacha_key chacha_key;
  crypto::generate_chacha_key(&derivation, sizeof(derivation), chacha_key, 1);
  iv = crypto::rand<crypto::chacha_iv>();
  ciphertext.resize(plaintext.size());
  crypto::chacha20(plaintext.data(), plaintext.size(), chacha_key, iv, &ciphertext[0]);
}

void message_store::decrypt(const std::string &ciphertext, const crypto::public_key &encryption_public_key, const crypto::chacha_iv &iv,
                            const crypto::secret_key &view_secret_key, std::string &plaintext)
{
  crypto::key_derivation derivation;
  bool success = crypto::generate_key_derivation(encryption_public_key, view_secret_key, derivation);
  THROW_WALLET_EXCEPTION_IF(!success, tools::error::wallet_internal_error, "Failed to generate key derivation for message decryption");
  crypto::chacha_key chacha_key;
  crypto::generate_chacha_key(&derivation, sizeof(derivation), chacha_key, 1);
  plaintext.resize(ciphertext.size());
  crypto::chacha20(ciphertext.data(), ciphertext.size(), chacha_key, iv, &plaintext[0]);
}

void message_store::send_message(const multisig_wallet_state &state, uint32_t id)
{
  message &m = get_message_ref_by_id(id);
  const authorized_signer &me = m_signers[0];
  const authorized_signer &receiver = m_signers[m.signer_index];
  transport_message dm;
  crypto::public_key public_key;

  dm.timestamp = (uint64_t)time(NULL);
  dm.subject = "MMS V0 " + tools::get_human_readable_timestamp(dm.timestamp);
  dm.source_transport_address = me.transport_address;
  dm.source_monero_address = me.monero_address;
  if (m.type == message_type::auto_config_data)
  {
     
     
    public_key = me.auto_config_public_key;
    dm.destination_transport_address = me.auto_config_transport_address;
     
    memset(&dm.destination_monero_address, 0, sizeof(cryptonote::account_public_address));
  }
  else
  {
     
    public_key = receiver.monero_address.m_view_public_key;
    const authorized_signer &receiver = m_signers[m.signer_index];
    dm.destination_monero_address = receiver.monero_address;
    dm.destination_transport_address = receiver.transport_address;
  }
  encrypt(public_key, m.content, dm.content, dm.encryption_public_key, dm.iv);
  dm.type = (uint32_t)m.type;
  dm.hash = crypto::cn_fast_hash(dm.content.data(), dm.content.size());
  dm.round = m.round;

  crypto::generate_signature(dm.hash, me.monero_address.m_view_public_key, state.view_secret_key, dm.signature);

  m_transporter.send_message(dm);

  m.state=message_state::sent;
  m.sent= (uint64_t)time(NULL);
}

bool message_store::check_for_messages(const multisig_wallet_state &state, std::vector<message> &messages)
{
  m_run.store(true, std::memory_order_relaxed);
  const authorized_signer &me = m_signers[0];
  std::vector<std::string> destinations;
  destinations.push_back(me.transport_address);
  for (uint32_t i = 1; i < m_num_authorized_signers; ++i)
  {
    const authorized_signer &m = m_signers[i];
    if (m.auto_config_running)
    {
      destinations.push_back(m.auto_config_transport_address);
    }
  }
  std::vector<transport_message> transport_messages;
  if (!m_transporter.receive_messages(destinations, transport_messages))
  {
    return false;
  }
  if (!m_run.load(std::memory_order_relaxed))
  {
     
     
    return false;
  }

  bool new_messages = false;
  for (size_t i = 0; i < transport_messages.size(); ++i)
  {
    transport_message &rm = transport_messages[i];
    if (any_message_with_hash(rm.hash))
    {
       
    }
    else
    {
      uint32_t sender_index;
      bool take = false;
      message_type type = static_cast<message_type>(rm.type);
      crypto::secret_key decrypt_key = state.view_secret_key;
      if (type == message_type::auto_config_data)
      {
         
         
        for (uint32_t i = 1; i < m_num_authorized_signers; ++i)
        {
          const authorized_signer &m = m_signers[i];
          if (m.auto_config_transport_address == rm.destination_transport_address)
          {
            take = true;
            sender_index = i;
            decrypt_key = m.auto_config_secret_key;
            break;
          }
        }
      }
      else if (type == message_type::signer_config)
      {
         
         
        take = true;
        sender_index = 0;
      }
      else
      {
         
        take = get_signer_index_by_monero_address(rm.source_monero_address, sender_index);
      }
      if (take && (type != message_type::auto_config_data))
      {
         
         
        take = rm.destination_monero_address == me.monero_address;
      }
      if (take)
      {
        crypto::hash actual_hash = crypto::cn_fast_hash(rm.content.data(), rm.content.size());
        THROW_WALLET_EXCEPTION_IF(actual_hash != rm.hash, tools::error::wallet_internal_error, "Message hash mismatch");

        bool signature_valid = crypto::check_signature(actual_hash, rm.source_monero_address.m_view_public_key, rm.signature);
        THROW_WALLET_EXCEPTION_IF(!signature_valid, tools::error::wallet_internal_error, "Message signature not valid");

        std::string plaintext;
        decrypt(rm.content, rm.encryption_public_key, rm.iv, decrypt_key, plaintext);
        size_t index = add_message(state, sender_index, (message_type)rm.type, message_direction::in, plaintext);
        message &m = m_messages[index];
        m.hash = rm.hash;
        m.transport_id = rm.transport_id;
        m.sent = rm.timestamp;
        m.round = rm.round;
        m.signature_count = rm.signature_count;
        messages.push_back(m);
        new_messages = true;
      }
    }
  }
  return new_messages;
}

void message_store::delete_transport_message(uint32_t id)
{
  const message &m = get_message_by_id(id);
  if (!m.transport_id.empty())
  {
    m_transporter.delete_message(m.transport_id);
  }
}

std::string message_store::account_address_to_string(const cryptonote::account_public_address &account_address) const
{
  return get_account_address_as_str(m_nettype, false, account_address);
}

const char* message_store::message_type_to_string(message_type type)
{
  switch (type)
  {
  case message_type::key_set:
    return tr("key set");
  case message_type::additional_key_set:
    return tr("additional key set");
  case message_type::multisig_sync_data:
    return tr("multisig sync data");
  case message_type::partially_signed_tx:
    return tr("partially signed tx");
  case message_type::fully_signed_tx:
    return tr("fully signed tx");
  case message_type::note:
    return tr("note");
  case message_type::signer_config:
    return tr("signer config");
  case message_type::auto_config_data:
    return tr("auto-config data");
  default:
    return tr("unknown message type");
  }
}

const char* message_store::message_direction_to_string(message_direction direction)
{
  switch (direction)
  {
  case message_direction::in:
    return tr("in");
  case message_direction::out:
    return tr("out");
  default:
    return tr("unknown message direction");
  }
}

const char* message_store::message_state_to_string(message_state state)
{
  switch (state)
  {
  case message_state::ready_to_send:
    return tr("ready to send");
  case message_state::sent:
    return tr("sent");
  case message_state::waiting:
    return tr("waiting");
  case message_state::processed:
    return tr("processed");
  case message_state::cancelled:
    return tr("cancelled");
  default:
    return tr("unknown message state");
  }
}

 
 
std::string message_store::signer_to_string(const authorized_signer &signer, uint32_t max_width)
{
  std::string s = "";
  s.reserve(max_width);
  uint32_t avail = max_width;
  uint32_t label_len = signer.label.length();
  if (label_len > avail)
  {
    s.append(signer.label.substr(0, avail - 2));
    s.append("..");
    return s;
  }
  s.append(signer.label);
  avail -= label_len;
  uint32_t transport_addr_len = signer.transport_address.length();
  if ((transport_addr_len > 0) && (avail > 10))
  {
    s.append(": ");
    avail -= 2;
    if (transport_addr_len <= avail)
    {
      s.append(signer.transport_address);
    }
    else
    {
      s.append(signer.transport_address.substr(0, avail-2));
      s.append("..");
    }
  }
  return s;
}

}

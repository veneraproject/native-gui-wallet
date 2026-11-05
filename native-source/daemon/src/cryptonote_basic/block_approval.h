#pragma once
#include "cryptonote_basic.h"

namespace cryptonote {
bool needs_block_approval(const block& b);
crypto::hash block_approval_message(const block& b, network_type nettype);
bool check_block_approval(const block& b, network_type nettype);
bool valid_block_approval_public_key(const crypto::public_key& key);
bool get_block_approval_public_key(crypto::public_key& key);
}

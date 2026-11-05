#pragma once
#include <cstdint>

#ifndef VENERA_BLOCK_APPROVAL_HEIGHT
#error "Configure the future VENERA_BLOCK_APPROVAL_HEIGHT before building"
#endif
#ifndef VENERA_BLOCK_APPROVAL_PUBKEY
#error "Configure VENERA_BLOCK_APPROVAL_PUBKEY before building"
#endif
namespace cryptonote {
constexpr uint64_t BLOCK_APPROVAL_HEIGHT = VENERA_BLOCK_APPROVAL_HEIGHT;
constexpr const char* BLOCK_APPROVAL_PUBLIC_KEY = VENERA_BLOCK_APPROVAL_PUBKEY;
static_assert(BLOCK_APPROVAL_HEIGHT > 1, "Approval must not change genesis");
}

#pragma once
#include <cstdint>
#include "cryptonote_config.h"
#include "reward_burn.h"

#ifndef VENERA_EMISSION_CAP_HEIGHT
#error "Configure VENERA_EMISSION_CAP_HEIGHT to the agreed subsidy-cap activation height"
#endif

namespace cryptonote {
constexpr uint64_t EMISSION_CAP_HEIGHT = VENERA_EMISSION_CAP_HEIGHT;
constexpr uint64_t VENERA_GROSS_DAILY_SUBSIDY_CAP = 10000ULL * 1000000000ULL;
constexpr uint64_t VENERA_TARGET_BLOCKS_PER_DAY = 86400 / DIFFICULTY_TARGET_V2;
constexpr uint64_t VENERA_MAX_BASE_REWARD =
    VENERA_GROSS_DAILY_SUBSIDY_CAP / VENERA_TARGET_BLOCKS_PER_DAY;
static_assert(DIFFICULTY_TARGET_V1 == 60 && DIFFICULTY_TARGET_V2 == 60,
    "The daily subsidy target requires 60-second blocks");
static_assert(EMISSION_CAP_HEIGHT >= VENERA_REWARD_BURN_HEIGHT,
    "The subsidy cap must activate after the 50% reward burn");
static_assert(VENERA_MAX_BASE_REWARD == 6944444444ULL && VENERA_MAX_BASE_REWARD % 2 == 0,
    "The even subsidy cap must preserve the post-burn issuance limit");
static_assert(FINAL_SUBSIDY_PER_MINUTE <= VENERA_MAX_BASE_REWARD,
    "The tail subsidy must not exceed the new cap");
}  

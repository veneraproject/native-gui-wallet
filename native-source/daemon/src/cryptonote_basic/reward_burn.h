#pragma once
#include <cstdint>
#include <limits>

namespace cryptonote {
constexpr uint64_t VENERA_REWARD_BURN_HEIGHT = 572960;

inline bool split_mining_reward(uint64_t height, uint64_t base_reward, uint64_t fee,
                                uint64_t &miner_amount, uint64_t &burned)
{
  if (fee > std::numeric_limits<uint64_t>::max() - base_reward)
    return false;
  const uint64_t total = base_reward + fee;
  burned = height >= VENERA_REWARD_BURN_HEIGHT ? total / 2 : 0;
  miner_amount = total - burned;
  return true;
}
}  

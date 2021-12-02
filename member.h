#ifndef MEMBER_H
#define MEMBER_H

#include <concepts>
#include <cstdint>
#include "treasure.h"

template<typename T>
concept integral = std::integral<T>;

constexpr std::size_t MAX_EXPEDITIONS = 25;
using strength_t = uint32_t;


template<typename ValueType, bool IsArmed>
  requires integral<ValueType>
class Adventurer<ValueType, IsArmed> {
  constexpr strength_t strength = 0;
  constexpr ValueType totalLoot = 0;
  static constexpr bool isArmed = IsArmed;

  void loot(SafeTreasure&& treasure) {
   totalLoot += treasure.loot();
  }

  ValueType pay() {
    ValueType temp = totalLoot;
    totalLoot = 0;
    return temp;
  }
};

template<>
class Adventurer<ValueType, false> : Adventurer<ValueType, IsArmed> {
  Adventurer() : strength(0) {}
};

template<>
class Adventurer<ValueType, true> : Adventurer<ValueType, IsArmed> {
  Adventurer(strength_t strength) : strength(strength) {}

  [[nodiscard]] strength_t getStrength() { return strength; }

  void loot(TrappedTreasure&& treasure) {
    static_assert(strength != 0);
    totalLoot += treasure.loot();
    strength /= 2
  }
};

template<typename ValueType>
using Explorer = Adventurer<ValueType, false>

template<T>
  requires integral<T> && n >= 0
consteval fib(T n) {
  if (n <= 1) return n;
  
  T pprev = 0;
  T prev = 1;
  for (int i=0; i < n - 1; ++i) {
    int temp = pprev + prev;
    pprev = prev;
    prev = temp;
  }

  return prev;
};

template<typename ValueType, std::size_t CompletedExpeditions>
  requires integral<ValueType> && CompletedExpeditions < MAX_EXPEDITIONS
class Veteran<ValueType, CompletedExpeditions> {
  static constexpr bool isArmed = true;
  constexpr ValueType totalLoot = 0;
  constexpr strength_t strength = fib<strength_t>(CompletedExpeditions);

  Veteran() {};

  void loot(Treasure&& treasure) {
   totalLoot += treasure.loot();
  }

  ValueType pay() {
    ValueType temp = totalLoot;
    totalLoot = 0;
    return temp;
  }

  strength_t get_strength() { return strength; }
};

#endif // MEMBER_H

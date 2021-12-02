#ifndef MEMBER_H
#define MEMBER_H

#include <concepts>
#include <cstdint>
#include "treasure.h"

constexpr std::size_t MAX_EXPEDITIONS = 25;
using strength_t = uint32_t;


template<typename ValueType, bool IsArmed>
  requires integral<ValueType> 
class Adventurer {
  strength_t strength = 0;
  ValueType totalLoot = 0;
  static constexpr bool isArmed = IsArmed;

public:
  constexpr Adventurer() requires (!IsArmed) {}

  constexpr Adventurer(strength_t strength) requires IsArmed
    : strength(strength) {}
  
  constexpr strength_t getStrength() { return strength; }

  constexpr void loot(SafeTreasure<ValueType>&& treasure) {
   totalLoot += treasure.loot();
  }

  constexpr void loot(TrappedTreasure<ValueType>&& treasure) requires (strength != 0) {
    totalLoot += treasure.loot();
    strength /= 2;
  }

  constexpr ValueType pay() {
    ValueType temp = totalLoot;
    totalLoot = 0;
    return temp;
  }
};

template<typename ValueType>
using Explorer = Adventurer<ValueType, false>;

template<typename ValueType, std::size_t CompletedExpeditions>
  requires integral<ValueType> && (CompletedExpeditions < MAX_EXPEDITIONS)
class Veteran {
  template<typename T>
    requires integral<T>
  static consteval T fib(int n) {
    // static_assert(n >= 0); // to nie dziala, bo ta funkcja moze dzialac w runtime
    if (n <= 1) return n;
    
    T pprev = 0;
    T prev = 1;
    for (int i=0; i < n - 1; ++i) {
      T temp = pprev + prev;
      pprev = prev;
      prev = temp;
    }

    return prev;
  };

  static constexpr bool isArmed = true;
  ValueType totalLoot = 0;
  strength_t strength = fib<strength_t>(static_cast<int>(CompletedExpeditions));

  Veteran() {};

  void loot(SafeTreasure<ValueType>&& treasure) { totalLoot += treasure.loot(); }
  void loot(TrappedTreasure<ValueType>&& treasure) requires (strength > 0) { totalLoot += treasure.loot(); }

  ValueType pay() {
    ValueType temp = totalLoot;
    totalLoot = 0;
    return temp;
  }

  strength_t get_strength() { return strength; }
};

#endif // MEMBER_H

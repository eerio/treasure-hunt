#ifndef MEMBER_H
#define MEMBER_H

#include <concepts>
#include <cstdint>
#include <typeinfo>
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
  constexpr Adventurer() requires (!IsArmed) = default;

  constexpr explicit Adventurer(strength_t strength) requires IsArmed
    : strength(strength) {}
  
  [[nodiscard]] constexpr strength_t getStrength() const { return strength; }

  // czemu nie sprawdzamy czy typy ValueType są takie same?
  constexpr void loot(SafeTreasure<ValueType>&& treasure) {
   totalLoot += treasure.loot();
  }

  constexpr void loot(TrappedTreasure<ValueType>&& treasure) requires (strength != 0) {
    totalLoot += treasure.loot();
    strength /= 2;
  }

  // czemu nie potrzeba dla zwykłego treasure? :o
  // i też czy wystarczy template<bool IsTrapped> i wtedy zawsze ValueType się będą zgadzać?
  template<typename TreasureValueType, bool IsTrapped>
  constexpr void loot(Treasure<TreasureValueType, IsTrapped> &&treasure) {
     static_assert(typeid(ValueType) == typeid(TreasureValueType));
     if (IsTrapped) {
        if (isArmed && strength > 0) {
            totalLoot += treasure.getLoot();
            strength /= 2;
        }
     } else {
        totalLoot += treasure.getLoot();
     }
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
  static constexpr T fib(int n) { // gdy tu jest consteval zamiast constexpr to hunt_examples.cc narzeka
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

public:
  constexpr Veteran() = default;

  constexpr void loot(SafeTreasure<ValueType>&& treasure) { totalLoot += treasure.loot(); }
  constexpr void loot(TrappedTreasure<ValueType>&& treasure) requires (strength > 0) { totalLoot += treasure.loot(); }

  // tu też nie potrzeba loota ze zwykłym Treasurem?
  template<typename TreasureValueType, bool IsTrapped>
  constexpr void loot(Treasure<ValueType, IsTrapped> &&treasure) {
      static_assert(typeid(ValueType) == typeid(ValueType));
      if (IsTrapped) {
          if (strength > 0) {
              totalLoot += treasure.getLoot();
          }
      } else {
          totalLoot += treasure.getLoot();
      }
  }

  constexpr ValueType pay() {
    ValueType temp = totalLoot;
    totalLoot = 0;
    return temp;
  }

  [[nodiscard]] constexpr strength_t getStrength() const { return strength; }
};

#endif // MEMBER_H

#ifndef MEMBER_H
#define MEMBER_H

#include <concepts>
#include <cstdint>

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
constexpr fib(T n) {
  return n <= 1 ? n : fib(n - 1) + fib(n - 2);
}

template<typename ValueType, std::size_t CompletedExpeditions>
  requires integral<ValueType> && CompletedExpeditions < MAX_EXPEDITIONS
class Veteran<ValueType, CompletedExpeditions> {
  static constexpr bool isArmed = true;
  constexpr strength_t strength = fib(CompletedExpeditions);

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

}

#endif // MEMBER_H

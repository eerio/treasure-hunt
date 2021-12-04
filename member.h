#ifndef MEMBER_H
#define MEMBER_H

#include <concepts>
#include <cstdint>
#include "treasure.h"

constexpr std::size_t MAX_EXPEDITIONS = 25;

template <typename ValueType, bool IsArmed>
requires integral<ValueType>
class Adventurer
{
public:
  using strength_t = uint32_t;

private:
  strength_t strength = 0;
  ValueType totalLoot = 0;

public:
  constexpr Adventurer() requires(!IsArmed) = default;
  static constexpr const bool isArmed = IsArmed;

  constexpr explicit Adventurer(strength_t strength) requires IsArmed
      : strength(strength) {}

  constexpr strength_t getStrength() const requires IsArmed { return this->strength; }

  constexpr void loot(SafeTreasure<ValueType> &&treasure) { this->totalLoot += treasure.getLoot(); }
  constexpr void loot(TrappedTreasure<ValueType> &&treasure)
  {
    if (this->strength > 0)
    {
      this->totalLoot += treasure.getLoot();
      this->strength /= 2;
    }
  }

  constexpr ValueType pay()
  {
    ValueType temp = this->totalLoot;
    this->totalLoot = 0;
    return temp;
  }
};

template <typename ValueType>
using Explorer = Adventurer<ValueType, false>;

template <typename ValueType, std::size_t CompletedExpeditions>
requires integral<ValueType> &&(CompletedExpeditions < MAX_EXPEDITIONS)
class Veteran
{
public:
  using strength_t = uint32_t;

private:
  template <typename T>
  requires integral<T>
  static constexpr T fib(int n)
  {
    if (n <= 1) { return n; }

    T pprev = 0;
    T prev = 1;
    for (int i = 0; i < n - 1; ++i)
    {
      T temp = pprev + prev;
      pprev = prev;
      prev = temp;
    }

    return prev;
  };

  ValueType totalLoot = 0;
  strength_t strength = fib<strength_t>(static_cast<int>(CompletedExpeditions));

public:
  constexpr Veteran() = default;
  static constexpr const bool isArmed = true;

  constexpr void loot(SafeTreasure<ValueType> &&treasure)
  {
    this->totalLoot += treasure.getLoot();
  }

  constexpr void loot(TrappedTreasure<ValueType> &&treasure)
  {
    if (this->strength > 0)
      this->totalLoot += treasure.getLoot();
  }

  constexpr ValueType pay()
  {
    ValueType temp = this->totalLoot;
    this->totalLoot = 0;
    return temp;
  }

  [[nodiscard]] constexpr strength_t getStrength() const { return this->strength; }
};

#endif // MEMBER_H

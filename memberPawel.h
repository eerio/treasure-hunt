#include <concepts>

template<typename T>
concept integral = std::integral<T>;


template<typename ValueType, bool IsArmed>
  requires integral<ValueType>
class Member<ValueType, IsArmed> {
  ValueType kaska = 0; // = 0 jest okej? nie znamy typu
  static isArmed;

  ValueType pay() {
    ValueType temp = kaska;
    kaska = 0;
    return temp;
  }
}


class Adventurer<ValueType, IsArmed> : Member<ValueType, IsArmed> {
  void loot(SafeTreasure&& treasure) {
    kaska += treasure.loot();
  }
};

class Adventurer<ValueType, false> : Adventurer<ValueType, IsArmed> {
  Adventurer() {}
};

class Adventurer<ValueType, true> : Adventurer<ValueType, IsArmed> {
  Adventurer(int strength) {}

  int getStrength();

  void loot(TrappedTreasure&& treasure) {
    // REQUIRE strength > 0
    kaska += treasure.loot();
    strength /= 2
  }
}

using Explorer<ValueType> = Adventurer<ValueType, false>


class Veteran
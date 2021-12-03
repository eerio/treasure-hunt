#ifndef TREASURE_H
#define TREASURE_H

#include <concepts>

template<typename T>
concept integral = std::integral<T>;

template<typename ValueType, bool IsTrapped>
  requires integral<ValueType>
class Treasure {
    // dlaczego to nie musi być private?
    // pola sa domyslnie private
  ValueType value;

public:
  static constexpr bool isTrapped = IsTrapped;

  constexpr explicit Treasure(ValueType value) : value(value) {};

  constexpr ValueType evaluate() { return value; }

  constexpr ValueType getLoot() {
      ValueType temp = value;
      value = 0;
      return temp;
  }
};

template <typename ValueType>
using SafeTreasure = Treasure<ValueType, false>;

template <typename ValueType>
using TrappedTreasure = Treasure<ValueType, true>;

#endif // TREASURE_H

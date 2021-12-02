#ifndef TREASURE_H
#define TREASURE_H

#include <concepts>

template<typename T>
concept integral = std::integral<T>;

template<typename ValueType, bool IsTrapped>
  requires integral<ValueType>
class Treasure {

private:
  ValueType value;

public:
  explicit Treasure(ValueType value) : value(value) {};

  ValueType evaluate() { return value; }

  ValueType getLoot() {
      ValueType temp = value;
      value = 0; // =0? czy to jest ok dla nieznanego typu?
      return temp;
  }

  bool isTrapped = IsTrapped; // imo to powinno być static ale kompilator nie pozwala nie wiem czemu :/
};

/* to mi się nie kompiluje :(
template<typename ValueType>
  requires integral<ValueType>
class Treasure<ValueType, false> : Treasure<ValueType, false> { }; */

template <typename ValueType> // może z tym jakoś nie wiem??
using SafeTreasure = Treasure<ValueType, false>;

template <typename ValueType>
using TrappedTreasure = Treasure<ValueType, true>;

#endif // TREASURE_H
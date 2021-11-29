#include <concepts>

template<typename T>
concept integral = std::integral<T>;

template<typename ValueType, bool IsTrapped>
  requires integral<ValueType>
class Treasure {
  ValueType value;

  Treasure(ValueType value) : value(value);

  ValueType evaluate() { return value; }

  ValueType getLoot() { return value; value = 0; } // =0? czy to jest ok dla nieznanego typu?

  bool isTrapped;
};

template<typename ValueType>
  // requires integral<ValueType>
class Treasure<ValueType, false> : Treasure<ValueType, false> { };

using SafeTreasure = Treasure<ValueType, false>;
using TrappedTreasure<ValueType> = Treasure<ValueType, true>;
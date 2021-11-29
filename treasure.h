#include <concepts>

template<typename T>
concept integral = std::integral<T>;

template<typename ValueType, bool IsTrapped>
  requires integral<ValueType>
class Treasure {
};
#ifndef TREASURE_HUNT_H
#define TREASURE_HUNT_H

#include <type_traits>
#include "member.h"

template<typename T>
concept WithStaticField = requires () {
     { [] () constexpr { return T::field; }() };
};

template <typename T>
concept EncounterSide = requires (T t) { { T } -> Treasure; };

template <typename sideA, typename sideB>
  requires EncounterSide<sideA> && EncounterSide<sideB>
class Encounter {
public:
  const sideA& a;
  const sideB& b;
};


// constexpr void run(Encounter&& ... encounter) {

// }

// constexpr void expedition(Encounter e1, e2, ...);

#endif // TREASURE_HUNT_H

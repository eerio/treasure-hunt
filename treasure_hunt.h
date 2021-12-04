#ifndef TREASURE_HUNT_H
#define TREASURE_HUNT_H

#include <concepts>
#include <type_traits>
#include <algorithm>
#include "member.h"
#include "treasure.h" // go chyba nawet nie trzeba bo member.h go includuje

template <typename T>
concept TreasureConcept = requires (T t) {
    { Treasure{t} } -> std::same_as<T>;
};

// trzeba poprawić ten koncept, bo coś z weteranami struggluje
template <typename T>
concept MemberConcept = std::same_as<T, T>;
// concept MemberConcept = requires (T t) {
//     // { T::strength_t } -> std::integral;
// };

template<typename T>
concept WithStaticField = requires () {
     { [] () constexpr { return T::field; }() };
};

template <typename T>
concept EncounterSide = TreasureConcept<T> || MemberConcept<T>;

// to powodowało porażkę soloHunt()
/*
template <typename sideA, typename sideB>
  requires EncounterSide<sideA> && EncounterSide<sideB>
class Encounter {
public:
  const sideA& a;
  const sideB& b;
}; */
template <EncounterSide sideA, EncounterSide sideB>
using Encounter = std::pair<sideA&, sideB&>;

template <TreasureConcept A, MemberConcept B>
constexpr void run(Encounter<A, B> encounter) {
    //encounter.b.loot(std::move(encounter.a));
    encounter.second.loot(std::move(encounter.first));
}

template <MemberConcept A, TreasureConcept B>
constexpr void run(Encounter<A, B> encounter) {
    //encounter.a.loot(std::move(encounter.b));
    encounter.first.loot(std::move(encounter.second));
}

template <MemberConcept A, MemberConcept B>
requires (!(A::isArmed)) && (!(B::isArmed))
constexpr void run(Encounter<A, B> encounter) {}

template <MemberConcept A, MemberConcept B>
requires A::isArmed && B::isArmed
constexpr void run(Encounter<A, B> encounter) {
    if (encounter.a.getStrength() > encounter.b.getStrength()) {
        //encounter.a.loot(SafeTreasure<decltype(encounter.b.pay())>(encounter.b.pay()));
        encounter.first.loot(SafeTreasure<decltype(encounter.second.pay())>(encounter.second.pay()));
    } else if (encounter.b.getStrength() > encounter.a.getStrength()) {
        //encounter.b.loot(SafeTreasure<decltype(encounter.a.pay())>(encounter.a.pay()));
        encounter.second.loot(SafeTreasure<decltype(encounter.first.pay())>(encounter.first.pay()));
    }
}

template <MemberConcept A, MemberConcept B>
requires A::isArmed && (!(B::isArmed))
constexpr void run(Encounter<A, B> encounter) {
    //encounter.a.loot(SafeTreasure<decltype(encounter.b.pay())>(encounter.b.pay()));
    encounter.first.loot(SafeTreasure<decltype(encounter.second.pay())>(encounter.second.pay()));
}

template <MemberConcept A, MemberConcept B>
requires (!(A::isArmed)) && B::isArmed
constexpr void run(Encounter<A, B> encounter) {
    //encounter.b.loot(SafeTreasure<decltype(encounter.a.pay())>(encounter.a.pay()));
    encounter.second.loot(SafeTreasure<decltype(encounter.first.pay())>(encounter.first.pay()));
}

// https://en.cppreference.com/w/cpp/language/fold
// tam na dole jest przyklad printer() ktory robi to wlasnie tak, wiec powinno dzialac kiedys
template <typename ...Ts>
constexpr void expedition(Ts&&... encounters) {
    // https://stackoverflow.com/questions/34314193/iterating-over-different-types
    (run(encounters), ...);
}
// działa<3

#endif // TREASURE_HUNT_H
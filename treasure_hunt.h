#ifndef TREASURE_HUNT_H
#define TREASURE_HUNT_H

#include <concepts>
#include <type_traits>
#include <algorithm>
#include "member.h"
#include "treasure.h"

template <typename T>
concept TreasureConcept = requires (T t) {
    { Treasure{t} } -> std::same_as<T>;
};

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

template <EncounterSide sideA, EncounterSide sideB>
using Encounter = std::pair<sideA&, sideB&>;

template <TreasureConcept A, MemberConcept B>
constexpr void run(Encounter<A, B> encounter) {
    encounter.second.loot(std::move(encounter.first));
}

template <MemberConcept A, TreasureConcept B>
constexpr void run(Encounter<A, B> encounter) {
    encounter.first.loot(std::move(encounter.second));
}

template <MemberConcept A, MemberConcept B>
requires (!(A::isArmed)) && (!(B::isArmed))
constexpr void run(Encounter<A, B> encounter) {}

template <MemberConcept A, MemberConcept B>
requires A::isArmed && B::isArmed
constexpr void run(Encounter<A, B> encounter) {
    if (encounter.first.getStrength() > encounter.second.getStrength()) {
        encounter.first.loot(SafeTreasure<decltype(encounter.second.pay())>(encounter.second.pay()));
    } else if (encounter.second.getStrength() > encounter.first.getStrength()) {
        encounter.second.loot(SafeTreasure<decltype(encounter.first.pay())>(encounter.first.pay()));
    }
}

template <MemberConcept A, MemberConcept B>
requires A::isArmed && (!(B::isArmed))
constexpr void run(Encounter<A, B> encounter) {
    encounter.first.loot(SafeTreasure<decltype(encounter.second.pay())>(encounter.second.pay()));
}

template <MemberConcept A, MemberConcept B>
requires (!(A::isArmed)) && B::isArmed
constexpr void run(Encounter<A, B> encounter) {
    encounter.second.loot(SafeTreasure<decltype(encounter.first.pay())>(encounter.first.pay()));
}


// https://en.cppreference.com/w/cpp/language/fold
// tam na dole jest przyklad printer() ktory robi to wlasnie tak, wiec powinno dzialac kiedys
template <typename ...Ts>
constexpr void expedition(Ts&&... encounters) {
    // https://stackoverflow.com/questions/34314193/iterating-over-different-types
    (run(encounters), ...);
}

#endif // TREASURE_HUNT_H

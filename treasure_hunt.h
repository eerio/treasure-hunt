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
constexpr void run(Encounter<A, B> encounter) {
    // po prostu się rozchodzą xd
}

template <MemberConcept A, MemberConcept B>
requires A::isArmed && B::isArmed
constexpr void run(Encounter<A, B> encounter) {
    // pojedynek, wygrywa go uczestnik o większej sile i zabiera on cały zebrany skarb
    // przegranemu, w przypadku remisu, gdy uczestnicy mają równe siły, nic się nie dzieje
    if (encounter.first.getStrength() > encounter.second.getStrength()) {
        // a wygrywa, można stworzyć safe treasure z majątkiem b i a go zlootuje
        // ale coś się nie da xdd
        encounter.first.loot(SafeTreasure(encounter.second.pay()));
    } else if (encounter.second.getStrength() > encounter.first.getStrength()) {
        // b wygrywa, jw
        encounter.second.loot(SafeTreasure(encounter.first.pay()));
    } else {
        //remis
    }
}

template <MemberConcept A, MemberConcept B>
requires A::isArmed && (!(B::isArmed))
constexpr void run(Encounter<A, B> encounter) {
    encounter.first.loot(SafeTreasure(encounter.second.pay()));
}

template <MemberConcept A, MemberConcept B>
requires (!(A::isArmed)) && B::isArmed
constexpr void run(Encounter<A, B> encounter) {
    encounter.second.loot(SafeTreasure(encounter.first.pay()));
}


// https://en.cppreference.com/w/cpp/language/fold
// tam na dole jest przyklad printer() ktory robi to wlasnie tak, wiec powinno dzialac kiedys
template <typename ...Ts>
constexpr void expedition(Ts&&... encounters) {
    // https://stackoverflow.com/questions/34314193/iterating-over-different-types
    (run(encounters), ...);
}

#endif // TREASURE_HUNT_H

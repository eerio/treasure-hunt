#ifndef TREASURE_HUNT_H
#define TREASURE_HUNT_H

#include <type_traits>
#include <algorithm>
#include "member.h"

template <typename T>
concept TreasureConcept = requires (T t) {
    { Treasure(t) } -> std::same_as<T>;
};

template <typename T>
concept MemberConcept = requires (T t) {
    // nie wiem co tu powinno być F
};

template<typename T>
concept WithStaticField = requires () {
     { [] () constexpr { return T::field; }() };
};

template <typename T>
concept EncounterSide = TreasureConcept<T> || MemberConcept<T>;

template <typename sideA, typename sideB>
  requires EncounterSide<sideA> && EncounterSide<sideB>
class Encounter {
public:
  const sideA& a;
  const sideB& b;
};

template <TreasureConcept A, MemberConcept B>
constexpr void run(Encounter<A, B> encounter) {
    encounter.a.loot(std::move(encounter.b)); // na grupie pisali żeby dać move xd
}

template <MemberConcept A, TreasureConcept B>
constexpr void run(Encounter<A, B> encounter) {
    encounter.b.loot(std::move(encounter.a));
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
    if (encounter.a.getStrength() > encounter.b.getStrength()) {
        // a wygrywa, można stworzyć safe treasure z majątkiem b i a go zlootuje
        // ale coś się nie da xdd
        encounter.a.loot(SafeTreasure(encounter.b.pay()));
    } else if (encounter.b.getStrength() > encounter.a.getStrength()) {
        // b wygrywa, jw
        encounter.b.loot(SafeTreasure(encounter.a.pay()));
    } else {
        //remis
    }
}

template <MemberConcept A, MemberConcept B>
requires A::isArmed && (!(B::isArmed))
constexpr void run(Encounter<A, B> encounter) {
    encounter.a.loot(SafeTreasure(encounter.b.pay()));
}

template <MemberConcept A, MemberConcept B>
requires (!(A::isArmed)) && B::isArmed
constexpr void run(Encounter<A, B> encounter) {
    encounter.b.loot(SafeTreasure(encounter.a.pay()));
}

// constexpr void run(Encounter&& ... encounter) {

// }

// constexpr void expedition(Encounter e1, e2, ...);

#endif // TREASURE_HUNT_H

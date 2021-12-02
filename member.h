#ifndef MEMBER_H
#define MEMBER_H

#include "treasure.h"
#include <typeinfo>

using strength_t = unsigned int; // nie wiem czy dobry typ
static const int MAX_EXPEDITIONS = 25;

template<typename ValueType, bool IsArmed>
  requires integral<ValueType>
class Adventurer<ValueType, IsArmed> {

private:
    strength_t strength = 0;
    ValueType total_loot = 0;

public:
    Adventurer() : strength(0) {
        static_assert(!isArmed);
    }

    explicit Adventurer(strength_t strength) : strength(strength) {
        static_assert(isArmed);
        static_assert(strength > 0);
    }

    [[nodiscard]] strength_t getStrength() const {
        return strength;
    }

    bool isArmed = IsArmed; // to też powinno być static :(

    void loot(Treasure<ValueType, IsTrapped> &&treasure) {
        static_assert(typeid(ValueType) == typeid(ValueType));
        if (IsTrapped) {
            if (isArmed && strength > 0) {
                total_loot += treasure.getLoot();
                strength /= 2;
            }
        } else {
            total_loot += treasure.getLoot();
        }
    }

    ValueType pay() {
        ValueType temp = total_loot;
        total_loot = 0;
        return temp;
    }
};

template<typename ValueType, std::size_t CompletedExpeditions>
  requires integral<ValueType> && (CompletedExpeditions < MAX_EXPEDITIONS)
class Veteran<ValueType, CompletedExpeditions>{
    strength_t strength; // liczba fibonacciego
    ValueType total_loot = 0;

    Veteran() = default;

    bool isArmed = true; // to też static :(

    void loot(Treasure<ValueType, IsTrapped> &&treasure) {
        static_assert(typeid(ValueType) == typeid(ValueType));
        if (IsTrapped) {
            if (isArmed && strength > 0) {
                total_loot += treasure.getLoot();
            }
        } else {
            total_loot += treasure.getLoot();
        }
    }

    ValueType pay() {
        ValueType temp = total_loot;
        total_loot = 0;
        return temp;
    }

    strength_t getStrength() {
        return strength;
    }

};

#endif //MEMBER_H
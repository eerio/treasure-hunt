#ifndef MEMBER_H
#define MEMBER_H

#include "treasure.h"
#include <concepts>

template<typename T>
concept integral = std::integral<T>;

using strength_t = unsigned int; // ?
static const int MAX_EXPEDITIONS = 25;

template<typename ValueType, bool IsArmed>
  requires integral<ValueType>
class Adventurer<ValueType, IsArmed> {
    strength_t strength = 0;
    ValueType total_loot = 0;

    Adventurer() = default;

    Adventurer(strength_t strength) : strength(strength) {
        static_assert(armed);
        static_assert(strength > 0);
    }

    strength_t getStrength() const {
        return strength;
    }

    static bool isArmed = IsArmed;

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

template<typename ValueType, size_t CompletedExpeditions>
  requires integral<ValueType> && CompletedExpeditions < MAX_EXPEDITIONS
class Veteran<ValueType, CompletedExpeditions>{
    strength_t strength; // liczba fibanocciego
    ValueType total_loot = 0;

    Veteran() = default;

    static bool isArmed = true;

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
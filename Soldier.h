#ifndef SOLDIER_H
#define SOLDIER_H
#include "Character.h"

namespace mtm
{
    static const int soldier_step = 3;
    static const int soldier_ammo_refill = 3;
    static constexpr double secondary_damage_devide = 3.0;
    static const char lower_case_soldier = 's';
    static const char upper_case_soldier = 'S';
    class Soldier : public Character
    {
    public:
        // constructs a new soldier using the Character's constructor.
        Soldier(units_t health, units_t ammo, units_t range, units_t power, Team team);
        Soldier(const Soldier &other_soldier) = default;
        ~Soldier() = default;
        Soldier &operator=(const Soldier &other) = default;
        /**
        * attack - Written in character.h, Make dmg equal to the soldier 'power' to the target(if there is a chracter in the GridPoint) 
        *   and make half of that dmg to the area close to it by maximum the sodier range/3.
        * @param dest - target coordinates.
        * @param src - soldier coordinates.
        * @param game_board - Game Board.
        *
        * @return void.
        * @possible_Exceptions - Illegaltarget - if the target is not in the same row or column us the attacker.
        */
        void attack(const GridPoint &dest, const GridPoint &src,
                    std::vector<std::vector<std::shared_ptr<Character>>> &game_board) override;

        // reloads 3 ammunition points to the soldier.
        void reloadAmmo() override;

        // returns the sodlier step limit per turn (3 steps).
        units_t getMovementRange() const override;

        // return a new shared pointer with a new sniper which is a clone of our sniper(this).
        std::shared_ptr<Character> clone() const override;
        /**
         * Letter - override function.
         * @return A 'S' if the element team is 'POWERLIFTERS' and 's' if the element team is 'CROSSFITTERS'.
         */
        char Letter() const override;
    };
}

#endif
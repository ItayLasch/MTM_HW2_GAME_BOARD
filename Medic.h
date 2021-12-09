#ifndef MEDIC_H
#define MEDIC_H
#include "Character.h"

namespace mtm
{
    static const int medic_step = 5;
    static const int medic_ammo_refill = 5;
    static const char lower_case_medic = 'm';
    static const char upper_case_medic = 'M';
    class Medic: public Character
    {
    public:
        
        // constructs a new medic using the Character's constructor
        Medic(const units_t health, const units_t ammo, const units_t range, const units_t power, const Team team);
        Medic(const Medic &other_medic) = default;
        ~Medic() = default;
        Medic &operator=(const Medic &other) = default;

        
        /**
         * @brief checks if the medic has enough ammo for an attack, no need for ammo if the 
         * target is from the same team as he.
         * @param target - target we need to check it's type to determine our need for ammunition.
         * @return true - if medic has ammo for an attack or if target is from same team , false - otherwise.
         */
        bool checkAmmoStatus(std::shared_ptr<Character> target) const override;

        /**
         * @brief Written in character.h, if target in destination coordinates is from same team 
         * as medic it's health points will heal.
         * @param dest - target coordinates.
         * @param src - medic coordinates.
         * @param game_board - game_board we play in.
         * @return (void)
         * @possible_Exceptions - Illegaltarget - if the target coordinates are empty or if medic tries to heal himslef.
         */
        void attack(const GridPoint &dest, const GridPoint &src,
                    std::vector<std::vector<std::shared_ptr<Character>>> &game_board) override;
        
        // reloads 5 ammunition points to the medic.
        void reloadAmmo() override;

        // returns the medic step limit per turn (5 steps).
        units_t getMovementRange() const override;

        // return a new shared pointer with a new medic which is a clone of our medic(this).
        std::shared_ptr<Character> clone() const override;

        // return 'M' if medic is from POWERLIFTERS team or 'm' if he is from CROSSFITTERS team.
        char Letter() const override;
    };
}

#endif
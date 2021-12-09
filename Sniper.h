#ifndef SNIPER_H
#define SNIPER_H
#include "Character.h"


namespace mtm
{
    static const int sniper_step = 4;
    static const int sniper_ammo_refill = 2;
    static const int third = 3;
    static const int double_damage_modifier = 2;
    static const char lower_case_sniper = 'n';
    static const char upper_case_sniper = 'N';
    class Sniper: public Character
    {
        int hit_counter;
    public:
        // constructs a new sniper using the Character's constructor.
        Sniper(units_t health, units_t ammo, units_t range, units_t power, Team team);
        Sniper(const Sniper &other_sniper) = default;
        ~Sniper() = default;
        Sniper &operator=(const Sniper &other) = default;

        /**
         *@brief Written in character.h, in every third attack of the sniper it's attack will
         * double in power. 
         * @param dest - target coordinates.
         * @param src - sniper coordinates.
         * @param game_board - game_board we play in.
         * @return (void)
         * @possible_Exceptions - Illegaltarget - if the target coordinates are empty or if it's team is same as
         * the sniper team.
         */
        void attack(const GridPoint &dest, const GridPoint &src,
                    std::vector<std::vector<std::shared_ptr<Character>>> &game_board) override;

        // reloads 2 ammunition points to the sniper.
        void reloadAmmo() override;

        // returns the sniper step limit per turn (4 steps).
        units_t getMovementRange() const override;

        // return a new shared pointer with a new sniper which is a clone of our sniper(this).
        std::shared_ptr<Character> clone() const override;

        /**
         * @brief checks if the target is within the sniper's range and no less than upper round of the range's half.
         * @param source - sniper coordinates.
         * @param dest - target coordinates.
         * @return true - if the target is within range boundries, false - otherwise.
         */
        bool inAttackRange(const GridPoint &source, const GridPoint &dest) const override;

        // return 'N' if sniper is from POWERLIFTERS team or 'n' if he is from CROSSFITTERS team.
        char Letter() const override;
    };
}

#endif
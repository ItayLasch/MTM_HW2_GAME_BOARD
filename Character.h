#ifndef CHARACTER_H
#define CHARACTER_H
#include <iostream>
#include "Auxiliaries.h"
#include "Exceptions.h"
#include <memory>
#include <vector>
#include <cmath>

namespace mtm
{
    class Character
    {
    protected:
        int health;
        int ammo;
        int range;
        int power;
        Team team;
        static constexpr double range_modifier = 2.0;

    public:
        /**
         * @brief Constructs a new chracter with the given arguments.
         * @param health - character's health
         * @param ammo - character's ammo
         * @param range -character's attack range
         * @param power - character's power of attack
         * @param team - character's team
         */
        Character(const units_t health, const units_t ammo, const units_t range, const units_t power,const Team team);

        /**
        * Copy Constractor - default function.
        * @brief Constructs a copy of the character that given as parameter.
        * @param other_character - The other character to make copy of himself.
        */
        Character(const Character &other_character) = default;

        /**
        * destractur - default function.
        * @brief Deallocates the character and all the data he contains.
        */
        virtual ~Character() =default;

        /**
        * operator= - default function.
        * @brief delelt the existing character and constructs a copy of the character that given as parameter.
        * @param other - The other character to make copy of himself.
        */
        Character &operator=(const Character &other) = default;


        /**
         * takeDmg - Recieves amount of damage and takes it of the Character's health.
         * @param dmg - Amount of damage recieved.
         * @return (void)
         */
        void takeDmg(const int dmg);


        /**
         * isAlive - Checks if the character is alive - if it's health is higher than zero.
         * @return true - if character is alive, false- otherwise.
         */
        bool isAlive() const;


        /**
         * attack - A virtual function, recieves coordinates for attacker and target,and the board.
         * If all the limitations for the attack according to the rules of the game and the specific
         * attacker are okay, it launches an attack on the target coordinates. 
         * @param dest - Target Coordinates.
         * @param src - Attacker Coordinates.
         * @param game_board - Game board.
         * @return (void)
         */
        virtual void attack(const GridPoint &dest, const GridPoint &src, 
                            std::vector<std::vector<std::shared_ptr<Character>>> &game_board) = 0;


        /**
         * canMove - Recieves source and destination coordinates, and the character step limit, 
         * @param source - Source Coordinates.
         * @param dest - Destination Coordinates.
         * @param range - Character step limit. 
         * @return true - if character can move between coordinates, false - otherwise.
         */
        bool canMove(const GridPoint &source, const GridPoint &dest, const units_t range) const;

        /**
         * inAttackRange - Recieves source and destination coordinates. Checks if the target is within attacker range.
         * @param source - Attacker coordinates.
         * @param dest - Target coordinates.
         * @return true - if target is within range, false - otherwise.
         */
        virtual bool inAttackRange(const GridPoint &source, const GridPoint &dest) const;

        /**
         * checkAmmoStatus - Checks if the Attacker has ammo for the attack.
         * @param target - A pointer to the target character (for the medic check)
         * @return 
         */
        virtual bool checkAmmoStatus(std::shared_ptr<Character> target) const;

        /**
         * reloadAmmo - virtual function. Reloads character's ammo according to it's reload rule.
         * @return (void)
         */
        virtual void reloadAmmo() = 0;

        /**
         * getMovementRange - virtual function. 
         * @return Returns character's step limit.
         */
        virtual units_t getMovementRange() const = 0;

        /**
         * getAttackRange  
         * @return Character's attack range.
         */
        units_t getAttackRange() const;

        /**
         * getAmmo 
         * @return Character's amount of ammo.
         */
        units_t getAmmo() const;

        /**
         * getTeam 
         * @return Character's team - CROSSFITERS\POWERLIFTERS.
         */
        Team getTeam() const;

        /**
         * Letter - virtual function. 
         * @return A lowercase/uppercase letter according to the type and team of the character.
         */
        virtual char Letter() const = 0;


        virtual std::shared_ptr<Character> clone() const = 0;
    };
}

#endif
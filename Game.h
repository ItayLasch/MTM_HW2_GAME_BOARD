#ifndef GAME_H
#define GAME_H
#include "Character.h"
#include "Soldier.h"
#include "Medic.h"
#include "Sniper.h"
#include <memory>
#include <vector>
namespace mtm
{
    class Game
    {
        int height;
        int width;
        std::vector<std::vector<std::shared_ptr<Character>>> game_board;

    public:
        /**
         * @brief Constructor which we use to create a new game with a game board.
         * @param height - height of board
         * @param width - width of board
         * @possible_Exceptions - IllegalArgument - if one of the input arguments is invalid according to the rules given.
         */
        Game(int const height, int const width);

        /**
         * @brief Copies another game and constructs a new one with a copy of the other's elements.
         * @param other - another game we want to copy to construct a new game.
         */
        Game(const Game &other);
        ~Game() = default;

        /**
         * @brief Copies and implements another game elements to an existing game.
         * @param other - another game we want to copy to an existing game.
         * @return this after it has changed.
         */
        Game &operator=(const Game &other);

        /**
         * addCharacter - Recieves coordinates on the board and a pointer to a character and 
         * adds it to the board. 
         * @param coordinates - Coordinates on the game board.
         * @param character - A pointer to the character we want to add.
         * @return (void)
         * @possible_Exceptions - IllegalCell - if the input coordinates are not inside the board.
         *                        OcuppiedCell - if there is already another character in the cell the coordinates
         *                                       are pointing to.
         */
        void addCharacter(const GridPoint &coordinates, std::shared_ptr<Character> character);

        /**
         * makeCharacter - Recieves elements to identify a certain character and creates a pointer for it.
         * @param type - If the Character is a Soldier/Medic/Sniper.
         * @param team - CROSSLIFTERS or POWERLIFTERS.
         * @param health - Amount of health of the character.
         * @param ammo - How much ammo the character has.
         * @param range - The character Attack range.
         * @param power - How much power the character has.
         * @return Returns a shared pointer to a specific character according to it's type.
         * @possible_Exceptions - IllegalArgument - if one of the input arguments is invalid according to the rules given.
         */
        static std::shared_ptr<Character> makeCharacter(CharacterType type, Team team,
                                                    units_t health, units_t ammo, units_t range, units_t power);

        /**
         * move - Recieves source coordinates and destination coordinates, if there is a character in the source
         * coordinates we move it to the destination coordinates, depending on its step limit. 
         * @param src_coordinates - Source Coordinates.
         * @param dst_coordinates - Destination Coordinates.
         * @return (void)
         * @possible_Exceptions - IllegalCell - if the input coordinates are not inside the board.
         *                        CellEmpty - if there is no character to move in the input coordinates.
         *                        OcuppiedCell - if there is already another character(or itself) in the cell the coordinates
         *                                       are pointing to.
         *                        MoveToFar - if we try to move a character an amount of cells that is higher than its step limit
         *                                    according to it's type.
         */
        void move(const GridPoint &src_coordinates, const GridPoint &dst_coordinates);

        /**
         * attack - Recieves source coordinates and destination coordinates, if there is a character in the source
         * coordinates we attack the destination coordinates, depending on the attacker limitations.  
         * @param src_coordinates - Source Coordinates.
         * @param dst_coordinates - Destination Coordinates.
         * @return (void)
         * @possible_Exceptions - IllegalCell - if the input coordinates are not inside the board.
         *                        CellEmpty - if there is no character to attack with in the input coordinates.
         *                        OutOfRange - if the target is out of the character attack range.
         *                        OutOfAmmo - if the character has no ammo to attack it's target with, differs according to the
         *                                    attacker type.
         *                        IllegalTarget - if the attacker can't attack it's given target because of limitations on it's type.
         */
        void attack(const GridPoint &src_coordinates, const GridPoint &dst_coordinates);

        /**
         * reload - Recieves cooridnates on the board, if there is a character there we reload it's ammo.
         * @param coordinates - Coordinates on the board.
         * @return (void)
         * @possible_Exceptions - IllegalCell - if the input coordinates are not inside the board.
         *                        CellEmpty - if there is no character to reload it's ammo in the input coordinates.
         */
        void reload(const GridPoint &coordinates);


        /**
         * @brief output operator which prints the board.
         * @param os - ostream variable the we insert the output to.
         * @param game - the game we want to print it's board.
         * @return the given os with the output in it.
         */
        friend std::ostream &operator<<(std::ostream &os, const Game &game);


        /**
         * isOver - Recieves a pointer from Team Type and checks if there is only one team in the board.
         * If so it will put the winning Team Type int the pointer.
         * @param winningTeam - A pointer from Team Type which holds the Type of the winners.
         * @return true - if there is a winning team, false - otherwise.
         */
        bool isOver(Team *winningTeam = NULL) const;
    };
}
#endif
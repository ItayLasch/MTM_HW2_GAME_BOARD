#include "Game.h"

namespace mtm
{
    using std::shared_ptr;
    using std::string;
    using std::vector;
    Game::Game(int const height, int const width)
    {
        if (height <= 0 || width <= 0)
        {
            throw IllegalArgument();
        }

        this->height = height;
        this->width = width;
        this->game_board = vector<vector<shared_ptr<Character>>>(height, 
                                                vector<shared_ptr<Character>>(width, nullptr));
    }

    Game::Game(const Game &other)
    {
        height = other.height;
        width = other.width;
        game_board = vector<vector<shared_ptr<Character>>>(height, vector<shared_ptr<Character>>(width, nullptr));
        for (int i = 0; i < (int)other.game_board.size(); i++)
        {
            for (int j = 0; j < (int)other.game_board[i].size(); j++)
            {
                shared_ptr<Character> tmp = other.game_board.at(i).at(j);
                if (tmp)
                {
                    this->game_board.at(i).at(j) = tmp->clone();
                }
            }
        }
    }

    Game &Game::operator=(const Game &other)
    {
        if (this == &other)
        {
            return *this;
        }

        vector<vector<shared_ptr<Character>>> tmp_board =
            vector<vector<shared_ptr<Character>>>(other.height, vector<shared_ptr<Character>>(other.width, nullptr));

        for (int i = 0; i < (int)other.game_board.size(); i++)
        {
            for (int j = 0; j < (int)other.game_board.at(i).size(); j++)
            {
                if (other.game_board.at(i).at(j) != nullptr)
                {
                    std::shared_ptr<Character> new_character(other.game_board.at(i).at(j)->clone());
                    tmp_board.at(i).at(j) = new_character;
                }
            }
        }

        this->height = other.height;
        this->width = other.width;
        this->game_board = tmp_board;
        return *this;
    }

    /**
     *  checkCoordinates - Recieves coordinates,the hieght and width of the board. 
     * @param coordinates - Certain coordinates
     * @param height - Game board height
     * @param width - Game board width
     * @return true - if coordinates are within board borders, false - otherwise.
     */
    static bool checkCoordinates(GridPoint coordinates, int height, int width)
    {
        if (coordinates.row < 0 || coordinates.row >= height || coordinates.col < 0 || coordinates.col >= width)
        {
            return false;
        }

        return true;
    }

    void Game::addCharacter(const GridPoint &coordinates, std::shared_ptr<Character> character)
    {
        if (checkCoordinates(coordinates, this->height, this->width) == false)
        {
            throw IllegalCell();
        }

        if (this->game_board.at(coordinates.row).at(coordinates.col) != nullptr)
        {
            throw CellOccupied();
        }

        this->game_board.at(coordinates.row).at(coordinates.col) = character;
    }

    std::shared_ptr<Character> Game::makeCharacter(CharacterType type, Team team,
                                                   units_t health, units_t ammo, units_t range, units_t power)
    {
        if (health <= 0 || ammo < 0 || range < 0 || power < 0)
        {
            throw IllegalArgument();
        }

        if (type == SOLDIER)
        {
            return shared_ptr<Character>(new Soldier(health, ammo, range, power, team));
        }
        else if (type == SNIPER)
        {
            return shared_ptr<Character>(new Sniper(health, ammo, range, power, team));
        }

        return shared_ptr<Character>(new Medic(health, ammo, range, power, team));
    }

    void Game::move(const GridPoint &src_coordinates, const GridPoint &dst_coordinates)
    {
        if (!checkCoordinates(src_coordinates, height, width) || !checkCoordinates(dst_coordinates, height, width))
        {
            throw IllegalCell();
        }

        if ((game_board.at(src_coordinates.row).at(src_coordinates.col)) == nullptr)
        {
            throw CellEmpty();
        }

        if (src_coordinates == dst_coordinates)
        {
            throw CellOccupied();
        }

        units_t range_of_movement = game_board.at(src_coordinates.row).at(src_coordinates.col)->getMovementRange();
        if (!(game_board.at(src_coordinates.row).at(src_coordinates.col)->
                        canMove(src_coordinates, dst_coordinates, range_of_movement)))
        {
            throw MoveTooFar();
        }

        if (game_board.at(dst_coordinates.row).at(dst_coordinates.col) != nullptr)
        {
            throw CellOccupied();
        }

        game_board.at(dst_coordinates.row).at(dst_coordinates.col) =
            game_board.at(src_coordinates.row).at(src_coordinates.col);
        game_board.at(src_coordinates.row).at(src_coordinates.col) = nullptr;
    }

    void Game::attack(const GridPoint &src_coordinates, const GridPoint &dst_coordinates)
    {
        if (!checkCoordinates(src_coordinates, height, width) || !checkCoordinates(dst_coordinates, height, width))
        {
            throw IllegalCell();
        }

        shared_ptr<Character> champion = game_board.at(src_coordinates.row).at(src_coordinates.col);
        if (champion == nullptr)
        {
            throw CellEmpty();
        }

        if(champion->inAttackRange(src_coordinates,dst_coordinates) == false){
            throw OutOfRange();
        }

        if (champion->checkAmmoStatus(game_board.at(dst_coordinates.row).at(dst_coordinates.col)) == false)
        {
            throw OutOfAmmo();
        }
        champion->attack(dst_coordinates, src_coordinates, game_board);
    }

    void Game::reload(const GridPoint &coordinates)
    {
        if (!checkCoordinates(coordinates, height, width))
        {
            throw IllegalCell();
        }

        shared_ptr<Character> champion = game_board.at(coordinates.row).at(coordinates.col);
        if (champion == nullptr)
        {
            throw CellEmpty();
        }

        champion->reloadAmmo();
        champion = nullptr;
    }

    std::ostream &operator<<(std::ostream &os, const Game &game)
    {
        char *print_game = new char[game.height * game.width];
        int char_index = 0;
        for (int i = 0; i < (int)game.game_board.size(); ++i)
        {
            for (int j = 0; j < (int)game.game_board.at(i).size(); j++)
            {
                if (game.game_board.at(i).at(j) == nullptr)
                {
                    print_game[char_index++] = ' ';
                    continue;
                }

                print_game[char_index++] = game.game_board.at(i).at(j)->Letter();
            }
        }

        printGameBoard(os, print_game, print_game + (game.height * game.width), game.width);
        delete[] print_game;
        return os;
    }

    bool Game::isOver(Team *winningTeam) const
    {
        int crossfitters_counter = 0, powerlifters_counter = 0;
        for (int i = 0; i < (int)this->game_board.size(); ++i)
        {
            for (int j = 0; j < (int)this->game_board.at(i).size(); j++)
            {
                if (this->game_board.at(i).at(j) == nullptr)
                {
                    continue;
                }
                Team character_team = this->game_board.at(i).at(j)->getTeam();

                if (character_team == POWERLIFTERS)
                {
                    powerlifters_counter++;
                }
                else
                {
                    crossfitters_counter++;
                }
            }
        }

        if ((crossfitters_counter != 0 && powerlifters_counter != 0) || 
                    (crossfitters_counter == 0 && powerlifters_counter == 0))
        {
            return false;
        }
        if (winningTeam != NULL)
        {
            if (crossfitters_counter == 0)
            {
                *winningTeam = POWERLIFTERS;
            }
            else
            {
                *winningTeam = CROSSFITTERS;
            }
        }

        return true;
    }
}
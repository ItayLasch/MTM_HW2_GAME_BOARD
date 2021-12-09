#include "Soldier.h"

namespace mtm
{
    using std::shared_ptr;
    using std::vector;

    Soldier::Soldier(units_t health, units_t ammo, units_t range, units_t power, Team team) : 
            Character(health, ammo, range, power, team)
    {
    }

    void Soldier::reloadAmmo()
    {
        this->ammo += soldier_ammo_refill;
    }

    units_t Soldier::getMovementRange() const
    {
        return soldier_step;
    }

    /**
     * attackSquare - Attacks a specific coordinate on the game board.  
     * @param dest - Target square coordinates.
     * @param game_board - Game board.
     * @param dmg - Amount of damage the target recieves.
     * @param attacker_team - the Team of the attacker.
     * @return (void)
     */
    static void attackSquare(const GridPoint &dest,
                             vector<vector<shared_ptr<Character>>> &game_board, double dmg, units_t attacker_team)
    {
        shared_ptr<Character> target = game_board.at(dest.row).at(dest.col);
        if (target == nullptr)
        {
            return;
        }
        if (target->getTeam() == attacker_team)
        {
            return;
        }

        target->takeDmg(dmg);
        if (!target->isAlive())
        {
            game_board.at(dest.row).at(dest.col).reset();
        }
    }

    void Soldier::attack(const GridPoint &dest, const GridPoint &src,
                         vector<vector<shared_ptr<Character>>> &game_board)
    {
        if (dest.row != src.row && dest.col != src.col)
        {
            throw IllegalTarget();
        }
        attackSquare(dest, game_board, this->power, this->team); 
        ammo--;
        units_t secondary_dmg_range = ceil((double)this->range / secondary_damage_devide);
        units_t dmg = ceil((double)this->power / range_modifier);
        for (int i = 0; i < (int)game_board.size(); ++i)
        {
            for (int j = 0; j < (int)game_board.at(i).size(); j++)
            {
                GridPoint tmp_dest(i, j);
                if (tmp_dest == dest)
                {
                    continue;
                }

                if (GridPoint::distance(dest, tmp_dest) <= secondary_dmg_range)
                {
                    attackSquare(tmp_dest, game_board, dmg, this->team); 
                }
            }
        }
    }

    shared_ptr<Character> Soldier::clone() const
    {
        return shared_ptr<Character>(new Soldier(*this));
    }

    char Soldier::Letter() const
    {
        if (this->team == POWERLIFTERS)
        {
            return upper_case_soldier;
        }

        return lower_case_soldier;
    }
}
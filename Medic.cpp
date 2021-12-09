#include "Medic.h"

namespace mtm
{
    using std::shared_ptr;
    using std::vector;
    static const int negative = -1;
    Medic::Medic(const units_t health, const units_t ammo, const units_t range, const units_t power, const Team team): 
                Character(health, ammo, range, power, team)
    {
    }

    void Medic::reloadAmmo()
    {
        this->ammo += medic_ammo_refill;
    }

    units_t Medic::getMovementRange() const
    {
        return medic_step;
    }

    bool Medic::checkAmmoStatus(std::shared_ptr<Character> target) const
    {
        if(target != nullptr)
        {
            if(this->team != target->getTeam()){
                if(this->ammo == 0)
                {
                    return false;
                }
            }
            else
            {
                return true;
            }
        }

        if(this->ammo == 0){
            return false;
        }

        return true;
    }

    void Medic::attack(const GridPoint &dest, const GridPoint &src,
                       vector<vector<shared_ptr<Character>>> &game_board)
    {
        if (dest == src)
        { 
            throw IllegalTarget();
        }
        int target_ally = 1;
        shared_ptr<Character> target = game_board.at(dest.row).at(dest.col);
        if (target == nullptr)
        {
            throw IllegalTarget();
        }

        if (this->team == game_board.at(dest.row).at(dest.col)->getTeam())
        {
            ammo++;
            target_ally = negative;
        }
        ammo--;

        target->takeDmg(this->power * target_ally);
        if (!target->isAlive())
        {
            game_board.at(dest.row).at(dest.col).reset();
        }
    }

    shared_ptr<Character> Medic::clone() const
    {
        return shared_ptr<Character>(new Medic(*this));
    }

    char Medic::Letter() const
    {
        if (this->team == POWERLIFTERS)
        {
            return upper_case_medic;
        }

        return lower_case_medic;
    }
}

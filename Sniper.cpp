#include "Sniper.h"

namespace mtm
{
    using std::shared_ptr;
    using std::vector;

    Sniper::Sniper(const units_t health, const units_t ammo, units_t range, units_t power, Team team)
        : Character(health, ammo, range, power, team)
    {
        hit_counter = 1;
    }

    void Sniper::reloadAmmo()
    {
        this->ammo += sniper_ammo_refill;
    }

    units_t Sniper::getMovementRange() const
    {
        return sniper_step;
    }

    bool Sniper::inAttackRange(const GridPoint &source, const GridPoint &dest) const
    {
        units_t distance = GridPoint::distance(source, dest);
        if (distance > range || distance < ceil((double)range / range_modifier))
        {
            return false;
        }
        return true;
    }

    void Sniper::attack(const GridPoint &dest, const GridPoint &src,
                        vector<vector<shared_ptr<Character>>> &game_board)
    {
        int attack_modifier = 1;
        if (hit_counter % third == 0)
        {
            attack_modifier = double_damage_modifier;
        }
        shared_ptr<Character> target = game_board.at(dest.row).at(dest.col);
        if (target == nullptr || target->getTeam() == this->getTeam())
        {
            throw IllegalTarget();
        }
        target->takeDmg(this->power * attack_modifier);
        if (!target->isAlive())
        {
            game_board.at(dest.row).at(dest.col).reset();
        }
        ammo--;
        hit_counter++;
    }

    shared_ptr<Character> Sniper::clone() const
    {
        return shared_ptr<Character>(new Sniper(*this));
    }

    char Sniper::Letter() const
    {
        if (this->team == POWERLIFTERS)
        {
            return upper_case_sniper;
        }

        return lower_case_sniper;
    }
}

#include "Character.h"
namespace mtm
{
    using std::shared_ptr;
    using std::vector;
    Character::Character(const units_t health, const units_t ammo, const units_t range, const units_t power,
             const Team team): health(health),ammo(ammo),range(range),power(power),team(team)
    {
    }

    void Character::takeDmg(const int dmg){
        this->health -= dmg;
    }

    bool Character::isAlive() const{
        if(this->health <= 0){
            return false;
        }
        return true;
    }

    bool Character::canMove(const GridPoint &source, const GridPoint &dest, const units_t range) const
    {
        if (GridPoint::distance(source, dest) > range)
        {
            return false;
        }

        return true;
    }

    bool Character::inAttackRange(const GridPoint &source, const GridPoint &dest) const{
        if (GridPoint::distance(source, dest) > this->range)
        {
            return false;
        }

        return true;
    }

    bool Character::checkAmmoStatus(std::shared_ptr<Character> target) const{
        if(this->ammo == 0)
        {
            return false;
        }

        return true;
    }

    units_t Character::getAttackRange() const
    {
        return this->range;
    }

    units_t Character::getAmmo() const
    {
        return this->ammo;
    }

    Team Character::getTeam() const
    {
        return this->team;
    }
}

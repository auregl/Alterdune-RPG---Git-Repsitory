#include "Entity.h"
#include <algorithm>
using namespace std;


Entity::Entity(const string& name, int hp, int atk, int def)
    : name(name), hp(hp), hpMax(hp), atk(atk), def(def) {}

string Entity::getName()  const { return name; }
int         Entity::getHp()    const { return hp; }
int         Entity::getHpMax() const { return hpMax; }
int         Entity::getAtk()   const { return atk; }
int         Entity::getDef()   const { return def; }

bool Entity::isAlive() const {
    return hp > 0;
}

void Entity::takeDamage(int dmg) {
    hp = max(0, hp - dmg);
}

#pragma once
#include <string>
using namespace std;


// Classe abstraite commune au joueur et aux monstres
class Entity {
protected:
    string name;
    int hp;
    int hpMax;
    int atk;
    int def;

public:
    Entity(const string& name, int hp, int atk, int def);
    virtual ~Entity() = default;

    string getName()  const;
    int         getHp()    const;
    int         getHpMax() const;
    int         getAtk()   const;
    int         getDef()   const;

    bool isAlive()              const;
    void takeDamage(int dmg);
};

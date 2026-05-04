#pragma once
#include "Player.h"
#include "Monster.h"
#include "ActCatalog.h"
using namespace std;


class Combat {
private:
    Player&     player;
    Monster&    monster;
    ActCatalog& catalog;

    int bonusAtkTour;   // bonus ATK actif uniquement ce tour (Arme)
    int bonusDefTour;   // bonus DEF% actif uniquement ce tour (Equipement)

public:
    Combat(Player& player, Monster& monster, ActCatalog& catalog);

    bool run();

private:
    bool playerTurn();
    bool monsterTurn();
    bool fight();
    void doAct();
    void useItem();
    bool doMercy();
    int  calcDamage(int maxHp);
    void showStatus() const;
    void handleDrops();     // roll les drops et les transfère au joueur
};
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
 
    int   bonusAtkTour;
    int   bonusDefTour;
 
public:
    Combat(Player& player, Monster& monster, ActCatalog& catalog);
 
    bool run();
 
private:
    bool  playerTurn();
    void  monsterTurn();
    bool  fight();
    void  doAct();
    void  menuItem();
    bool  doMercy();
    int   calcDamage(int maxHp);
    float calcMultiplier();
    void  showStatus() const;
};
#pragma once
#include "Player.h"
#include "Monster.h"
#include "ActCatalog.h"
#include "FileLoader.h"
using namespace std;


class Combat {
private:
    Player&           player;
    Monster&          monster;
    ActCatalog&       catalog;
    const ItemPools&  itemPools;   // référence aux pools globaux (non-owning)

    int bonusAtkTour;
    int bonusDefTour;

public:
    Combat(Player& player, Monster& monster, ActCatalog& catalog,
           const ItemPools& itemPools);

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
    void handleDrops();
};
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
    // Retourne true si le combat est termine (victoire ou fuite)
    // Retourne false si le tour du joueur est termine normalement
    // Retourne false aussi si le joueur utilise un item (le tour continue)
    bool playerTurn();
    bool monsterTurn();
    bool fight();
    void doAct();
    void menuItem();    // renomme pour clarifier : ouvre le menu item sans terminer le tour
    bool doMercy();
    int  calcDamage(int maxHp);
    float calcMultiplier();
    void showStatus() const;
    void handleDrops();
};
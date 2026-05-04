#pragma once
#include "Player.h"
#include "Bestiary.h"
#include "ActCatalog.h"
#include "Monster.h"
#include "FileLoader.h"
#include <vector>
using namespace std;


class Game {
private:
    Player               player;
    vector<Monster*>     monsters;
    Bestiary             bestiary;
    ActCatalog           catalog;
    ItemPools            itemPools;   // pools globaux pour les drops

public:
    Game();
    ~Game();

    void start();

    const ItemPools& getItemPools() const { return itemPools; }

private:
    void mainMenu();
    void clearScreen() const;
    void displayMenu() const;
    void startCombat();
    void showBestiary();
    void showStats();
    void showItems();
    bool checkEndGame() const;
    void displayEnding() const;
};
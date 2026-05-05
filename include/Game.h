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
    Player           player;
    vector<Monster*> monsters;
    Bestiary         bestiary;
    ActCatalog       catalog;

public:
    Game();
    ~Game();

    void start();

private:
    void mainMenu();
    void displayMenu() const;
    void startCombat();
    void showBestiary();
    void showStats();
    void showItems();
    bool checkEndGame() const;
    void displayEnding() const;
};

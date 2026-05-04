#pragma once
#include "Player.h"
#include "Bestiary.h"
#include "ActCatalog.h"
#include "Monster.h"
#include <vector>
using namespace std;


class Game {
private:
    Player               player;
    vector<Monster*> monsters;
    Bestiary             bestiary;
    ActCatalog           catalog;

public:
    Game();
    ~Game(); // libère les monstres alloués dynamiquement

    void start();

private:
    void mainMenu();
    void clearScreen() const;
    void displayMenu() const;
    void startCombat();
    void showBestiary();
    void showStats();
    void showItems();
    bool checkEndGame() const; // true si 10 victoires atteintes
    void displayEnding() const;
};

#pragma once
#include "Player.h"
#include "Monster.h"
#include <vector>
#include <string>
using namespace std;

class FileLoader {
public:
    // Charge les items depuis le CSV et attribue 3 potions, 2 armes, 2 armures
    // aleatoires directement au joueur.
    static void loadItems(const string& path, Player& player);

    static vector<Monster*> loadMonsters(const string& path);

private:
    static Utilisable* parseItemLine(const string& line);
    static Monster*    parseMonsterLine(const string& line);
};

#pragma once
#include "Player.h"
#include "Monster.h"
#include <vector>
#include <string>
using namespace std;

class FileLoader {
public:
    static void loadItems(const string& path, Player& player);
    static vector<Monster*> loadMonsters(const string& path);

private:
    static Utilisable* parseItemLine(const string& line);   // retourne Potion/Arme/Equipement
    static Monster*    parseMonsterLine(const string& line);
};

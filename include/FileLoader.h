#pragma once
#include "Player.h"
#include "Monster.h"
#include <vector>
#include <string>
using namespace std;


struct ItemPools {
    vector<Utilisable*> heals;
    vector<Utilisable*> weapons;
    vector<Utilisable*> armors;
};

class FileLoader {
public:
    // Charge tous les items, attribue le kit de départ au joueur,
    // et retourne les 3 pools complets (Game en prend ownership).
    static ItemPools loadItems(const string& path, Player& player);

    static vector<Monster*> loadMonsters(const string& path);

private:
    static Utilisable* parseItemLine(const string& line);
    static Monster*    parseMonsterLine(const string& line);
};
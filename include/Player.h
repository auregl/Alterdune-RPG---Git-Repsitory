#pragma once
#include "Entity.h"
#include "Item.h"
#include <vector>
using namespace std;

class Player : public Entity {
private:
    int kills;
    int spared;
    vector<Utilisable*> inventory;

public:
    Player(const string& name);
    ~Player();

    int getKills()  const;
    int getSpared() const;

    void addKill();
    void addSpare();

    void addItem(Utilisable* item);

    // bonusAtk et bonusDef sont modifies directement par Combat (option B)
    bool useItem(int index, Player& player, int& bonusAtk, int& bonusDef);

    void displayInventory() const;
    void displayStats()     const;
};

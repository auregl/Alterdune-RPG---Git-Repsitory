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

    Arme*       armeEquipee;
    Equipement* armureEquipee;

public:
    Player(const string& name);
    ~Player();

    int getKills()  const;
    int getSpared() const;

    void addKill();
    void addSpare();

    void addItem(Utilisable* item);

    void equiperArme(int index, int& bonusAtk);
    void equiperArmure(int index, int& bonusDef);
    bool utiliserPotion(int index);
    bool useItem(int index, int& bonusAtk, int& bonusDef);

    // Remet le slot a nullptr apres utilisation (l'item reste dans l'inventaire)
    void desequiperArme();
    void desequiperArmure();

    void displayInventory() const;
    void displayStats()     const;

    Arme*       getArmeEquipee()      const { return armeEquipee; }
    Equipement* getArmureEquipee()    const { return armureEquipee; }
    int         getEquippedAtkBonus() const;
    int         getEquippedDefBonus() const;

    string getItemType(int index) const {
        if (index < 0 || index >= (int)inventory.size()) return "";
        return inventory[index]->getType();
    }
};

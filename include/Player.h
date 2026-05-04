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

    Arme*       armeEquipee;    // slot unique : 1 arme à la fois
    Equipement* armureEquipee;  // slot unique : 1 armure à la fois

public:
    Player(const string& name);
    ~Player();

    int getKills()  const;
    int getSpared() const;

    void addKill();
    void addSpare();

    void addItem(Utilisable* item);

    // Equipe une arme (remplace l'existante, remet l'ancienne en inventaire)
    void equiperArme(int index, int& bonusAtk);
    // Equipe une armure (remplace l'existante, remet l'ancienne en inventaire)
    void equiperArmure(int index, int& bonusDef);
    // Utilise une potion de l'inventaire
    bool utiliserPotion(int index);

    // Pour l'affichage de l'inventaire + slots equipes
    void displayInventory() const;
    void displayStats()     const;

    // Acces aux slots equipes (pour Combat)
    Arme*       getArmeEquipee()    const { return armeEquipee; }
    Equipement* getArmureEquipee()  const { return armureEquipee; }

    // Retourne le type de l'item a l'index donne ("HEAL", "WEAPON", "ARMOR", "" si invalide)
    string getItemType(int index) const {
        if (index < 0 || index >= (int)inventory.size()) return "";
        return inventory[index]->getType();
    }
};

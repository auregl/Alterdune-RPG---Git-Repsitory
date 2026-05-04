#include "Player.h"
#include <iostream>
#include <algorithm>
using namespace std;

Player::Player(const string& name)
    : Entity(name, 100, 10, 5), kills(0), spared(0) {}

Player::~Player() {
    for (Utilisable* u : inventory) delete u;
}

int Player::getKills()  const { return kills; }
int Player::getSpared() const { return spared; }

void Player::addKill()  { ++kills; }
void Player::addSpare() { ++spared; }

void Player::addItem(Utilisable* item) {
    inventory.push_back(item);
}

// ─── Utilisation d'un item ────────────────────────────────────────────────────
// bonusAtk et bonusDef sont les champs "ce tour" de Combat, passes par reference
bool Player::useItem(int index, Player& player, int& bonusAtk, int& bonusDef) {
    if (index < 0 || index >= (int)inventory.size()) {
        cout << "Index invalide.\n";
        return false;
    }

    Utilisable* u = inventory[index];

    if (!u->estDisponible()) {
        cout << u->getName() << " est epuise ou hors d'usage !\n";
        return false;
    }

    // Potion : soin immediat
    if (Potion* p = dynamic_cast<Potion*>(u)) {
        p->utiliser();
        int soin = min(p->getValeurSoin(), player.hpMax - player.hp);
        player.hp += soin;
        cout << "Vous utilisez " << p->getName()
             << " et recuperez " << soin << " HP. "
             << "(HP : " << player.hp << "/" << player.hpMax << ")\n";
        return true;
    }

    // Arme : ajoute le bonus ATK pour ce tour uniquement
    if (Arme* a = dynamic_cast<Arme*>(u)) {
        a->utiliser();
        bonusAtk += a->getBonusAtk();
        cout << "Vous brandissez " << a->getName()
             << " ! (ATK +" << a->getBonusAtk()
             << " ce tour | Durabilite : " << a->getDurabilite() << " restante)\n";
        return true;
    }

    // Equipement : ajoute le bonus DEF% pour ce tour uniquement
    if (Equipement* e = dynamic_cast<Equipement*>(u)) {
        e->utiliser();
        bonusDef += e->getBonusDefPct();
        cout << "Vous activez " << e->getName()
             << " ! (DEF +" << e->getBonusDefPct()
             << "% ce tour | Usure : " << e->getUsure()
             << "/" << e->getSeuilMax() << ")\n";
        return true;
    }

    return false;
}

void Player::displayInventory() const {
    cout << "=== Inventaire ===\n";
    if (inventory.empty()) { cout << "(vide)\n"; return; }
    for (int i = 0; i < (int)inventory.size(); ++i) {
        cout << "[" << i << "] ";
        inventory[i]->afficherDetails();
    }
}

void Player::displayStats() const {
    cout << "=== Statistiques ===\n";
    cout << "Nom      : " << name   << "\n";
    cout << "HP       : " << hp     << "/" << hpMax << "\n";
    cout << "ATK base : " << atk    << "\n";
    cout << "DEF base : " << def    << "\n";
    cout << "Tues     : " << kills  << "\n";
    cout << "Epargnees: " << spared << "\n";
    cout << "Victoires: " << (kills + spared) << "/10\n";
}

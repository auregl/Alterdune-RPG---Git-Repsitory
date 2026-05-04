#include "Player.h"
#include <iostream>
#include <algorithm>
using namespace std;

Player::Player(const string& name)
    : Entity(name, 100, 10, 5), kills(0), spared(0),
      armeEquipee(nullptr), armureEquipee(nullptr) {}

Player::~Player() {
    for (Utilisable* u : inventory) delete u;
    // armeEquipee et armureEquipee sont dans l'inventaire, deja supprimes
}

int Player::getKills()  const { return kills; }
int Player::getSpared() const { return spared; }

void Player::addKill()  { ++kills; }
void Player::addSpare() { ++spared; }

void Player::addItem(Utilisable* item) {
    inventory.push_back(item);
}

// ─── Equipe une arme depuis l'inventaire ──────────────────────────────────────
// Le bonus ATK est applique pour le tour en cours (passe par reference depuis Combat)
void Player::equiperArme(int index, int& bonusAtk) {
    if (index < 0 || index >= (int)inventory.size()) {
        cout << "Index invalide.\n";
        return;
    }

    Arme* a = dynamic_cast<Arme*>(inventory[index]);
    if (!a) {
        cout << "Cet item n'est pas une arme.\n";
        return;
    }
    if (!a->estDisponible()) {
        cout << a->getName() << " est hors d'usage !\n";
        return;
    }

    // Si une arme etait deja equipee, on la remet en inventaire (elle n'est pas perdue)
    if (armeEquipee) {
        cout << "[" << armeEquipee->getName() << " desequipee et remise en inventaire.]\n";
    }

    armeEquipee = a;
    a->utiliser(); // consomme 1 charge de durabilite
    bonusAtk = a->getBonusAtk(); // remplace (pas de cumul)

    cout << "Vous brandissez " << a->getName()
         << " ! (ATK +" << a->getBonusAtk()
         << " ce tour | Durabilite : " << a->getDurabilite()
         << "/" << a->getDurabilite() + 1 << ")\n"; // +1 car deja decremente
}

// ─── Equipe une armure depuis l'inventaire ────────────────────────────────────
void Player::equiperArmure(int index, int& bonusDef) {
    if (index < 0 || index >= (int)inventory.size()) {
        cout << "Index invalide.\n";
        return;
    }

    Equipement* e = dynamic_cast<Equipement*>(inventory[index]);
    if (!e) {
        cout << "Cet item n'est pas une armure.\n";
        return;
    }
    if (!e->estDisponible()) {
        cout << e->getName() << " est hors d'usage !\n";
        return;
    }

    if (armureEquipee) {
        cout << "[" << armureEquipee->getName() << " desequipee et remise en inventaire.]\n";
    }

    armureEquipee = e;
    e->utiliser(); // consomme de l'usure
    bonusDef = e->getBonusDefPct(); // remplace (pas de cumul)

    cout << "Vous activez " << e->getName()
         << " ! (DEF +" << e->getBonusDefPct()
         << "% ce tour | Usure : " << e->getUsure()
         << "/" << e->getSeuilMax() << ")\n";
}

// ─── Utilise une potion de l'inventaire ──────────────────────────────────────
bool Player::utiliserPotion(int index) {
    if (index < 0 || index >= (int)inventory.size()) {
        cout << "Index invalide.\n";
        return false;
    }

    Potion* p = dynamic_cast<Potion*>(inventory[index]);
    if (!p) {
        cout << "Cet item n'est pas une potion. Choisissez une potion pour soigner.\n";
        return false;
    }
    if (!p->estDisponible()) {
        cout << p->getName() << " est epuisee !\n";
        return false;
    }

    p->utiliser();
    int soin = min(p->getValeurSoin(), hpMax - hp);
    hp += soin;
    cout << "Vous utilisez " << p->getName()
         << " et recuperez " << soin << " HP. "
         << "(HP : " << hp << "/" << hpMax << ")\n";
    return true;
}

// ─── Affichage inventaire ─────────────────────────────────────────────────────
void Player::displayInventory() const {
    cout << "=== Inventaire ===\n";
    if (inventory.empty()) { cout << "(vide)\n"; return; }
    for (int i = 0; i < (int)inventory.size(); ++i) {
        cout << "[" << i << "] ";
        inventory[i]->afficherDetails();
    }
    if (armeEquipee)
        cout << "  [Arme equipee ce tour : " << armeEquipee->getName() << "]\n";
    if (armureEquipee)
        cout << "  [Armure equipee ce tour : " << armureEquipee->getName() << "]\n";
}

// ─── Affichage stats ──────────────────────────────────────────────────────────
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

#include "Player.h"
#include <iostream>
#include <algorithm>
using namespace std;

Player::Player(const string& name)
    : Entity(name, 100, 10, 5), kills(0), spared(0),
      armeEquipee(nullptr), armureEquipee(nullptr) {}

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

// ─── Equipe une arme ─────────────────────────────────────────────────────────
// Ne consomme PAS la durabilite : c'est fight() qui le fait au moment de l'attaque.
void Player::equiperArme(int index, int& bonusAtk) {
    bonusAtk = 0;

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

    if (armeEquipee && armeEquipee != a)
        cout << "[" << armeEquipee->getName() << " desequipee.]\n";

    armeEquipee = a;
    bonusAtk    = a->getBonusAtk();

    cout << "Vous equipez " << a->getName()
         << " (ATK +" << a->getBonusAtk()
         << " | Durabilite : " << a->getDurabilite() << ")\n";
}

// ─── Equipe une armure ───────────────────────────────────────────────────────
// Ne consomme PAS l'usure : c'est monsterTurn() qui le fait quand elle absorbe.
void Player::equiperArmure(int index, int& bonusDef) {
    bonusDef = 0;

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

    if (armureEquipee && armureEquipee != e)
        cout << "[" << armureEquipee->getName() << " desequipee.]\n";

    armureEquipee = e;
    bonusDef      = e->getBonusDefPct();

    cout << "Vous equipez " << e->getName()
         << " (DEF +" << e->getBonusDefPct()
         << "% | Usure : " << e->getUsure()
         << "/" << e->getSeuilMax() << ")\n";
}

// ─── Desequipe apres utilisation (slot -> nullptr, item reste en inventaire) ─
void Player::desequiperArme()   { armeEquipee   = nullptr; }
void Player::desequiperArmure() { armureEquipee = nullptr; }

// ─── Utilise une potion ──────────────────────────────────────────────────────
bool Player::utiliserPotion(int index) {
    if (index < 0 || index >= (int)inventory.size()) {
        cout << "Index invalide.\n";
        return false;
    }
    Potion* p = dynamic_cast<Potion*>(inventory[index]);
    if (!p) {
        cout << "Cet item n'est pas une potion.\n";
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

// ─── Routeur generique (utilise par Game::showItems et Combat::menuItem) ─────
bool Player::useItem(int index, int& bonusAtk, int& bonusDef) {
    bonusAtk = 0;
    bonusDef = 0;

    string type = getItemType(index);
    if (type == "WEAPON") { equiperArme(index, bonusAtk);   return true; }
    if (type == "ARMOR")  { equiperArmure(index, bonusDef); return true; }
    if (type == "HEAL")   { return utiliserPotion(index); }

    cout << "Index invalide.\n";
    return false;
}

int Player::getEquippedAtkBonus() const {
    if (!armeEquipee || !armeEquipee->estDisponible()) return 0;
    return armeEquipee->getBonusAtk();
}

int Player::getEquippedDefBonus() const {
    if (!armureEquipee || !armureEquipee->estDisponible()) return 0;
    return armureEquipee->getBonusDefPct();
}

void Player::displayInventory() const {
    cout << "=== Inventaire ===\n";
    if (inventory.empty()) { cout << "(vide)\n"; return; }
    for (int i = 0; i < (int)inventory.size(); ++i) {
        cout << "[" << i << "] ";
        inventory[i]->afficherDetails();
    }
    if (armeEquipee)
        cout << "  [Arme equipee   : " << armeEquipee->getName() << "]\n";
    if (armureEquipee)
        cout << "  [Armure equipee : " << armureEquipee->getName() << "]\n";
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

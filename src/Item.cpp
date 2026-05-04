#include "Item.h"
using namespace std;

// ─── Générateur aléatoire partagé ────────────────────────────────────────────
mt19937& itemRng() {
    static mt19937 rng(random_device{}());
    return rng;
}

// =============================================================================
//  Utilisable
// =============================================================================
Utilisable::Utilisable(const string& name, const string& type)
    : name(name), type(type) {}

string Utilisable::getName() const { return name; }
string Utilisable::getType() const { return type; }

// =============================================================================
//  Potion
// =============================================================================
Potion::Potion(const string& name, int soin, int qte)
    : Utilisable(name, "HEAL"), valeurSoin(soin), quantite(qte) {}

int Potion::getValeurSoin() const { return valeurSoin; }
int Potion::getQuantite()   const { return quantite; }

bool Potion::utiliser() {
    if (quantite <= 0) return false;
    --quantite;
    return true;
}

bool Potion::estDisponible() const { return quantite > 0; }

void Potion::afficherDetails() const {
    cout << "[POTION] " << name
         << " | Soin : +" << valeurSoin << " HP"
         << " | Quantite : " << quantite << "\n";
}

// =============================================================================
//  Arme
// =============================================================================
Arme::Arme(const string& name, int bonusAtk, int durabilite)
    : Utilisable(name, "WEAPON"),
      bonusAtk(bonusAtk), durabilite(durabilite), durabiliteMax(durabilite) {}

int Arme::getBonusAtk()   const { return bonusAtk; }
int Arme::getDurabilite() const { return durabilite; }

bool Arme::utiliser() {
    if (durabilite <= 0) return false;
    --durabilite;
    return true;
}

bool Arme::estDisponible() const { return durabilite > 0; }

void Arme::afficherDetails() const {
    cout << "[ARME]   " << name
         << " | +" << bonusAtk << " damage"
         << " | Durabilite : " << durabilite << "/" << durabiliteMax << "\n";
}

// =============================================================================
//  Equipement
// =============================================================================
Equipement::Equipement(const string& name, int bonusDefPct, int seuilMax)
    : Utilisable(name, "ARMOR"),
      bonusDefPct(bonusDefPct), usure(0), seuilMax(seuilMax) {}

int Equipement::getBonusDefPct() const { return bonusDefPct; }
int Equipement::getUsure()       const { return usure; }
int Equipement::getSeuilMax()    const { return seuilMax; }

bool Equipement::utiliser() {
    if (usure >= seuilMax) return false;
    uniform_int_distribution<int> dist(10, 40);
    int gain = dist(itemRng());
    usure = min(usure + gain, seuilMax);
    return true;
}

bool Equipement::estDisponible() const { return usure < seuilMax; }

void Equipement::afficherDetails() const {
    cout << "[ARMOR]  " << name
         << " | +" << bonusDefPct << " DEF"
         << " | Usure : " << usure << "/" << seuilMax << "\n";
}

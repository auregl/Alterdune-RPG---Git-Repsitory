#include "Monster.h"
#include <algorithm>
#include <random>
#include <iostream>
using namespace std;

static mt19937 dropRng(random_device{}());

Monster::Monster(const string& name, int hp, int atk, int def,
                 int mercyGoal, const vector<int>& acts)
    : Entity(name, hp, atk, def),
      mercy(0), mercyGoal(mercyGoal),
      actIds(acts), wasKilled(false) {}

string             Monster::getCategory()     const { return category; }
int                Monster::getMercy()        const { return mercy; }
int                Monster::getMercyGoal()    const { return mercyGoal; }
const vector<int>& Monster::getActIds()       const { return actIds; }
bool               Monster::wasKilledResult() const { return wasKilled; }

void Monster::setKilledResult(bool v) { wasKilled = v; }

void Monster::modifyMercy(int delta) {
    mercy = clamp(mercy + delta, 0, mercyGoal);
}

bool Monster::isMercyFull() const {
    return mercy >= mercyGoal;
}

// ─── rollDrops ────────────────────────────────────────────────────────────────
// Tous les items du CSV peuvent dropper.
// On pioche aléatoirement dans chaque pool, avec des chances selon la catégorie.
//
//  NORMAL   : 1 HEAL  (50%)
//
//  MINIBOSS : 1 HEAL  (70%)
//             1 WEAPON (45%)
//
//  BOSS     : 1 HEAL   (100% garanti)
//             1 WEAPON  (65%)
//             1 ARMOR   (55%)
//
// Pour chaque slot : on tire un item aléatoire dans le pool correspondant,
// puis on roll sa chance. Si succès → copie instanciée donnée au joueur.
// Les pools passés en paramètre sont non-owning (Game en est propriétaire).

static Utilisable* cloneItem(const Utilisable* src) {
    // Recrée un item identique (nouvel objet, ownership à l'appelant)
    if (const Potion* p = dynamic_cast<const Potion*>(src))
        return new Potion(p->getName(), p->getValeurSoin(), p->getQuantite());
    if (const Arme* a = dynamic_cast<const Arme*>(src))
        return new Arme(a->getName(), a->getBonusAtk(), a->getDurabilite());
    if (const Equipement* e = dynamic_cast<const Equipement*>(src))
        return new Equipement(e->getName(), e->getBonusDefPct(), e->getSeuilMax());
    return nullptr;
}

static Utilisable* tryDrop(const vector<Utilisable*>& pool, int chancePct) {
    if (pool.empty()) return nullptr;
    uniform_int_distribution<int> pickDist(0, (int)pool.size() - 1);
    uniform_int_distribution<int> pctDist(1, 100);
    if (pctDist(dropRng) <= chancePct)
        return cloneItem(pool[pickDist(dropRng)]);
    return nullptr;
}

vector<Utilisable*> Monster::rollDrops(
    const vector<Utilisable*>& healPool,
    const vector<Utilisable*>& weaponPool,
    const vector<Utilisable*>& armorPool) const
{
    vector<Utilisable*> result;
    Utilisable* dropped = nullptr;

    if (category == "NORMAL") {
        dropped = tryDrop(healPool, 50);
        if (dropped) result.push_back(dropped);
    }
    else if (category == "MINIBOSS") {
        dropped = tryDrop(healPool,   70); if (dropped) result.push_back(dropped);
        dropped = tryDrop(weaponPool, 45); if (dropped) result.push_back(dropped);
    }
    else if (category == "BOSS") {
        dropped = tryDrop(healPool,   100); if (dropped) result.push_back(dropped);
        dropped = tryDrop(weaponPool,  65); if (dropped) result.push_back(dropped);
        dropped = tryDrop(armorPool,   55); if (dropped) result.push_back(dropped);
    }

    return result;
}
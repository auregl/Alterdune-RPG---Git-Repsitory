#include "Monster.h"
#include <algorithm>
#include <random>
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
//
//  NORMAL   : 1 slot
//    - Herbe du désert  (HEAL  5 HP  x3)   60 %
//
//  MINIBOSS : 2 slots
//    - Potion de sable  (HEAL 15 HP  x1)   70 %
//    - Gourdin de pierre(ATK +6  dur 3)    45 %
//
//  BOSS     : 3 slots
//    - Elixir nomade        (HEAL 20 HP x1)  100 % garanti
//    - Lance solaire        (ATK +10  dur 2)   65 %
//    - Plastron des anciens (DEF +25 %)        55 %
//
// Les items dont le roll échoue sont delete-és ici (pas de fuite mémoire).

vector<Utilisable*> Monster::rollDrops() const {
    uniform_int_distribution<int> pct(1, 100);
    vector<Utilisable*> result;
    vector<Drop> pool;

    if (category == "NORMAL") {
        pool = {
            { new Potion("Herbe du désert", 5, 3), 60 }
        };
    }
    else if (category == "MINIBOSS") {
        pool = {
            { new Potion("Potion de sable",  15, 1), 70 },
            { new Arme("Gourdin de pierre",   6, 3), 45 }
        };
    }
    else if (category == "BOSS") {
        pool = {
            { new Potion("Elixir nomade",            20, 1), 100 },
            { new Arme("Lance solaire",              10, 2),  65 },
            { new Equipement("Plastron des anciens", 25, 80), 55 }
        };
    }

    for (Drop& d : pool) {
        if (pct(dropRng) <= d.chancePct)
            result.push_back(d.item);
        else
            delete d.item;
    }

    return result;
}
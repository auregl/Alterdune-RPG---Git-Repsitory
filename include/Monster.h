#pragma once
#include "Entity.h"
#include "Item.h"
#include <vector>
#include <string>
using namespace std;



//struct Drop {
//    Utilisable* item;
//    int         chancePct;
//};


class Monster : public Entity {
protected:
    string      category;
    int         mercy;
    int         mercyGoal;
    vector<int> actIds;
    bool        wasKilled;

public:
    Monster(const string& name, int hp, int atk, int def,
            int mercyGoal, const vector<int>& acts);
    virtual ~Monster() = default;

    string             getCategory()     const;
    int                getMercy()        const;
    int                getMercyGoal()    const;
    const vector<int>& getActIds()       const;
    bool               wasKilledResult() const;

    void setKilledResult(bool v);
    void modifyMercy(int delta);
    bool isMercyFull() const;
    virtual Monster* clone() const = 0;

// Pas utilisé pour l'instant 
    vector<Utilisable*> rollDrops(
        const vector<Utilisable*>& healPool,
        const vector<Utilisable*>& weaponPool,
        const vector<Utilisable*>& armorPool) const;

    virtual int getMaxActCount() const = 0;
};
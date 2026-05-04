#pragma once
#include "Entity.h"
#include "Item.h"
#include <vector>
#include <string>
using namespace std;


// ─── Drop : item généré + probabilité ────────────────────────────────────────
struct Drop {
    Utilisable* item;      // ownership transmis au joueur si le roll réussit
    int         chancePct; // 0–100
};


// Classe abstraite : ne peut pas être instanciée directement
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

    // Roule les drops selon la catégorie du monstre.
    // Retourne uniquement les items dont le roll a réussi (ownership à l'appelant).
    vector<Utilisable*> rollDrops() const;

    virtual int getMaxActCount() const = 0;
};
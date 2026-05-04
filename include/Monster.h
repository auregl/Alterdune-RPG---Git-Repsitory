#pragma once
#include "Entity.h"
#include <vector>
#include <string>
using namespace std;


// Classe abstraite : ne peut pas être instanciée directement
class Monster : public Entity {
protected:
    string      category;
    int              mercy;
    int              mercyGoal;
    vector<int> actIds;    // identifiants des actions ACT disponibles
    bool             wasKilled; // résultat du combat (tué ou épargné)

public:
    Monster(const string& name, int hp, int atk, int def,
            int mercyGoal, const vector<int>& acts);
    virtual ~Monster() = default;

    string            getCategory()     const;
    int                    getMercy()        const;
    int                    getMercyGoal()    const;
    const vector<int>& getActIds()      const;
    bool                   wasKilledResult() const;

    void setKilledResult(bool v);
    void modifyMercy(int delta);
    bool isMercyFull() const;
    virtual Monster* clone() const = 0;

    // Méthode purement virtuelle : polymorphisme sur le nb d'actions ACT
    virtual int getMaxActCount() const = 0;
};

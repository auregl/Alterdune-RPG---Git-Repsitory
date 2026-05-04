#pragma once
#include "ActAction.h"
#include <map>
using namespace std;


// Catalogue de toutes les actions ACT disponibles dans le jeu (pré-défini dans le code)
class ActCatalog {
private:
    map<int, ActAction> actions;

public:
    ActCatalog();   // Remplit le catalogue avec les actions prédéfinies

    const ActAction* getAction(int id) const;
    bool             exists(int id)    const;
    void             displayAll()      const;
};

// ──────────────────────────────────────────────────────
//  Identifiants des actions ACT (enum pour lisibilité)
// ──────────────────────────────────────────────────────
enum ActId {
    JOKE          = 1,
    COMPLIMENT    = 2,
    INSULT        = 3,   // impact négatif
    DISCUSS       = 4,
    OBSERVE       = 5,
    PET           = 6,
    OFFER_SNACK   = 7,
    REASON        = 8,
    DANCE         = 9,
    TAUNT         = 10,  // impact négatif
};

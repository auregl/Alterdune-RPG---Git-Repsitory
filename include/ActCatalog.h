#pragma once
#include "ActAction.h"
#include <map>
using namespace std;


class ActCatalog {
private:
    map<int, ActAction> actions;

public:
    ActCatalog(); 

    const ActAction* getAction(int id) const;
    bool             exists(int id)    const;
    void             displayAll()      const;
};

//  Identifiants des actions ACT (enum pour lisibilité)
enum ActId {
    JOKE          = 1,
    COMPLIMENT    = 2,
    INSULT        = 3,   
    DISCUSS       = 4,
    OBSERVE       = 5,
    PET           = 6,
    OFFER_SNACK   = 7,
    REASON        = 8,
    DANCE         = 9,
    TAUNT         = 10, 
    CHANT          = 11,
    BRAG          = 12,
    APOLOGIZE     = 13,
};

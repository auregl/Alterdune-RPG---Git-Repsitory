#include "Bestiary.h"
#include <iostream>
using namespace std;

Bestiary::~Bestiary() {
    for (Monster* m : defeatedMonsters) delete m;
}

void Bestiary::add(Monster* m) {
    defeatedMonsters.push_back(m);
}

void Bestiary::display() const {
    cout << "=== Bestiaire (" << defeatedMonsters.size() << " monstres) ===\n";
    if (defeatedMonsters.empty()) {
        cout << "(aucun monstre vaincu pour l'instant)\n";
        return;
    }
    for (const Monster* m : defeatedMonsters) {
        cout << "- " << m->getName()
                  << " [" << m->getCategory() << "]"
                  << " | HP max:" << m->getHpMax()
                  << " ATK:" << m->getAtk()
                  << " DEF:" << m->getDef()
                  << " | " << (m->wasKilledResult() ? "Tué" : "Épargné")
                  << "\n";
    }
}

int Bestiary::getCount() const {
    return (int)defeatedMonsters.size();
}

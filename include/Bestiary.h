#pragma once
#include "Monster.h"
#include <vector>
using namespace std;


class Bestiary {
private:
    vector<Monster*> defeatedMonsters; // pointeurs non-owning (Game possède les monstres)

public:
    Bestiary() = default;
    ~Bestiary();

    void add(Monster* m);
    void display()   const;
    int  getCount()  const;
};

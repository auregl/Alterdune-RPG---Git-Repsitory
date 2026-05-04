#pragma once
#include "Monster.h"
using namespace std;


// ─── Monstre normal : 2 actions ACT ──────────────────────────────────────────
class NormalMonster : public Monster {
public:
    NormalMonster(const string& name, int hp, int atk, int def,
                  int mercyGoal, const vector<int>& acts);
    Monster* clone() const override;
    int getMaxActCount() const override;
};

// ─── Miniboss : 3 actions ACT ────────────────────────────────────────────────
class MinibossMonster : public Monster {
public:
    MinibossMonster(const string& name, int hp, int atk, int def,
                    int mercyGoal, const vector<int>& acts);
    Monster* clone() const override;
    int getMaxActCount() const override;
};

// ─── Boss : 4 actions ACT ────────────────────────────────────────────────────
class BossMonster : public Monster {
public:
    BossMonster(const string& name, int hp, int atk, int def,
                int mercyGoal, const vector<int>& acts);
    Monster* clone() const override;
    int getMaxActCount() const override;
};

#include "MonsterTypes.h"
using namespace std;


// ─── NormalMonster ────────────────────────────────────────────────────────────
NormalMonster::NormalMonster(const string& name, int hp, int atk, int def,
                             int mercyGoal, const vector<int>& acts)
    : Monster(name, hp, atk, def, mercyGoal, acts) {
    category = "NORMAL";
}
int NormalMonster::getMaxActCount() const { return 2; }

// ─── MinibossMonster ──────────────────────────────────────────────────────────
MinibossMonster::MinibossMonster(const string& name, int hp, int atk, int def,
                                 int mercyGoal, const vector<int>& acts)
    : Monster(name, hp, atk, def, mercyGoal, acts) {
    category = "MINIBOSS";
}
int MinibossMonster::getMaxActCount() const { return 3; }

// ─── BossMonster ──────────────────────────────────────────────────────────────
BossMonster::BossMonster(const string& name, int hp, int atk, int def,
                         int mercyGoal, const vector<int>& acts)
    : Monster(name, hp, atk, def, mercyGoal, acts) {
    category = "BOSS";
}
int BossMonster::getMaxActCount() const { return 4; }

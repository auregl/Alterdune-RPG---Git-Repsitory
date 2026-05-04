#include "Monster.h"
#include <algorithm>
using namespace std;


Monster::Monster(const string& name, int hp, int atk, int def,
                 int mercyGoal, const vector<int>& acts)
    : Entity(name, hp, atk, def),
      mercy(0), mercyGoal(mercyGoal),
      actIds(acts), wasKilled(false) {}

string             Monster::getCategory()     const { return category; }
int                     Monster::getMercy()        const { return mercy; }
int                     Monster::getMercyGoal()    const { return mercyGoal; }
const vector<int>& Monster::getActIds()       const { return actIds; }
bool                    Monster::wasKilledResult() const { return wasKilled; }

void Monster::setKilledResult(bool v) { wasKilled = v; }

void Monster::modifyMercy(int delta) {
    mercy = clamp(mercy + delta, 0, mercyGoal);
}

bool Monster::isMercyFull() const {
    return mercy >= mercyGoal;
}

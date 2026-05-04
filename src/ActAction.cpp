#include "ActAction.h"
using namespace std;


ActAction::ActAction(int id, const string& text, int impact)
    : id(id), displayText(text), mercyImpact(impact) {}

int         ActAction::getId()          const { return id; }
string ActAction::getDisplayText() const { return displayText; }
int         ActAction::getMercyImpact() const { return mercyImpact; }

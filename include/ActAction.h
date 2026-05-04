#pragma once
#include <string>
using namespace std;


// Représente une action ACT du catalogue (structure de données pure)
class ActAction {
private:
    int         id;
    string displayText;
    int         mercyImpact;  // positif = apaise, négatif = énerve

public:
    ActAction(int id, const string& text, int impact);

    int         getId()          const;
    string getDisplayText() const;
    int         getMercyImpact() const;
};

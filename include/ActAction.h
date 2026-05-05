#pragma once
#include <string>
using namespace std;



class ActAction {
private:
    int         id;
    string displayText;
    int         mercyImpact;

public:
    ActAction(int id, const string& text, int impact);

    int         getId()          const;
    string getDisplayText() const;
    int         getMercyImpact() const;
};

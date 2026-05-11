#pragma once
#include <iostream>
#include <limits>
#include <string>
using namespace std;


inline int safeReadInt(int default_val = -1) {
    int value;
    if (cin >> value) {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return value;
    }

    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "[Saisie invalide. Entier attendu.]\n";
    return default_val;
}

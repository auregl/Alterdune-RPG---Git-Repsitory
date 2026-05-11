#pragma once
#include <iostream>
#include <limits>
#include <string>
using namespace std;

// Lit un entier depuis cin de façon sécurisée.
// Si l'utilisateur entre autre chose qu'un entier, vide le buffer,
// affiche un message et retourne la valeur par défaut (default_val).
inline int safeReadInt(int default_val = -1) {
    int value;
    if (cin >> value) {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return value;
    }
    // Saisie invalide : nettoie le flag d'erreur et vide le buffer
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "[Saisie invalide. Entier attendu.]\n";
    return default_val;
}

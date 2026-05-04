#include "FileLoader.h"
#include "MonsterTypes.h"
#include "ActCatalog.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <random>
#include <algorithm>
using namespace std;

static mt19937 loaderRng(random_device{}());

// ─── Trim ─────────────────────────────────────────────────────────────────────
static string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end   = s.find_last_not_of(" \t\r\n");
    return (start == string::npos) ? "" : s.substr(start, end - start + 1);
}

// ─── Nom d'action → identifiant ───────────────────────────────────────────────
static int actNameToId(const string& name) {
    if (name == "JOKE")         return JOKE;
    if (name == "COMPLIMENT")   return COMPLIMENT;
    if (name == "INSULT")       return INSULT;
    if (name == "DISCUSS")      return DISCUSS;
    if (name == "OBSERVE")      return OBSERVE;
    if (name == "PET")          return PET;
    if (name == "OFFER_SNACK")  return OFFER_SNACK;
    if (name == "REASON")       return REASON;
    if (name == "DANCE")        return DANCE;
    if (name == "TAUNT")        return TAUNT;
    return -1;
}

// ─── Chargement des items ─────────────────────────────────────────────────────
// Sépare le pool en 3 catégories (HEAL / WEAPON / ARMOR), mélange chacune,
// puis attribue au joueur :
//   - 2 ou 3 potions  (tirage aléatoire parmi les HEAL)
//   - 2 armes         (tirées aléatoirement parmi les WEAPON)
//   - 2 armures       (tirées aléatoirement parmi les ARMOR)
// Les items non attribués sont libérés proprement.
void FileLoader::loadItems(const string& path, Player& player) {
    ifstream file(path);
    if (!file.is_open()) {
        cerr << "[ERREUR] Fichier introuvable : " << path << "\n";
        exit(1);
    }

    vector<Utilisable*> heals, weapons, armors;

    string line;
    while (getline(file, line)) {
        if (trim(line).empty()) continue;
        try {
            Utilisable* u = parseItemLine(line);
            if (!u) continue;
            string t = u->getType();
            if      (t == "HEAL")   heals.push_back(u);
            else if (t == "WEAPON") weapons.push_back(u);
            else if (t == "ARMOR")  armors.push_back(u);
            else delete u;
        } catch (const exception& e) {
            cerr << "[AVERTISSEMENT] Ligne ignorée : " << e.what() << "\n";
        }
    }

    // Mélange indépendant de chaque catégorie
    shuffle(heals.begin(),   heals.end(),   loaderRng);
    shuffle(weapons.begin(), weapons.end(), loaderRng);
    shuffle(armors.begin(),  armors.end(),  loaderRng);

    // Nombre de potions de départ : 2 ou 3
    uniform_int_distribution<int> healCount(2, 3);
    int nHeals   = min(healCount(loaderRng), (int)heals.size());
    int nWeapons = min(2, (int)weapons.size());
    int nArmors  = min(2, (int)armors.size());

    // Attribution au joueur
    cout << "\n[ Équipement de départ ]\n";

    for (int i = 0; i < nHeals; ++i) {
        cout << "  + "; heals[i]->afficherDetails();
        player.addItem(heals[i]);
    }
    for (int i = 0; i < nWeapons; ++i) {
        cout << "  + "; weapons[i]->afficherDetails();
        player.addItem(weapons[i]);
    }
    for (int i = 0; i < nArmors; ++i) {
        cout << "  + "; armors[i]->afficherDetails();
        player.addItem(armors[i]);
    }

    // Libération des items non attribués
    for (int i = nHeals;   i < (int)heals.size();   ++i) delete heals[i];
    for (int i = nWeapons; i < (int)weapons.size(); ++i) delete weapons[i];
    for (int i = nArmors;  i < (int)armors.size();  ++i) delete armors[i];
}

Utilisable* FileLoader::parseItemLine(const string& line) {
    stringstream ss(line);
    string typeStr, nom, val1Str, val2Str;

    if (!getline(ss, typeStr, ';') ||
        !getline(ss, nom,     ';') ||
        !getline(ss, val1Str, ';') ||
        !getline(ss, val2Str, ';'))
        throw runtime_error("Ligne mal formée : " + line);

    string t  = trim(typeStr);
    string n  = trim(nom);
    int val1  = stoi(trim(val1Str));
    int val2  = stoi(trim(val2Str));

    if (t == "HEAL")   return new Potion(n, val1, val2);
    if (t == "WEAPON") return new Arme(n, val1, val2);
    if (t == "ARMOR")  return new Equipement(n, val1, val2);

    throw runtime_error("Type d'item inconnu : " + t);
}

// ─── Chargement des monstres ──────────────────────────────────────────────────
vector<Monster*> FileLoader::loadMonsters(const string& path) {
    ifstream file(path);
    if (!file.is_open()) {
        cerr << "[ERREUR] Fichier introuvable : " << path << "\n";
        exit(1);
    }

    vector<Monster*> monsters;
    string line;
    while (getline(file, line)) {
        if (trim(line).empty()) continue;
        try {
            Monster* m = parseMonsterLine(line);
            if (m) monsters.push_back(m);
        } catch (const exception& e) {
            cerr << "[AVERTISSEMENT] Ligne ignorée : " << e.what() << "\n";
        }
    }
    return monsters;
}

Monster* FileLoader::parseMonsterLine(const string& line) {
    stringstream ss(line);
    string cat, nom, hpStr, atkStr, defStr, mercyStr;
    string act1, act2, act3, act4;

    if (!getline(ss, cat,     ';') || !getline(ss, nom,     ';') ||
        !getline(ss, hpStr,   ';') || !getline(ss, atkStr,  ';') ||
        !getline(ss, defStr,  ';') || !getline(ss, mercyStr,';') ||
        !getline(ss, act1,    ';') || !getline(ss, act2,    ';') ||
        !getline(ss, act3,    ';') || !getline(ss, act4,    ';'))
        throw runtime_error("Ligne mal formée : " + line);

    cat = trim(cat); nom = trim(nom);
    int hp    = stoi(trim(hpStr));
    int atk   = stoi(trim(atkStr));
    int def   = stoi(trim(defStr));
    int mercy = stoi(trim(mercyStr));

    vector<int> acts;
    for (const string& a : {act1, act2, act3, act4}) {
        int id = actNameToId(trim(a));
        if (id != -1) acts.push_back(id);
    }

    if (cat == "NORMAL")   return new NormalMonster(nom, hp, atk, def, mercy, acts);
    if (cat == "MINIBOSS") return new MinibossMonster(nom, hp, atk, def, mercy, acts);
    if (cat == "BOSS")     return new BossMonster(nom, hp, atk, def, mercy, acts);

    throw runtime_error("Catégorie inconnue : " + cat);
}
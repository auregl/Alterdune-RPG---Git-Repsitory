#include "FileLoader.h"
#include "MonsterTypes.h"
#include "ActCatalog.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
using namespace std;

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
// Format CSV :
//   HEAL   ; nom ; valeurSoin ; quantite
//   WEAPON ; nom ; bonusAtk   ; durabilite
//   ARMOR  ; nom ; bonusDef%  ; seuilMax
void FileLoader::loadItems(const string& path, Player& player) {
    ifstream file(path);
    if (!file.is_open()) {
        cerr << "[ERREUR] Fichier introuvable : " << path << "\n";
        exit(1);
    }

    string line;
    while (getline(file, line)) {
        if (trim(line).empty()) continue;
        try {
            Utilisable* u = parseItemLine(line);
            if (u) player.addItem(u);
        } catch (const exception& e) {
            cerr << "[AVERTISSEMENT] Ligne ignoree : " << e.what() << "\n";
        }
    }
}

Utilisable* FileLoader::parseItemLine(const string& line) {
    stringstream ss(line);
    string typeStr, nom, val1Str, val2Str;

    if (!getline(ss, typeStr, ';') ||
        !getline(ss, nom,     ';') ||
        !getline(ss, val1Str, ';') ||
        !getline(ss, val2Str, ';'))
        throw runtime_error("Ligne mal formee : " + line);

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
            cerr << "[AVERTISSEMENT] Ligne ignoree : " << e.what() << "\n";
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
        throw runtime_error("Ligne mal formee : " + line);

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

    throw runtime_error("Categorie inconnue : " + cat);
}

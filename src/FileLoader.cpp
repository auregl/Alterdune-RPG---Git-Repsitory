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

static string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end   = s.find_last_not_of(" \t\r\n");
    return (start == string::npos) ? "" : s.substr(start, end - start + 1);
}

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
// 1. Lit tout le CSV et sépare en 3 pools (HEAL / WEAPON / ARMOR)
// 2. Mélange chaque pool
// 3. Donne au joueur : 2-3 potions, 2 armes, 2 armures (clones)
// 4. Retourne les 3 pools complets — Game en prend ownership pour les drops
ItemPools FileLoader::loadItems(const string& path, Player& player) {
    ifstream file(path);
    if (!file.is_open()) {
        cerr << "[ERREUR] Fichier introuvable : " << path << "\n";
        exit(1);
    }

    ItemPools pools;
    string line;
    while (getline(file, line)) {
        if (trim(line).empty()) continue;
        try {
            Utilisable* u = parseItemLine(line);
            if (!u) continue;
            string t = u->getType();
            if      (t == "HEAL")   pools.heals.push_back(u);
            else if (t == "WEAPON") pools.weapons.push_back(u);
            else if (t == "ARMOR")  pools.armors.push_back(u);
            else delete u;
        } catch (const exception& e) {
            cerr << "[AVERTISSEMENT] Ligne ignorée : " << e.what() << "\n";
        }
    }

    // Mélange de chaque pool
    shuffle(pools.heals.begin(),   pools.heals.end(),   loaderRng);
    shuffle(pools.weapons.begin(), pools.weapons.end(), loaderRng);
    shuffle(pools.armors.begin(),  pools.armors.end(),  loaderRng);

    // Kit de départ : clones donnés au joueur (les originaux restent dans les pools)
    uniform_int_distribution<int> healCount(2, 3);
    int nHeals   = min(healCount(loaderRng), (int)pools.heals.size());
    int nWeapons = min(2, (int)pools.weapons.size());
    int nArmors  = min(2, (int)pools.armors.size());

    auto cloneItem = [](Utilisable* src) -> Utilisable* {
        if (Potion* p    = dynamic_cast<Potion*>(src))
            return new Potion(p->getName(), p->getValeurSoin(), p->getQuantite());
        if (Arme* a      = dynamic_cast<Arme*>(src))
            return new Arme(a->getName(), a->getBonusAtk(), a->getDurabilite());
        if (Equipement* e = dynamic_cast<Equipement*>(src))
            return new Equipement(e->getName(), e->getBonusDefPct(), e->getSeuilMax());
        return nullptr;
    };

    cout << "\n[ Équipement de départ ]\n";
    for (int i = 0; i < nHeals;   ++i) { auto c = cloneItem(pools.heals[i]);   if (c) { cout << "  + "; c->afficherDetails(); player.addItem(c); } }
    for (int i = 0; i < nWeapons; ++i) { auto c = cloneItem(pools.weapons[i]); if (c) { cout << "  + "; c->afficherDetails(); player.addItem(c); } }
    for (int i = 0; i < nArmors;  ++i) { auto c = cloneItem(pools.armors[i]);  if (c) { cout << "  + "; c->afficherDetails(); player.addItem(c); } }

    return pools;
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
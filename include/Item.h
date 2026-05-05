#pragma once
#include <iostream>
#include <string>
#include <random>
using namespace std;

//Générateur aléatoire partagé 
mt19937& itemRng();

//  CLASSE DE BASE : Utilisable  (en polymorphe)
class Utilisable {
protected:
    string name;
    string type;

public:
    Utilisable(const string& name, const string& type);
    virtual ~Utilisable() = default;

    string getName() const;
    string getType() const;

    virtual bool utiliser()          = 0;
    virtual void afficherDetails()   const = 0;
    virtual bool estDisponible()     const = 0;
};

//  POTION  — soigne des HP, quantite limitee
class Potion : public Utilisable {
private:
    int valeurSoin;
    int quantite;

public:
    Potion(const string& name, int soin, int qte);

    int  getValeurSoin()        const;
    int  getQuantite()          const;
    bool utiliser()             override;
    bool estDisponible()        const override;
    void afficherDetails()      const override;
};

//  ARME  — augmente l'ATK, durabilite entre 1 et 4 utilisations a peu pres
class Arme : public Utilisable {
private:
    int bonusAtk;
    int durabilite;
    int durabiliteMax;

public:
    Arme(const string& name, int bonusAtk, int durabilite);

    int  getBonusAtk()          const;
    int  getDurabilite()        const;
    bool utiliser()             override;
    bool estDisponible()        const override;
    void afficherDetails()      const override;
};

//  EQUIPEMENT  — augmente la DEF (%), usure aleatoire entre 10 et 40 par usage max
class Equipement : public Utilisable {
private:
    int bonusDefPct;
    int usure;
    int seuilMax;

public:
    Equipement(const string& name, int bonusDefPct, int seuilMax = 80);

    int  getBonusDefPct()       const;
    int  getUsure()             const;
    int  getSeuilMax()          const;
    bool utiliser()             override;
    bool estDisponible()        const override;
    void afficherDetails()      const override;
};

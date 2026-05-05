#include "Combat.h"
#include <iostream>
#include <random>
#include <limits>
using namespace std;

static mt19937 rng(random_device{}());

Combat::Combat(Player& player, Monster& monster, ActCatalog& catalog)
    : player(player), monster(monster), catalog(catalog),
      bonusAtkTour(0), bonusDefTour(0) {}

// ─── Boucle principale ────────────────────────────────────────────────────────
bool Combat::run() {
    bonusAtkTour = 0;
    bonusDefTour = 0;

    cout << "\n*** Combat contre " << monster.getName()
         << " [" << monster.getCategory() << "] ***\n";

    while (player.isAlive() && monster.isAlive()) {
        showStatus();

        bool combatOver = playerTurn();

        // Verifie si le monstre est mort (via fight())
        if (!monster.isAlive()) {
            monster.setKilledResult(true);
            player.addKill();
            cout << monster.getName() << " est vaincu !\n";
            return true;
        }

        // combatOver sans mort du monstre = grace accordee
        if (combatOver) return true;

        monsterTurn();

        // L'arme et l'armure expirent en fin de tour
        if (bonusAtkTour > 0) {
            cout << "[Arme " << player.getArmeEquipee()->getName()
                 << " desequipee (fin de tour).]\n";
            player.desequiperArme();
            bonusAtkTour = 0;
        }
        if (bonusDefTour > 0) {
            cout << "[Armure desequipee (fin de tour).]\n";
            player.desequiperArmure();
            bonusDefTour = 0;
        }

        if (!player.isAlive()) {
            cout << "Vous etes mort. Fin de partie.\n";
            return false;
        }
    }

    return false;
}

// ─── Tour du joueur ───────────────────────────────────────────────────────────
// ITEM ne consomme pas le tour : la boucle reboucle jusqu'a FIGHT/ACT/MERCY.
bool Combat::playerTurn() {
    while (true) {
        int atkBonus = player.getEquippedAtkBonus();
        int defBonus = player.getEquippedDefBonus();
        if (atkBonus > 0 || defBonus > 0) {
            cout << "  [Equipement actif ce tour : ATK +" << atkBonus
                 << " | DEF +" << defBonus << "%]\n";
        }

        cout << "\n  FIGHT    ACT    ITEM    MERCY\n";
        cout << "  [1]      [2]    [3]     [4]\n> ";

        int choice = 0;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: return fight();
            case 2: doAct();     return false;
            case 3: menuItem();  break;   // ne consomme pas le tour
            case 4: return doMercy();
            default:
                cout << "Choix invalide.\n";
                break;
        }
    }
}

// ─── Tour du monstre ──────────────────────────────────────────────────────────
// Degats = atk * multiplicateur [0.0, 2.0]
// < 0.3 : rate | > 1.7 : critique
// L'armure absorbe une partie des degats et consomme de l'usure.
void Combat::monsterTurn() {
    bonusDefTour = player.getEquippedDefBonus();

    float multi   = calcMultiplier();
    int   dmgBrut = static_cast<int>(monster.getAtk() * multi);
    int   reduction = dmgBrut * bonusDefTour / 100;
    int   dmgFinal  = max(0, dmgBrut - reduction);

    // Consomme l'usure de l'armure si elle a absorbe quelque chose
    if (reduction > 0 && player.getArmureEquipee()) {
        player.getArmureEquipee()->utiliser();
    }

    player.takeDamage(dmgFinal);

    if (multi < 0.3f) {
        cout << monster.getName() << " a rate son attaque !\n";
    } else if (multi > 1.7f) {
        cout << monster.getName() << " a fait un coup critique ! ";
        cout << "Vous subissez " << dmgFinal << " degats";
        if (reduction > 0)
            cout << " (" << dmgBrut << " bruts - " << reduction << " absorbes par l'armure)";
        cout << ".\nHP restants : " << player.getHp() << "/" << player.getHpMax() << "\n";
    } else {
        cout << monster.getName() << " vous inflige " << dmgFinal << " degats";
        if (reduction > 0)
            cout << " (" << dmgBrut << " bruts - " << reduction << " absorbes par l'armure)";
        cout << ".\nHP restants : " << player.getHp() << "/" << player.getHpMax() << "\n";
    }
}

// ─── FIGHT ────────────────────────────────────────────────────────────────────
// L'arme est consommee (durabilite -1) au moment de l'attaque.
bool Combat::fight() {
    bonusAtkTour = player.getEquippedAtkBonus();

    if (bonusAtkTour > 0 && player.getArmeEquipee()) {
        player.getArmeEquipee()->utiliser(); // consomme 1 durabilite
    }

    int dmgBase  = calcDamage(monster.getHpMax());
    int dmgTotal = dmgBase + bonusAtkTour;
    monster.takeDamage(dmgTotal);

    if (dmgTotal == 0) {
        cout << "Vous ratez votre attaque !\n";
    } else {
        cout << "Vous infligez " << dmgTotal << " degats";
        if (bonusAtkTour > 0)
            cout << " (" << dmgBase << " aleatoire + " << bonusAtkTour << " via l'arme)";
        cout << " a " << monster.getName() << ".\n";
        cout << monster.getName() << " HP : " << monster.getHp()
             << "/" << monster.getHpMax() << "\n";
    }

    return !monster.isAlive();
}

// ─── ACT ──────────────────────────────────────────────────────────────────────
void Combat::doAct() {
    const vector<int>& ids = monster.getActIds();
    int maxActs = monster.getMaxActCount();

    cout << "=== Actions ACT disponibles ===\n";
    for (int i = 0; i < maxActs && i < (int)ids.size(); ++i) {
        const ActAction* a = catalog.getAction(ids[i]);
        if (a) cout << "[" << (i + 1) << "] " << a->getDisplayText() << "\n";
    }
    cout << "> ";

    int choice = 0;
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    int idx = choice - 1;
    if (idx < 0 || idx >= maxActs || idx >= (int)ids.size()) {
        cout << "Choix invalide.\n";
        return;
    }

    const ActAction* a = catalog.getAction(ids[idx]);
    if (!a) return;

    cout << a->getDisplayText() << "\n";
    monster.modifyMercy(a->getMercyImpact());
    cout << "Mercy de " << monster.getName() << " : "
         << monster.getMercy() << "/" << monster.getMercyGoal() << "\n";
}

// ─── ITEM consomme pas le tour ──────────────────────────────────────────
void Combat::menuItem() {
    player.displayInventory();
    cout << "Choisir un item (index, -1 pour annuler) > ";

    int idx = 0;
    cin >> idx;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (idx < 0) { cout << "Annule.\n"; return; }

    player.useItem(idx, bonusAtkTour, bonusDefTour);
}

// ─── MERCY ────────────────────────────────────────────────────────────────────
bool Combat::doMercy() {
    if (!monster.isMercyFull()) {
        cout << "La jauge Mercy n'est pas pleine ("
             << monster.getMercy() << "/" << monster.getMercyGoal() << ").\n";
        return false;
    }
    cout << monster.getName() << " accepte votre grace et s'en va.\n";
    monster.setKilledResult(false);
    player.addSpare();
    return true;
}

int Combat::calcDamage(int maxHp) {
    uniform_int_distribution<int> dist(0, maxHp);
    return dist(rng);
}

float Combat::calcMultiplier() {
    uniform_real_distribution<float> dist(0.0f, 2.0f);
    return dist(rng);
}

void Combat::showStatus() const {
    int atkBonus = player.getEquippedAtkBonus();
    int defBonus = player.getEquippedDefBonus();

    cout << "\n--- " << player.getName()
         << " HP:" << player.getHp() << "/" << player.getHpMax()
         << " | ATK:" << player.getAtk();
    if (atkBonus > 0) cout << "(+" << atkBonus << ")";
    cout << " DEF:" << player.getDef();
    if (defBonus > 0) cout << "(+" << defBonus << "%)";
    cout << "  ||  " << monster.getName()
         << " HP:" << monster.getHp() << "/" << monster.getHpMax()
         << " Mercy:" << monster.getMercy() << "/" << monster.getMercyGoal()
         << " ---\n";
}

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
        if (combatOver) return true;

        if (!monster.isAlive()) {
            monster.setKilledResult(true);
            player.addKill();
            cout << monster.getName() << " est vaincu !\n";
            return true;
        }

        bool playerDead = monsterTurn();

        // ── Fin du tour : les bonus sont consommes ────────────────────────────
        if (bonusAtkTour > 0) {
            cout << "[Bonus ATK +" << bonusAtkTour << " expire en fin de tour.]\n";
            bonusAtkTour = 0;
        }
        if (bonusDefTour > 0) {
            cout << "[Bonus DEF +" << bonusDefTour << "% expire en fin de tour.]\n";
            bonusDefTour = 0;
        }

        if (playerDead) {
            cout << "Vous etes mort. Fin de partie.\n";
            return false;
        }
    }
    return false;
}

// ─── Tour du joueur ───────────────────────────────────────────────────────────
bool Combat::playerTurn() {
    // Rappel du bonus actif ce tour
    if (bonusAtkTour > 0 || bonusDefTour > 0) {
        cout << "  [Bonus ce tour : ATK +" << bonusAtkTour
             << " | DEF +" << bonusDefTour << "%]\n";
    }

    cout << "\n  FIGHT    ACT    ITEM    MERCY\n";
    cout << "  [1]      [2]    [3]     [4]\n> ";

    int choice = 0;
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    switch (choice) {
        case 1: return fight();
        case 2: doAct();   return false;
        case 3: useItem(); return false;
        case 4: return doMercy();
        default:
            cout << "Choix invalide.\n";
            return false;
    }
}

// ─── Tour du monstre ──────────────────────────────────────────────────────────
// Le bonus DEF protege ce tour, puis expire
bool Combat::monsterTurn() {
    int dmgBrut   = calcDamage(player.getHpMax());
    int reduction = dmgBrut * bonusDefTour / 100;
    int dmgFinal  = max(0, dmgBrut - reduction);

    player.takeDamage(dmgFinal);

    if (dmgFinal == 0)
        cout << monster.getName() << " rate son attaque !\n";
    else {
        cout << monster.getName() << " vous inflige " << dmgFinal << " degats";
        if (reduction > 0)
            cout << " (" << dmgBrut << " bruts - " << reduction << " (DEF+" << bonusDefTour << "%))";
        cout << ".\n";
        cout << "HP restants : " << player.getHp() << "/" << player.getHpMax() << "\n";
    }

    return !player.isAlive();
}

// ─── FIGHT : degats aleatoires + bonus ATK du tour ───────────────────────────
bool Combat::fight() {
    int dmgBase  = calcDamage(monster.getHpMax());
    int dmgTotal = dmgBase + bonusAtkTour;
    monster.takeDamage(dmgTotal);

    if (dmgTotal == 0)
        cout << "Vous ratez votre attaque !\n";
    else {
        cout << "Vous infligez " << dmgTotal << " degats";
        if (bonusAtkTour > 0)
            cout << " (" << dmgBase << " aleatoire + " << bonusAtkTour << " (ATK bonus))";
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

// ─── ITEM ─────────────────────────────────────────────────────────────────────
// L'effet est applique immediatement ; le bonus dure jusqu'a la fin du tour
void Combat::useItem() {
    player.displayInventory();
    cout << "Choisir un item (index, -1 pour annuler) > ";

    int idx = 0;
    cin >> idx;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (idx < 0) return;

    // On recupere le bonus genere par l'item et on l'ajoute au tour
    int atkAvant = bonusAtkTour;
    int defAvant = bonusDefTour;

    player.useItem(idx, player, bonusAtkTour, bonusDefTour);

    if (bonusAtkTour > atkAvant)
        cout << "  -> Bonus ATK +" << (bonusAtkTour - atkAvant)
             << " actif pour ce tour !\n";
    if (bonusDefTour > defAvant)
        cout << "  -> Bonus DEF +" << (bonusDefTour - defAvant)
             << "% actif pour ce tour !\n";
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

// ─── Calcul degats de base ────────────────────────────────────────────────────
int Combat::calcDamage(int maxHp) {
    uniform_int_distribution<int> dist(0, maxHp);
    return dist(rng);
}

// ─── Statut : affiche les bonus actifs ce tour ────────────────────────────────
void Combat::showStatus() const {
    cout << "\n--- " << player.getName()
         << " HP:" << player.getHp() << "/" << player.getHpMax()
         << " | ATK:" << player.getAtk();
    if (bonusAtkTour > 0) cout << "(+" << bonusAtkTour << ")";
    cout << " DEF:" << player.getDef();
    if (bonusDefTour > 0) cout << "(+" << bonusDefTour << "%)";
    cout << "  ||  " << monster.getName()
         << " HP:" << monster.getHp() << "/" << monster.getHpMax()
         << " Mercy:" << monster.getMercy() << "/" << monster.getMercyGoal()
         << " ---\n";
}

#include "Game.h"
#include "Combat.h"
#include "FileLoader.h"

#include <cstdlib>
#include <iostream>
#include <limits>
#include <random>
using namespace std;

static mt19937 gameRng(random_device{}());

Game::Game() : player("???") {}

Game::~Game() {
    for (Monster* m : monsters) delete m;
}

void Game::start() {
    cout << "=============================\n";
    cout << "       ALTERDUNE v1.0        \n";
    cout << "=============================\n";

    string nom;
    cout << "Entrez le nom de votre personnage : ";
    getline(cin, nom);
    player = Player(nom);

    FileLoader::loadItems("data/items.csv", player);
    monsters = FileLoader::loadMonsters("data/monsters.csv");

    if (monsters.empty()) {
        cerr << "[ERREUR] Aucun monstre charge. Verifiez monsters.csv.\n";
        return;
    }

    cout << "\nBienvenue, " << player.getName() << " !\n";
    cout << "HP : " << player.getHp() << "/" << player.getHpMax() << "\n";
    player.displayInventory();

    mainMenu();
}

void Game::mainMenu() {
    clearScreen();
    displayMenu();

    while (true) {
        cout << "> ";

        int choice = 0;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        clearScreen();
        displayMenu();

        switch (choice) {
            case 1:
                cout << "\n--- Combat ---\n";
                startCombat();
                break;
            case 2:
                cout << "\n--- Bestiaire ---\n";
                showBestiary();
                break;
            case 3:
                cout << "\n--- Statistiques ---\n";
                showStats();
                break;
            case 4:
                cout << "\n--- Items ---\n";
                showItems();
                break;
            case 5:
                cout << "Au revoir !\n";
                return;
            default:
                cout << "Choix invalide.\n";
                break;
        }

        if (checkEndGame()) {
            displayEnding();
            return;
        }
    }
}

void Game::clearScreen() const {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Game::displayMenu() const {
    cout << "=== Menu principal ===\n";
    cout << "[1] Demarrer un combat\n";
    cout << "[2] Bestiaire\n";
    cout << "[3] Statistiques\n";
    cout << "[4] Items\n";
    cout << "[5] Quitter\n";
}

void Game::startCombat() {
    uniform_int_distribution<int> dist(0, static_cast<int>(monsters.size()) - 1);
    Monster* modele = monsters[dist(gameRng)];
    Monster* combatant = modele->clone();

    Combat combat(player, *combatant, catalog);
    bool won = combat.run();

    if (won) {
        bestiary.add(combatant);
        clearScreen();
        displayMenu();
        cout << "\n--- Combat ---\n";
        cout << "Victoire ! Total : "
             << (player.getKills() + player.getSpared()) << "/10\n";
    } else {
        delete combatant;
        cout << "Defaite. La partie est terminee.\n";
        displayEnding();
        exit(0);
    }
}

void Game::showBestiary() {
    bestiary.display();
}

void Game::showStats() {
    player.displayStats();
}

void Game::showItems() {
    player.displayInventory();
    cout << "Utiliser un item ? (index ou -1 pour annuler) > ";

    int idx = -1;
    cin >> idx;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (idx < 0) {
        cout << "Annule.\n";
        return;
    }

    int bonusAtk = 0;
    int bonusDef = 0;
    if (player.useItem(idx, bonusAtk, bonusDef)) {
        if (bonusAtk > 0) {
            cout << "L'arme equipee sera prise en compte au prochain combat.\n";
        }
        if (bonusDef > 0) {
            cout << "L'armure equipee sera prise en compte au prochain combat.\n";
        }
    }
}

bool Game::checkEndGame() const {
    return (player.getKills() + player.getSpared()) >= 10;
}

void Game::displayEnding() const {
    int k = player.getKills();
    int s = player.getSpared();

    cout << "\n==============================\n";
    if (k == 0 && s > 0) {
        cout << "  FIN PACIFISTE\n  Vous n'avez tue personne.\n";
    } else if (s == 0 && k > 0) {
        cout << "  FIN GENOCIDAIRE\n  Aucune pitie.\n";
    } else {
        cout << "  FIN NEUTRE\n  Tues : " << k << " | Epargnes : " << s << "\n";
    }

    cout << "==============================\n";
    cout << "Merci d'avoir joue a ALTERDUNE !\n";
}

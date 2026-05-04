#include "Game.h"
#include "FileLoader.h"
#include "Combat.h"

#include <cstdlib>
#include <iostream>
#include <limits>
#include <random>

static std::mt19937 gameRng(std::random_device{}());

Game::Game() : player("???") {}

Game::~Game() {
    for (Monster* m : monsters) delete m;
}

void Game::start() {
    std::cout << "=============================\n";
    std::cout << "       ALTERDUNE v1.0        \n";
    std::cout << "=============================\n";

    std::string nom;
    std::cout << "Entrez le nom de votre personnage : ";
    std::getline(std::cin, nom);
    player = Player(nom);

    FileLoader::loadItems("data/items.csv", player);
    monsters = FileLoader::loadMonsters("data/monsters.csv");

    if (monsters.empty()) {
        std::cerr << "[ERREUR] Aucun monstre charge. Verifiez monsters.csv.\n";
        return;
    }

    std::cout << "\nBienvenue, " << player.getName() << " !\n";
    std::cout << "HP : " << player.getHp() << "/" << player.getHpMax() << "\n";
    player.displayInventory();

    mainMenu();
}

void Game::mainMenu() {
    clearScreen();
    displayMenu();

    while (true) {
        std::cout << "> ";

        int choice = 0;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        clearScreen();
        displayMenu();

        switch (choice) {
            case 1:
                std::cout << "\n--- Combat ---\n";
                startCombat();
                break;
            case 2:
                std::cout << "\n--- Bestiaire ---\n";
                showBestiary();
                break;
            case 3:
                std::cout << "\n--- Statistiques ---\n";
                showStats();
                break;
            case 4:
                std::cout << "\n--- Items ---\n";
                showItems();
                break;
            case 5:
                std::cout << "Au revoir !\n";
                return;
            default:
                std::cout << "Choix invalide.\n";
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
    std::system("cls");
#else
    std::system("clear");
#endif
}

void Game::displayMenu() const {
    std::cout << "=== Menu principal ===\n";
    std::cout << "[1] Demarrer un combat\n";
    std::cout << "[2] Bestiaire\n";
    std::cout << "[3] Statistiques\n";
    std::cout << "[4] Items\n";
    std::cout << "[5] Quitter\n";
}

void Game::startCombat() {
    std::uniform_int_distribution<int> dist(0, static_cast<int>(monsters.size()) - 1);
    Monster* m = monsters[dist(gameRng)];

    Combat combat(player, *m, catalog);
    bool won = combat.run();

    if (won) {
        bestiary.add(m);
        clearScreen();
        displayMenu();
        std::cout << "\n--- Combat ---\n";
        std::cout << "Victoire ! Total : "
                  << (player.getKills() + player.getSpared()) << "/10\n";
    } else {
        std::cout << "Defaite. La partie est terminee.\n";
        displayEnding();
        std::exit(0);
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
    std::cout << "Utiliser un item ? (index ou -1 pour annuler) > ";

    int idx;
    std::cin >> idx;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    int dummyAtk = 0;
    int dummyDef = 0;
    if (idx >= 0) player.useItem(idx, player, dummyAtk, dummyDef);
}

bool Game::checkEndGame() const {
    return (player.getKills() + player.getSpared()) >= 10;
}

void Game::displayEnding() const {
    int k = player.getKills();
    int s = player.getSpared();

    std::cout << "\n==============================\n";
    if (k == 0 && s > 0) {
        std::cout << "  FIN PACIFISTE\n  Vous n'avez tue personne.\n";
    } else if (s == 0 && k > 0) {
        std::cout << "  FIN GENOCIDAIRE\n  Aucune pitie.\n";
    } else {
        std::cout << "  FIN NEUTRE\n  Tues : " << k << " | Epargnes : " << s << "\n";
    }

    std::cout << "==============================\n";
    std::cout << "Merci d'avoir joue a ALTERDUNE !\n";
}

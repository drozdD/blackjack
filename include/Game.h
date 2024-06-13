#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include "API.h"
#include "Player.h"
#include "Dealer.h"

using namespace std;

class EXPORT_API Game {
public:
    struct card {
        sf::Texture texture;
        int value;
    };

    Game();
    static map<string, sf::Texture> btnsTxt;
    static map<int, card> cards;
    static vector<int> liczby;
    map<int, Game::card> loadCards();
    map<string, sf::Texture> loadButtons();
    void drawBoardGame(sf::RenderWindow& window, Player& player, Dealer& dealer);
    void reset(sf::RenderWindow& window, int bank);
    static bool asDealer;
    static bool asPlayer;
};

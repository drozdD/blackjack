#include "Game.h"
#include "Player.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <algorithm>

// Inicjalizacja statycznych pól cards i btnsTxt
map<int, Game::card> Game::cards;
map<string, sf::Texture> Game::btnsTxt;
vector<int> Game::liczby;
bool Game::asDealer = 0;
bool Game::asPlayer = 0;


map<int, Game::card> Game::loadCards() {
    const std::string textureNames[] = {
        "2_of_clubs", "2_of_diamonds", "2_of_hearts", "2_of_spades",
        "3_of_clubs", "3_of_diamonds", "3_of_hearts", "3_of_spades",
        "4_of_clubs", "4_of_diamonds", "4_of_hearts", "4_of_spades",
        "5_of_clubs", "5_of_diamonds", "5_of_hearts", "5_of_spades",
        "6_of_clubs", "6_of_diamonds", "6_of_hearts", "6_of_spades",
        "7_of_clubs", "7_of_diamonds", "7_of_hearts", "7_of_spades",
        "8_of_clubs", "8_of_diamonds", "8_of_hearts", "8_of_spades",
        "9_of_clubs", "9_of_diamonds", "9_of_hearts", "9_of_spades",
        "10_of_clubs", "10_of_diamonds", "10_of_hearts", "10_of_spades",
        "ace_of_clubs", "ace_of_diamonds", "ace_of_hearts", "ace_of_spades",
        "jack_of_clubs", "jack_of_diamonds", "jack_of_hearts", "jack_of_spades",
        "king_of_clubs", "king_of_diamonds", "king_of_hearts", "king_of_spades",
        "queen_of_clubs", "queen_of_diamonds", "queen_of_hearts", "queen_of_spades"
    };

    const int cardValues[] = {
        2, 2, 2, 2,
        3, 3, 3, 3,
        4, 4, 4, 4,
        5, 5, 5, 5,
        6, 6, 6, 6,
        7, 7, 7, 7,
        8, 8, 8, 8,
        9, 9, 9, 9,
        10, 10, 10, 10,
        11, 11, 11, 11,  // Ace can be 11 or 1, here we assume 11
        10, 10, 10, 10,  // Face cards
        10, 10, 10, 10,
        10, 10, 10, 10
    };

    std::map<int, card> cards;

    for (int i = 0; i < 52; ++i) {
        sf::Texture texture;
        if (!texture.loadFromFile("images/" + textureNames[i] + ".png")) {
            std::cerr << "Error loading texture: " << textureNames[i] << std::endl;
            continue;
        }
        cards[i] = { texture, cardValues[i] };
    }

    return cards;
}

map<string, sf::Texture> Game::loadButtons() {
    const std::string textureNames[] = {
        "cardBack", "bg", "reset",
        "1", "10", "100", "1000", "max",
        "double", "hit", "stand", "place", "deck", "win", "lose", "again", "draw"
    };

    std::map<std::string, sf::Texture> textures;

    for (int i = 0; i < 17; ++i) {
        sf::Texture texture;
        if (!texture.loadFromFile("images/" + textureNames[i] + ".png")) {
            std::cerr << "Error loading texture: " << textureNames[i] << std::endl;
            continue;
        }
        textures[textureNames[i]] = texture;
    }

    return textures;
}

Game::Game() {
    if (Game::cards.empty() || Game::btnsTxt.empty()) {
        Game::btnsTxt = loadButtons();
        Game::cards = loadCards();
    }

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0, 51);

    while (liczby.size() < 15) {
        int num = dist(mt);
        if (std::find(liczby.begin(), liczby.end(), num) == liczby.end()) {
            liczby.push_back(num);
        }
    }

    std::random_shuffle(liczby.begin(), liczby.end());

    

}

void Game::drawBoardGame(sf::RenderWindow& window, Player& player, Dealer& dealer) {
    sf::Font font;
    try {
        if (!font.loadFromFile("fonts/casino.ttf")) {
            throw std::runtime_error("Error loading font");
        }
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return;
    }
    window.clear(sf::Color::Black);

    sf::Sprite backgroundSprite(Game::btnsTxt["bg"]);
    window.draw(backgroundSprite);

    sf::Sprite deck(Game::btnsTxt["deck"]);
    deck.setPosition(40, 40);
    window.draw(deck);

    string bank = "BANK: " + to_string(player.getBank());
    sf::Text text;
    text.setFont(font);  // Ustaw czcionkê
    text.setString(bank);  // Ustaw tekst
    text.setCharacterSize(60);  // Ustaw rozmiar tekstu (w punktach, nie pikselach!)
    text.setFillColor(sf::Color::White);  // Ustaw kolor tekstu
    text.setPosition(40, 400);
    window.draw(text);

    string bet = "BET: " + to_string(player.getBet());
    text.setString(bet);
    text.setPosition(40, 450);
    window.draw(text);

    sf::Sprite btnReset(Game::btnsTxt["reset"]);
    btnReset.setPosition(70, 530);
    window.draw(btnReset);

    sf::Sprite btn1(Game::btnsTxt["1"]);
    btn1.setPosition(70, 590);
    window.draw(btn1);

    sf::Sprite btn10(Game::btnsTxt["10"]);
    btn10.setPosition(70, 650);
    window.draw(btn10);

    sf::Sprite btn100(Game::btnsTxt["100"]);
    btn100.setPosition(70, 710);
    window.draw(btn100);

    sf::Sprite btn1000(Game::btnsTxt["1000"]);
    btn1000.setPosition(70, 770);
    window.draw(btn1000);

    sf::Sprite btnMax(Game::btnsTxt["max"]);
    btnMax.setPosition(70, 830);
    window.draw(btnMax);

    text.setString("PLAYER");
    text.setCharacterSize(30);
    text.setPosition(580, 860);
    window.draw(text);

    text.setString("DEALER");
    text.setPosition(580, 0);
    window.draw(text);

    cout << player.getState() << endl;
    cout << "DEALER: " << dealer.points << endl;
    cout << "PLAYER: " << player.points << endl;

    for (int i = 0; i < player.cards.size(); i++) {
        sf::Sprite card(Game::cards[player.cards[i]].texture);
        card.setPosition(300 + i * 50, 530);
        window.draw(card);
    }

    if (player.getState() == "remis") {
        player.setBank(player.getBank() + player.getBet());
        player.setBet(0);
        for (int i = 0; i < dealer.cards.size(); i++) {
            sf::Sprite card(Game::cards[dealer.cards[i]].texture);
            card.setPosition(300 + i * 50, 40);
            window.draw(card);
        }
        for (int i = 0; i < player.cards.size(); i++) {
            sf::Sprite card(Game::cards[player.cards[i]].texture);
            card.setPosition(300 + i * 50, 530);
            window.draw(card);
        }

        sf::Sprite deck(Game::btnsTxt["draw"]);
        deck.setPosition(500, 300);
        window.draw(deck);

        sf::Sprite again(Game::btnsTxt["again"]);
        again.setPosition(850, 350);
        window.draw(again);
        text.setString("PLAY AGAIN");
        text.setCharacterSize(50);
        text.setPosition(840, 570);
        window.draw(text);
        dealer.points = 0;
        Game::asDealer = 0;
        Game::asPlayer = 0;
        Game();
    }

    if (player.getState() == "loss") {
        player.setBet(0);
        for (int i = 0; i < dealer.cards.size(); i++) {
            sf::Sprite card(Game::cards[dealer.cards[i]].texture);
            card.setPosition(300 + i * 50, 40);
            window.draw(card);
        }
        for (int i = 0; i < player.cards.size(); i++) {
            sf::Sprite card(Game::cards[player.cards[i]].texture);
            card.setPosition(300 + i * 50, 530);
            window.draw(card);
        }
        sf::Sprite deck(Game::btnsTxt["lose"]);
        deck.setPosition(500, 300);
        window.draw(deck);

        sf::Sprite again(Game::btnsTxt["again"]);
        again.setPosition(850, 350);
        window.draw(again);
        text.setString("PLAY AGAIN");
        text.setCharacterSize(50);
        text.setPosition(840, 570);
        window.draw(text);
        dealer.points = 0;
        Game::asDealer = 0;
        Game::asPlayer = 0;
        Game();
    }

    if (player.getState() == "win") {
        player.setBank(player.getBank() + player.getBet() * 2);
        player.setBet(0);
        for (int i = 0; i < dealer.cards.size(); i++) {
            sf::Sprite card(Game::cards[dealer.cards[i]].texture);
            card.setPosition(300 + i * 50, 40);
            window.draw(card);
        }
        for (int i = 0; i < player.cards.size(); i++) {
            sf::Sprite card(Game::cards[player.cards[i]].texture);
            card.setPosition(300 + i * 50, 530);
            window.draw(card);
        }
        sf::Sprite deck(Game::btnsTxt["win"]);
        deck.setPosition(500, 300);
        window.draw(deck);
        sf::Sprite again(Game::btnsTxt["again"]);
        again.setPosition(860, 350);
        window.draw(again);
        text.setString("PLAY AGAIN");
        text.setCharacterSize(50);
        text.setPosition(840, 570);
        window.draw(text);
        dealer.points = 0;
        Game::asDealer = 0;
        Game::asPlayer = 0;
        Game();
    }

    if (player.getState() == "betting") {
        sf::Sprite place(Game::btnsTxt["place"]);
        place.setPosition(560, 350);
        window.draw(place);
    }
    if (player.getState() == "hit") {
        player.cards.push_back(Game::liczby[player.cards.size() + dealer.cards.size() + 1]);
        player.points = 0;
        for (int i = 0; i < player.cards.size(); i++) {
            player.points += Game::cards[player.cards[i]].value;
            //if (Game::cards[player.cards[i]].value == 11) Game::asPlayer = 1;
            sf::Sprite card(Game::cards[player.cards[i]].texture);
            card.setPosition(300 + i * 50, 530);
            window.draw(card);
        }
        if (player.points == 21) {
            player.setState("win");
            cout << "WIN" << endl;
        }
        if (player.points > 21) {
            //if (Game::asPlayer) {
             //   for (int i = 0; i < player.cards.size(); i++) {
             //       if (Game::cards[player.cards[i]].value == 11) Game::cards[player.cards[i]].value = 1;
             //   }
            //}else 
            player.setState("loss");
        }
        cout << player.points << endl;
        player.setState("playing");
    }
    if (player.getState() == "stand") {
       
        for (int i = 0; i < dealer.cards.size(); i++) {
            sf::Sprite card(Game::cards[dealer.cards[i]].texture);
            card.setPosition(300 + i * 50, 40);
            window.draw(card);
        }
        if (dealer.points <= 16) {
            dealer.cards.push_back(Game::liczby[player.cards.size() + dealer.cards.size() + 1]);
            //if (Game::cards[dealer.cards[dealer.cards.size() - 1]].value = 11) Game::asDealer = 1;
            dealer.points = dealer.points + Game::cards[dealer.cards[dealer.cards.size() - 1]].value;
        }
        else if (dealer.points > player.points && dealer.points <= 21) {
            player.setState("loss");
        }
        else if (dealer.points > 21) {
           // if (Game::asDealer) {
             //   for (int i = 0; i < dealer.cards.size(); i++) {
              //      if (Game::cards[dealer.cards[i]].value == 11) Game::cards[dealer.cards[i]].value = 1;
              //      Game::asDealer = 0;
            //    }
            //}else 
            player.setState("win");
        }
        else if (player.points > dealer.points) {
            player.setState("win");
        }
        else if (player.points == dealer.points) {
            player.setState("remis");
        }
           
         
        for (int i = 0; i < dealer.cards.size(); i++) {
            sf::Sprite card(Game::cards[dealer.cards[i]].texture);
            card.setPosition(300 + i * 50, 40);
            window.draw(card);
        }
    }
    if (player.getState() == "playing") {
        sf::Sprite hit(Game::btnsTxt["hit"]);
        hit.setPosition(600, 350);
        window.draw(hit);

        sf::Sprite dbl(Game::btnsTxt["double"]);
        dbl.setPosition(350, 350);
        window.draw(dbl);

        sf::Sprite stand(Game::btnsTxt["stand"]);
        stand.setPosition(720, 350);
        window.draw(stand);

        if (player.cards.empty()) {
            dealer.cards.clear();
            player.cards.push_back(Game::liczby[0]);
            dealer.cards.push_back(Game::liczby[1]);
            dealer.cards.push_back(Game::liczby[2]);
        }
        player.points = 0;
        for (int i = 0; i < player.cards.size(); i++) {
            player.points += Game::cards[player.cards[i]].value;
            sf::Sprite card(Game::cards[player.cards[i]].texture);
            card.setPosition(300 + i * 50, 530);
            window.draw(card);
        }

        sf::Sprite card(Game::cards[dealer.cards[0]].texture);
        card.setPosition(300 + 0 * 100, 40);
        window.draw(card);

        sf::Sprite card1(Game::btnsTxt["cardBack"]);
        card1.setPosition(300 + 100, 40);
        window.draw(card1);
        
        dealer.points = 0;
        for (int i = 0; i < dealer.cards.size(); i++) {
            dealer.points += Game::cards[dealer.cards[i]].value;
            //if (Game::cards[dealer.cards[i]].value == 11) Game::asDealer = 1;
        }

        if (dealer.points == 21) {
            for (int i = 0; i < dealer.cards.size(); i++) {
                sf::Sprite card(Game::cards[dealer.cards[i]].texture);
                card.setPosition(300 + i * 50, 40);
                window.draw(card);
            }
            player.setState("loss");
        }
        if (player.points > 21) {
            for (int i = 0; i < dealer.cards.size(); i++) {
                sf::Sprite card(Game::cards[dealer.cards[i]].texture);
                card.setPosition(300 + i * 50, 40);
                window.draw(card);
            }
            player.setState("loss");
        }
        if (player.points == 21) {
            for (int i = 0; i < dealer.cards.size(); i++) {
                sf::Sprite card(Game::cards[dealer.cards[i]].texture);
                card.setPosition(300 + i * 50, 40);
                window.draw(card);
            }
            player.setBet(player.getBet() * 1.5);
            player.setState("win");
        }
    }
}

void Game::reset(sf::RenderWindow& window, int bank) {
    sf::IntRect txtRect(40, 400, 1300, 60);
    sf::Sprite kawalek(this->btnsTxt["bg"], txtRect);
    kawalek.setPosition(40, 400);
    window.draw(kawalek);
}

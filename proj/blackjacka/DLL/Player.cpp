#include "Player.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <exception>

using namespace std;

Player::Player() {
    string inputFileName = "balance.txt";
    try {
        std::ifstream inputFile(inputFileName);
        if (!inputFile.is_open()) {
            throw std::runtime_error("Nie mo¿na otworzyæ pliku " + inputFileName);
        }
        inputFile >> this->bank;
        inputFile.close();

        if (inputFile.fail()) {
            throw std::runtime_error("B³¹d podczas wczytywania liczby z pliku " + inputFileName);
        }

        this->bet = 0;
        this->points = 0;
        this->state = "betting";
        clickableAreas["reset"] = sf::IntRect(70, 530, 50, 50);
        clickableAreas["area1"] = sf::IntRect(70, 590, 50, 50);
        clickableAreas["area10"] = sf::IntRect(70, 650, 50, 50);
        clickableAreas["area100"] = sf::IntRect(70, 710, 50, 50);
        clickableAreas["area1000"] = sf::IntRect(70, 770, 50, 50);
        clickableAreas["max"] = sf::IntRect(70, 830, 50, 50);
        clickableAreas["place"] = sf::IntRect(560, 350, 180, 60);
        clickableAreas["double"] = sf::IntRect(350, 350, 230, 60);
        clickableAreas["hit"] = sf::IntRect(600, 350, 100, 60);
        clickableAreas["stand"] = sf::IntRect(720, 350, 180, 60);
        clickableAreas["again"] = sf::IntRect(650, 300, 300, 300);
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

void Player::setState(string state) {
    this->state = state;
}

string Player::getState() {
    return this->state;
}

void Player::setBank(int bank) {
    this->bank = bank;
}

int Player::getBank() {
    return this->bank;
}

void Player::setBet(int bet) {
    this->bet = bet;
}

int Player::getBet() {
    return this->bet;
}

void Player::handleBetting(const sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (this->state == "betting") {
            if (this->clickableAreas["reset"].contains(mousePos)) {
                this->bank = this->bank + this->bet;
                this->bet = 0;
            }
            if (this->clickableAreas["area1"].contains(mousePos) && this->bank >= 1) {
                this->bet = this->bet + 1;
                this->bank--;
            }
            if (this->clickableAreas["area10"].contains(mousePos) && this->bank >= 10) {
                this->bet = this->bet + 10;
                this->bank = this->bank - 10;
            }
            if (this->clickableAreas["area100"].contains(mousePos) && this->bank >= 100) {
                this->bet = this->bet + 100;
                this->bank = this->bank - 100;
            }
            if (this->clickableAreas["area1000"].contains(mousePos) && this->bank >= 1000) {
                this->bet = this->bet + 1000;
                this->bank = this->bank - 1000;
            }
            if (this->clickableAreas["max"].contains(mousePos)) {
                this->bet = this->bet + this->bank;
                this->bank = 0;
            }
            if (this->clickableAreas["place"].contains(mousePos)) {
                this->state = "playing";
            }
        }
        else if (this->state == "playing") {
            if (this->clickableAreas["double"].contains(mousePos) && this->bank >= this->bet) {
                this->bank = this->bank - this->bet;
                this->bet = this->bet * 2;
                this->state = "hit";
            }
            if (this->clickableAreas["hit"].contains(mousePos)) {
                this->state = "hit";
            }
            if (this->clickableAreas["stand"].contains(mousePos)) {
                this->state = "stand";
            }
        }
        else if (this->state == "win" || this->state == "loss" || this->state == "remis") {
            try {
                std::ofstream outputFile("balance.txt");
                if (!outputFile.is_open()) {
                    throw std::runtime_error("Nie mo¿na otworzyæ pliku balance.txt");
                }
                outputFile << this->bank;
                outputFile.close();

                if (this->clickableAreas["again"].contains(mousePos)) {
                    this->bet = 0;
                    this->points = 0;
                    this->state = "betting";
                    this->cards.clear();
                    Player();
                }
            }
            catch (const std::exception& e) {
                std::cerr << e.what() << std::endl;
            }
        }
    }
}

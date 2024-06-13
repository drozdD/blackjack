#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include "API.h"
#include <vector>

using namespace std;

class EXPORT_API Player {
private:
	string state;
	int bank;
	int bet;
	std::map<std::string, sf::IntRect> clickableAreas;
public:
	Player();
	void setState(string state);
	string getState();
	void setBank(int bank);
	int getBank();
	void setBet(int bet);
	int getBet();
	void handleBetting(const sf::Event& event,sf::RenderWindow& window);
	vector<int> cards;
	int points;
};
#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include "API.h"
#include <vector>

using namespace std;

class EXPORT_API Dealer {
private:
	string state;
public:
	Dealer();
	vector<int> cards;
	int points;
};
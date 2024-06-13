#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "Dealer.h"

using namespace std;

Dealer::Dealer() {
	this->state = "chill";
	this->points = 0;
}
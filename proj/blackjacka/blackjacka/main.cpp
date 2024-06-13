#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Player.h"
#include "Dealer.h"

using namespace sf;

int main()
{
	RenderWindow window(VideoMode(1300, 900), "BLACKJACK!");

	Game game;
	Player player;
	Dealer dealer;
	
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			player.handleBetting(event, window);
		}

		game.drawBoardGame(window, player, dealer);
		window.display();
	}
	return 0;
}
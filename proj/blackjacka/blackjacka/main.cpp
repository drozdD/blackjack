#include <SFML/Graphics.hpp>

using namespace sf;

int main()
{
	RenderWindow window(VideoMode(1300, 900), "Hello SFML!");

	Texture backgroundTexture, t1;
	if (!backgroundTexture.loadFromFile("images/background1.png")) {
		return -1; // Obs³uga b³êdu ³adowania
	}

	t1.loadFromFile("images/2_of_clubs.png");
	Sprite two_of_clubs(t1);

	Sprite backgroundSprite;
	backgroundSprite.setTexture(backgroundTexture);

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear(Color::Magenta);
		window.draw(backgroundSprite);
		window.draw(two_of_clubs);
		window.display();
	}
	return 0;
}
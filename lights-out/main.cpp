#include <SFML/Graphics.hpp>

int main()
{
	
	sf::RenderWindow window(sf::VideoMode(800, 600), "Lights Out");

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{ 
				window.close();
			}
			if (event.type == sf::Event::KeyPressed)
			{
				window.close();
			}
		}

		window.clear();
		//window.draw(shape);
		window.display();
	}

	return EXIT_SUCCESS;
}

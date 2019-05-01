#include <SFML/Graphics.hpp>

#include <iostream>

int main()
{

    sf::Texture slowCarTexture;
    sf::Texture fastCarTexture;

    slowCarTexture.loadFromFile("car-1.png");
    fastCarTexture.loadFromFile("car-2.png");

    sf::Sprite slowCarSprite(slowCarTexture);
    sf::Sprite fastCarSprite(fastCarTexture);

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Race Math", sf::Style::Fullscreen);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if ((event.type == sf::Event::Closed) || (event.type == sf::Event::KeyPressed)
                || (event.type == sf::Event::MouseButtonPressed))
            {
                window.close();
            }
        }

        window.clear();
        window.draw(slowCarSprite);
        window.draw(fastCarSprite);
        window.display();
    }

    return 0;
}

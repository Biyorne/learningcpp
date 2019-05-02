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

    const sf::Vector2f WINDOW_SIZE(window.getSize());

    slowCarSprite.setPosition(
        (WINDOW_SIZE.x * 0.25f),
        (fastCarSprite.getGlobalBounds().top + fastCarSprite.getGlobalBounds().height));

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

        slowCarSprite.move(sf::Vector2f(1.0f, 0.0f));
        fastCarSprite.move(sf::Vector2f(1.0f, 0.0f));

        if (slowCarSprite.getPosition().x > WINDOW_SIZE.x)
        {
            slowCarSprite.setPosition(
                -slowCarSprite.getGlobalBounds().width, slowCarSprite.getPosition().y);
        }

        if (fastCarSprite.getPosition().x > WINDOW_SIZE.x)
        {
            fastCarSprite.setPosition(
                -fastCarSprite.getGlobalBounds().width, fastCarSprite.getPosition().y);
        }
    }

    return 0;
}

#include <SFML/Graphics.hpp>

#include <cmath>
#include <iostream>

int main()
{
    sf::Texture lazyMethHeadTexture;
    sf::Texture greedyMethHeadTexture;
    sf::Texture lootCheapTexture;
    sf::Texture lootValuableTexture;

    lazyMethHeadTexture.loadFromFile("head-1.png");
    greedyMethHeadTexture.loadFromFile("head-2.png");
    lootCheapTexture.loadFromFile("loot.png");

    sf::Sprite lazyMethHeadSprite(lazyMethHeadTexture);
    sf::Sprite greedyMethHeadSprite(greedyMethHeadTexture);
    sf::Sprite lootCheapSprite(lootCheapTexture);

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Race Math", sf::Style::Fullscreen);

    const sf::Vector2f WINDOW_SIZE(window.getSize());

    const sf::Vector2f lazyMethHeadPos(
        (WINDOW_SIZE.x * 0.25f),
        (greedyMethHeadSprite.getGlobalBounds().top
         + greedyMethHeadSprite.getGlobalBounds().height));

    const sf::Vector2f lootCheapPos(
        0,
        (greedyMethHeadSprite.getGlobalBounds().top
         + greedyMethHeadSprite.getGlobalBounds().height));

    const sf::Vector2f lootValuablePos((WINDOW_SIZE.x * 0.25f), 0);

    const sf::Vector2f greedyMethHeadStartPos;

    lazyMethHeadSprite.setPosition(lazyMethHeadPos);
    lootCheapSprite.setPosition(lootCheapPos);

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
        window.draw(lazyMethHeadSprite);
        window.draw(greedyMethHeadSprite);
        window.draw(lootCheapSprite);
        window.display();
    }
}

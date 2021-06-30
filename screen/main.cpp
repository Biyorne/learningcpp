#include "bloom-shader.hpp"
#include "random.hpp"

#include <iostream>

#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(2880, 1800), "Screen", sf::Style::Fullscreen);

    util::BloomEffectHelper bloomWindow(window);
    bloomWindow.isEnabled(true);
    bloomWindow.blurMultipassCount(5);
    if (!bloomWindow.isEnabled())
    {
        std::cerr << "Bloom effect not supported on this video card.  Bail." << std::endl;
    }

    sf::Font font;
    if (!font.loadFromFile("Giovedi.ttf"))
    {
        return EXIT_FAILURE;
    }

    sf::Text text("Hello SFML", font, 90);
    text.setPosition(100.0f, 100.0f);

    float letterSpacing = 1.0f;

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
                if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                }
            }
        }

        bloomWindow.clear();

        bloomWindow.draw(text);
        letterSpacing *= 1.001f;
        text.setLetterSpacing(letterSpacing);
        text.move(0.1f, 0.0f);

        bloomWindow.display();
    }

    return EXIT_SUCCESS;
}

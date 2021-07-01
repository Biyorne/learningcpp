#include "bloom-shader.hpp"
#include "color-range.hpp"
#include "random.hpp"
#include "slider-color.hpp"

#include <iostream>

#include <SFML/Graphics.hpp>

int main()
{
    util::Random random;

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

    util::ColorSlider bgColorSlider(
        sf::Color::Black,
        colors::random(random),
        random.fromTo(0.1f, 1.5f),
        util::WillOscillate::No,
        util::WillAutoStart::Yes);

    sf::Clock frameClock;

    while (window.isOpen())
    {
        const float frameTimeSec = frameClock.restart().asSeconds();

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

        const bool IS_BG_COLOR_CHANGING = bgColorSlider.updateAndReturnIsMoving(frameTimeSec);

        if (!IS_BG_COLOR_CHANGING)
        {
            std::cout << "changing color" << std::endl;
            const auto CURRENT_COLOR = bgColorSlider.value();

            bgColorSlider = util::ColorSlider(
                CURRENT_COLOR,
                colors::random(random),
                random.fromTo(0.1f, 1.5f),
                util::WillOscillate::No,
                util::WillAutoStart::Yes);
        }

        bloomWindow.clear(bgColorSlider.value());

        bloomWindow.draw(text);
        letterSpacing *= 1.001f;
        text.setLetterSpacing(letterSpacing);
        text.move(0.1f, 0.0f);

        bloomWindow.display();
    }

    return EXIT_SUCCESS;
}

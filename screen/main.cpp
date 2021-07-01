#include "bloom-shader.hpp"
#include "color-range.hpp"
#include "random.hpp"
#include "slider-color.hpp"
#include "slider-position.hpp"

#include <iostream>

#include <SFML/Graphics.hpp>

//
sf::Vector2f randomWindowPos(const util::Random & random, const sf::RenderWindow & window)
{
    return { static_cast<float>(random.zeroTo(window.getSize().x)),
             static_cast<float>(random.zeroTo(window.getSize().y)) };
}

//
util::ColorSlider makeBgColorSlider(const util::Random & random, const sf::Color & startColor)
{
    return util::ColorSlider(
        startColor,
        colors::random(random),
        random.fromTo(0.1f, 1.5f),
        util::WillOscillate::No,
        util::WillAutoStart::Yes);
}

//
util::PosSlider makeTextPosSlider(
    const util::Random & random, const sf::RenderWindow & window, const sf::Vector2f & startPos)
{
    return util::PosSlider(
        startPos,
        randomWindowPos(random, window),
        random.fromTo(1.0f, 5.0f),
        util::WillOscillate::No,
        util::WillAutoStart::Yes);
}

//
int main()
{
    util::Random random;

    sf::RenderWindow window(sf::VideoMode(1600, 1200), "Screen", sf::Style::Fullscreen);
    window.setFramerateLimit(60);

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
    util::setOriginToCenter(text);

    float letterSpacing = 1.0f;

    util::ColorSlider bgColorSlider = makeBgColorSlider(random, sf::Color::Black);

    util::PosSlider textPositionSlider = makeTextPosSlider(random, window, {});

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

        // background color
        if (!bgColorSlider.updateAndReturnIsMoving(frameTimeSec))
        {
            bgColorSlider = makeBgColorSlider(random, bgColorSlider.value());
        }

        // text position
        if (!textPositionSlider.updateAndReturnIsMoving(frameTimeSec))
        {
            textPositionSlider = makeTextPosSlider(random, window, textPositionSlider.value());
        }
        text.setPosition(textPositionSlider.value());

        // text
        // letterSpacing *= 1.001f;
        // text.setLetterSpacing(letterSpacing);

        // bloomWindow.clear(bgColorSlider.value());
        bloomWindow.draw(text);
        bloomWindow.display();
    }

    return EXIT_SUCCESS;
}

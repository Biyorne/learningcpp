#include "bloom-shader.hpp"
#include "color-range.hpp"
#include "random.hpp"
#include "slider-color.hpp"
#include "slider-position.hpp"

#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>

//
struct Context
{
    Context()
        : random()
        , window(sf::VideoMode(1600, 1200), "Screen", sf::Style::Fullscreen)
        , bloomWindow(window)
    {
        window.setFramerateLimit(60);

        bloomWindow.isEnabled(true);
        bloomWindow.blurMultipassCount(5);
        if (!bloomWindow.isEnabled())
        {
            std::cerr << "Bloom effect not supported on this video card.  Bail." << std::endl;
            window.close();
        }
    }

    util::Random random;
    sf::RenderWindow window;
    util::BloomEffectHelper bloomWindow;
};

//
class ScreenSlider : public util::PosSlider
{
  public:
    ScreenSlider(const sf::Vector2f & FROM, const sf::Vector2f & TO, const float SPEED)
        : PosSlider(FROM, TO, SPEED, util::WillOscillate::No, util::WillAutoStart::Yes, 0)
    {}

    virtual ~ScreenSlider() = default;

    void updateAndLoopIfStopped(
        const float FRAME_TIME_SEC, const sf::Vector2f & TO, const float SPEED)
    {
        if (!updateAndReturnIsMoving(FRAME_TIME_SEC))
        {
            from_ = to_;
            to_ = TO;
            speed(SPEED);
            resetFrom();
        }
    }
};

//
sf::Vector2f randomWindowPos(const Context & context)
{
    return { static_cast<float>(context.random.zeroTo(context.window.getSize().x)),
             static_cast<float>(context.random.zeroTo(context.window.getSize().y)) };
}

//
util::ColorSlider makeBgColorSlider(const Context & context, const sf::Color & startColor)
{
    return util::ColorSlider(
        startColor,
        colors::random(context.random),
        context.random.fromTo(0.1f, 1.5f),
        util::WillOscillate::No,
        util::WillAutoStart::Yes);
}

//
float randomTextSpeed(const Context & context) { return context.random.fromTo(0.5f, 1.75f); }
float randomLineSpeed(const Context & context) { return context.random.fromTo(0.1f, 2.0f); }

//
ScreenSlider makeRandomPosSlider(const Context & context, const sf::Vector2f & startPos)
{
    return ScreenSlider(startPos, randomWindowPos(context), context.random.fromTo(1.0f, 2.0f));
}

//
int main()
{
    Context context;

    sf::Font font;
    if (!font.loadFromFile("Giovedi.ttf"))
    {
        std::cerr << "Failed to load font Giovedi.ttf.  Bail." << std::endl;
        return EXIT_FAILURE;
    }

    sf::Text text("Hello SFML", font, 90);
    text.setPosition(100.0f, 100.0f);
    util::setOriginToCenter(text);

    util::ColorSlider bgColorSlider = makeBgColorSlider(context, sf::Color::Black);

    ScreenSlider textPositionSlider({}, randomWindowPos(context), randomTextSpeed(context));

    const std::size_t lineCount = 100;
    std::vector<ScreenSlider> linePosSliders;
    for (std::size_t i(0); i < (lineCount * 2); ++i)
    {
        linePosSliders.push_back(makeRandomPosSlider(context, {}));
    }

    std::vector<sf::Vertex> lineVerts;
    lineVerts.resize(lineCount * 2);

    sf::Clock frameClock;

    while (context.window.isOpen())
    {
        const float frameTimeSec = frameClock.restart().asSeconds();

        sf::Event event;
        while (context.window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                context.window.close();
            }

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    context.window.close();
                }
            }
        }

        // background color
        if (!bgColorSlider.updateAndReturnIsMoving(frameTimeSec))
        {
            bgColorSlider = makeBgColorSlider(context, bgColorSlider.value());
        }

        // text position
        textPositionSlider.updateAndLoopIfStopped(
            frameTimeSec, randomWindowPos(context), randomTextSpeed(context));

        text.setPosition(textPositionSlider.value());

        // lines end-point positions
        for (std::size_t i(0); i < (lineCount * 2); ++i)
        {
            linePosSliders[i].updateAndLoopIfStopped(
                frameTimeSec, randomWindowPos(context), randomLineSpeed(context));

            lineVerts[i].color = bgColorSlider.value();
            lineVerts[i].position = linePosSliders[i].value();

            ++i;

            linePosSliders[i].updateAndLoopIfStopped(
                frameTimeSec, randomWindowPos(context), randomLineSpeed(context));

            lineVerts[i].color = sf::Color::White;
            lineVerts[i].position = linePosSliders[i].value();
        }

        context.bloomWindow.clear(bgColorSlider.value());
        context.bloomWindow.draw(text);
        context.bloomWindow.draw(&lineVerts[0], lineVerts.size(), sf::LineStrip);
        context.bloomWindow.display();
    }

    return EXIT_SUCCESS;
}

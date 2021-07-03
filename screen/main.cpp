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
        , lineType(sf::Lines)
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
    sf::PrimitiveType lineType;
};

//
class PositionCycler : public util::PosSlider
{
  public:
    PositionCycler(const sf::Vector2f & FROM, const sf::Vector2f & TO, const float SPEED)
        : PosSlider(FROM, TO, SPEED, util::WillOscillate::No, util::WillAutoStart::Yes, 0)
    {}

    virtual ~PositionCycler() = default;

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
class ColorCycler : public util::ColorSlider
{
  public:
    ColorCycler(const sf::Color & FROM, const sf::Color & TO, const float SPEED)
        : ColorSlider(FROM, TO, SPEED, util::WillOscillate::No, util::WillAutoStart::Yes, 0)
    {}

    virtual ~ColorCycler() = default;

    void updateAndLoopIfStopped(const float FRAME_TIME_SEC, const sf::Color & TO, const float SPEED)
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
float randomBgColorSpeed(const Context & context) { return context.random.fromTo(0.1f, 1.5f); }
float randomTextSpeed(const Context & context) { return context.random.fromTo(0.5f, 1.75f); }
float randomLineSpeed(const Context & context) { return context.random.fromTo(0.1f, 2.0f); }
float randomLineColorSpeed(const Context & context) { return context.random.fromTo(0.1f, 1.0f); }

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
    util::setOriginToCenter(text);

    //
    ColorCycler bgColorCycler(
        sf::Color::Black, colors::random(context.random), randomBgColorSpeed(context));

    //
    PositionCycler textPositionCycler({}, randomWindowPos(context), randomTextSpeed(context));

    //
    const std::size_t lineCount = 100;
    std::vector<PositionCycler> linePosCyclers;
    for (std::size_t i(0); i < (lineCount * 2); ++i)
    {
        linePosCyclers.push_back(
            PositionCycler({}, randomWindowPos(context), randomLineSpeed(context)));
    }

    std::vector<sf::Vertex> lineVerts;
    lineVerts.resize(lineCount * 2);

    util::ColorSlider lineColorSlider(
        sf::Color::White,
        sf::Color::Black,
        randomLineColorSpeed(context),
        util::WillOscillate::Yes,
        util::WillAutoStart::Yes);

    sf::Clock frameClock;

    while (context.window.isOpen())
    {
        const float FRAME_TIME_SEC = frameClock.restart().asSeconds();

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
        bgColorCycler.updateAndLoopIfStopped(
            FRAME_TIME_SEC, colors::random(context.random), randomBgColorSpeed(context));

        // text position
        textPositionCycler.updateAndLoopIfStopped(
            FRAME_TIME_SEC, randomWindowPos(context), randomTextSpeed(context));

        text.setPosition(textPositionCycler.value());

        // line color
        const sf::Color LINE_COLOR = lineColorSlider.updateAndReturnValue(FRAME_TIME_SEC);

        // lines end-point positions
        for (std::size_t i(0); i < (lineCount * 2); ++i)
        {
            linePosCyclers[i].updateAndLoopIfStopped(
                FRAME_TIME_SEC, randomWindowPos(context), randomLineSpeed(context));

            lineVerts[i].position = linePosCyclers[i].value();
            lineVerts[i].color = LINE_COLOR;

            ++i;

            linePosCyclers[i].updateAndLoopIfStopped(
                FRAME_TIME_SEC, randomWindowPos(context), randomLineSpeed(context));

            lineVerts[i].position = linePosCyclers[i].value();
            lineVerts[i].color = bgColorCycler.value();
        }

        for (std::size_t i(0); i < (lineCount * 2); i += 2)
        {
            if (i < lineCount)
            {
                lineVerts[i].position.y = 0.0f;
            }
            else
            {
                lineVerts[i].position.y = static_cast<float>(context.window.getSize().y);
            }
        }

        context.bloomWindow.clear(bgColorCycler.value());
        context.bloomWindow.draw(&lineVerts[0], lineVerts.size(), sf::Lines);
        context.bloomWindow.draw(text);
        context.bloomWindow.display();
    }

    return EXIT_SUCCESS;
}

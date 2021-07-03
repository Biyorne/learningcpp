#include "bloom-shader.hpp"
#include "color-range.hpp"
#include "random.hpp"
#include "slider-color.hpp"
#include "slider-position.hpp"

#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>

//
enum class ChangeMode
{
    Waiting = 0,
    FadeOut
};

//
float randomBgColorSpeed(const util::Random & random) { return random.fromTo(0.1f, 1.5f); }
float randomTextSpeed(const util::Random & random) { return random.fromTo(0.5f, 1.75f); }
float randomLineSpeed(const util::Random & random) { return random.fromTo(0.1f, 2.0f); }
float randomLineColorSpeed(const util::Random & random) { return random.fromTo(0.1f, 1.0f); }
float randomChangeTime(const util::Random & random) { return random.fromTo(10.0f, 20.0f); };

//
struct Context
{
    Context()
        : random()
        , window(sf::VideoMode(1600, 1200), "Screen", sf::Style::Fullscreen)
        , bloomWindow(window)
        , lineType(sf::Lines)
        , changeTimerSec(0.0f)
        , changeMode(ChangeMode::Waiting)
        , frameTimeSec(0.0f)
        , lineColorSlider(
              sf::Color::Black,
              sf::Color::White,
              randomLineColorSpeed(random),
              util::WillOscillate::Yes,
              util::WillAutoStart::Yes)
        , frameBgColor(sf::Color::Black)
        , frameLineColor(sf::Color::Black)
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
    float changeTimerSec;
    ChangeMode changeMode;
    float frameTimeSec;
    util::ColorSlider lineColorSlider;
    sf::Color frameBgColor;
    sf::Color frameLineColor;
};

//
sf::Vector2f randomWindowPos(const Context & context)
{
    return { static_cast<float>(context.random.zeroTo(context.window.getSize().x)),
             static_cast<float>(context.random.zeroTo(context.window.getSize().y)) };
}

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
void HandleChangeMode(Context & context);
void HandleEvents(Context & context);

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
        sf::Color::Black, colors::random(context.random), randomBgColorSpeed(context.random));

    //
    PositionCycler textPositionCycler(
        {}, randomWindowPos(context), randomTextSpeed(context.random));

    //
    const std::size_t lineCount = 100;
    std::vector<PositionCycler> linePosCyclers;
    for (std::size_t i(0); i < (lineCount * 2); ++i)
    {
        linePosCyclers.push_back(
            PositionCycler({}, randomWindowPos(context), randomLineSpeed(context.random)));
    }

    std::vector<sf::Vertex> lineVerts;
    lineVerts.resize(lineCount * 2);

    sf::Clock frameClock;

    while (context.window.isOpen())
    {
        context.frameTimeSec = frameClock.restart().asSeconds();

        HandleEvents(context);

        HandleChangeMode(context);

        // background color
        const sf::Color BG_COLOR_TARGET =
            ((ChangeMode::FadeOut == context.changeMode) ? sf::Color::Black
                                                         : colors::random(context.random));

        bgColorCycler.updateAndLoopIfStopped(
            context.frameTimeSec, BG_COLOR_TARGET, randomBgColorSpeed(context.random));

        context.frameBgColor = bgColorCycler.value();

        // text message
        if (ChangeMode::Waiting == context.changeMode)
        {
            text.setString("Waiting");
        }
        else
        {
            text.setString("FadeOut");
        }

        // text position
        textPositionCycler.updateAndLoopIfStopped(
            context.frameTimeSec, randomWindowPos(context), randomTextSpeed(context.random));

        text.setPosition(textPositionCycler.value());

        // line color
        context.frameLineColor = context.lineColorSlider.updateAndReturnValue(context.frameTimeSec);

        // line position
        for (std::size_t i(0); i < (lineCount * 2); ++i)
        {
            linePosCyclers[i].updateAndLoopIfStopped(
                context.frameTimeSec, randomWindowPos(context), randomLineSpeed(context.random));

            lineVerts[i].position = linePosCyclers[i].value();
            lineVerts[i].color = context.frameLineColor;

            ++i;

            linePosCyclers[i].updateAndLoopIfStopped(
                context.frameTimeSec, randomWindowPos(context), randomLineSpeed(context.random));

            lineVerts[i].position = linePosCyclers[i].value();
            lineVerts[i].color = bgColorCycler.value();
        }

        // for (std::size_t i(0); i < (lineCount * 2); i += 2)
        //{
        //    if (i < lineCount)
        //    {
        //        lineVerts[i].position.y = 0.0f;
        //    }
        //    else
        //    {
        //        lineVerts[i].position.y = static_cast<float>(context.window.getSize().y);
        //    }
        //}

        // draw
        context.bloomWindow.clear(context.frameBgColor);
        context.bloomWindow.draw(&lineVerts[0], lineVerts.size(), context.lineType);
        context.bloomWindow.draw(text);
        context.bloomWindow.display();
    }

    return EXIT_SUCCESS;
}

//
//
//

//
void HandleChangeMode(Context & context)
{
    if (ChangeMode::Waiting == context.changeMode)
    {
        context.changeTimerSec -= context.frameTimeSec;

        if (context.changeTimerSec < 0.0f)
        {
            context.changeMode = ChangeMode::FadeOut;

            context.lineColorSlider = util::ColorSlider(
                context.lineColorSlider.value(),
                sf::Color::Black,
                context.lineColorSlider.speed(),
                util::WillOscillate::No,
                util::WillAutoStart::Yes);
        }
    }
    else
    {
        if (context.frameLineColor == context.frameBgColor)
        {
            context.changeMode = ChangeMode::Waiting;
            context.changeTimerSec = randomChangeTime(context.random);

            context.lineColorSlider = util::ColorSlider(
                sf::Color::Black,
                sf::Color::White,
                context.lineColorSlider.speed(),
                util::WillOscillate::Yes,
                util::WillAutoStart::Yes);

            if (sf::Lines == context.lineType)
            {
                context.lineType = sf::Triangles;
            }
            else
            {
                context.lineType = sf::Lines;
            }
        }
    }
}

//
void HandleEvents(Context & context)
{
    sf::Event event;
    while (context.window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            context.window.close();
        }

        if (event.type == sf::Event::KeyPressed)
        {
            // all keystrokes trigger a change
            context.changeTimerSec = -1.0f;

            if (event.key.code == sf::Keyboard::Escape)
            {
                context.window.close();
            }
        }
    }
}

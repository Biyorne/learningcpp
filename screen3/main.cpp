#include "bloom-shader.hpp"
#include "color-range.hpp"
#include "random.hpp"
#include "slider-color.hpp"
#include "slider-position.hpp"

#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>

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
        , window(sf::VideoMode(1920, 1080), "Screen", sf::Style::Fullscreen)
        , bloomWindow(window)
        , frameTimeSec(0.0f)
        , bgTexture()
        , bgSprite()
    {
        window.setFramerateLimit(60);

        bloomWindow.isEnabled(true);
        bloomWindow.blurMultipassCount(5);
        if (!bloomWindow.isEnabled())
        {
            std::cerr << "Bloom effect not supported on this video card.  Bail." << std::endl;
            window.close();
        }

        bgTexture.loadFromFile("image/wood.jpg");
        bgTexture.setSmooth(true);
        bgTexture.setRepeated(true);

        bgSprite.setTexture(bgTexture);
        bgSprite.setTextureRect(sf::IntRect(0, 0, window.getSize().x, window.getSize().y));
        bgSprite.setScale(2.0f, 2.0f);
    }

    util::Random random;
    sf::RenderWindow window;
    util::BloomEffectHelper bloomWindow;

    float frameTimeSec;

    sf::Texture bgTexture;
    sf::Sprite bgSprite;
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
void HandleEvents(Context & context);

//
int main()
{
    Context context;

    sf::Clock frameClock;

    while (context.window.isOpen())
    {
        context.frameTimeSec = frameClock.restart().asSeconds();

        HandleEvents(context);

        // draw
        context.bloomWindow.clear();
        context.bloomWindow.draw(context.bgSprite);
        context.bloomWindow.display();
    }

    return EXIT_SUCCESS;
}

//
//
//

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
            if (event.key.code == sf::Keyboard::Escape)
            {
                context.window.close();
            }
        }
    }
}

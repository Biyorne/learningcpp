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
        , window(sf::VideoMode(1920, 1200), "Screensaver", sf::Style::Fullscreen)
        , bloomWindow(window)
        , windowSize(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y))
        , windowRect({}, windowSize)
        , frameTimeSec(0.0f)
        , isPaused(false)
        , pauseTimerSec(6.0f)
        , swirlTextures()
        , swirlSprites()
    {
        std::cout << window.getSize() << std::endl;

        window.setFramerateLimit(60);

        bloomWindow.isEnabled(true);
        bloomWindow.blurMultipassCount(5);
        if (!bloomWindow.isEnabled())
        {
            std::cerr << "Bloom effect not supported on this video card.  Bail." << std::endl;
            window.close();
        }

        // swirl images
        swirlTextures.resize(6);
        swirlTextures.at(0).loadFromFile("image/swirl-2.png");
        swirlTextures.at(1).loadFromFile("image/swirl-3.png");
        swirlTextures.at(2).loadFromFile("image/swirl-4.png");
        swirlTextures.at(3).loadFromFile("image/swirl-5.png");
        swirlTextures.at(4).loadFromFile("image/swirl-6.png");
        swirlTextures.at(5).loadFromFile("image/swirl-7.png");

        swirlSprites.resize(swirlTextures.size());
        for (std::size_t i(0); i < swirlSprites.size(); ++i)
        {
            swirlTextures.at(i).setSmooth(true);

            swirlSprites.at(i).setTexture(swirlTextures.at(i));
            swirlSprites.at(i).setScale(0.3f, 0.3f);
            util::setOriginToCenter(swirlSprites.at(i));
            swirlSprites.at(i).setPosition(util::center(windowRect));
        }
    }

    util::Random random;
    sf::RenderWindow window;
    util::BloomEffectHelper bloomWindow;
    sf::Vector2f windowSize;
    sf::FloatRect windowRect;

    float frameTimeSec;
    bool isPaused;
    float pauseTimerSec;

    std::vector<sf::Texture> swirlTextures;
    std::vector<sf::Sprite> swirlSprites;
};

//
template <typename T = float>
class ValueDrifter
{
  public:
    ValueDrifter()
        : valueRange(0.0f, 0.0f)
        , speedRange(0.0f, 0.0f)
        , slider()
    {}

    ValueDrifter(
        const Context & context,
        const sf::Vector2<T> & VALUE_RANGE,
        const sf::Vector2<T> & SPEED_RANGE)
        : valueRange(VALUE_RANGE)
        , speedRange(SPEED_RANGE)
        , slider()
    {
        reset(context);
    }

    void update(const Context & context)
    {
        slider.update(context.frameTimeSec);

        if (slider.isStopped())
        {
            reset(context);
        }
    }

    float value() const { return slider.value(); }
    float ratio() const { return util::mapToRatio(slider.value(), valueRange.x, valueRange.y); }

    void reset(const Context & context)
    {
        slider = util::SliderFromTo<float>(
            slider.value(),
            context.random.fromTo(valueRange.x, valueRange.y),
            context.random.fromTo(speedRange.x, speedRange.y));
    }

  private:
    sf::Vector2<T> valueRange;
    sf::Vector2<T> speedRange;
    util::SliderFromTo<T> slider;
};

//
class PositionDrifter
{
  public:
    PositionDrifter()
        : valueRange(0.0f, 0.0f, 0.0f, 0.0f)
        , speedRange(0.0f, 0.0f)
        , horizDrifter()
        , vertDrifter()
    {}

    PositionDrifter(
        const Context & context,
        const sf::FloatRect & VALUE_RANGE,
        const sf::Vector2f & SPEED_RANGE)
        : valueRange(VALUE_RANGE)
        , speedRange(SPEED_RANGE)
        , horizDrifter()
        , vertDrifter()
    {
        reset(context);
    }

    void update(const Context & context)
    {
        horizDrifter.update(context);
        vertDrifter.update(context);
    }

    sf::Vector2f value() const { return { horizDrifter.value(), vertDrifter.value() }; }

    void reset(const Context & context)
    {
        horizDrifter =
            ValueDrifter<float>(context, { valueRange.left, util::right(valueRange) }, speedRange);

        vertDrifter =
            ValueDrifter<float>(context, { valueRange.top, util::bottom(valueRange) }, speedRange);
    }

  private:
    sf::FloatRect valueRange;
    sf::Vector2f speedRange;
    ValueDrifter<float> horizDrifter;
    ValueDrifter<float> vertDrifter;
};

//
class ColorDrifter
{
  public:
    ColorDrifter()
        : from_(sf::Color::White)
        , to_(sf::Color::White)
        , current_(sf::Color::White)
        , speedRange_(0.0f, 0.0f)
        , slider_()
    {}

    ColorDrifter(const Context & context, const sf::Vector2f & SPEED_RANGE)
        : from_(sf::Color::White)
        , to_(sf::Color::White)
        , current_(sf::Color::White)
        , speedRange_(SPEED_RANGE)
        , slider_()
    {
        reset(context);
    }

    void update(const Context & context)
    {
        slider_.update(context.frameTimeSec);

        const auto RATIO{ slider_.value() };

        const auto RED_DIFF{ static_cast<float>(to_.r - from_.r) };
        const auto GREEN_DIFF{ static_cast<float>(to_.g - from_.g) };
        const auto BLUE_DIFF{ static_cast<float>(to_.b - from_.b) };
        const auto ALPHA_DIFF{ static_cast<float>(to_.a - from_.a) };

        const auto RED{ static_cast<float>(from_.r) + (RED_DIFF * RATIO) };
        const auto GREEN{ static_cast<float>(from_.g) + (GREEN_DIFF * RATIO) };
        const auto BLUE{ static_cast<float>(from_.b) + (BLUE_DIFF * RATIO) };
        const auto ALPHA{ static_cast<float>(from_.a) + (ALPHA_DIFF * RATIO) };

        current_ = sf::Color(
            static_cast<sf::Uint8>(RED),
            static_cast<sf::Uint8>(GREEN),
            static_cast<sf::Uint8>(BLUE),
            static_cast<sf::Uint8>(ALPHA));

        if (slider_.isStopped())
        {
            reset(context);
        }
    }

    sf::Color value() const { return current_; }

    void reset(const Context & context)
    {
        from_ = to_;
        to_ = colors::randomVibrant(context.random);
        slider_ = util::SliderZeroToOne(context.random.fromTo(speedRange_.x, speedRange_.y));
    }

  private:
    sf::Color from_;
    sf::Color to_;
    sf::Color current_;
    sf::Vector2f speedRange_;
    util::SliderZeroToOne slider_;
};

//
class Swirl
{
  public:
    Swirl(const Context & context)
        : sprite_(context.random.from(context.swirlSprites))
        , positionDrifter_(context, context.windowRect, { 0.15f, 1.65f })
        , rotateSpeedDrifter_(context, { -35.0f, -0.05f }, { 0.5f, 2.0f })
        , colorDrifter_(context, { 0.1f, 1.0f })
    {}

    void update(const Context & context)
    {
        positionDrifter_.update(context);
        sprite_.setPosition(positionDrifter_.value());

        rotateSpeedDrifter_.update(context);
        sprite_.rotate(rotateSpeedDrifter_.value());

        colorDrifter_.update(context);
        sprite_.setColor(colorDrifter_.value());

        const float scale = util::mapRatioTo(rotateSpeedDrifter_.ratio(), 0.01f, 0.75f);
        sprite_.setScale(scale, scale);
    }

    void draw(sf::RenderTarget & target) const
    {
        target.draw(sprite_, sf::BlendAdd);
        target.draw(sprite_, sf::BlendAdd);
    }

  private:
    sf::Sprite sprite_;
    PositionDrifter positionDrifter_;
    ValueDrifter<float> rotateSpeedDrifter_;
    ColorDrifter colorDrifter_;
};

//
void HandleEvents(Context & context);

//
int main()
{
    Context context;

    std::vector<Swirl> swirls;
    for (std::size_t i(0); i < 35; ++i)
    {
        swirls.push_back(Swirl(context));
    }

    sf::Clock frameClock;

    while (context.window.isOpen())
    {
        context.frameTimeSec = frameClock.restart().asSeconds();

        HandleEvents(context);

        // update
        if (!context.isPaused)
        {
            for (Swirl & swirl : swirls)
            {
                swirl.update(context);
            }
        }

        context.pauseTimerSec -= context.frameTimeSec;
        if (context.pauseTimerSec < 0.0f)
        {
            if (context.isPaused)
            {
                context.pauseTimerSec = 10.0f;
            }
            else
            {
                context.pauseTimerSec = 3.0f;
            }

            context.isPaused = !context.isPaused;
        }

        // draw
        context.bloomWindow.clear();

        for (const Swirl & swirl : swirls)
        {
            swirl.draw(context.bloomWindow.renderTarget());
        }

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
            else if (event.key.code == sf::Keyboard::Space)
            {
                context.isPaused = !context.isPaused;
            }
        }
    }
}

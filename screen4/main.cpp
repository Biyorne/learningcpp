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
        , novaTexture()
        , novaSprite()
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

        // nova image
        novaTexture.loadFromFile("image/nova.png");
        novaTexture.setSmooth(true);

        novaSprite.setTexture(novaTexture);
        novaSprite.setScale(2.0f, 2.0f);
        util::setOriginToCenter(novaSprite);
        novaSprite.setPosition(util::center(windowRect));
    }

    util::Random random;
    sf::RenderWindow window;
    util::BloomEffectHelper bloomWindow;
    sf::Vector2f windowSize;
    sf::FloatRect windowRect;

    float frameTimeSec;
    bool isPaused;

    sf::Texture novaTexture;
    sf::Sprite novaSprite;
};

//
template <typename T = float>
class ValueDrifter
{
  public:
    ValueDrifter()
        : valueRange_(0.0f, 0.0f)
        , speedRange_(0.0f, 0.0f)
        , slider_()
    {}

    ValueDrifter(
        const Context & context,
        const sf::Vector2<T> & VALUE_RANGE,
        const sf::Vector2<T> & SPEED_RANGE)
        : valueRange_(VALUE_RANGE)
        , speedRange_(SPEED_RANGE)
        , slider_(
              context.random.fromTo(VALUE_RANGE.x, VALUE_RANGE.y),
              context.random.fromTo(VALUE_RANGE.x, VALUE_RANGE.y),
              context.random.fromTo(speedRange_.x, speedRange_.y))
    {
        reset(context);
    }

    void update(const Context & context)
    {
        slider_.update(context.frameTimeSec);

        if (slider_.isStopped())
        {
            reset(context);
        }
    }

    float value() const { return slider_.value(); }
    float ratio() const { return util::mapToRatio(slider_.value(), valueRange_.x, valueRange_.y); }

    void reset(const Context & context)
    {
        slider_ = util::SliderFromTo<float>(
            slider_.value(),
            context.random.fromTo(valueRange_.x, valueRange_.y),
            context.random.fromTo(speedRange_.x, speedRange_.y));
    }

  private:
    sf::Vector2<T> valueRange_;
    sf::Vector2<T> speedRange_;
    util::SliderFromTo<T> slider_;
};

//
class PositionDrifter
{
  public:
    PositionDrifter()
        : valueArea_(0.0f, 0.0f, 0.0f, 0.0f)
        , speedRange_(0.0f, 0.0f)
        , horizDrifter_()
        , vertDrifter_()
    {}

    PositionDrifter(
        const Context & context, const sf::FloatRect & VALUE_AREA, const sf::Vector2f & SPEED_RANGE)
        : valueArea_(VALUE_AREA)
        , speedRange_(SPEED_RANGE)
        , horizDrifter_()
        , vertDrifter_()
    {
        reset(context);
    }

    void update(const Context & context)
    {
        horizDrifter_.update(context);
        vertDrifter_.update(context);
    }

    sf::Vector2f value() const { return { horizDrifter_.value(), vertDrifter_.value() }; }

    void reset(const Context & context)
    {
        horizDrifter_ =
            ValueDrifter<float>(context, { valueArea_.left, util::right(valueArea_) }, speedRange_);

        vertDrifter_ =
            ValueDrifter<float>(context, { valueArea_.top, util::bottom(valueArea_) }, speedRange_);
    }

  private:
    sf::FloatRect valueArea_;
    sf::Vector2f speedRange_;
    ValueDrifter<float> horizDrifter_;
    ValueDrifter<float> vertDrifter_;
};

//
class ColorDrifter
{
  public:
    ColorDrifter()
        : from_(sf::Color::White)
        , to_(sf::Color::White)
        , current_(sf::Color::White)
        , speedRange__(0.0f, 0.0f)
        , slider_()
    {}

    ColorDrifter(const Context & context, const sf::Vector2f & SPEED_RANGE)
        : from_(sf::Color::White)
        , to_(sf::Color::White)
        , current_(sf::Color::White)
        , speedRange__(SPEED_RANGE)
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
        slider_ = util::SliderZeroToOne(context.random.fromTo(speedRange__.x, speedRange__.y));
    }

  private:
    sf::Color from_;
    sf::Color to_;
    sf::Color current_;
    sf::Vector2f speedRange__;
    util::SliderZeroToOne slider_;
};

//
class GlowSpeck
{
  public:
    GlowSpeck(const Context & context, const sf::Sprite & TARGET_SPRITE)
        : sprite_(context.novaSprite)
        , spawnRect_()
        , positionDrifter_()
        , scaleSlider_(context.random.fromTo(0.1f, 1.0f))
        , isGrowing_(true)
        , maxScale_(0.2f)
    {
        spawnRect_ = TARGET_SPRITE.getGlobalBounds();
        util::scaleRectInPlace(spawnRect_, 1.0f);

        positionDrifter_ = PositionDrifter(context, spawnRect_, { 0.0075f, 0.3f });

        sprite_.setScale(0.0f, 0.0f);

        sprite_.setPosition(
            context.random.fromTo(spawnRect_.left, util::right(spawnRect_)),
            context.random.fromTo(spawnRect_.top, util::bottom(spawnRect_)));
    }

    void update(const Context & context)
    {
        positionDrifter_.update(context);
        sprite_.setPosition(positionDrifter_.value());

        if (isGrowing_)
        {
            const float scale = { maxScale_ * scaleSlider_.update(context.frameTimeSec) };
            sprite_.setScale(scale, scale);

            if (scaleSlider_.isStopped())
            {
                isGrowing_ = false;
                scaleSlider_ = util::SliderZeroToOne(context.random.fromTo(0.1f, 1.0f));
            }
        }
        else
        {
            const float scale = { maxScale_ * (1.0f - scaleSlider_.update(context.frameTimeSec)) };
            sprite_.setScale(scale, scale);
        }
    }

    void draw(sf::RenderTarget & target) const { target.draw(sprite_, sf::BlendAdd); }

    bool isAlive() { return (isGrowing_ || scaleSlider_.isMoving()); }

  private:
    sf::Sprite sprite_;
    sf::FloatRect spawnRect_;
    PositionDrifter positionDrifter_;
    util::SliderZeroToOne scaleSlider_;
    bool isGrowing_;
    float maxScale_;
};

//
void HandleEvents(Context & context);

//
int main()
{
    Context context;

    // nova center image
    ValueDrifter novaSpinDrifter(context, { -5.0f, 5.0f }, { 0.1f, 1.0f });

    // nova glow specks
    std::vector<GlowSpeck> specks;
    for (std::size_t i(0); i < 15; ++i)
    {
        specks.push_back(GlowSpeck(context, context.novaSprite));
    }

    // radial spinning sprites
    sf::Sprite spinSprite = context.novaSprite;
    spinSprite.setColor(sf::Color::Yellow);
    float angleTweak = 0.0f;

    ColorDrifter spinColorDrifter(context, { 0.01f, 0.1f });
    ValueDrifter spinRadialDrifter(context, { -0.005f, 0.005f }, { 0.25f, 2.5f });
    ValueDrifter radiusDrifter(context, { -200.0f, 200.0f }, { 0.1f, 1.0f });

    //
    sf::Clock frameClock;
    while (context.window.isOpen())
    {
        context.frameTimeSec = frameClock.restart().asSeconds();

        HandleEvents(context);

        // update
        if (!context.isPaused)
        {
            for (GlowSpeck & speck : specks)
            {
                speck.update(context);

                if (!speck.isAlive())
                {
                    speck = GlowSpeck(context, context.novaSprite);
                }
            }
        }

        // draw
        context.bloomWindow.clear();

        //
        spinRadialDrifter.update(context);
        angleTweak += spinRadialDrifter.value();
        const float stepCount = 20.0f;
        const float angle = ((3.141f * 2.0f) / stepCount) + angleTweak;
        radiusDrifter.update(context);
        const float radius = 100.0f + radiusDrifter.value();
        for (float step(0); step < (stepCount * 2.0f); step += 1.0f)
        {
            const float radiusActual = radius + (step * 20.0f);

            const float posX = radiusActual * std::sin(angle * step);
            const float posY = -radiusActual * std::cos(angle * step);
            const sf::Vector2f pos = { posX, posY };

            spinSprite.setPosition((context.windowSize / 2.0f) + pos);

            //
            spinColorDrifter.update(context);
            spinSprite.setColor(spinColorDrifter.value());

            context.bloomWindow.draw(spinSprite);
        }

        //
        for (GlowSpeck & speck : specks)
        {
            speck.draw(context.bloomWindow.renderTarget());
        }

        //
        novaSpinDrifter.update(context);
        context.novaSprite.rotate(novaSpinDrifter.value());
        context.bloomWindow.draw(context.novaSprite);

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

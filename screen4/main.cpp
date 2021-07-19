#include "bloom-shader.hpp"
#include "color-range.hpp"
#include "random.hpp"
#include "sliders-sfml.hpp"
#include "sliders.hpp"

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
class GlowSpeck
{
  public:
    GlowSpeck(const Context & context, const sf::Sprite & TARGET_SPRITE)
        : sprite_(context.novaSprite)
        , spawnRect_()
        , positionDrifter_()
        , scaleSlider_(context.random.fromTo(0.25f, 2.0f))
        , isGrowing_(true)
        , maxScale_(0.2f)
    {
        spawnRect_ = TARGET_SPRITE.getGlobalBounds();
        util::scaleRectInPlace(spawnRect_, 1.0f);

        positionDrifter_ = util::PositionDrifter(context.random, spawnRect_, { 2.0075f, 10.2f });

        sprite_.setScale(0.0f, 0.0f);

        sprite_.setPosition(
            context.random.fromTo(spawnRect_.left, util::right(spawnRect_)),
            context.random.fromTo(spawnRect_.top, util::bottom(spawnRect_)));
    }

    void update(const Context & context)
    {
        positionDrifter_.update(context.random, context.frameTimeSec);
        sprite_.setPosition(positionDrifter_.value());

        if (isGrowing_)
        {
            const float scale = { maxScale_ * scaleSlider_.update(context.frameTimeSec) };
            sprite_.setScale(scale, scale);

            if (!scaleSlider_.isMoving())
            {
                isGrowing_ = false;
                scaleSlider_ = util::SliderRatio(context.random.fromTo(0.1f, 1.0f));
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
    util::PositionDrifter positionDrifter_;
    util::SliderRatio<float> scaleSlider_;
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
    util::SliderDrift<float> novaSpinDrifter(context.random, { -5.0f, 5.0f }, { 0.1f, 1.0f });

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

    util::ColorDrifter spinColorDrifter(context.random, { 0.01f, 0.1f });

    util::SliderDrift<float> spinRadialDrifter(
        context.random, { -0.005f, 0.005f }, { 0.25f, 2.5f });

    util::SliderDrift<float> radiusDrifter(context.random, { -100.0f, 200.0f }, { 0.1f, 1.0f });
    util::SliderDrift<float> scaleDrifter(context.random, { 0.5f, 4.0f }, { 0.2f, 2.0f });
    util::SliderDrift<float> stepDrifter(context.random, { 5.0f, 25.0f }, { 0.1f, 1.0f });

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
        stepDrifter.update(context.random, context.frameTimeSec);
        scaleDrifter.update(context.random, context.frameTimeSec);
        const float scale = scaleDrifter.value();
        spinSprite.setScale(scale, scale);
        spinRadialDrifter.update(context.random, context.frameTimeSec);
        angleTweak += spinRadialDrifter.value();
        const float stepCount = 40.0f;
        const float angle = ((3.141f * 2.0f) / stepCount) + angleTweak;
        radiusDrifter.update(context.random, context.frameTimeSec);
        const float radius = 100.0f + radiusDrifter.value();
        for (float step(0); step < (stepCount * 2.0f); step += 1.0f)
        {
            const float radiusActual = radius + (step * stepDrifter.value());

            const float posX = radiusActual * std::sin(angle * step);
            const float posY = -radiusActual * std::cos(angle * step);
            const sf::Vector2f pos = { posX, posY };

            spinSprite.setPosition((context.windowSize / 2.0f) + pos);

            //
            spinColorDrifter.update(context.random, context.frameTimeSec);
            spinSprite.setColor(spinColorDrifter.value());

            context.bloomWindow.draw(spinSprite);
        }

        //
        for (GlowSpeck & speck : specks)
        {
            speck.draw(context.bloomWindow.renderTarget());
        }

        //
        novaSpinDrifter.update(context.random, context.frameTimeSec);
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

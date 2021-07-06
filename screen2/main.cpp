#include "random.hpp"
#include "slider-color.hpp"

#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>

//
struct Context
{
    Context()
        : random()
        , window(sf::VideoMode(1920, 1080), "Screen", sf::Style::Fullscreen)
        , windowSize(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y))
        , windowRect({}, windowSize)
        , frameTimeSec(0.0f)
        , bgTextures(4, sf::Texture())
        , bgSprites(bgTextures.size(), sf::Sprite())
        , swirlTextures(8, sf::Texture())
    {
        window.setFramerateLimit(60);

        // load background images
        bgTextures.at(0).loadFromFile("image/paper-1.png");
        bgTextures.at(1).loadFromFile("image/paper-2.jpg");
        bgTextures.at(2).loadFromFile("image/paper-3.jpg");
        bgTextures.at(3).loadFromFile("image/paper-4.jpg");

        // setup backgrouond images
        for (std::size_t i(0); i < bgSprites.size(); ++i)
        {
            bgTextures.at(i).setSmooth(true);

            bgSprites.at(i).setTexture(bgTextures[i]);

            const float scaleX{ windowSize.x / bgSprites.at(i).getGlobalBounds().width };
            const float scaleY{ windowSize.y / bgSprites.at(i).getGlobalBounds().height };
            bgSprites.at(i).setScale(scaleX, scaleY);
        }

        // load swirl textures
        swirlTextures.at(0).loadFromFile("image/swirl-1.png");
        swirlTextures.at(1).loadFromFile("image/swirl-2.png");
        swirlTextures.at(2).loadFromFile("image/swirl-3.png");
        swirlTextures.at(3).loadFromFile("image/swirl-4.png");
        swirlTextures.at(4).loadFromFile("image/swirl-5.png");
        swirlTextures.at(5).loadFromFile("image/swirl-6.png");
        swirlTextures.at(6).loadFromFile("image/swirl-7.png");
        swirlTextures.at(7).loadFromFile("image/swirl-8.png");

        for (sf::Texture & texture : swirlTextures)
        {
            texture.setSmooth(true);
        }
    }

    util::Random random;

    sf::RenderWindow window;
    sf::Vector2f windowSize;
    sf::FloatRect windowRect;

    float frameTimeSec;

    std::vector<sf::Texture> bgTextures;
    std::vector<sf::Sprite> bgSprites;
    std::vector<sf::Texture> swirlTextures;
};

//
sf::Vector2f randomWindowPos(const Context & context)
{
    return { static_cast<float>(context.random.zeroTo(context.window.getSize().x)),
             static_cast<float>(context.random.zeroTo(context.window.getSize().y)) };
}

//
class Swirl
{
  public:
    Swirl(const Context & context)
        : sprite(context.random.from(context.swirlTextures))
        , colorSlider(
              sf::Color::Transparent,
              sf::Color(255, 255, 255, context.random.fromTo<sf::Uint8>(64, 128)),
              context.random.fromTo(0.75f, 3.0f),
              util::WillOscillate::Yes,
              util::WillAutoStart::Yes,
              1)
        , spinSpeed(context.random.fromTo(0.5f, 5.0f))
        , scaleSlider(
              context.random.fromTo(0.001f, 0.01f),
              context.random.fromTo(1.0f, 6.0f),
              context.random.fromTo(0.25f, 1.5f))
    {
        util::setOriginToCenter(sprite);
        sprite.setPosition(randomWindowPos(context));
        sprite.setColor(colorSlider.value());
        sprite.setScale(scaleSlider.value(), scaleSlider.value());
    }

    void update(const float FRAME_TIME_SEC)
    {
        sprite.rotate(spinSpeed);

        colorSlider.update(FRAME_TIME_SEC);
        sprite.setColor(colorSlider.value());

        scaleSlider.Update(FRAME_TIME_SEC);
        sprite.setScale(scaleSlider.value(), scaleSlider.value());
    }

    bool isAlive() { return (colorSlider.isMoving()); }

    void draw(sf::RenderWindow & window) { window.draw(sprite); }

  private:
    sf::Sprite sprite;
    util::ColorSlider colorSlider;
    float spinSpeed;
    util::SliderFromTo<float> scaleSlider;
};

//
class BackgroundFader
{
  public:
    BackgroundFader(const Context & context)
        : isFading(false)
        , bgIndex(0)
        , fgIndex(0)
        , colorSlider()
        , delayTimerSec(context.random.fromTo(1.0f, 1.1f))
    {}

    void update(Context & context)
    {
        if (isFading)
        {
            colorSlider.update(context.frameTimeSec);
            context.bgSprites.at(fgIndex).setColor(colorSlider.value());

            if (colorSlider.isAtTo())
            {
                isFading = false;
                delayTimerSec = context.random.fromTo(1.0f, 1.1f);
            }
        }
        else
        {
            delayTimerSec -= context.frameTimeSec;

            if (delayTimerSec < 0.0f)
            {
                isFading = true;

                colorSlider = util::ColorSlider(
                    sf::Color::Transparent,
                    sf::Color::White,
                    context.random.fromTo(0.1f, 1.0f),
                    util::WillOscillate::No,
                    util::WillAutoStart::Yes);

                bgIndex = fgIndex;

                while (bgIndex == fgIndex)
                {
                    fgIndex = context.random.fromTo<std::size_t>(0, context.bgSprites.size() - 1);
                }

                context.bgSprites.at(bgIndex).setColor(sf::Color::White);
                context.bgSprites.at(fgIndex).setColor(sf::Color::Transparent);
            }
        }
    }

    void draw(Context & context)
    {
        context.window.draw(context.bgSprites.at(bgIndex));
        context.window.draw(context.bgSprites.at(fgIndex));
    }

  private:
    bool isFading;
    std::size_t bgIndex;
    std::size_t fgIndex;
    util::ColorSlider colorSlider;
    float delayTimerSec;
};

//
void HandleEvents(Context & context);

//
int main()
{
    Context context;

    BackgroundFader bgFader(context);

    std::vector<Swirl> swirls;
    for (std::size_t i(0); i < 10; ++i)
    {
        swirls.push_back(Swirl(context));
    }

    sf::Clock frameClock;
    while (context.window.isOpen())
    {
        context.frameTimeSec = frameClock.restart().asSeconds();

        HandleEvents(context);

        // update
        bgFader.update(context);

        for (Swirl & swirl : swirls)
        {
            swirl.update(context.frameTimeSec);

            if (!swirl.isAlive())
            {
                swirl = Swirl(context);
            }
        }

        // draw
        context.window.clear();

        bgFader.draw(context);

        for (Swirl & swirl : swirls)
        {
            if (swirl.isAlive())
            {
                swirl.draw(context.window);
            }
        }

        context.window.display();
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

#include "random.hpp"

#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>

//
struct Context
{
    Context()
        : random()
        , window(sf::VideoMode(1920, 1200), "Snow", sf::Style::Fullscreen)
        , windowSize(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y))
        , windowRect({}, windowSize)
        , frameTimeSec(0.0f)
        , isPaused(false)
        , snowTexture()
        , snowSprite()
    {
        std::cout << window.getSize() << std::endl;

        window.setFramerateLimit(60);

        // snow image
        snowTexture.loadFromFile("image/nova.png");
        snowTexture.setSmooth(true);

        snowSprite.setTexture(snowTexture);
        util::setOriginToCenter(snowSprite);
        snowSprite.setPosition(util::center(windowRect));
    }

    util::Random random;
    sf::RenderWindow window;
    sf::Vector2f windowSize;
    sf::FloatRect windowRect;

    float frameTimeSec;
    bool isPaused;

    sf::Texture snowTexture;
    sf::Sprite snowSprite;
};

//
class Flake
{
  public:
    Flake(const Context & context)
        : scaleMin_(0.05f)
        , scaleMax_(0.5f)
        , scale_(context.random.fromTo(scaleMin_, scaleMax_))
        , position_(context.random.fromTo(0.0f, context.windowSize.x), 0.0f)
        , velocity_(0.0f, 0.0f)
    {
        velocity_.x = context.random.fromTo(-10.0f, 10.0f);
        velocity_.y = util::map(scale_, scaleMin_, scaleMax_, 200.0f, 1500.0f);
    }

    void update(const Context & context) { position_ += (velocity_ * context.frameTimeSec); }

    void draw(Context & context) const
    {
        context.snowSprite.setPosition(position_);
        context.snowSprite.setScale(scale_, scale_);
        context.window.draw(context.snowSprite);
    }

    bool isAlive(const Context & context) { return (position_.y < context.windowSize.y); }

  private:
    float scaleMin_;
    float scaleMax_;
    float scale_;
    sf::Vector2f position_;
    sf::Vector2f velocity_;
};

//
void HandleEvents(Context & context);

//
int main()
{
    Context context;

    // snow flakes
    std::vector<Flake> flakes;
    for (std::size_t i(0); i < 100; ++i)
    {
        flakes.push_back(Flake(context));
    }

    //
    sf::Clock frameClock;
    while (context.window.isOpen())
    {
        context.frameTimeSec = frameClock.restart().asSeconds();

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

        // update
        if (!context.isPaused)
        {
            for (Flake & flake : flakes)
            {
                flake.update(context);

                if (!flake.isAlive(context))
                {
                    flake = Flake(context);
                }
            }
        }

        // draw
        context.window.clear();

        //
        for (Flake & flake : flakes)
        {
            flake.draw(context);
        }

        context.window.display();
    }

    return EXIT_SUCCESS;
}

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cstdlib>
#include <iostream>

namespace movement
{
class Bouncer
{
public:
    Bouncer(const float START, const float END, const float SPEED)
        : m_start(START)
        , m_end(END)
        , m_speed(SPEED)
        , m_pos(START)
    {}
    void update(const float TIMEELAPSED)
    {
        if (m_pos < m_end)
        {
            m_pos += m_speed * TIMEELAPSED;
        }
    }
    float getPosition() const { return m_pos; }

private:
    float m_start;
    float m_end;
    float m_speed;
    float m_pos;
};
} // namespace movement

int main(void)
{
    sf::Texture texture;
    if (texture.loadFromFile("cinder_magical_pony.png") == false)
    {
        std::cout << "Failed to open file" << std::endl;
        char ignore;
        std::cin >> ignore;
        return EXIT_FAILURE;
    }

    sf::RenderWindow window;
    window.create(sf::VideoMode(1920,1080), "SFML works!", sf::Style::Fullscreen);

    const float WINDOW_WIDTH(static_cast<float>(window.getSize().x));
    const float WINDOW_HEIGHT(static_cast<float>(window.getSize().y));
    std::cout << "size=" << WINDOW_WIDTH << "x" << WINDOW_HEIGHT << std::endl;
    sf::Clock clock;
    sf::Sprite sprite(texture);
    sprite.setColor(sf::Color(255, 255, 0));
    sf::Image image(texture.copyToImage());
    image.createMaskFromColor(sf::Color::Black);
    texture.loadFromImage(image);
    float red(0.0f);
    const float SPEED(500.0f);
    movement::Bouncer myBouncer(0.0f, WINDOW_WIDTH - sprite.getGlobalBounds().width, SPEED);

    while (window.isOpen())
    {
        const float TIME_ELAPSED(clock.getElapsedTime().asSeconds());
        clock.restart();

        // close window on mouse click
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed)
            {
                window.close();
            }
        }

        myBouncer.update(TIME_ELAPSED);
        sprite.setPosition(
            myBouncer.getPosition(),
            (WINDOW_HEIGHT * 0.5f) - (sprite.getGlobalBounds().height * 0.5f));

        // Change background color
        const float COLOR_SPEED(20.0f);
        red = red + TIME_ELAPSED * COLOR_SPEED;
        window.clear(sf::Color(static_cast<sf::Uint8>(red), 0, 0, 255));
        window.draw(sprite);
        window.display();
    }

    return EXIT_SUCCESS;
}

// Make sprite move back and forth

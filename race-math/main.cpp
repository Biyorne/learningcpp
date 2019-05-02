#include <SFML/Graphics.hpp>

#include <cmath>
#include <iostream>

float calcPos(
    const float startPos, const float velocity, const float timeSec, const float acceleration)
{
    return (startPos + (velocity * timeSec) + (acceleration * timeSec * timeSec));
}

float calcOvertakeTimeSec(
    const float slowStartPos, const float fastStartPos, const float slowAcc, const float fastAcc)
{
    return std::sqrt((fastStartPos - slowStartPos) / (slowAcc - fastAcc));
}

int main()
{
    sf::Texture slowCarTexture;
    sf::Texture fastCarTexture;

    slowCarTexture.loadFromFile("car-1.png");
    fastCarTexture.loadFromFile("car-2.png");

    sf::Sprite slowCarSprite(slowCarTexture);
    sf::Sprite fastCarSprite(fastCarTexture);

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Race Math", sf::Style::Fullscreen);

    const sf::Vector2f WINDOW_SIZE(window.getSize());

    const float slowCarAcc(10.0f);
    const float fastCarAcc(slowCarAcc * 2.0f);

    const sf::Vector2f slowCarStartPos(
        (WINDOW_SIZE.x * 0.25f),
        (fastCarSprite.getGlobalBounds().top + fastCarSprite.getGlobalBounds().height));

    const sf::Vector2f fastCarStartPos;

    slowCarSprite.setPosition(slowCarStartPos);

    const float overtakePredictedTimeSec(
        calcOvertakeTimeSec(slowCarStartPos.x, fastCarStartPos.x, slowCarAcc, fastCarAcc));

    std::cout << "Prediction = " << overtakePredictedTimeSec << std::endl;

    float overtakeActualTimeSec(0.0f);

    sf::Clock raceClock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if ((event.type == sf::Event::Closed) || (event.type == sf::Event::KeyPressed)
                || (event.type == sf::Event::MouseButtonPressed))
            {
                window.close();
            }
        }

        window.clear();
        window.draw(slowCarSprite);
        window.draw(fastCarSprite);
        window.display();

        const float elapsedTimeSec(raceClock.getElapsedTime().asSeconds());

        slowCarSprite.setPosition(
            calcPos(slowCarStartPos.x, 0.0f, elapsedTimeSec, slowCarAcc), 0.0f);

        fastCarSprite.setPosition(
            calcPos(fastCarStartPos.x, 0.0f, elapsedTimeSec, fastCarAcc), 0.0f);

        if (fastCarSprite.getPosition().x > slowCarSprite.getPosition().x)
        {
            overtakeActualTimeSec = elapsedTimeSec;
            std::cout << "Actual = " << overtakeActualTimeSec << std::endl;
            window.close();
        }
    }

    std::cout << "We were off by " << std::fixed
              << std::abs(overtakeActualTimeSec - overtakePredictedTimeSec) << "." << std::endl;

    return 0;
}

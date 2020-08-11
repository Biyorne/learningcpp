// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// main.cpp
//
#include "context.hpp"
#include "image.hpp"
#include "map.hpp"
#include "sfx.hpp"

#include <cstdlib>

//

using namespace mario;

//

void handleEvent(Context & context, const sf::Event & event);

//

int main()
{
    Map::setup();
    Image::setup();
    Sfx::setup();

    Context context;

    sf::Clock clock;
    while (context.window.isOpen())
    {
        // handle events
        sf::Event event;
        while (context.window.pollEvent(event))
        {
            handleEvent(context, event);
        }

        // get elapsed time since last frame
        const float elapsedTimeSec{ clock.getElapsedTime().asSeconds() * context.game_time_mult };
        clock.restart();

        // update
        context.update(elapsedTimeSec);

        // draw
        context.draw();
    }

    return EXIT_SUCCESS;
}

//

void handleEvent(Context & context, const sf::Event & event)
{
    if ((event.type == sf::Event::Closed) || (event.type == sf::Event::MouseButtonPressed))
    {
        context.window.close();
        return;
    }

    // if (event.type == sf::Event::MouseWheelScrolled)
    //{
    //    const float scrollAmount{ event.mouseWheelScroll.delta };
    //
    //    // if (sf::Keyboard::isKeyPressed(
    //}

    if (event.type != sf::Event::KeyPressed)
    {
        return;
    }

    const sf::Keyboard::Key key{ event.key.code };

    // const bool isShiftPressed{ sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) };
    // const bool isCntrlPressed{ sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) };

    if (sf::Keyboard::R == key)
    {
        context.reset();
    }
    else if (sf::Keyboard::Space == key)
    {
        if (context.is_paused)
        {
            context.music.play();
        }
        else
        {
            context.music.pause();
        }
    }
    else if (sf::Keyboard::Right == key)
    {
        context.game_time_mult *= 1.1f;
    }
    else if (sf::Keyboard::Left == key)
    {
        context.game_time_mult *= 0.9f;
    }
    else if (sf::Keyboard::S == key)
    {
        GameObject turtle(context, Image::AngryTurtle, context.drop_pos);

        context.drop_pos.x += (context.block_global_size.x * 10.0f);
        if (context.drop_pos.x > (context.window_size.x - 100.0f))
        {
            context.drop_pos.x = 100.0f;
        }

        if ((static_cast<int>(context.drop_pos.x) % 2) == 0)
        {
            // turtle.sprite.move((context.block_global_size.x * 2.0f), 0.0f);
            turtle.flipHorizDirection();
        }

        context.objects.push_back(turtle);
    }
}
// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// context.cpp
//
#include "context.hpp"

#include "map.hpp"

#include <iostream>

namespace mario
{
    Context::Context()
    {
        //
        const sf::VideoMode videoMode(2880, 1800, sf::VideoMode::getDesktopMode().bitsPerPixel);
        window.create(videoMode, "Mario-Demo", sf::Style::Fullscreen);
        window.setFramerateLimit(60);
        window_size = sf::Vector2f(window.getSize());
        window_rect = sf::FloatRect({ 0.0f, 0.0f }, window_size);

        block_local_size.x = 16.0f;
        block_local_size.y = block_local_size.x;
        //
        block_counts.x = static_cast<std::size_t>(std::sqrt(window_size.x));
        //
        block_global_size.x = (window_size.x / static_cast<float>(block_counts.x));
        block_global_size.y = block_global_size.x;
        //
        block_counts.y = static_cast<std::size_t>((window_size.y / block_global_size.y) + 1.0f);
        //
        block_scale = (block_global_size / block_local_size);

        //
        sky_vert_array = Map::makeSkyVerts(*this);

        //
        const std::string musicPathStr{ "media/music-overworld.ogg" };
        if (music.openFromFile(musicPathStr))
        {
            music.setLoop(true);
            music.setVolume(50.0f);
        }
        else
        {
            std::cout << "Failed to load music: \"" << musicPathStr << "\"." << std::endl;
        }

        reset();
    }

    void Context::reset()
    {
        is_paused = false;

        states = sf::RenderStates();

        game_time_mult = 1.0f;

        mouse_pos = { 0.0f, 0.0f };

        Sfx::stopAll();

        music.play();

        objects.clear();
        Map::populateWorld(*this);
    }

    void Context::update(const float elapsedTimeSec)
    {
        mouse_pos = sf::Vector2f{ sf::Mouse::getPosition(window) };

        for (GameObject & thing : objects)
        {
            thing.update(*this, elapsedTimeSec);
        }

        handleCollisions();
    }

    void Context::draw()
    {
        window.clear();
        window.draw(sky_vert_array);

        for (const GameObject & thing : objects)
        {
            if (Image::isBackground(thing.image()))
            {
                window.draw(thing);
            }
        }

        for (const GameObject & thing : objects)
        {
            if (!Image::isBackground(thing.image()))
            {
                window.draw(thing);
            }
        }

        window.display();
    }

    void Context::handleCollisions()
    {
        for (std::size_t outerIndex(0); outerIndex < objects.size(); ++outerIndex)
        {
            GameObject & outerObject{ objects.at(outerIndex) };

            if (!outerObject.can_move || !outerObject.can_hit_obstacles)
            {
                continue;
            }

            bool outerHasLanded{ false };

            for (std::size_t innerIndex(0); innerIndex < objects.size(); ++innerIndex)
            {
                GameObject & innerObject{ objects.at(innerIndex) };

                if ((outerIndex == innerIndex) || (!innerObject.is_obstacle))
                {
                    continue;
                }

                handleVertCollision(outerObject, innerObject, outerHasLanded);
                handleHorizCollision(outerObject, innerObject);
            }

            // if (!outerObject.is_standing_on_something)
            {
                outerObject.is_standing_on_something = outerHasLanded;
            }
        }
    }

    void Context::handleHorizCollision(GameObject & outerObject, GameObject & innerObject)
    {
        sf::FloatRect intersection;

        if (!outerObject.sprite.getGlobalBounds().intersects(
                innerObject.sprite.getGlobalBounds(), intersection))
        {
            return;
        }

        if (intersection.width > 0.0f)
        {
            if (outerObject.mover.velocity.x > 0.0f)
            {
                outerObject.sprite.move(-intersection.width, 0.0f);
            }
            else
            {
                outerObject.sprite.move(intersection.width, 0.0f);
            }

            const bool wereMovingInOppositeDirections{ (innerObject.mover.velocity.x < 0.0f) !=
                                                       (outerObject.mover.velocity.x < 0.0f) };

            outerObject.flipHorizDirection();

            if (wereMovingInOppositeDirections)
            {
                innerObject.flipHorizDirection();
            }
        }
    }

    void Context::handleVertCollision(
        GameObject & outerObject, GameObject & innerObject, bool & outerHasLanded)
    {
        sf::FloatRect intersection;

        if (!outerObject.sprite.getGlobalBounds().intersects(
                innerObject.sprite.getGlobalBounds(), intersection))
        {
            return;
        }

        if ((intersection.height > 0.0f) &&
            !(intersection.height > std::abs(outerObject.mover.velocity.y)))
        {
            if (outerObject.mover.velocity.y > 0.0f)
            {
                outerObject.sprite.move(0.0f, -(intersection.height + 1.0f));
                outerHasLanded = true;
                outerObject.mover.velocity.y *= -1.0f;
            }
        }
    }
} // namespace mario
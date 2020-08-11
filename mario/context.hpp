#ifndef MARIO_CONTEXT_HPP_INCLUDED
#define MARIO_CONTEXT_HPP_INCLUDED
//
// context.hpp
//
#include "game-object.hpp"
#include "util.hpp"

#include <string>
#include <vector>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

namespace mario
{
    struct Context
    {
        Context();

        void reset();

        void update(const float elapsedTimeSec);
        void draw();

        void handleCollisions();
        void handleHorizCollision(GameObject & outerObject, GameObject & innerObject);

        void handleVertCollision(
            GameObject & outerObject, GameObject & innerObject, bool & hasLanded);

        sf::RenderWindow window;
        sf::Vector2f window_size;
        sf::FloatRect window_rect;

        sf::Vector2s block_counts;
        sf::Vector2f block_local_size;
        sf::Vector2f block_global_size;
        sf::Vector2f block_scale;

        bool is_paused;
        sf::Music music;
        sf::RenderStates states;
        sf::Vector2f mouse_pos;
        float game_time_mult;
        sf::VertexArray sky_vert_array;

        std::vector<GameObject> objects;

        sf::Vector2f drop_pos = { 100.0f, 1.0f };
    };
} // namespace mario

#endif // MARIO_CONTEXT_HPP_INCLUDED
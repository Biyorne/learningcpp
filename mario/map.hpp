#ifndef MARIO_MAP_HPP_INCLUDED
#define MARIO_MAP_HPP_INCLUDED
//
// map.hpp
//
#include "context.hpp"
#include "image.hpp"

#include <map>

#include <SFML/Graphics.hpp>

namespace mario
{
    struct Map
    {
        static inline const std::map<char, Image::Enum> map_table = {
            { 'H', Image::Hill },        { 'C', Image::CloudBig },    { 'c', Image::CloudSmall },
            { 'T', Image::TreeBig },     { 't', Image::TreeSmall },   { 'F', Image::BushBig },
            { 'f', Image::BushSmall },   { '1', Image::AngryTurtle }, { '2', Image::AngryMushroom },
            { '3', Image::AngryBall },   { '4', Image::AngrySpikey }, { 'P', Image::Pipe },
            { 'Q', Image::BlockQLight }, { 'q', Image::BlockQDark },  { 'B', Image::BlockBrick },
            { 'G', Image::BlockGround }, { 'R', Image::BlockRock }
        };

        // clang-format off
        static inline const std::vector<std::string> mapVec =
        {
            "R                                                   ",
            "R                                                   ",
            "R                QQQ                                ",
            "R                                                   ",
            "R                                             BBBBBB",
            "R               R 2 R      QQ                             R",
            "R              BRRRRRBBB                                  R",
            "R                                         QQQQ            R",
            "RRR       BQQB           BRQRB                            R",
            "RR                                                        R",
            "R                                BBB   BBRRBB             R",
            "R                                                         R",
            "R      BRRB                3                              R",
            "RRR                        R                              R",
            "RR                    T    R   H    H P H H               R",
            "R                   t      R                             RR",
            "R F F 1R                  RRR                   R  4f 4 RRR",
            "GGGGGGGGGGGGGG    GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG",
            "GGGGGGGGGGGGGG    GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG"
        };
        // clang-format on

        static void setup()
        {
            // um...anything to do here?
        }

        static sf::VertexArray makeSkyVerts(const Context & context)
        {
            const sf::Color top_sky_color = sf::Color(172, 220, 255);
            const sf::Color bottom_sky_color = sf::Color(64, 127, 250);

            sf::VertexArray verts(sf::Quads);
            verts.append(sf::Vertex({ 0.0f, 0.0f }, top_sky_color));
            verts.append(sf::Vertex({ context.window_size.x, 0.0f }, top_sky_color));
            verts.append(sf::Vertex(context.window_size, bottom_sky_color));
            verts.append(sf::Vertex({ 0.0f, context.window_size.y }, bottom_sky_color));

            return verts;
        }

        static void populateWorld(Context & context)
        {
            for (std::size_t y(0); y < Map::mapVec.size(); ++y)
            {
                const std::string & mapRowStr{ Map::mapVec[y] };
                for (std::size_t x(0); x < mapRowStr.size(); ++x)
                {
                    const auto foundIter{ Map::map_table.find(mapRowStr[x]) };
                    if (foundIter == std::end(Map::map_table))
                    {
                        continue;
                    }

                    const Image::Enum which_image{ foundIter->second };

                    const float blockGlobalPosLeft{ static_cast<float>(x) *
                                                    context.block_global_size.x };

                    float blockGlobalPosTop{ context.window_size.y };

                    blockGlobalPosTop -= (static_cast<float>(y) * context.block_global_size.y);

                    blockGlobalPosTop -=
                        (static_cast<float>(Image::textureRect(which_image).height) *
                         context.block_scale.y);

                    const sf::Vector2f blockGlobalPos(blockGlobalPosLeft, blockGlobalPosTop);

                    // if (Image::canHitObstacles(which_image))
                    //{
                    //    blockGlobalPosTop -= 2.0f;
                    //}

                    context.objects.push_back(GameObject(context, which_image, blockGlobalPos));
                }
            }

            context.objects.push_back(GameObject(
                context, Image::CloudBig, (context.window_size * sf::Vector2f(0.1f, 0.05f))));

            context.objects.push_back(GameObject(
                context, Image::CloudSmall, (context.window_size * sf::Vector2f(0.8f, 0.2f))));

            context.objects.push_back(GameObject(
                context, Image::CloudSmall, (context.window_size * sf::Vector2f(0.52f, 0.4f))));
        }
    };
} // namespace mario

#endif // MARIO_MAP_HPP_INCLUDED
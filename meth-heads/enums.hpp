#ifndef METHHEADS_ENUMS_HPP_INCLUDED
#define METHHEADS_ENUMS_HPP_INCLUDED
//
// enum.hpp
//
#include "nameof.hpp"
#include "utils.hpp"

#include <array>
#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>

namespace methhead
{
    class Actors
    {
        enum Enum : std::size_t
        {
            Lazy = 0,
            Greedy,
            Count
        };

      private:
        struct Media
        {
            Media() = default;

            Media(
                const Enum which,
                const std::string & imagePath,
                const sf::Color & clr,
                const sf::Vector2f & size)
                : enumeration(which)
                , name(nameof::nameof_enum(which))
                , texture()
                , sprite()
                , color(clr)
            {
                if (!texture.loadFromFile(imagePath))
                {
                    std::cerr << "Error:  Unable to load Actor " << name
                              << "'s image from: " << imagePath << std::endl;
                }

                sprite.setTexture(texture, true);
                scale(sprite, size);
            }

            Enum enumeration{ Count };
            std::string name;
            sf::Texture texture;
            sf::Sprite sprite;
            sf::Color color;
        };

        static inline std::array<Media, Count> media;
    };

    /*

    {
        std::string name(const Enum piece) const { return ; }

      private:
        std::string getImagePath(const Enum piece) const noexcept
        {
            // clang-format off
            switch (piece)
            {
                default:
                case Lazy:    { return "image/head-1.png"; }
                case Greedy:  { return "image/head-2.png"; }
                case Loot:    { return "image/loot.png";   }
            }
            // clang-format on
        };

        */
} // namespace methhead

#endif // METHHEADS_ENUMS_HPP_INCLUDED
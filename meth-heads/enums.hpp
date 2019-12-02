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

/*
namespace methhead
{
    struct Pieces
    {
        using type_t = std::uint_fast32_t;

        // clang-format off
        enum Enum : type_t
        {
            None    = 0,
            Lazy    = 1 << 0,
            Greedy  = 1 << 1,
            Loot    = 1 << 2
        };
        // clang-format on

        static Enum actor(const Enum en) noexept { return static_cast<Enum>(en & allActors); }
        static Enum pickup(const Enum en) noexept { return static_cast<Enum>(en & allPickups); }

        static bool hasActor(const Enum en) noexcept { return (actor(en) != None); }
        static bool hasPickup(const Enum en) noexcept { return (pickup(en) != None); }

      private:
        static const type_t allActors{ Lazy | Greedy };
        static const type_t allPickups{ Loot };
    };

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

        //

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

        std::string name;
        sf::Texture texture;
        sf::Sprite sprite;
        sf::Color color;
    };
} // namespace methhead

        */
#endif // METHHEADS_ENUMS_HPP_INCLUDED
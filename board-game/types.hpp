#ifndef BOARDGAME_TYPES_HPP_INCLUDED
#define BOARDGAME_TYPES_HPP_INCLUDED
//
// types.hpp
//
#include <cstddef> //for std::size_t
#include <filesystem>
#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>

//

namespace boardgame
{
    struct Piece
    {
        enum Enum : std::size_t
        {
            Wall = 0,
            Door,
            Villan,
            Player,
            Count
        };

        static std::string name(const Piece::Enum piece)
        {
            switch (piece)
            {
                case Piece::Villan: return "Villan";
                case Piece::Player: return "Player";
                case Piece::Wall: return "Wall";
                case Piece::Door: return "Door";
                case Piece::Count: return "Count";
                default:
                    return "Piece::Enum value undefined: " +
                           std::to_string(static_cast<std::size_t>(piece));
            }
        }

        static bool canWalkOn(const Piece::Enum takingStep, const Piece::Enum steppedOn)
        {
            if (Player == takingStep)
            {
                return ((Count == steppedOn) || (Door == steppedOn) || (Villan == steppedOn));
            }

            if (Villan == takingStep)
            {
                return ((Count == steppedOn) || (Player == steppedOn));
            }

            return false;
        }

        static sf::Color color(const Piece::Enum piece)
        {
            switch (piece)
            {
                case Piece::Villan: return sf::Color::Red;

                case Piece::Player:
                    return sf::Color::Green;

                    // dark-brown
                case Piece::Wall:
                    return sf::Color(100, 60, 5);

                    // light-brown
                case Piece::Door: return sf::Color(200, 130, 10);

                // Count/default case is valid
                case Piece::Count:
                default: return sf::Color::White;
            }
        }

        static std::filesystem::path imageRelativePath(const Piece::Enum piece)
        {
            std::filesystem::path path("image");

            switch (piece)
            {
                case Piece::Villan:
                case Piece::Player: return (path / "brick-tiny-bw-actor.png");

                case Piece::Wall: return ((path / "seamless") / "brick-tiny-bw.png");

                case Piece::Door:
                    return ((path / "seamless") / "brick-tiny-bw-door.png");

                    // Count/default case is valid, but it is just the root media dir
                case Piece::Count:
                default: return path;
            }
        }

        static Piece::Enum mapCharTo(const char ch)
        {
            switch (ch)
            {
                case 'W': return Piece::Wall;
                case 'D': return Piece::Door;
                case 'P': return Piece::Player;
                case 'V': return Piece::Villan;
                case ' ':
                    return Piece::Count;

                    // default case is NOT valid
                default: {
                    std::cout << "Error:  A character in the map ('" << ch
                              << "') is unknown and will become an empty cell with nothing in it."
                              << std::endl;

                    return Piece::Count;
                }
            }
        }
    };
} // namespace boardgame

#endif // #define BOARDGAME_TYPES_HPP_INCLUDED
#ifndef CASTLECRAWL_KEYS_HPP_INCLUDED
#define CASTLECRAWL_KEYS_HPP_INCLUDED
//
// keys.hpp
//
#include "map.hpp"

#include <cassert>
#include <optional>
#include <ostream>
#include <string>
#include <vector>

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>

namespace castlecrawl
{
    using DirKeyOpt_t = std::optional<sf::Keyboard::Key>;

    //

    namespace keys
    {
        static const sf::Keyboard::Key not_a_key{ sf::Keyboard::Unknown };

        inline std::string toString(const sf::Keyboard::Key dir)
        {
            // clang-format off
            if      (dir == sf::Keyboard::Up)    { return "Down";  }
            else if (dir == sf::Keyboard::Down)  { return "Up";    }
            else if (dir == sf::Keyboard::Left)  { return "Right"; }
            else if (dir == sf::Keyboard::Right) { return "Left";  }
            else { return ""; }
            // clang-format on
        }

        inline bool isHoriz(const sf::Keyboard::Key dir)
        {
            return ((dir == sf::Keyboard::Left) || (dir == sf::Keyboard::Right));
        }

        inline bool isVert(const sf::Keyboard::Key dir)
        {
            return ((dir == sf::Keyboard::Up) || (dir == sf::Keyboard::Down));
        }

        inline bool isArrow(const sf::Keyboard::Key dir) { return (isHoriz(dir) || isVert(dir)); }

        inline sf::Keyboard::Key opposite(const sf::Keyboard::Key dir)
        {
            // clang-format off
            if      (dir == sf::Keyboard::Up)    { return sf::Keyboard::Down;  }
            else if (dir == sf::Keyboard::Down)  { return sf::Keyboard::Up;    }
            else if (dir == sf::Keyboard::Left)  { return sf::Keyboard::Right; }
            else if (dir == sf::Keyboard::Right) { return sf::Keyboard::Left;  }
            else { return dir; }
            // clang-format on
        }

        inline bool isOpposite(const sf::Keyboard::Key first, const sf::Keyboard::Key second)
        {
            return (opposite(first) == second);
        }

        inline bool isLateral(const sf::Keyboard::Key first, const sf::Keyboard::Key second)
        {
            return ((isHoriz(first) && isVert(second)) || (isHoriz(second) && isVert(first)));
        }

        inline std::pair<sf::Keyboard::Key, sf::Keyboard::Key>
            lateralPair(const sf::Keyboard::Key dir)
        {
            if (isHoriz(dir))
            {
                return { sf::Keyboard::Up, sf::Keyboard::Down };
            }

            if (isVert(dir))
            {
                return { sf::Keyboard::Left, sf::Keyboard::Right };
            }

            return { not_a_key, not_a_key };
        }

        inline MapPos_t moveIfDir(const MapPos_t & oldPos, const sf::Keyboard::Key dir)
        {
            MapPos_t newPos{ oldPos };

            // clang-format off
            if      (dir == sf::Keyboard::Up)    { --newPos.y; }
            else if (dir == sf::Keyboard::Down)  { ++newPos.y; }
            else if (dir == sf::Keyboard::Left)  { --newPos.x; }
            else if (dir == sf::Keyboard::Right) { ++newPos.x; }
            // clang-format on

            return newPos;
        }

        inline sf::Keyboard::Key theOnlyArrowKeyDown()
        {
            sf::Keyboard::Key dir{ not_a_key };
            std::size_t count{ 0 };

            // clang-format off
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    { ++count; dir = sf::Keyboard::Up;    }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  { ++count; dir = sf::Keyboard::Down;  }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  { ++count; dir = sf::Keyboard::Left;  }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) { ++count; dir = sf::Keyboard::Right; }
            // clang-format on

            if ((not_a_key != dir) && (1 == count))
            {
                return dir;
            }
            else
            {
                return not_a_key;
            }
        }

        inline void allTheArrowKeysDown(std::vector<sf::Keyboard::Key> & arrowKeys)
        {
            // clang-format off
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))     { arrowKeys.push_back( sf::Keyboard::Up   ); }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))   { arrowKeys.push_back( sf::Keyboard::Down ); }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))   { arrowKeys.push_back( sf::Keyboard::Left ); }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))  { arrowKeys.push_back( sf::Keyboard::Right); }
            // clang-format on
        }

        //

        inline std::ostream & operator<<(std::ostream & os, const sf::Keyboard::Key dir)
        {
            os << keys::toString(dir);
            return os;
        }

        template <typename T>
        std::optional<T> toNumberOpt(const sf::Keyboard::Key key)
        {
            static_assert(std::is_arithmetic_v<T>);

            // clang-format off
            if (key == sf::Keyboard::Num0) { return T(0); }
            if (key == sf::Keyboard::Num1) { return T(1); }
            if (key == sf::Keyboard::Num2) { return T(2); }
            if (key == sf::Keyboard::Num3) { return T(3); }
            if (key == sf::Keyboard::Num4) { return T(4); }
            if (key == sf::Keyboard::Num5) { return T(5); }
            if (key == sf::Keyboard::Num6) { return T(6); }
            if (key == sf::Keyboard::Num7) { return T(7); }
            if (key == sf::Keyboard::Num8) { return T(8); }
            if (key == sf::Keyboard::Num9) { return T(9); }
            // clang-format on

            return std::nullopt;
        }
    } // namespace keys
} // namespace castlecrawl

#endif // CASTLECRAWL_KEYS_HPP_INCLUDED

#ifndef BOARDGAME_MAP_HPP_INCLUDED
#define BOARDGAME_MAP_HPP_INCLUDED
//
// map.hpp
//
#include "context.hpp"
#include "random.hpp"

#include <string>
#include <vector>

#include <SFML/Graphics/Rect.hpp>

namespace boardgame
{

    using MapPos_t = sf::Vector2i;
    using MapChars_t = std::vector<std::string>;

    class Map
    {
      public:
        void reset(Context & context, const MapChars_t & mapChars)
        {
            m_mapChars = mapChars;
            m_floorChars = mapChars;
            randomizeFloorTiles(context);
            addWallCorners();
        }

        bool isPosValid(const MapPos_t & pos) const
        {
            return ((pos.x >= 0) && (pos.y >= 0) && (pos.x < size().x) && (pos.y < size().y));
        }

        bool isPosValid(const int x, const int y) const { return isPosValid(MapPos_t{ x, y }); }

        char getChar(const MapPos_t & pos) const
        {
            if (empty() || !isPosValid(pos))
            {
                return '\'';
            }
            else
            {
                return m_mapChars[static_cast<std::size_t>(pos.y)][static_cast<std::size_t>(pos.x)];
            }
        }

        char getChar(const int x, const int y) const { return getChar(MapPos_t{ x, y }); }

        void setChar(const int x, const int y, const char newChar)
        {
            if (isPosValid(x, y))
            {
                m_mapChars[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)] = newChar;
            }
        }

        MapPos_t size() const
        {
            if (m_mapChars.empty())
            {
                return { 0, 0 };
            }
            else
            {
                return { static_cast<int>(m_mapChars.front().size()),
                         static_cast<int>(m_mapChars.size()) };
            }
        }

        bool empty() const
        {
            const MapPos_t mapSize{ size() };
            return !((mapSize.x > 0) && (mapSize.y > 0));
        }

        void randomizeFloorTiles(Context & context)
        {
            for (std::string & str : m_floorChars)
            {
                for (char & ch : str)
                {
                    if ('`' != ch)
                    {
                        ch = '0' + context.random.fromTo<char>(1, 6);
                    }
                }
            }
        }

        void addWallCorners()
        {
            if (empty())
            {
                return;
            }

            const MapPos_t mapSize(size());

            for (int y(0); y < mapSize.y; ++y)
            {
                for (int x(0); x < mapSize.x; ++x)
                {
                    const char ch{ getChar(x, y) };

                    if ((ch != '|') && (ch != '-'))
                    {
                        continue;
                    }

                    const char up{ getChar(x, y - 1) };
                    const char down{ getChar(x, y + 1) };
                    const char left{ getChar(x - 1, y) };
                    const char right{ getChar(x + 1, y) };

                    // std::size_t vertWallCount{ 0 };

                    if (ch == '|')
                    {
                        if ((left == '-') || (right == '-'))
                        {
                            setChar(x, y, 'B');
                        }

                        if ((down == '|') && ((up == ' ') || (up == '\'')))
                        {
                            setChar(x, y, 'B');
                        }

                        if ((up == '|') && ((down == ' ') || (down == '\'')))
                        {
                            setChar(x, y, 'B');
                        }
                    }
                    else //'-'
                    {
                        if ((up == '|') || (down == '|'))
                        {
                            setChar(x, y, 'B');
                        }

                        if ((left == '-') && ((right == ' ') || (right == '\'')))
                        {
                            setChar(x, y, 'B');
                        }

                        if ((right == '-') && ((left == ' ') || (left == '\'')))
                        {
                            setChar(x, y, 'B');
                        }
                    }

                    if (getChar(x, y) != 'B')
                    {
                        continue;
                    }

                    if ((down == '|') || (down == '-'))
                    {
                        if ((right == '|') || (right == '-'))
                        {
                            setChar(x, y, 'C');
                        }
                        else
                        {
                            setChar(x, y, 'T');
                        }
                    }
                }
            }
        }

        // private:
        MapChars_t m_mapChars;
        MapChars_t m_floorChars;
    };

} // namespace boardgame

#endif // BOARDGAME_MAP_HPP_INCLUDED

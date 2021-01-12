// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// map.cpp
//
#include "map.hpp"

//

namespace castlecrawl
{

    void Map::reset(Context & context, const MapChars_t & mapChars)
    {
        m_mapChars = mapChars;
        m_floorChars = mapChars;
        randomizeFloorTiles(context);
        addWallCorners();
    }

    char Map::getChar(const MapPos_t & pos) const
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

    void Map::setChar(const int x, const int y, const char newChar)
    {
        if (!isPosValid(x, y))
        {
            return;
        }

        m_mapChars[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)] = newChar;
    }

    MapPos_t Map::size() const
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

    void Map::randomizeFloorTiles(Context & context)
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

    void Map::addWallCorners()
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

} // namespace castlecrawl

// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// map.cpp
//
#include "map.hpp"

#include "board.hpp"
#include "door.hpp"
#include "random.hpp"
#include "resources.hpp"
#include "settings.hpp"
#include "util.hpp"

#include <SFML/Graphics.hpp>

namespace castlecrawl
{
    Map::Map()
        : m_chars()
        , m_floorChars(false)
        , m_links()
    {}

    Map::Map(
        const Context & context,
        const bool isFloorStone,
        const MapChars_t & chars,
        const MapLinks_t & links)
        : m_isFloorStone(isFloorStone)
        , m_chars(chars)
        , m_floorChars()
        , m_links(links)
    {
        addWalls();
        addWallCorners();

        m_floorChars = m_chars;
        randomizeFloorTiles(context);
    }

    void Map::load(Context & context) { makeDoors(context); }

    char Map::getChar(const MapPos_t & pos) const
    {
        if (empty() || !isPosValid(pos))
        {
            return '.';
        }
        else
        {
            return m_chars[static_cast<std::size_t>(pos.y)][static_cast<std::size_t>(pos.x)];
        }
    }

    void Map::setChar(const int x, const int y, const char newChar)
    {
        if (!isPosValid(x, y))
        {
            return;
        }

        m_chars[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)] = newChar;
    }

    MapPos_t Map::size() const
    {
        if (m_chars.empty())
        {
            return { 0, 0 };
        }
        else
        {
            return { static_cast<int>(m_chars.front().size()), static_cast<int>(m_chars.size()) };
        }
    }

    void Map::randomizeFloorTiles(const Context & context)
    {
        for (std::string & str : m_floorChars)
        {
            for (char & ch : str)
            {
                if ('.' != ch)
                {
                    if (m_isFloorStone)
                    {
                        ch = '6';
                    }
                    else
                    {
                        ch = '0';
                    }

                    ch += context.random.fromTo<char>(0, 5);
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

                if (ch == '|')
                {
                    if ((left == '-') || (right == '-'))
                    {
                        setChar(x, y, 'B');
                    }

                    if ((down == '|') && ((up == ' ') || (up == '.')))
                    {
                        setChar(x, y, 'B');
                    }

                    if ((up == '|') && ((down == ' ') || (down == '.')))
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

                    if ((left == '-') && ((right == ' ') || (right == '.')))
                    {
                        setChar(x, y, 'B');
                    }

                    if ((right == '-') && ((left == ' ') || (left == '.')))
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

    void Map::addWalls()
    {
        if (empty())
        {
            return;
        }

        auto isNotObs = [](const char ch) { return ((ch != '.') && (ch != '|') && (ch != '-')); };

        const MapPos_t mapSize(size());

        for (int y(0); y < mapSize.y; ++y)
        {
            for (int x(0); x < mapSize.x; ++x)
            {
                const char ch{ getChar(x, y) };

                if (ch != '.')
                {
                    continue;
                }

                // check all four sides
                const char up{ getChar(x, y - 1) };
                const char down{ getChar(x, y + 1) };
                const char left{ getChar(x - 1, y) };
                const char right{ getChar(x + 1, y) };

                if (isNotObs(left) || isNotObs(right))
                {
                    setChar(x, y, '|');
                    continue;
                }
                else if (isNotObs(up) || isNotObs(down))
                {
                    setChar(x, y, '-');
                    continue;
                }

                // check all four corners
                const char tl{ getChar(x - 1, y - 1) };
                const char tr{ getChar(x + 1, y - 1) };
                const char bl{ getChar(x - 1, y + 1) };
                const char br{ getChar(x + 1, y + 1) };

                if (isNotObs(tl) || isNotObs(tr) || isNotObs(bl) || isNotObs(br))
                {
                    setChar(x, y, '|'); // '-' works here too
                    continue;
                }
            }
        }
    }

    void Map::draw(Context & context, sf::RenderTarget & target, sf::RenderStates states) const
    {
        drawChars(context, target, states, m_floorChars);
        drawChars(context, target, states, m_chars);
    }

    void Map::drawChars(
        Context & context,
        sf::RenderTarget & target,
        sf::RenderStates states,
        const MapChars_t & mapChars) const
    {
        sf::Sprite tileSprite = context.media.sprite(TileImage::Empty);
        sf::Sprite shadowSprite = context.media.sprite(TileImage::WallHorizShadow);

        const float mapCellDimm{ context.config.mapCellDimm() };
        const sf::Vector2f boardPos{ util::position(context.layout.boardBounds()) };
        sf::Vector2f pos{ boardPos };

        for (const std::string & mapLine : mapChars)
        {
            char prevMapChar(0);
            for (const char mapChar : mapLine)
            {
                tileSprite = context.media.sprite(mapCharToTileImage(mapChar));
                tileSprite.setPosition(pos);
                target.draw(tileSprite, states);

                // draw horiz wall shadow accents
                if (('-' == mapChar) && ('-' != prevMapChar))
                {
                    shadowSprite.setPosition(pos);
                    target.draw(shadowSprite, states);
                }

                prevMapChar = mapChar;
                pos.x += mapCellDimm;
            }

            pos.x = boardPos.x;
            pos.y += mapCellDimm;
        }
    }

    void Map::makeDoors(Context & context)
    {
        context.board.doors.clear();

        if (empty())
        {
            return;
        }

        const MapPos_t mapSize(size());

        for (int y(0); y < mapSize.y; ++y)
        {
            for (int x(0); x < mapSize.x; ++x)
            {
                const MapPos_t mapPos{ x, y };
                const char ch{ getChar(mapPos) };

                if ((ch != 'D') && (ch != 'd'))
                {
                    continue;
                }

                // add piece to board
                Door door;
                door.reset(context, mapPos, (ch == 'D'));
                context.board.doors.push_back(door);
            }
        }
    }

} // namespace castlecrawl

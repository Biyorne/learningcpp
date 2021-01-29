// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// map.cpp
//
#include "map.hpp"

#include "board.hpp"
#include "door-piece.hpp"
#include "random.hpp"
#include "resources.hpp"
#include "settings.hpp"
#include "util.hpp"

#include <SFML/Graphics.hpp>

namespace castlecrawl
{
    Map::Map()
        : m_isFloorStone(false)
        , m_chars()
        , m_floorChars(false)
        , m_links()
        , m_floorVerts()
        , m_borderVerts()
        , m_wallVerts()
        , m_transVerts()

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
        , m_floorVerts()
        , m_borderVerts()
        , m_wallVerts()
        , m_transVerts()
    {
        addWalls();
        addWallCorners();

        m_floorChars = m_chars;
        randomizeFloorTiles(context);
    }

    void Map::load(Context & context)
    {
        makeDoors(context);

        m_floorVerts.clear();
        m_borderVerts.clear();
        m_wallVerts.clear();
        m_transVerts.clear();

        m_floorVerts.reserve(4000);
        m_borderVerts.reserve(4000);
        m_wallVerts.reserve(4000);
        m_transVerts.reserve(4000);

        makeVerts(context, m_floorChars, m_floorVerts);
        makeBorderVerts(context, m_floorChars, m_borderVerts);
        makeVerts(context, m_chars, m_wallVerts);
        makeStoneTransitionVerts(context, m_transVerts);
    }

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

    void
        Map::draw(const Context & context, sf::RenderTarget & target, sf::RenderStates states) const
    {
        states.texture = &context.media.tileTexture();

        if (!m_floorVerts.empty())
        {
            target.draw(&m_floorVerts[0], m_floorVerts.size(), sf::Quads, states);
        }

        if (!m_borderVerts.empty())
        {
            // do not use states because these verts have no texture
            target.draw(&m_borderVerts[0], m_borderVerts.size(), sf::Quads);
        }

        if (!m_wallVerts.empty())
        {
            target.draw(&m_wallVerts[0], m_wallVerts.size(), sf::Quads, states);
        }

        if (!m_transVerts.empty())
        {
            target.draw(&m_transVerts[0], m_transVerts.size(), sf::Quads, states);
        }
    }

    void Map::makeVerts(const Context & context, const MapChars_t & mapChars, VertVec_t & verts)
    {
        sf::Sprite tileSprite = context.media.sprite(TileImage::Empty);
        sf::Sprite shadowSprite = context.media.sprite(TileImage::WallHorizShadow);

        const float mapCellDimm{ context.layout.mapCellDimm() };
        const sf::Vector2f boardPos{ util::position(context.layout.boardBounds()) };
        sf::Vector2f pos{ boardPos };

        for (const std::string & mapLine : mapChars)
        {
            char prevMapChar(0);
            for (const char mapChar : mapLine)
            {
                tileSprite = context.media.sprite(mapCharToTileImage(mapChar));
                tileSprite.setPosition(pos);

                util::appendQuadVerts(
                    tileSprite.getGlobalBounds(), tileSprite.getTextureRect(), verts);

                // draw horiz wall shadow accents
                if (('-' == mapChar) && ('-' != prevMapChar))
                {
                    shadowSprite.setPosition(pos);

                    util::appendQuadVerts(
                        shadowSprite.getGlobalBounds(), shadowSprite.getTextureRect(), verts);
                }

                prevMapChar = mapChar;
                pos.x += mapCellDimm;
            }

            pos.x = boardPos.x;
            pos.y += mapCellDimm;
        }
    }

    void Map::makeBorderVerts(
        const Context & context, const MapChars_t & mapChars, VertVec_t & verts)
    {
        sf::Sprite sprite = context.media.sprite(TileImage::Black);
        sprite.setColor(context.config.background_color);
        sprite.scale(1.5f, 1.5f);

        const float mapCellDimm{ context.layout.mapCellDimm() };
        const sf::Vector2f boardPos{ util::position(context.layout.boardBounds()) };

        const float overlapDimm{ mapCellDimm * 0.25f };
        const sf::Vector2f overlap{ overlapDimm, overlapDimm };

        sf::Vector2f pos{ boardPos };

        for (const std::string & mapLine : mapChars)
        {
            for (const char mapChar : mapLine)
            {
                if (mapChar == '.')
                {
                    sprite.setPosition(pos);
                    sprite.move(-overlapDimm, -overlapDimm);

                    util::appendQuadVerts(
                        sprite.getGlobalBounds(), verts, context.config.background_color);
                }

                pos.x += mapCellDimm;
            }

            pos.x = boardPos.x;
            pos.y += mapCellDimm;
        }
    }

    void Map::makeStoneTransitionVerts(const Context & context, VertVec_t & verts)
    {
        if (empty())
        {
            return;
        }

        auto notLiq = [](const char ch) { return ((ch != 'l') && (ch != 'w')); };

        sf::Sprite sprite = context.media.sprite(TileImage::Empty);

        const float mapCellDimm{ context.layout.mapCellDimm() };
        const sf::Vector2f boardPos{ util::position(context.layout.boardBounds()) };
        sf::Vector2f pos{ boardPos };

        const MapPos_t mapSize(size());
        for (int y(0); y < mapSize.y; ++y)
        {
            for (int x(0); x < mapSize.x; ++x)
            {
                const char ch{ getChar(x, y) };

                if (notLiq(ch))
                {
                    pos.x += mapCellDimm;
                    continue;
                }

                // check all four sides
                const char up{ getChar(x, y - 1) };
                const char down{ getChar(x, y + 1) };
                const char left{ getChar(x - 1, y) };
                const char right{ getChar(x + 1, y) };

                if (notLiq(up) && notLiq(left))
                {
                    sprite = context.media.sprite(TileImage::StoneTopLft);
                    sprite.setPosition(pos);
                    util::appendQuadVerts(sprite.getGlobalBounds(), sprite.getTextureRect(), verts);
                }

                if (notLiq(up) && notLiq(right))
                {
                    sprite = context.media.sprite(TileImage::StoneTopRgt);
                    sprite.setPosition(pos);
                    util::appendQuadVerts(sprite.getGlobalBounds(), sprite.getTextureRect(), verts);
                }

                if (notLiq(down) && notLiq(left))
                {
                    sprite = context.media.sprite(TileImage::StoneBotLft);
                    sprite.setPosition(pos);
                    util::appendQuadVerts(sprite.getGlobalBounds(), sprite.getTextureRect(), verts);
                }

                if (notLiq(down) && notLiq(right))
                {
                    sprite = context.media.sprite(TileImage::StoneBotRgt);
                    sprite.setPosition(pos);
                    util::appendQuadVerts(sprite.getGlobalBounds(), sprite.getTextureRect(), verts);
                }

                if (notLiq(up))
                {
                    sprite = context.media.sprite(TileImage::StoneTop);
                    sprite.setPosition(pos);
                    util::appendQuadVerts(sprite.getGlobalBounds(), sprite.getTextureRect(), verts);
                }

                if (notLiq(down))
                {
                    sprite = context.media.sprite(TileImage::StoneBot);
                    sprite.setPosition(pos);
                    util::appendQuadVerts(sprite.getGlobalBounds(), sprite.getTextureRect(), verts);
                }

                if (notLiq(left))
                {
                    sprite = context.media.sprite(TileImage::StoneLft);
                    sprite.setPosition(pos);
                    util::appendQuadVerts(sprite.getGlobalBounds(), sprite.getTextureRect(), verts);
                }

                if (notLiq(right))
                {
                    sprite = context.media.sprite(TileImage::StoneRgt);
                    sprite.setPosition(pos);
                    util::appendQuadVerts(sprite.getGlobalBounds(), sprite.getTextureRect(), verts);
                }

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
                DoorPiece door;
                door.reset(context, mapPos, (ch == 'D'));
                context.board.doors.push_back(door);
            }
        }
    }

} // namespace castlecrawl

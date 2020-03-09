// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// meth-head.cpp
//
#include "pieces.hpp"

#include "board.hpp"
#include "check-macros.hpp"
#include "context.hpp"
#include "keys.hpp"
#include "random.hpp"
#include "resources.hpp"
#include "settings.hpp"
#include "sound-player.hpp"

#include <cassert>

namespace boardgame
{
    SimplePiece::SimplePiece(Context &, const Piece piece, const BoardPos_t & pos)
        : m_piece(piece)
        , m_position(pos)
        , m_sprite()
    {}

    SimplePiece::SimplePiece(
        Context &, const Piece piece, const BoardPos_t & pos, const sf::Sprite & sprite)
        : m_piece(piece)
        , m_position(pos)
        , m_sprite(sprite)
    {}

    SimplePiece::SimplePiece(
        Context & context,
        const Piece piece,
        const BoardPos_t & pos,
        const sf::Color & color,
        const bool willSkewToFitExactly)
        : SimplePiece(
              context,
              piece,
              pos,
              context.media.makeDefaultSprite(context, piece, pos, color, willSkewToFitExactly))
    {}

    void SimplePiece::draw(sf::RenderTarget & target, sf::RenderStates states) const
    {
        target.draw(m_sprite, states);
    }

    void SimplePiece::move(Context & context, const BoardPos_t & newPos)
    {
        const auto posBefore{ m_position };
        M_CHECK_SS((newPos != m_position), newPos);
        context.board.removePiece(newPos);
        m_position = newPos;
        util::centerInside(m_sprite, context.layout.cellBounds(position()));
        M_CHECK(context.board.pieceEnumAt(newPos) == m_piece);
        M_CHECK(context.board.pieceEnumAt(posBefore) == Piece::Count);
    }

    //

    FoodPiece::FoodPiece(Context & context, const BoardPos_t & pos, const Piece)
        : SimplePiece(context, Piece::Food, pos, toColor(Piece::Food), true)
    {
        const float pad{ context.config.between_cells_pad_ratio };
        m_sprite.scale(pad, pad);
    }

    //

    WallPiece::WallPiece(Context & context, const BoardPos_t & pos, const Piece)
        : SimplePiece(context, Piece::Wall, pos, toColor(Piece::Wall), true)
    {
        const float pad{ context.config.between_cells_pad_ratio };
        m_sprite.scale(pad, pad);
    }

    //

    EaterPiece::EaterPiece(Context & context, const BoardPos_t & pos, const Piece)
        : SimplePiece(context, Piece::Eater, pos, toColor(Piece::Eater), true)
        , m_direction(context.random.from(
              { sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Left, sf::Keyboard::Right }))
    {
        const float pad{ context.config.between_cells_pad_ratio };
        m_sprite.scale(pad, pad);
    }

    // void EaterPiece::takeTurn(Context & context)
    //{
    //    static std::set<sf::Keyboard::Key> directions;
    //    directions.clear();
    //    directions.insert(sf::Keyboard::Up);
    //    directions.insert(sf::Keyboard::Down);
    //    directions.insert(sf::Keyboard::Left);
    //    directions.insert(sf::Keyboard::Right);
    //
    //    // directions.erase(keys::opposite(m_direction));
    //
    //    m_direction = context.random.from(directions);
    //
    //    const BoardPos_t newPos{ keys::move(position(), m_direction) };
    //    M_CHECK((newPos != position()));
    //
    //    const Piece newPosPieceEnum{ context.board.pieceEnumAt(newPos) };
    //    if (newPosPieceEnum == Piece::Wall)
    //    {
    //        return;
    //    }
    //
    //    SimplePiece::move(context, newPos);
    //}

    void EaterPiece::takeTurn(Context & context)
    {
        static std::vector<PosInfo> posInfos;

        posInfos.clear();
        posInfos.push_back({ context, sf::Keyboard::Up, position() });
        posInfos.push_back({ context, sf::Keyboard::Down, position() });
        posInfos.push_back({ context, sf::Keyboard::Left, position() });
        posInfos.push_back({ context, sf::Keyboard::Right, position() });

        posInfos.erase(
            std::remove_if(
                std::begin(posInfos),
                std::end(posInfos),
                [&](const PosInfo & info) { return (info.piece == Piece::Wall); }),
            std::end(posInfos));

        if (posInfos.empty())
        {
            return;
        }

        context.random.shuffle(posInfos);
        m_direction = posInfos.front().dir;
        const BoardPos_t newPos{ keys::move(position(), m_direction) };
        SimplePiece::move(context, newPos);

        // if (posInfos.size() == 1)
        //{
        //    m_direction = posInfos.front().dir;
        //}
        // else
        //{
        //    std::sort(
        //        std::begin(posInfos),
        //        std::end(posInfos),
        //        [&](const PosInfo & A, const PosInfo & B) {
        //            if (A.isOccupiedBy(Piece::Eater) != B.isOccupiedBy(Piece::Eater))
        //            {
        //                return A.isOccupiedBy(Piece::Eater);
        //            }
        //            else if (A.isOccupiedBy(Piece::Food) != B.isOccupiedBy(Piece::Food))
        //            {
        //                return A.isOccupiedBy(Piece::Food);
        //            }
        //            else
        //            {
        //                return (A.dir < B.dir);
        //            }
        //        });
        //
        //    m_direction = posInfos.front().dir;
        //}
        //
        // const BoardPos_t newPos{ keys::move(position(), m_direction) };
        // SimplePiece::move(context, newPos);
        // return;
    }

    //

    PosInfo::PosInfo(
        const Context & context, const sf::Keyboard::Key direction, const BoardPos_t & selfPos)
        : dir(direction)
        , pos(keys::move(selfPos, dir))
        , piece(context.board.pieceEnumAt(pos))
    {}

    bool PosInfo::isOccupied() const { return (Piece::Count != piece); }

    bool PosInfo::isOccupiedBy(const Piece potentialPiece) const
    {
        return (isOccupied() && (potentialPiece == piece));
    }

    bool PosInfo::isOccupiedButNotBy(const Piece potentialPiece) const
    {
        return (isOccupied() && (potentialPiece != piece));
    }
} // namespace boardgame
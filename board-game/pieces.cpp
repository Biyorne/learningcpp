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
        context.board.removePiece(newPos);
        m_position = newPos;
        util::centerInside(m_sprite, context.layout.cellBounds(position()));
    }

    //

    CellPiece::CellPiece(Context & context, const BoardPos_t & pos, const Piece)
        : SimplePiece(context, Piece::Cell, pos, makeDefaultSprite(context, pos, Piece::Cell))
    {}

    sf::Sprite CellPiece::makeDefaultSprite(
        const Context & context, const BoardPos_t & boardPos, const Piece piece)
    {
        sf::Sprite sprite{ context.media.makeDefaultSprite(
            context, Piece::Cell, boardPos, toColor(piece), true) };

        // shrink the size of the cell to make a nice looking border around them all
        const float pad{ context.config.between_cells_pad_ratio };

        sprite.scale(pad, pad);

        return sprite;
    }
} // namespace boardgame
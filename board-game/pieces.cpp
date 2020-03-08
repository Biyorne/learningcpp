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
        : BoardPosKeeper(pos)
        , m_piece(piece)
        , m_sprite()
    {}

    SimplePiece::SimplePiece(
        Context &, const Piece piece, const BoardPos_t & pos, const sf::Sprite & sprite)
        : BoardPosKeeper(pos)
        , m_piece(piece)
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
        context.board.movePiece(context, *this, newPos);
        util::centerInside(m_sprite, context.layout.cellBounds(position()));
    }

    //

    CellPiece::CellPiece(Context & context, const BoardPos_t & pos, const Piece piece)
        : SimplePiece(context, piece, pos, toColor(piece), true)
    {
        // shrink the size of the cell to make a nice looking border around them all
        const float pad{ context.config.between_cells_pad_ratio };
        m_sprite.scale(pad, pad);
    }

    void CellPiece::takeTurn(Context &)
    {
        if (Piece::On == m_piece)
        {
            m_piece = Piece::Off;
        }
        else
        {
            m_piece = Piece::On;
        }

        m_sprite.setColor(toColor(m_piece));
    }
} // namespace boardgame
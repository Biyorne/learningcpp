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

    void SimplePiece::move(Context & context, const BoardPos_t & posNew)
    {
        M_CHECK_SS((posNew != m_position), "posCurrent=" << m_position << ", posNew=" << m_piece);

        context.board.removePiece(context, posNew);

        // we don't use Board::placePiece() because there is no need
        m_position = posNew;
        util::centerInside(m_sprite, context.layout.cellBounds(posNew));

        M_CHECK(context.board.pieceEnumAt(posNew) == m_piece);
    }

    //

    CellPiece::CellPiece(Context & context, const BoardPos_t & pos, const Piece piece)
        : SimplePiece(context, piece, pos, toColor(piece), true)
    {
        // shrink the size of the cell to make a nice looking border around them all
        const float m_betweenCellsPadRatio{ 0.975f };
        m_sprite.scale(m_betweenCellsPadRatio, m_betweenCellsPadRatio);
    }

    void CellPiece::toggleOnOff(Context &)
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

    bool CellPiece::handleEvent(Context & context, const sf::Event & event)
    {
        if (event.type != sf::Event::MouseButtonPressed)
        {
            return false;
        }

        const sf::Vector2f clickPos{ sf::Vector2i(event.mouseButton.x, event.mouseButton.y) };
        if (bounds().contains(clickPos))
        {
            takeTurn(context);
            return true;
        }

        return false;
    }
} // namespace boardgame
// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// meth-head.cpp
//
#include "pieces.hpp"

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
    /*
    SimplePiece::SimplePiece(Context &, const Piece piece, const BoardPos_t & pos)
        : m_piece(piece)
        , m_sprite()
        , m_position(pos)
    {}

    SimplePiece::SimplePiece(
        Context &, const Piece piece, const BoardPos_t & pos, const sf::Sprite & sprite)
        : m_piece(piece)
        , m_sprite(sprite)
        , m_position(pos)
    {}

    // SimplePiece::SimplePiece(
    //    Context & context, const Piece piece, const BoardPos_t & pos, const sf::Color & color)
    //    : SimplePiece(
    //          context, piece, pos, context.media.makeDefaultSprite(context, piece, pos, color))
    //{}

    void SimplePiece::draw(sf::RenderTarget & target, sf::RenderStates states) const
    {
        target.draw(m_sprite, states);
    }

    void SimplePiece::updateAfterMove(Context & context, const BoardPos_t & newPos)
    {
        m_position = newPos;
        util::centerInside(m_sprite, context.layout.cellBounds(position()));
    }
    */

} // namespace boardgame

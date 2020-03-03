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
#include "settings.hpp"
#include "sound-player.hpp"
#include "util.hpp"

#include <cassert>
#include <functional>

namespace boardgame
{
    void PieceBase::move(Context & context, const BoardPos_t & posNew)
    {
        M_CHECK_SS((posNew != m_position), "posCurrent=" << m_position << ", posNew=" << m_piece);

        context.board.removePiece(context, posNew);

        // we don't use Board::placePiece() because there is no need
        m_position = posNew;
        util::centerInside(m_sprite, context.board.layout().bounds(posNew));

        M_CHECK(context.board.pieceEnumAt(posNew) == m_piece);
    }

    //

    void HeadPiece::handleEvent(Context & context, const sf::Event & event)
    {
        if (sf::Event::KeyPressed != event.type)
        {
            return;
        }

        if (keys::isArrow(event.key.code))
        {
            if (m_directionKeyNext != event.key.code)
            {
                context.audio.play("tap-1-a.ogg");
                m_directionKeyNext = event.key.code;
            }

            return;
        }
    }

    void HeadPiece::update(Context & context, const float elapsedTimeSec)
    {
        m_timeBetweenTurnsElapsedSec += elapsedTimeSec;
        if (m_timeBetweenTurnsElapsedSec > m_timeBetweenTurnsSec)
        {
            m_timeBetweenTurnsElapsedSec = 0.0f;
            takeTurn(context);
        }
    }

    void HeadPiece::takeTurn(Context & context)
    {
        const BoardPos_t posOld{ m_position };
        const BoardPos_t posNew{ keys::move(m_position, m_directionKeyNext) };
        M_CHECK(posOld != posNew);

        const Piece posNewPieceEnum{ context.board.pieceEnumAt(posNew) };

        // move head
        move(context, posNew);
        M_CHECK(context.board.pieceEnumAt(posNew) == Piece::Head);
        M_CHECK(context.board.pieceEnumAt(posOld) == Piece::Count);

        // place a new tail piece behind the head
        context.board.addPiece(context, Piece::Tail, posOld);

        // handle whatever we just ate
        if (posNewPieceEnum == Piece::Food)
        {
            context.audio.play("shine", m_eatSfxPitch);
            m_eatSfxPitch += m_eatSfxPitchAdj;
            m_timeBetweenTurnsSec *= m_timeBetweenTurnsShrinkRatio;
            return;
        }

        if (posNewPieceEnum != Piece::Count)
        {
            context.audio.play("rpg-game-over");
            m_sprite.setColor(sf::Color::Red);
            context.settings.isGameOver(true);
            return;
        }
    }
} // namespace boardgame
// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// player-piece.cpp
//
#include "player-piece.hpp"

#include "animation-player.hpp"
#include "board.hpp"
#include "check-macros.hpp"
#include "door-piece.hpp"
#include "keys.hpp"
#include "settings.hpp"
#include "sound-player.hpp"

namespace castlecrawl
{
    void PlayerPiece::reset(Context & context, const MapPos_t & pos)
    {
        PieceBase::reset(context, pos, 'p', false);
    }

    void PlayerPiece::handleEvent(Context & context, const sf::Event & event)
    {
        if (sf::Event::KeyPressed != event.type)
        {
            return;
        }

        const sf::Keyboard::Key key{ event.key.code };

        if (keys::isArrow(key))
        {
            handleArrowKey(context, key);
        }

        // TODO REMOVE TEMP ANIMATION TEST
        if (sf::Keyboard::Num1 == key)
        {
            util::AnimConfig config(1.0f, sf::Color(150, 255, 255), sf::BlendAdd);

            sf::FloatRect rect = context.layout.cellBounds(position());
            const float cellDimm{ context.layout.mapCellDimm() };
            rect.left -= (cellDimm * 0.25f);
            rect.top -= (cellDimm * 0.25f);
            rect.width *= 1.5f;
            rect.height *= 1.5f;

            context.anim.play("sparkle-burst", rect, config);
        }
        else if (sf::Keyboard::Num2 == key)
        {
            util::AnimConfig config(1.0f, sf::Color(255, 255, 150), sf::BlendAdd);

            sf::FloatRect rect = context.layout.cellBounds(position());
            const float cellDimm{ context.layout.mapCellDimm() };
            rect.left -= (cellDimm * 1.0f);
            rect.top -= (cellDimm * 1.0f);
            rect.width *= 3.0f;
            rect.height *= 3.0f;

            context.anim.play("star-flash", rect, config);
        }
    }

    void PlayerPiece::handleArrowKey(Context & context, const sf::Keyboard::Key arrowKey)
    {
        const MapPos_t newPos = keys::moveIfDir(position(), arrowKey);

        for (const MapLink & link : context.map().links())
        {
            if (link.from_pos == newPos)
            {
                const char currentChar = context.map().getChar(position());

                if ((currentChar == 'S') || (currentChar == 's'))
                {
                    context.audio.play("stairs.ogg");
                }
                else
                {
                    context.audio.play("tick-on-2.ogg");
                }

                context.switchToMap(link);
                return;
            }
        }

        const char newChar = context.map().getChar(newPos);

        if ((newChar != ' ') && (newChar != 'D') && (newChar != 'd') && (newChar != 'S') &&
            (newChar != 's'))
        {
            context.audio.play("tap-wood-low.ogg");
            return;
        }

        for (const DoorPiece & door : context.board.doors)
        {
            if (door.position() != newPos)
            {
                continue;
            }

            if (door.isLocked())
            {
                context.audio.play("locked.ogg");
                return;
            }
            else
            {
                move(context, arrowKey);
                context.audio.play("door-open.ogg");
                return;
            }
        }

        move(context, arrowKey);
        context.audio.play("tick-on-2.ogg");
    }

} // namespace castlecrawl

// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// player.cpp
//
#include "player.hpp"

#include "board.hpp"
#include "door.hpp"
#include "keys.hpp"
#include "sound-player.hpp"

namespace castlecrawl
{
    void Player::reset(Context & context, const MapPos_t & pos)
    {
        PieceBase::reset(context, pos, 'p', false);
    }

    void Player::handleEvent(Context & context, const sf::Event & event)
    {
        if (sf::Event::KeyPressed != event.type)
        {
            return;
        }

        const sf::Keyboard::Key key{ event.key.code };

        if (keys::isArrow(key))
        {
            const MapPos_t newPos = keys::moveIfDir(position(), key);

            for (const MapLink & link : context.map().links())
            {
                if (link.from_pos == newPos)
                {
                    context.map_name = link.to_name;
                    context.map().load(context);
                    context.board.player.reset(context, link.to_pos);
                    return;
                }
            }

            const char newChar = context.map().getChar(newPos);

            if ((newChar != ' ') && (newChar != 'D') && (newChar != 'd'))
            {
                context.audio.play("tap-wood-low.ogg");
                return;
            }

            for (const Door & door : context.board.doors)
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
                    move(context, key);
                    context.audio.play("door-open.ogg");
                    return;
                }
            }

            move(context, key);
            context.audio.play("tick-on-2.ogg");
        }
    }

} // namespace castlecrawl

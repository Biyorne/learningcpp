// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// player.cpp
//
#include "player.hpp"

#include "keys.hpp"
#include "sound-player.hpp"

//

namespace castlecrawl
{
    Player::Player(Context & context, const MapPos_t & pos)
        : PieceBase(context, pos, 'p', false)
    {}

    void Player::handleEvent(Context & context, const sf::Event & event)
    {

        if (sf::Event::KeyPressed != event.type)
        {
            return;
        }

        const sf::Keyboard::Key key{ event.key.code };

        if (keys::isArrow(key))
        {
            const MapPos_t newPos{ keys::moveIfDir(position(), key) };

            if (context.map.getChar(newPos) == ' ')
            {
                move(context, key);
                context.audio.play("tick-on-2.ogg");
            }
            else
            {
                context.audio.play("tap-wood-low.ogg");
            }
        }
    }

} // namespace castlecrawl

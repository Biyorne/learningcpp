#ifndef MARIO_SFX_HPP_INCLUDED
#define MARIO_SFX_HPP_INCLUDED
//
// sfx.hpp
//
#include "context.hpp"
#include "image.hpp"

#include <iostream>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

namespace mario
{
    struct Sfx
    {
        enum Enum : std::size_t
        {
            BreakBlock = 0,
            Bump,
            Coin,
            Death,
            Fireball,
            GameOver,
            JumpSmall,
            JumpSuper,
            Kick,
            Pause,
            Pipe,
            Stomp,
            Count
        };

        struct BufferSound
        {
            sf::SoundBuffer buffer;
            sf::Sound sound;
        };

        static void setup()
        {
            m_sounds.resize(Enum::Count);

            for (std::size_t i(0); i < Enum::Count; ++i)
            {
                const Enum which{ static_cast<Enum>(i) };
                const std::string pathStr{ path(which) };

                sf::SoundBuffer & buffer{ m_sounds.at(i).buffer };
                if (!buffer.loadFromFile(pathStr))
                {
                    std::cout << "Failed to load sfx: \"" << pathStr << "\"" << std::endl;
                }

                sf::Sound & sound{ m_sounds.at(i).sound };
                sound.setVolume(50.0f);
            }
        }

        static void play(const Enum which)
        {
            const std::size_t index{ static_cast<size_t>(which) };
            if (index < static_cast<size_t>(Enum::Count))
            {
                m_sounds.at(index).sound.play();
            }
        }

        static void stopAll()
        {
            for (std::size_t i(0); i < Enum::Count; ++i)
            {
                m_sounds.at(i).sound.stop();
            }
        }

      private:
        // clang-format off
        static std::string path(const Enum which)
        {
            switch(which)
            {
                case BreakBlock:    return "mario-media/sfx-break-block.ogg";
                case Bump:          return "mario-media/sfx-bump.ogg";
                case Coin:          return "mario-media/sfx-coin.ogg";
                case Death:         return "mario-media/sfx-death.ogg";
                case Fireball:      return "mario-media/sfx-fireball.ogg";
                case GameOver:      return "mario-media/sfx-game-over.ogg";
                case JumpSmall:     return "mario-media/sfx-jump-small.ogg";
                case JumpSuper:     return "mario-media/sfx-jump-super.ogg";
                case Kick:          return "mario-media/sfx-kick.ogg";
                case Pause:         return "mario-media/sfx-pause.ogg";
                case Pipe:          return "mario-media/sfx-pipe.ogg";
                case Stomp:         return "mario-media/sfx-stomp.ogg";
                case Count:
                default:            throw std::runtime_error("Sfx::path(enum) switch default");
            }
        }
        // clang-format on

      private:
        static inline std::vector<BufferSound> m_sounds;
    };
} // namespace mario

#endif // MARIO_SFX_HPP_INCLUDED
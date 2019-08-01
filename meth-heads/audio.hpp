// ----------------------------------------------------------------------------
// "THE BEER-WARE LICENSE" (Revision 42):
// <ztn@zurreal.com> wrote this file.  As long as you retain this notice you
// can do whatever you want with this stuff. If we meet some day, and you think
// this stuff is worth it, you can buy me a beer in return.  Ziesche Til Newman
// ----------------------------------------------------------------------------
#ifndef METH_HEAD_AUDIO_HPP_INCLUDED
#define METH_HEAD_AUDIO_HPP_INCLUDED
//
// audio.hpp
//
#include <algorithm>
#include <iostream>

#include <SFML/Audio.hpp>

namespace methhead
{

    class Audio
    {
    public:
        Audio()
            : m_volumeMin(0.0f) // this is just the min that sfml uses
            , m_volumeMax(100.0f) // this is just the max that sfml uses
            , m_volumeAdj(m_volumeMax / 10.0f) // only ten different vol levels possible
            , m_volume(m_volumeMax / 2.0f) // start at half volume
            , m_bufferWalk()
            , m_soundWalk()
            , m_bufferCoin1()
            , m_soundCoin1()
            , m_bufferCoin2()
            , m_soundCoin2()
        {
            load("sound/walk.ogg", m_bufferWalk, m_soundWalk);
            load("sound/coins-1.ogg", m_bufferCoin1, m_soundCoin1);
            load("sound/coins-2.ogg", m_bufferCoin2, m_soundCoin2);
        }

        void playWalk() { play(m_soundWalk); }
        void playCoin1() { play(m_soundCoin1); }
        void playCoin2() { play(m_soundCoin2); }

        void volumeUp()
        {
            m_volume += m_volumeAdj;

            if (m_volume > m_volumeMax)
            {
                m_volume = m_volumeMax;
            }
        }

        void volumeDown()
        {
            m_volume -= m_volumeAdj;

            if (m_volume < m_volumeMin)
            {
                m_volume = m_volumeMin;
            }
        }

    private:
        void play(sf::Sound &)
        {
            // TODO turn sound back on here
            // if ((sound.getBuffer() != nullptr) && (m_volume > 0.0f))
            //{
            //    if (sound.getBuffer() == &m_bufferWalk)
            //    {
            //        sound.setVolume(m_volume * 0.1f);
            //    }
            //    else
            //    {
            //        sound.setVolume(m_volume * 2.0f);
            //    }
            //
            //    if (sound.getStatus() != sf::Sound::Status::Playing)
            //    {
            //        sound.play();
            //    }
            //}
        }

        void load(const std::string & filePath, sf::SoundBuffer & buffer, sf::Sound & sound)
        {
            if (buffer.loadFromFile(filePath))
            {
                sound.setBuffer(buffer);
                sound.setVolume(m_volume);
            }
            else
            {
                std::cout << "Failed to load sfx: \"" << filePath
                          << "\".  That sfx will not play during the sim." << std::endl;

                sound.resetBuffer();
            }
        }

    private:
        float m_volumeMin;
        float m_volumeMax;
        float m_volumeAdj;
        float m_volume;

        sf::SoundBuffer m_bufferWalk;
        sf::Sound m_soundWalk;

        sf::SoundBuffer m_bufferCoin1;
        sf::Sound m_soundCoin1;

        sf::SoundBuffer m_bufferCoin2;
        sf::Sound m_soundCoin2;
    };

} // namespace methhead

#endif // METH_HEAD_AUDIO_HPP_INCLUDED

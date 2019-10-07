#ifndef METHHEADS_SOUND_PLAYER_HPP_INCLUDED
#define METHHEADS_SOUND_PLAYER_HPP_INCLUDED
//
// sound-player.hpp
//
#include "random.hpp"
#include "utils.hpp"

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include <SFML/Audio.hpp>

namespace methhead
{

    class SoundPlayer
    {
    public:
        SoundPlayer(Random & random);
        ~SoundPlayer();

        void setup();

        void play(const std::string & name);

        void volumeUp();
        void volumeDown();
        inline float volume() const { return m_volume; }

        void muteButton();
        inline bool isMuted() const { return m_isMuted; }

    private:
        void volume(const float newVolume);

        void loadFiles();
        void loadFile(const std::filesystem::path & path);
        bool willLoad(const std::filesystem::directory_entry & entry);

        struct Sound
        {
            std::string toString() const;

            std::string name;
            sf::Sound sound;
            sf::SoundBuffer buffer;
        };

    private:
        bool m_isMuted;
        float m_volume;
        float m_volumeMin;
        float m_volumeMax;
        float m_volumeInc;

        Random & m_random;
        std::string m_supportedFileExtensions;
        std::vector<std::unique_ptr<Sound>> m_sounds;
    };

} // namespace methhead

#endif // METHHEADS_SOUND_PLAYER_HPP_INCLUDED

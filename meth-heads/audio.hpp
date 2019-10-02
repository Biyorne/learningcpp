#ifndef METHHEADS_AUDIO_HPP_INCLUDED
#define METHHEADS_AUDIO_HPP_INCLUDED
//
// audio.hpp
//
#include "random.hpp"
#include "utils.hpp"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <map>
#include <memory>
#include <thread>
#include <utility>

#include <SFML/Audio.hpp>

namespace methhead
{

    class Audio
    {
        struct Sfx
        {
            std::string name;
            sf::Sound sound;
            sf::SoundBuffer buffer;
        };

    public:
        Audio(const Mode mode, Random & random)
            : m_random(random)
            , m_sfxUPtrs()
            , m_isMuted(false)
            , m_volume(0.0f)
            , m_volumeMin(0.0f) // sfml's volume min
            , m_volumeMax(100.0f) // sfml's volume max
            , m_volumeInc(m_volumeMax / 10.0f) // only ten different vol levels possible
        {
            if (Mode::SpeedTest == mode)
            {
                muteButton();
                std::cout << "Audio disabled during speed tests." << std::endl;
                return;
            }

            loadFiles();

            if (m_sfxUPtrs.empty())
            {
                std::cerr << "Error:  The audio engine could not find any sound effects to load.  "
                             "Only wav, ogg, and flac files are supported.  (not mp3)"
                          << std::endl;

                muteButton();
                return;
            }

            volume(m_volumeMax * 0.25f); // start at quarter volume

            // pitchListeningTests();
        }

        void volumeUp()
        {
            if (m_isMuted)
            {
                m_isMuted = false;
            }

            volume(m_volumeMin + m_volumeInc);
        }

        void volumeDown()
        {
            if (m_isMuted)
            {
                return;
            }

            volume(m_volume - m_volumeInc);
        }

        void muteButton()
        {
            m_isMuted = !m_isMuted;

            if (m_isMuted)
            {
                volume(m_volumeMin);
            }
            else
            {
                volume(m_volume);
            }
        }

        void play(const std::string & name)
        {
            if (m_isMuted || m_sfxUPtrs.empty())
            {
                return;
            }

            std::vector<std::size_t> nameMatchingSfxIndexes;

            for (std::size_t i(0); i < m_sfxUPtrs.size(); ++i)
            {
                if (startsWith(m_sfxUPtrs.at(i)->name, name))
                {
                    nameMatchingSfxIndexes.push_back(i);
                }
            }

            if (nameMatchingSfxIndexes.empty())
            {
                return;
            }

            const std::size_t soundIndex(m_random.from(nameMatchingSfxIndexes));
            m_sfxUPtrs.at(soundIndex)->sound.play();
        }

    private:
        inline bool startsWith(const std::string & searchIn, const std::string & searchFor) const
        {
            return (searchIn.find(searchFor) == 0);
        }

        void volume(const float newVolume)
        {
            m_volume = std::clamp(newVolume, m_volumeMin, m_volumeMax);

            for (auto & sfxUPtr : m_sfxUPtrs)
            {
                sfxUPtr->sound.setVolume(m_volume);
            }
        }

        void loadFiles()
        {
            std::filesystem::recursive_directory_iterator dirIter(std::filesystem::current_path());

            for (const std::filesystem::directory_entry & entry : dirIter)
            {
                if (isSupportedFile(entry))
                {
                    loadFile(entry.path());
                }
            }
        }

        bool isSupportedFile(const std::filesystem::directory_entry & entry)
        {
            if (!entry.is_regular_file())
            {
                return false;
            }

            const std::string ext(entry.path().filename().extension().string());

            return ((".ogg" == ext) || (".flac" == ext) || (".wav" == ext));
        }

        void loadFile(const std::filesystem::path & path)
        {
            auto sfxUPtr(std::make_unique<Sfx>());

            if (!sfxUPtr->buffer.loadFromFile(path.string()))
            {
                std::cerr << "Error:  Found a supported audio file: \"" << path.string()
                          << "\", but an error occurred while loading it." << std::endl;

                return;
            }

            sfxUPtr->sound.setBuffer(sfxUPtr->buffer);
            sfxUPtr->name = path.filename().string();

            printSfx(*sfxUPtr);
            m_sfxUPtrs.push_back(std::move(sfxUPtr));
        }

        void printSfx(const Sfx & sfx)
        {
            std::cout << "Sound Effect Loaded:  ";

            std::cout << std::setw(22) << std::left;
            std::cout << sfx.name;
            std::cout << std::setw(0) << std::right;

            // duration in seconds
            const double durationMs(static_cast<double>(sfx.buffer.getDuration().asMilliseconds()));
            const double durationSec(durationMs / 1000.0);

            std::cout << std::setprecision(2) << std::setfill('0') << std::setw(3) << std::fixed;
            std::cout << durationSec;
            std::cout << std::setprecision(0) << std::setfill(' ') << std::setw(0)
                      << std::defaultfloat;

            std::cout << "s ";

            // channels
            const auto channelCount(sfx.buffer.getChannelCount());
            if (1 == channelCount)
            {
                std::cout << "mono  ";
            }
            else if (2 == channelCount)
            {
                std::cout << "stereo";
            }
            else
            {
                std::cout << channelCount << "ch";
            }

            // sample rate in kHz
            const double sampleRateHz(static_cast<double>(sfx.buffer.getSampleRate()));
            const double sampleRakeKHz(sampleRateHz / 1000.0);

            std::cout << " " << sampleRakeKHz << "kHz" << std::endl;
        }

        void pitchListeningTests()
        {
            const std::vector<std::string> namesToTest { "barf-1",        "death-2",
                                                         "bang-dynamite", "punch-1",
                                                         "gross-1",       "power-up-glitter",
                                                         "power-up-ring", "trip-balls-3" };

            for (auto & sfxUPtr : m_sfxUPtrs)
            {
                const std::string & nameCurrent(sfxUPtr->name);

                for (const std::string & nameToTest : namesToTest)
                {
                    if (startsWith(nameCurrent, nameToTest))
                    {
                        pitchListeningTest(*sfxUPtr);
                        break;
                    }
                }
            }
        }

        void pitchListeningTest(Sfx & sfx)
        {
            std::cout << "\n\nNext pitch listening test is of \"" << sfx.name << "\"";
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::cout << "   In...";
            std::this_thread::sleep_for(std::chrono::seconds(1));

            for (std::size_t i(0); i < 3; ++i)
            {
                std::cout << (3 - i) << "...";
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }

            std::cout << std::endl;

            float pitch(0.25f);
            while (pitch < 2.1f)
            {
                std::cout << "\tpitch=" << pitch << std::endl;

                sfx.sound.setPitch(pitch);
                sfx.sound.play();

                while (sfx.sound.getStatus() == sf::SoundSource::Status::Playing)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                }

                pitch += 0.25f;
            }

            sfx.sound.setPitch(1.0f);
        }

    private:
        Random & m_random;

        bool m_isMuted;

        float m_volume;
        float m_volumeMin;
        float m_volumeMax;
        float m_volumeInc;

        std::vector<std::unique_ptr<Sfx>> m_sfxUPtrs;
    };

} // namespace methhead

#endif // METHHEADS_AUDIO_HPP_INCLUDED

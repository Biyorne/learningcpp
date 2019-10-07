// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// sound-player.cpp
//
#include "sound-player.hpp"

#include <algorithm>
#include <iostream>
#include <memory>
#include <sstream>

namespace methhead
{

    std::string SoundPlayer::Sound::toString() const
    {
        std::ostringstream ss;

        ss << std::setw(22) << std::left;
        ss << name;
        // ss << std::setw(0) << std::right;

        // duration in seconds
        const double durationMs(static_cast<double>(buffer.getDuration().asMilliseconds()));
        const double durationSec(durationMs / 1000.0);

        ss << " " << std::setprecision(2) << std::setfill('0') << std::setw(3) << std::fixed;
        ss << durationSec;
        // ss << std::setprecision(0) << std::setfill(' ') << std::setw(0) << std::defaultfloat;

        ss << "s ";

        // channels
        const auto channelCount(buffer.getChannelCount());
        if (1 == channelCount)
        {
            ss << "mono  ";
        }
        else if (2 == channelCount)
        {
            ss << "stereo";
        }
        else
        {
            ss << channelCount << "ch";
        }

        // sample rate in kHz
        const double sampleRateHz(static_cast<double>(buffer.getSampleRate()));
        const double sampleRakeKHz(sampleRateHz / 1000.0);

        ss << " " << sampleRakeKHz << "kHz";

        return ss.str();
    }

    SoundPlayer::SoundPlayer(Random & random)
        : m_isMuted(false)
        , m_volume(0.0f)
        , m_volumeMin(0.0f) // this is what sfml uses
        , m_volumeMax(100.0f) // this is what sfml uses
        , m_volumeInc(m_volumeMax / 10.0f) // only ten different vol levels possible
        , m_random(random)
        , m_supportedFileExtensions(".ogg/.flac/.wav")
        , m_sounds()
    {}

    SoundPlayer::~SoundPlayer()
    {
        for (auto & sound : m_sounds)
        {
            sound->sound.stop();
        }
    }

    void SoundPlayer::setup()
    {
        try
        {
            loadFiles();
        }
        catch (const std::exception & ex)
        {
            std::cerr << "SoundPlayer Error while trying to find and load audio files: \""
                      << ex.what() << "\"" << std::endl;
        }

        if (m_sounds.empty())
        {
            std::cerr << "SoundPlayer Error:  No sound files were found.  Remember that "
                         "MP3s are not supported, only: "
                      << m_supportedFileExtensions << std::endl;

            muteButton();
            return;
        }

        volume(m_volumeMax * 0.25f); // start at quarter volume
    }

    void SoundPlayer::play(const std::string & name)
    {
        if (m_isMuted || m_sounds.empty() || (m_volume < 1.0f))
        {
            return;
        }

        std::vector<std::size_t> nameMatchingIndexes;

        for (std::size_t i(0); i < m_sounds.size(); ++i)
        {
            if (name.empty() || startsWith(m_sounds.at(i)->name, name))
            {
                nameMatchingIndexes.push_back(i);
            }
        }

        if (nameMatchingIndexes.empty())
        {
            std::cerr << "SoundPlayer Error:  .play(\"" << name
                      << "\") called, but none had that name." << std::endl;

            return;
        }

        const std::size_t soundIndex(m_random.from(nameMatchingIndexes));
        m_sounds.at(soundIndex)->sound.play();
    }

    void SoundPlayer::volumeUp()
    {
        if (m_isMuted)
        {
            m_isMuted = false;
        }

        volume(m_volumeMin + m_volumeInc);
    }

    void SoundPlayer::volumeDown()
    {
        if (m_isMuted)
        {
            return;
        }

        volume(m_volume - m_volumeInc);
    }

    void SoundPlayer::muteButton()
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

    void SoundPlayer::volume(const float newVolume)
    {
        m_volume = std::clamp(newVolume, m_volumeMin, m_volumeMax);

        for (auto & soundUPtr : m_sounds)
        {
            soundUPtr->sound.setVolume(m_volume);
        }
    }

    void SoundPlayer::loadFiles()
    {
        std::filesystem::recursive_directory_iterator dirIter(std::filesystem::current_path());

        for (const std::filesystem::directory_entry & entry : dirIter)
        {
            if (willLoad(entry))
            {
                loadFile(entry.path());
            }
        }
    }

    void SoundPlayer::loadFile(const std::filesystem::path & path)
    {
        auto soundUPtr(std::make_unique<Sound>());

        if (!soundUPtr->buffer.loadFromFile(path.string()))
        {
            std::cerr << "SoundPlayer Error:  Found a supported file: \"" << path.string()
                      << "\", but an error occurred while loading it." << std::endl;

            return;
        }

        soundUPtr->sound.setBuffer(soundUPtr->buffer);
        soundUPtr->name = path.filename().string();

        std::cout << "Loaded: " << soundUPtr->toString() << std::endl;

        m_sounds.push_back(std::move(soundUPtr));
    }

    bool SoundPlayer::willLoad(const std::filesystem::directory_entry & entry)
    {
        if (!entry.is_regular_file())
        {
            return false;
        }

        const std::string extension(entry.path().filename().extension().string());

        if (extension.empty())
        {
            return false;
        }

        return (m_supportedFileExtensions.find(extension) < m_supportedFileExtensions.size());
    }

} // namespace methhead

// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// sound-player.cpp
//
#include "sound-player.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>

namespace methhead
{

    SoundPlayer::SoundPlayer()
        : m_isMuted(false)
        , m_volume(0.0f)
        , m_volumeMin(0.0f) // this is what sfml uses
        , m_volumeMax(100.0f) // this is what sfml uses
        , m_volumeInc(m_volumeMax / 10.0f) // only ten different vol levels possible
        , m_fileExtensions(".ogg.flac.wav") // dots are required here
        , m_soundEffects()
    {
        loadFiles();

        // start all sounds at quarter volume
        volume(m_volumeMax * 0.25f);
    }

    void SoundPlayer::play(const std::string & name, const Random & random)
    {
        if (name.empty() || m_soundEffects.empty() || (m_volume < 1.0f))
        {
            return;
        }

        const std::vector<std::size_t> nameMatchingIndexes(findNameMatchingIndexes(name));
        if (nameMatchingIndexes.empty())
        {
            std::cerr << "SoundPlayer Error:  .play(\"" << name
                      << "\") called, but none had that name." << std::endl;

            return;
        }

        const std::size_t index(random.from(nameMatchingIndexes));
        auto & sfx(m_soundEffects.at(index));

        if (sfx->sound.getStatus() == sf::SoundSource::Playing)
        {
            return;
        }

        sfx->sound.play();
    }

    std::vector<std::size_t> SoundPlayer::findNameMatchingIndexes(const std::string & name) const
    {
        std::vector<std::size_t> indexes;

        for (std::size_t i(0); i < m_soundEffects.size(); ++i)
        {
            if (startsWith(m_soundEffects.at(i)->filename, name))
            {
                indexes.push_back(i);
            }
        }

        return indexes;
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

        for (auto & sfx : m_soundEffects)
        {
            sfx->sound.setVolume(m_volume);
        }
    }

    void SoundPlayer::loadFiles()
    {
        std::filesystem::recursive_directory_iterator dirIter(std::filesystem::current_path());

        for (const std::filesystem::directory_entry & entry : dirIter)
        {
            if (willLoad(entry))
            {
                loadFile(entry);
            }
        }

        if (m_soundEffects.empty())
        {
            std::cerr << "SoundPlayer Error:  No sound files were found.  Remember that "
                         "MP3s are not supported, only: "
                      << m_fileExtensions << std::endl;
        }
    }

    void SoundPlayer::loadFile(const std::filesystem::directory_entry & entry)
    {
        auto sfx(std::make_unique<SoundEffect>());

        if (!sfx->buffer.loadFromFile(entry.path().string()))
        {
            std::cerr << "SoundPlayer Error:  Found a supported file: \"" << entry.path().string()
                      << "\", but an error occurred while loading it." << std::endl;

            return;
        }

        sfx->sound.setBuffer(sfx->buffer);
        sfx->filename = entry.path().filename().string();

        std::cout << "Loaded: " << sfx->toString() << std::endl;
        m_soundEffects.push_back(std::move(sfx));
    }

    bool SoundPlayer::willLoad(const std::filesystem::directory_entry & entry) const
    {
        if (!entry.is_regular_file())
        {
            return false;
        }

        const std::string extension(entry.path().filename().extension().string());

        if ((extension.size() != 4) && (extension.size() != 5))
        {
            return false;
        }

        return (m_fileExtensions.find(extension) < m_fileExtensions.size());
    }

    std::string SoundPlayer::SoundEffect::toString() const
    {
        const std::string pad("  ");

        std::ostringstream ss;

        ss << std::setw(20) << std::right;
        ss << filename << pad;

        // duration in seconds
        const auto durationMs(buffer.getDuration().asMilliseconds());
        const double durationSec(static_cast<double>(durationMs) / 1000.0);

        ss << std::setprecision(2) << std::setw(3) << std::setfill('0') << std::fixed;
        ss << durationSec << "s" << pad;

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
        ss << pad;

        // sample rate in kHz
        const auto sampleRateHz(buffer.getSampleRate());
        const double sampleRakeKHz(static_cast<double>(sampleRateHz) / 1000.0);

        ss << std::setprecision(1) << std::setw(1) << std::setfill('0') << sampleRakeKHz << "kHz";

        return ss.str();
    }

} // namespace methhead

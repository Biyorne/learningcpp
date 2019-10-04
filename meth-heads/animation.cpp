// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "animation.hpp"

#include "utils.hpp"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <map>
#include <memory>
#include <thread>
#include <utility>

namespace methhead
{
    /*
    void Animation::loadFiles()
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

    bool Animation::willLoad(const std::filesystem::directory_entry & entry)
    {
        if (!entry.is_regular_file())
        {
            return false;
        }

        const std::string ext(entry.path().filename().extension().string());

        return ((".ogg" == ext) || (".flac" == ext) || (".wav" == ext));
    }

    void Animation::loadFile(const std::filesystem::path & path)
    {
        auto sfxUPtr(std::make_unique<Sfx>());

        if (!sfxUPtr->buffer.loadFromFile(path.string()))
        {
            std::cerr << "Error:  Found a supported soundPlayer file: \"" << path.string()
                      << "\", but an error occurred while loading it." << std::endl;

            return;
        }

        sfxUPtr->sound.setBuffer(sfxUPtr->buffer);
        sfxUPtr->name = path.filename().string();

        printSfx(*sfxUPtr);
        m_sfxUPtrs.push_back(std::move(sfxUPtr));
    }
    */
} // namespace methhead

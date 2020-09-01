// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//
// extension-fixer-2.cpp
//

#include "extension-fixer.hpp"

#include "ascii.hpp"

#include <array>
#include <map>

namespace fixer::extension
{
    void Extension::makeLowerCase(const std::filesystem::path & path)
    {
        namespace fs = std::filesystem;

        std::vector<fs::directory_entry> entries;

        for (const fs::directory_entry & entry : fs::recursive_directory_iterator(path))
        {
            if (!entry.is_regular_file())
            {
                continue;
            }

            entries.push_back(entry);
        }

        std::map<std::wstring, std::size_t> extSkipped;
        std::map<std::wstring, std::size_t> extChanged;

        for (const fs::directory_entry & entry : entries)
        {
            if (makeLowerCase(entry))
            {
                extChanged[entry.path().extension().wstring()]++;
            }
            else
            {
                extSkipped[entry.path().extension().wstring()]++;
            }
        }

        // TODO use structured bindings after fixing CMake
        Ascii::printOnlySupported(L"Changed extensions: ", true);
        for (const auto & extCountPair : extChanged)
        {
            Ascii::printOnlySupported(L"\t ");
            Ascii::printOnlySupported(extCountPair.first);
            Ascii::printOnlySupported(L"\t x");
            Ascii::printOnlySupported(std::to_wstring(extCountPair.second), true);
        }

        // Everything that DIDN'T get changed
        // Ascii::printOnlySupported(L"Unchanged extensions: ", true);
        // for (const auto & extCountPair : extSkipped)
        //{
        //    Ascii::printOnlySupported(L"\t ");
        //    Ascii::printOnlySupported(extCountPair.first);
        //    Ascii::printOnlySupported(L"\t x");
        //    Ascii::printOnlySupported(std::to_wstring(extCountPair.second), true);
        //}
    }

    bool Extension::makeLowerCase(const std::filesystem::directory_entry & entry)
    {
        using namespace util;
        namespace fs = std::filesystem;

        const std::array<std::wstring, 48> allowedExtensions = { //
                                                                 // Audio
                                                                 //
                                                                 L".aif",
                                                                 L".cue",
                                                                 L".flac",
                                                                 L".log",
                                                                 L".m3u",
                                                                 L".m3u8",
                                                                 L".m4p",
                                                                 L".m4r",
                                                                 L".mp4",
                                                                 L".spc",
                                                                 L".wma",
                                                                 L".wav",
                                                                 L".mp3",
                                                                 L".m4a",
                                                                 L".flac",
                                                                 L".wav",
                                                                 L".ogg",
                                                                 L".pcm",
                                                                 //
                                                                 // images
                                                                 //
                                                                 L".gif",
                                                                 L".png",
                                                                 L".bmp",
                                                                 L".tif",
                                                                 L".itc2",
                                                                 L".itdb",
                                                                 L".pic",
                                                                 L".tga",
                                                                 L".jpg",
                                                                 L".jpeg",
                                                                 L".svg",
                                                                 //
                                                                 // Art misc
                                                                 //
                                                                 L".psd",
                                                                 L".clip",
                                                                 L".sai",
                                                                 L".sai2",
                                                                 //
                                                                 // Misc Documents
                                                                 //
                                                                 L".db",
                                                                 L".exe",
                                                                 L".7z",
                                                                 L".ini",
                                                                 L".nfo",
                                                                 L".txt",
                                                                 L".url",
                                                                 L".pdf",
                                                                 L".rar",
                                                                 L".zip",
                                                                 L".htm",
                                                                 L".html",
                                                                 L".webp",
                                                                 L".doc"
        };
        // get a copy of the WHOLE path, even thought we will ONLY change the extension
        const fs::path pathOrig(entry.path());
        fs::path pathNew(pathOrig);

        fs::path extensionLower(Ascii::toLower(pathNew.extension().wstring()));

        fs::path jpegKiller(Ascii::toLower(pathNew.wstring()));
        const bool shouldBeKilled(jpegKiller.extension().wstring() == L".jpeg");

        const auto foundIter(
            std::find(std::begin(allowedExtensions), std::end(allowedExtensions), extensionLower));

        const bool isInReplacementList { foundIter != std::end(allowedExtensions) };

        if ((pathOrig.extension() == extensionLower) || !isInReplacementList)
        {
            if (!shouldBeKilled)
            {
                return false;
            }
        }

        // replace the extensions, but ONLY on the path NOT on the file (not yet...)
        if (shouldBeKilled)
        {
            pathNew.replace_extension(L".jpg");
        }
        else
        {
            pathNew.replace_extension(extensionLower);
        }

        fs::rename(pathOrig, pathNew);
        return true;
    }
} // namespace fixer::extension

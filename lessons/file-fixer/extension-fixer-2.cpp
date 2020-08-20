// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//
// extension-fixer-2.cpp
//

#include "extension-fixer-2.hpp"

#include "ascii.hpp"
#include "filesys.hpp"

#include <array>
#include <map>

namespace fixer::ext2
{
    void Extension::makeLowerCase(const std::filesystem::path & path)
    {
        FileSys fileSystemHelper;

        const SearchResult finderResult { fileSystemHelper.recursiveFinder(path) };
        if (finderResult.entries.empty())
        {
            Ascii::printOnlySupported(
                L"Finished:  Failed to find anything in the directory specified.", true);

            return;
        }

        std::map<std::wstring, std::size_t> extSkipped;
        std::map<std::wstring, std::size_t> extChanged;

        for (const std::filesystem::directory_entry & entry : finderResult.entries)
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

        Ascii::printOnlySupported(L"Unchanged extensions: ", true);
        for (const auto & extCountPair : extSkipped)
        {
            Ascii::printOnlySupported(L"\t ");
            Ascii::printOnlySupported(extCountPair.first);
            Ascii::printOnlySupported(L"\t x");
            Ascii::printOnlySupported(std::to_wstring(extCountPair.second), true);
        }
    }

    bool Extension::makeLowerCase(const std::filesystem::directory_entry & entry)
    {
        using namespace util;
        namespace fs = std::filesystem;

        const std::array<std::wstring, 8> allowedExtensions
            = { L".mp3", L".m4a", L".flac", L".wav", L".ogg", L".pcm", L".jpg", L".jpeg" };

        // get a copy of the WHOLE path, even thought we will ONLY change the extension
        const fs::path pathOrig(entry.path());
        fs::path pathNew(pathOrig);

        fs::path extensionLower(Ascii::toLower(pathNew.extension().wstring()));

        fs::path jpegKiller(Ascii::toLower(pathNew));
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
} // namespace fixer::ext2

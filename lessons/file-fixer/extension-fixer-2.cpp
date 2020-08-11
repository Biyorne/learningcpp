// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//
// extension-fixer-2.cpp
//

#include "extension-fixer-2.hpp"

#include "ascii.hpp"
#include "filesys.hpp"

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

        for (const std::filesystem::directory_entry & entry : finderResult.entries)
        {
            makeLowerCase(entry);
        }
    }

    void Extension::makeLowerCase(const std::filesystem::directory_entry &)
    {
        // TODO
    }
} // namespace fixer::ext2

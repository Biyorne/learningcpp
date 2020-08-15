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

    void Extension::makeLowerCase(const std::filesystem::directory_entry & entry)
    {
        using namespace util;
        namespace fs = std::filesystem;

        // print we are making a change
        Ascii::printOnlySupported(L"Making extension lower case: \"");

        // get a copy of the WHOLE path, even thought we will ONLY change the extension
        fs::path pathNew(entry.path());
        const fs::path pathOrig(pathNew);

        // print which file we are going to change, and add the arrow to make it easy on hte eyes
        Ascii::printOnlySupported(pathNew.filename().wstring());
        Ascii::printOnlySupported(L"\"\t --> \"");

        // replace the extensions, but ONLY on the path NOT on the file (not yet...)
        pathNew.replace_extension(Ascii::toLowerStr(pathNew.extension().wstring()));

        // print the new path (filename only) that should now be lower case
        Ascii::printOnlySupported(pathNew.wstring());
        Ascii::printOnlySupported(L"\"", true);

        fs::rename(pathOrig, pathNew);
        // std::filesystem::rename(
        // for(wchar_t & ch : entry.path().extension().wstring())
        //{
        //
        //}

        //
    }
} // namespace fixer::ext2

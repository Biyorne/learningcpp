// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//
// extension-fixer-1.cpp
//
#include "extension-fixer-1.hpp"

#include "ascii.hpp"
#include "filesys.hpp"

namespace fixer::ext1
{

    void Extension::makeLowerCase(const std::filesystem::path & path)
    {
        namespace fs = std::filesystem;

        FileSys filesysemHelper;
        SearchResult searchResult;

        // TODO actually change the ext to be lower-case,
        // but for now just print each ext to the console as is
        for (const fs::directory_entry & entry :
             fs::recursive_directory_iterator(path, fs::directory_options::skip_permission_denied))
        {
            if (!filesysemHelper.isTypical(
                    TypicalEntry::File, entry, searchResult, WillPrint::Errors))
            {
                Ascii::printOnlySupported(Ascii::fileSizeHumanReadable(entry));
                Ascii::printOnlySupported(L"\t");
                Ascii::printOnlySupported(entry.path().filename().wstring(), true);
            }
        }

        Ascii::printOnlySupported(searchResult.entityCountReportStr(), true);
    }

} // namespace fixer::ext1

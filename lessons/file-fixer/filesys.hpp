#ifndef FILESYS_HPP_INCLUDED
#define FILESYS_HPP_INCLUDED
//
// filesys.hpp
//
#include "util.hpp"

#include <filesystem>
#include <string>
#include <vector>

//

namespace fixer
{

    enum class TypicalEntry
    {
        File,
        Diretory
    };

    //

    struct SearchResult
    {
        std::wstring entityCountReportStr() const;

        void countEntryType(const std::filesystem::directory_entry & entry);
        bool countIfAccessErrorMessage(const std::wstring & message);
        bool countIfAccessErrorException(const std::filesystem::filesystem_error & exception);

        std::vector<std::filesystem::directory_entry> entries;

        std::size_t fileEntryCount { 0 };
        std::size_t drectoryEntryCount { 0 };
        std::size_t otherEntryCount { 0 };
        std::size_t accessErrorCount { 0 };
        std::size_t invalidCharCount { 0 };
    };

    //

    enum class WillPrint
    {
        Nothing,
        Errors
    };

    //

    struct FileSys
    {
        SearchResult recursiveFinder(std::filesystem::path path);

        bool isAccessError(const std::wstring & message);

        bool isTypical(
            const TypicalEntry type,
            const std::filesystem::directory_entry & entry,
            SearchResult & result,
            const WillPrint willPrint = WillPrint::Errors);

        bool isTypical(
            const TypicalEntry type,
            const std::filesystem::path & path,
            SearchResult & result,
            const WillPrint willPrint = WillPrint::Errors);

        std::wstring toString(const std::error_code & errorCode);
        std::wstring toString(const std::filesystem::file_type fileType);
    };

} // namespace fixer

#endif // FILESYS_HPP_INCLUDED

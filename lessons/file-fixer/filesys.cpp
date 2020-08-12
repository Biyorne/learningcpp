// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//
// filesys.cpp
//
#include "filesys.hpp"

#include "ascii.hpp"
#include "util.hpp"

#include <cmath>
#include <sstream>

namespace fixer
{

    //
    // SearchResult Functions
    //

    std::wstring SearchResult::entityCountReportStr() const
    {
        using namespace fixer::util;

        const std::size_t totalEntries { fileEntryCount + drectoryEntryCount + otherEntryCount };
        if (totalEntries == 0)
        {
            return {};
        }

        std::wstringstream wss;

        // clang-format off
        wss << L"Found a total of " << totalEntries << " entries:";
        wss << L"\n   Files         = " << fileEntryCount     << ratioToPercentStr(fileEntryCount, totalEntries);
        wss << L"\n   Directories   = " << drectoryEntryCount << ratioToPercentStr(drectoryEntryCount, totalEntries);
        wss << L"\n   Others        = " << otherEntryCount    << ratioToPercentStr(otherEntryCount, totalEntries);
        wss << L"\n   Invalid Chars = " << invalidCharCount;
        wss << L"\n   Access Errors = " << accessErrorCount   << ratioToPercentStr(accessErrorCount, totalEntries);
        // clang-format on

        return wss.str();
    }

    bool SearchResult::countIfAccessErrorMessage(const std::wstring & message)
    {
        FileSys filesystemHelper;

        if (filesystemHelper.isAccessError(message))
        {
            ++accessErrorCount;
            return true;
        }

        return false;
    }

    bool SearchResult::countIfAccessErrorException(const std::filesystem::filesystem_error & fsEx)
    {
        return countIfAccessErrorMessage(Ascii::toWide(fsEx.what()));
    }

    //
    // FileSys Functions
    //

    SearchResult FileSys::recursiveFinder(std::filesystem::path path)
    {
        using namespace util;
        namespace fs = std::filesystem;

        SearchResult result;

        try
        {
            if (!fs::exists(path))
            {
                Ascii::printOnlySupported(L"FileSys::recursiveFinder(\"");
                Ascii::printOnlySupported(path.wstring());

                Ascii::printOnlySupported(
                    L"\") failed beause that directory does not exist.", true);

                return {};
            }

            path = fs::canonical(path);

            Ascii::printOnlySupported(
                std::wstring(L"Starting Directory Search:  \"")
                    .append(path.wstring())
                    .append(L"\""),
                true);

            if (!isTypical(TypicalEntry::Diretory, path, result, WillPrint::Errors))
            {
                Ascii::printOnlySupported(
                    L"Path given is not a directory, or not an accessible directory.", true);

                return {};
            }
        }
        catch (const fs::filesystem_error & fsEx)
        {
            if (!result.countIfAccessErrorException(fsEx))
            {
                Ascii::printOnlySupported(path.wstring());
                Ascii::printOnlySupported(L"\t Caught Filesystem Exception in recursiveFinder:");
                Ascii::printOnlySupported(L"\t what=\"");
                Ascii::printOnlySupported(Ascii::toWide(fsEx.what()), true);
            }
        }
        catch (const std::exception & stdEx)
        {
            Ascii::printOnlySupported(path.wstring());
            Ascii::printOnlySupported(
                L"\t Caught Stdanard Exception in recursiveFinder():  what=\"", false);
            Ascii::printOnlySupported(Ascii::toWide(stdEx.what()));
            Ascii::printOnlySupported(L"\".Will continue.", true);
        }
        catch (...)
        {
            Ascii::printOnlySupported(path.wstring());
            Ascii::printOnlySupported(
                L"Caught Unknown Exception in recursiveFinder():  what=\"unknown "
                L"exception\".  Will continue.",
                true);
        }

        // Can't use a ranged for loop because the ++iter can throw,
        // yes, even with the fs::directory_options::skip_permission_denied.  Joy.
        fs::recursive_directory_iterator iter(path, fs::directory_options::skip_permission_denied);
        const fs::recursive_directory_iterator iterEnd;
        while (iter != iterEnd)
        {
            try
            {
                result.invalidCharCount += Ascii::countUnsupported(iter->path().wstring());

                const fs::directory_entry & entry { *iter };

                if (isTypical(TypicalEntry::File, entry, result, fixer::WillPrint::Errors))
                {
                    result.entries.push_back(entry);
                }
            }
            catch (const fs::filesystem_error & fsEx)
            {
                if (!result.countIfAccessErrorException(fsEx))
                {
                    Ascii::printOnlySupported(iter->path().wstring());
                    Ascii::printOnlySupported(
                        L"\t Caught Filesystem Exception in recursiveFinder:");
                    Ascii::printOnlySupported(L"\t what=\"");
                    Ascii::printOnlySupported(Ascii::toWide(fsEx.what()), true);
                }
            }
            catch (const std::exception & stdEx)
            {
                Ascii::printOnlySupported(iter->path().wstring());
                Ascii::printOnlySupported(
                    L"\t Caught Stdanard Exception in recursiveFinder():  what=\"", false);
                Ascii::printOnlySupported(Ascii::toWide(stdEx.what()));
                Ascii::printOnlySupported(L"\".Will continue.", true);
            }
            catch (...)
            {
                Ascii::printOnlySupported(iter->path().wstring());
                Ascii::printOnlySupported(
                    L"\t Caught Unknown Exception in recursiveFinder():  what=\"unknown "
                    L"exception\".  Will continue.",
                    true);
            }

            try
            {
                ++iter;
            }
            catch (const fs::filesystem_error & fsEx)
            {
                if (!result.countIfAccessErrorException(fsEx))
                {
                    Ascii::printOnlySupported(iter->path().wstring());
                    Ascii::printOnlySupported(
                        L"\t Caught Filesystem Exception in recursiveFinder ++ITER:");
                    Ascii::printOnlySupported(L"\t what=\"");
                    Ascii::printOnlySupported(Ascii::toWide(fsEx.what()), true);
                }
            }
            catch (const std::exception & stdEx)
            {
                Ascii::printOnlySupported(iter->path().wstring());
                Ascii::printOnlySupported(
                    L"\t Caught Stdanard Exception in recursiveFinder() ++ITER:  what=\"", false);
                Ascii::printOnlySupported(Ascii::toWide(stdEx.what()));
                Ascii::printOnlySupported(L"\".Will continue.", true);
            }
            catch (...)
            {
                Ascii::printOnlySupported(iter->path().wstring());
                Ascii::printOnlySupported(
                    L"\t Caught Unknown Exception in recursiveFinder() ++ITER:  what=\"unknown "
                    L"exception\".  Will continue.",
                    true);
            }
        }

        // This app counts the directory it is told to search as one of the directories it found,
        // but that's unintuitive and makes the directory count not match what the OS will say.
        if (result.drectoryEntryCount > 0)
        {
            --result.drectoryEntryCount;
        }

        Ascii::printOnlySupported(L"Finished Entry Search.  ");
        Ascii::printOnlySupported(result.entityCountReportStr(), true);

        return result;
    }

    void SearchResult::countEntryType(const std::filesystem::directory_entry & entry)
    {
        namespace fs = std::filesystem;

        if (fs::is_directory(entry))
        {
            ++drectoryEntryCount;
        }
        else if (fs::is_regular_file(entry))
        {
            ++fileEntryCount;
        }
        else
        {
            ++otherEntryCount;
        }
    }

    bool FileSys::isTypical(
        const TypicalEntry type,
        const std::filesystem::directory_entry & entry,
        SearchResult & result,
        const WillPrint willPrint)
    {
        using namespace util;
        namespace fs = std::filesystem;

        std::vector<std::wstring> errors;

        const fs::path path { entry.path() };

        // check for all kinds of status and type errors
        try
        {
            const std::size_t unsupportedCharCount { Ascii::countUnsupported(path.wstring()) };
            // if (unsupportedCharCount > 0)
            //{
            //    errors.push_back(std::to_wstring(unsupportedCharCount));
            //    errors.back() += L" chars_not_supported";
            //}

            // NOTE:
            //  I know the names look backwards but they are not.
            //  "symlink_status()" gives you info about the file that is also symlink
            //  "status()" follows the symlink, so be careful not to use it here.
            fs::file_status status { entry.symlink_status() };

            if (!fs::exists(entry))
            {
                errors.push_back(L"does_not_exist?!");
            }
            else
            {
                // keep counters up to date
                result.countEntryType(entry);

                // check that it is what it should be
                if (type == TypicalEntry::File)
                {
                    if (!fs::is_regular_file(entry))
                    {
                        // errors.push_back(L"should_be_regular_file");
                        return false;
                    }
                }
                else
                {
                    if (!fs::is_directory(entry))
                    {
                        // errors.push_back(L"should_be_directory");
                        return false;
                    }
                }

                // clang-format off
                if (fs::is_block_file(entry)    ) { errors.push_back(L"block_file"); }    
                if (fs::is_character_file(entry)) { errors.push_back(L"character_file"); }
                if (fs::is_fifo(entry)          ) { errors.push_back(L"fifo"); }          
                if (fs::is_socket(entry)        ) { errors.push_back(L"socket"); }        
                if (fs::is_other(entry)         ) { errors.push_back(L"other_type"); }
                // clang-format on

                // On Windows you can safely say there are no symlinks, or, if on the rarest of
                // occasions, they are not anything you can work with normally.  On Linux/MacOS you
                // can count on finding all kinds of symlinks.  This treats them only as flat files.
                if (fs::is_symlink(entry) && Platform::is_windows)
                {
                    errors.push_back(L"symlink");
                }
            }
        }
        catch (const fs::filesystem_error & fsEx)
        {
            if (result.countIfAccessErrorException(fsEx))
            {
                errors.push_back(L"status_access_error");
            }
            else
            {
                if (willPrint == WillPrint::Errors)
                {
                    Ascii::printOnlySupported(path.wstring());

                    Ascii::printOnlySupported(L"\t Caught Filesystem Exception in isTypical(\"");

                    Ascii::printOnlySupported(
                        ((type == TypicalEntry::File) ? L"File" : L"Directory"));

                    Ascii::printOnlySupported(L"\")\t what=\"");
                    Ascii::printOnlySupported(Ascii::toWide(fsEx.what()), true);
                }
            }

            throw;
        }
        catch (...)
        {
            errors.push_back(L"status_error");
        }

        try
        {
            // if directory, make sure it can be entered/traversed
            // Yes, even with the directory_options::skip_permission_denied,
            // the filesystem has all kinds of directories that will still
            // throw access/permissions exceptions.  Aren't filesystems fun?
            if (entry.is_directory() && (type == TypicalEntry::Diretory))
            {
                fs::directory_iterator iter(path, fs::directory_options::skip_permission_denied);

                const fs::directory_iterator iterEnd;

                std::size_t count { 0 };
                while ((count <= 3) && (iter != iterEnd))
                {
                    ++count;
                    ++iter;
                }
            }
        }
        catch (const fs::filesystem_error & fsEx)
        {
            if (result.countIfAccessErrorException(fsEx))
            {
                errors.push_back(L"status_access_error");
            }
            else
            {
                if (willPrint == WillPrint::Errors)
                {
                    Ascii::printOnlySupported(path.wstring());

                    Ascii::printOnlySupported(
                        L"\t *** Caught Filesystem Exception in isTypical(\"");

                    Ascii::printOnlySupported(
                        ((type == TypicalEntry::File) ? L"File" : L"Directory"));
                    Ascii::printOnlySupported(L"\")\t what=\"");
                    Ascii::printOnlySupported(Ascii::toWide(fsEx.what()), true);
                }
            }

            throw;
        }
        catch (...)
        {
            errors.push_back(L"dir_access_error");
        }

        if (!errors.empty())
        {
            if (willPrint == WillPrint::Errors)
            {
                Ascii::printOnlySupported(path.wstring());
                Ascii::printOnlySupported(L"     (x");
                Ascii::printOnlySupported(std::to_wstring(errors.size()));
                Ascii::printOnlySupported(L" Errors)(");

                // combine all errors into one string wrapped in parens
                std::wstring str;
                for (const std::wstring & error : errors)
                {
                    if (!str.empty())
                    {
                        str += L",";
                    }

                    str += error;
                }

                Ascii::printOnlySupported(str);
                Ascii::printOnlySupported(L")", true);
            }

            return false;
        }

        return true;
    }

    bool FileSys::isTypical(
        const TypicalEntry type,
        const std::filesystem::path & path,
        SearchResult & result,
        const WillPrint willPrint)
    {
        return isTypical(type, std::filesystem::directory_entry(path), result, willPrint);
    }

    std::wstring FileSys::toString(const std::error_code & errorCode)
    {
        std::string str;
        str += "error_code=";
        str += std::to_string(errorCode.value());
        str += '=';
        str += errorCode.category().name();
        str += "=\"";
        str += errorCode.message();
        str += '\"';
        return Ascii::toWide(str);
    }

    bool FileSys::isAccessError(const std::wstring & message)
    {
        if (message.empty())
        {
            return false;
        }

        if (message.find(L"denied") < message.size())
        {
            return true;
        }

        if (message.find(L"permitted") < message.size())
        {
            return true;
        }

        if (message.find(L"The file cannot be accessed by the system.") < message.size())
        {
            return true;
        }

        if (message.find(L"cannot access the file because it is being used by another process")
            < message.size())
        {
            return true;
        }

        return false;
    }

    std::wstring FileSys::toString(const std::filesystem::file_type fileType)
    {
        namespace fs = std::filesystem;

        // clang-format off
        if (fileType == fs::file_type::none)      { return L"none";      }
        if (fileType == fs::file_type::not_found) { return L"not_found"; }
        if (fileType == fs::file_type::regular)   { return L"regular";   }
        if (fileType == fs::file_type::directory) { return L"directory"; }
        if (fileType == fs::file_type::symlink)   { return L"symlink";   }
        if (fileType == fs::file_type::unknown)   { return L"unknown";   }
        // clang-format on        

        return L"other";
    }

}


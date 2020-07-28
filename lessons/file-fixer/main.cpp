// ----------------------------------------------------------------------------
// "THE BEER-WARE LICENSE" (Revision 42):
// Nel Carlson wrote this file.  As long as you retain this notice you
// can do whatever you want with this stuff. If we meet some day, and you think
// this stuff is worth it, you can buy me a beer in return.  <biyorne@gmail.com>
// ----------------------------------------------------------------------------
//
// main.cpp
//

//
// The File Fixer App
//  Features:
//      Makes all file extensions in a directory (recursive) lower case.
//      Print out the path of all files fixed.
//

#include "util-ascii.hpp"

#include <cassert>
#include <cctype>
#include <cstdlib>
#include <filesystem>
#include <iostream>

inline void recursiveFinder(const std::filesystem::path & path) { recursiveFinder_Impl(path); }

inline void recursiveFinder_Impl(const std::filesystem::path & path)
{
    namespace fs = std::filesystem;

    const std::wstring showPathsForExtension { L".itc2" };

    if (!fs::is_directory(path))
    {
        std::cerr << "Path given was not a directory: " << path << std::endl;
        return;
    }

    fs::recursive_directory_iterator dirIters(path, fs::directory_options::skip_permission_denied);

    for (const fs::directory_entry & entry : dirIters)
    {
        try
        {
            if (!entry.is_regular_file())
            {
                continue;
            }

            if (!showPathsForExtension.empty() && std::isdigit(showPathsForExtension.back()))
            {
                std::wcout << entry.path().wstring() << L"\n" << std::flush;
            }

            std::wcout << entry.path().extension().wstring() << '\n';
        }
        catch (const std::exception & ex)
        {
            std::wcerr << "Exception Error: " << ex.what() << ".  Will continue." << std::endl;
            std::wcout.clear();
            std::wcout.flush();
        }
        catch (...)
        {
            std::wcerr << "Unknown Exception Error.  Will continue." << std::endl;
            std::wcout.clear();
            std::wcout.flush();
        }
    }
}

int main()
{
    recursiveFinder("N:/My Stuff/Music");
    return EXIT_SUCCESS;
}

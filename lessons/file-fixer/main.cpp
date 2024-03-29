// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//
// main.cpp
//
#include "ascii.hpp"
#include "extension-fixer.hpp"

#include <iostream>

//
// The File Fixer App
//  Features:
//      Make Filename Extensions Lower Case:  (in some directory) (recursive)
//
//  Common Features (or goals if still TODO):
//      Not stopped by strange filesystem.
//      Print the path of all files fixed.
//      Print paths with unicode, unprintable chars, or just unsupported ascii.
//      Strange chars are shown as '?' and do not crash the app.
//

int main(int argc, char * argv[])
{
    const std::vector<std::string> args(argv, (argv + argc));

    if (argc != 2)
    {
        std::cout << "Requires path" << std::endl;
        return EXIT_SUCCESS;
    }

    //  N:/My Stuff/Music
    //  C:/Users/z
    //  C:/Users/z/Desktop/test/1
    //  Z:/
    //  Z:/temp/test/1/../2"
    //  Z:/Music/Music
    //  Z:/src/learningcpp/media/sfx
    //  ../../Music/Music
    //  ..\\../build\\file-fixer"
    //  /Volumes/Macintosh HD/System/Volumes/Data/private/
    //

    using namespace fixer;
    using namespace fixer::util;
    using namespace fixer::extension;
    namespace fs = std::filesystem;

    try
    {
        Extension extFixer;
        // DO NOT USE ON ROOT DIRECTORIES
        extFixer.makeLowerCase(Ascii::toWide(argv[1]));
    }
    catch (const fs::filesystem_error & fsEx)
    {
        Ascii::printOnlySupported(L"\nCaught Filesystem Exception Error in main():  what=\"");
        Ascii::printOnlySupported(Ascii::toWide(fsEx.what()));
        Ascii::printOnlySupported(L"\".  Bail.", true);
    }
    catch (const std::exception & stdEx)
    {
        Ascii::printOnlySupported(L"\nCaught Standard Exception Error in main():  what=\"");
        Ascii::printOnlySupported(Ascii::toWide(stdEx.what()));
        Ascii::printOnlySupported(L"\".  Bail.", true);
    }
    catch (...)
    {
        Ascii::printOnlySupported(
            L"\nCaught Unknown Exception Error in main():  what=\"unknown "
            L"exception\".  Bail.",
            true);
    }

    return EXIT_SUCCESS;
}

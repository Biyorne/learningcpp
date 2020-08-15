// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//
// main.cpp
//
#include "ascii.hpp"
#include "extension-fixer-1.hpp"
#include "extension-fixer-2.hpp"

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

int main()
{

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
    namespace fs = std::filesystem;

    try
    {
        ext2::Extension extFixer;
        extFixer.makeLowerCase(L"C:/temp");
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

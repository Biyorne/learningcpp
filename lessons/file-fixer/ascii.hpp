#ifndef ASCII_HPP_INCLUDED
#define ASCII_HPP_INCLUDED
//
// ascii.hpp
//
#include "util.hpp"

#include <iosfwd>
#include <string>

namespace fixer
{

    class Ascii
    {
    public:
        Ascii() = delete;

        static bool isSupported(const char ch);
        static std::size_t countUnsupported(const std::wstring & wideStr);

        // returns how many chars were NOT unsupported
        static std::size_t
            printOnlySupported(const std::wstring & wideStr, const bool appendNewline = false);

        static char toNarrow(const wchar_t wideChar);
        static std::string toNarrow(const std::wstring & wide);

        static wchar_t toWide(const char narrowChar);
        static std::wstring toWide(const std::string & narrowStr);

        static std::wstring fileSizeHumanReadable(const std::filesystem::directory_entry & entry);

        // old test functions not used anymore
        // static bool test();
        // static void printAll();

    private:
        // returns how many chars were NOT unsupported
        static std::size_t streamOnlySupported(
            std::wostream & wos, const std::wstring & wideStr, const bool appendNewline = false);

    private:
        static inline const char replace_char_narrow { '?' };
        static inline const wchar_t replace_char_wide { L'?' };
    };
} // namespace fixer

#endif // ASCII_HPP_INCLUDED

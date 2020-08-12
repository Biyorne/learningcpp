// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//
// ascii.cpp
//

#include "ascii.hpp"

#include <codecvt>
#include <cstdlib>
#include <iostream>
#include <sstream>

namespace fixer
{
    bool Ascii::isSupported(const char ch)
    {
        if ((ch == '\t') || (ch == '\r') || (ch == '\n'))
        {
            return true;
        }

        return ((ch >= ' ') && (ch <= 126)); // 127 is DEL
    }

    std::wstring Ascii::toWide(const std::string & narrowStr)
    {
        if (narrowStr.empty())
        {
            return L"";
        }

        static std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
        return converter.from_bytes(narrowStr.data(), (narrowStr.data() + narrowStr.size()));
    }

    wchar_t Ascii::toWide(const char narrowChar)
    {
        const std::string narrowStr(1, narrowChar);
        const std::wstring wideStr { toWide(narrowStr) };

        if (wideStr.empty())
        {
            return replace_char_wide;
        }

        return wideStr.front();
    }

    std::string Ascii::toNarrow(const std::wstring & wide)
    {
        if (wide.empty())
        {
            return "";
        }

        static std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
        return converter.to_bytes(wide.data(), (wide.data() + wide.size()));
    }

    char Ascii::toNarrow(const wchar_t wideChar)
    {
        const std::wstring wideStr(1, wideChar);
        const std::string result { toNarrow(wideStr) };

        if (result.empty())
        {
            return replace_char_narrow;
        }

        return result.front();
    }

    std::size_t Ascii::streamOnlySupported(
        std::wostream & wos, const std::wstring & wideStr, const bool appendNewline)
    {
        // this will force all conversion errors to not only set badbit but also throw
        wos.exceptions(std::ios::badbit);

        std::size_t invalidCharCount { 0 };

        for (std::size_t i(0); i < wideStr.size(); ++i)
        {
            const wchar_t wideChar { wideStr[i] };

            const char narrowChar { Ascii::toNarrow(wideChar) };
            if (!Ascii::isSupported(narrowChar))
            {
                ++invalidCharCount;
                wos << replace_char_wide;
                continue;
            }

            try
            {
                wos << wideChar;
            }
            catch (...)
            {
                wos.clear();
                wos.flush();
                wos << replace_char_wide;
                ++invalidCharCount;
                continue;
            }
        }

        if (appendNewline)
        {
            wos << L'\n';
        }

        return invalidCharCount;
    }

    std::size_t Ascii::printOnlySupported(const std::wstring & wideStr, const bool appendNewline)
    {
        return streamOnlySupported(std::wcout, wideStr, appendNewline);
    }

    std::size_t Ascii::countUnsupported(const std::wstring & wideStr)
    {
        std::wostringstream wss;
        return streamOnlySupported(wss, wideStr, false);
    }

    std::wstring Ascii::fileSizeHumanReadable(const std::filesystem::directory_entry & entry)
    {
        if (!entry.is_regular_file())
        {
            if (entry.is_directory())
            {
                return L"dir";
            }

            return L"";
        }

        const std::size_t size { entry.file_size() };

        std::wostringstream wss;
        wss.imbue(std::locale("")); // this puts commas in big numbers...somehow?

        // clang-format off
        if (size >= 1'000'000'000) { wss << (size / 1'000'000'000) << "G"; }
        else if (size >= 1'000'000) { wss << (size / 1'000'000) << "M"; }
        else if (size >= 1'000) { wss << (size / 1'000) << "K"; }
        else { wss << size << "B"; }
        // clang-format on

        return wss.str();
    }

    bool Ascii::isUpper(const wchar_t & ch) { return ((ch >= L'A') && (ch <= L'Z')); }

    bool Ascii::isLower(const wchar_t & ch) { return ((ch >= L'a') && (ch <= L'z')); }

    wchar_t Ascii::toLower(wchar_t ch)
    {
        if (isUpper(ch))
        {
            return (ch + 32);
        }
        else
        {
            return ch;
        }
    }

    wchar_t Ascii::toUpper(wchar_t ch)
    {
        if (isLower(ch))
        {
            return (ch - 32);
        }
        else
        {
            return ch;
        }
    }

    std::wstring Ascii::toLower(std::wstring str)
    {
        for (wchar_t & ch : str)
        {
            ch = toLower(ch);
        }

        return str;
    }

    std::wstring Ascii::toUpper(std::wstring str)
    {
        for (wchar_t & ch : str)
        {
            ch = toUpper(ch);
        }

        return str;
    }

    bool Ascii::testChars()
    {
        std::size_t count(0);

        for (char ch(std::numeric_limits<char>::lowest());; ++ch)
        {
            if (isSupported(ch))
            {
                ++count;
            }

            if (std::numeric_limits<char>::max() == ch)
            {
                break;
            }
        }

        // This magic numnber was added up by hand  -zTn 2020-6-1
        const std::size_t supportedCharCount { 98 };
        const bool success(count == supportedCharCount);

        if (success)
        {
            std::wcout << L"Char test PASS" << L'\n';
        }
        else
        {
            std::wcout << L"Error:  "
                       << L"Some chars that were supposed to be supported no longer are, or the "
                          L"other way around."
                       << std::endl;
        }

        return success;
    }

    void Ascii::printChars()
    {
        for (char ch(std::numeric_limits<char>::lowest());; ++ch)
        {
            char charToUse { ch };
            if (!isSupported(charToUse))
            {
                charToUse = replace_char_narrow;
            }

            std::wcout << static_cast<int>(ch) << L" \'" << charToUse << L"\', is_supported= "
                       << std::boolalpha << isSupported(ch);

            if (isLower(ch))
            {
                std::wcout << ", upper='" << toUpper(ch) << "'";
            }
            else if (isUpper(ch))
            {
                std::wcout << ", lower='" << toLower(ch) << "'";
            }

            std::wcout << L'\n';

            if (std::numeric_limits<char>::max() == ch)
            {
                break;
            }
        }
    }

    bool Ascii::testStrings()
    {
        // const std::wstring testSetUpper{ "ABCDEFGHIJKLMNOPQRSTUVWXYZ" };
        // const std::wstring testSetLower{ "abcdefghijklmnopqrstuvwxyz" };
        //
        // std::wcout << L"   " << testSetUpper << L'\n';
        // for (std::size_t i(0); i < testSetUpper.size(); ++i)
        //{
        //    std::wcout <<
        //}
        //
        //
        // std::wcout << L"   " << testSetLower << L'\n';
        return false;
    }
} // namespace fixer

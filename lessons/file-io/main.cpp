#include <cassert>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <regex>

//
class Ascii
{
public:
    static char replaceWith() { return m_replacementChar; }

    // defaults to empty space if not set, or if set to !isSupported()
    static void replaceWith(const char ch)
    {
        if (isSupported(ch))
        {
            m_replacementChar = ch;
        }
        else
        {
            m_replacementChar = ' ';
        }
    }

    static char replaceIfNeeded(const char ch)
    {
        if (isSupported(ch))
        {
            return ch;
        }
        else
        {
            return m_replacementChar;
        }
    }

    static bool test()
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

        const bool success(m_supportedCount == count);

        if (!success)
        {
            std::cout << "Error:  "
                      << "Some chars that were supposed to be supported no longer are, or the "
                         "other way around."
                      << std::endl;
        }

        return success;
    }

    static void printAll()
    {
        for (char ch(std::numeric_limits<char>::lowest());; ++ch)
        {
            std::cout << static_cast<int>(ch) << " \'" << replaceIfNeeded(ch) << "\' "
                      << std::boolalpha << isSupported(ch) << '\n';

            if (std::numeric_limits<char>::max() == ch)
            {
                break;
            }
        }
    }

    [[nodiscard]] static bool isSupported(const char ch)
    {
        if ((ch == '\t') || (ch == '\r') || (ch == '\n'))
        {
            return true;
        }

        return ((ch >= ' ') && (ch <= '\176')); // 176oct is del
    }

private:
    static inline std::size_t m_supportedCount { 98 }; // found with printAll() -zTn 2020-6
    static inline char m_replacementChar { ' ' };
};

//
int main()
{

    Ascii::printAll();
    Ascii::test();

    std::vector<char> fileBytes;

    // NOTE:
    //  We don't specify "stdf::ios_base::in", above because the type is 'ifstream'.
    //  Here are the open modes.  The only two you are ever likely to need have been starred:
    //
    //   std::ios::in      allows reading, but don't bother if type is: ifstream, or fstream
    //   std::ios::out     allows writing, but don't bother if type is: ofstream, or fstream
    // * std::ios::ate     seek to the end of stream immediately after open (short for "at-end")
    //   std::ios::app     seek to the end of stream before each write
    // * std::ios::trunc   deletes the contents of the whole file
    // * std::ios::binary  use fstream.read/write() instead of streams operators >> and <<
    //
    //  File streams are always text mode by default, which is typically
    //  words/numbers/punctuation/etc separated (delimited) by whitespace...but
    //  what is whitespace is determined by the locale. <cue thunder and creepy pipe organ>
    //
    {
        std::ifstream fileStream(
            "C:/src/learningcpp/media/text/monsters-100/monsters-100.tsv", std::ios_base::binary);

        // Note: These operations are wise EVERY time you open a file
        fileStream.exceptions(std::ios::badbit);
        assert(fileStream.is_open());
        assert(fileStream.good());

        std::size_t unsupportedCount(0);
        char ch(0);
        while (fileStream.get(ch))
        {
            if (!Ascii::isSupported(ch))
            {
                ++unsupportedCount;
            }

            std::cout << Ascii::replaceIfNeeded(ch);
            fileBytes.push_back(ch);
        }

        std::cout << "File size is " << fileBytes.size() << " bytes, with " << unsupportedCount
                  << " unsupported." << std::endl;
    }

    return EXIT_SUCCESS;
}

#include <fstream>
#include <cstdlib>
#include <limits>
#include <iostream>
#include <cctype>

class SupportedAscii
{
public:

    static char replaceWith()
    {
        return m_replacementChar;
    }

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
            std::cout << "Error:  " << "Some chars that were supposed to be supported no longer are, or the other way around." << std::endl;
        }

        return success;
    }

    static void printAll()
    {
        for (char ch(std::numeric_limits<char>::lowest());; ++ch)
        {
            std::cout << static_cast<int>(ch) 
                      << " \'" << ch << "\' "
                      << std::boolalpha << isSupported(ch)
                      << '\n';

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

        return ((ch >= ' ') && (ch <= '\176')); //176oct is del
    }

private:
    static inline std::size_t m_supportedCount{ 98 };// found with printAll() -zTn 2020-6
    static inline char m_replacementChar{' '};
};

int main()
{
    SupportedAscii::printAll();
    SupportedAscii::test();


    //std::ifstream fileStream("", std::ios_base::openmode);

    return EXIT_SUCCESS;
}

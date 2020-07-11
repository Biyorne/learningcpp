#include <fstream>
#include <cstdlib>
#include <limits>
#include <iostream>

namespace ascii
{

    const char unSupported{-88};

inline bool isSupported (const char ch)
{
    if((ch == '\t') || (ch == '\r') || (ch == '\n'))
    {
        return true;
    } 

    return ((ch >= '\040') && (ch <= '\176'));
}

}

int main()
{
    using namespace ascii;

    for(char ch(std::numeric_limits<char>::lowest()); ch <= std::numeric_limits<char>::max(); ++ch)
    {
        std::cout << static_cast<int>(ch) << '\040' << ((isSupported(ch)) ? ch:unSupported ) << '\040' << std::boolalpha << isSupported(ch) << '\n';

        if (std::numeric_limits<char>::max() == ch)
        {
            break;
        }
    }

    return EXIT_SUCCESS;
}
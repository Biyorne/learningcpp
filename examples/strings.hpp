#ifndef EXAMPLES_STRINGS_HPP_INCLUDED
#define EXAMPLES_STRINGS_HPP_INCLUDED
//
// vector-capacity.hpp
//
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <vector>

namespace strings
{

    inline std::size_t Ex1_getStringLength_StdStr(const std::string & str) { return str.length(); }

    inline std::size_t Ex1_getStringLength_Cstr(const char * cstrPtr)
    {
        if (nullptr == cstrPtr)
        {
            return 0;
        }

        std::size_t count(0);
        while ((*cstrPtr) != 0)
        {
            ++cstrPtr;
            ++count;
        }

        return count;
    }

    inline void Ex1()
    {
        auto testStrLength = [&](const std::size_t length) {
            std::string str(length, 'a');

            const auto stdLength { Ex1_getStringLength_StdStr(str) };
            if (stdLength == length)
            {
                std::cout << "PASS!" << std::endl;
            }
            else
            {
                std::cout << "FAIL:  Ex1_getStringLength_StdStr() = " << stdLength << " instead of "
                          << length << "." << std::endl;

                // exit(1);
            }

            const auto cLength { Ex1_getStringLength_Cstr(str.c_str()) };
            if (cLength == length)
            {
                std::cout << "PASS! :)" << std::endl;
            }
            else
            {
                std::cout << "FAIL:  Ex1_getStringLength_Cstr()   = " << cLength << " instead of "
                          << length << "." << std::endl;

                // exit(1);
            }
        };

        const auto cEmptyLength { Ex1_getStringLength_Cstr(nullptr) };
        if (cEmptyLength == 0)
        {
            std::cout << "PASS! :)" << std::endl;
        }
        else
        {
            std::cout << "FAIL:  Ex1_getStringLength_Cstr(nulptr)   = " << cEmptyLength
                      << " instead of " << 0 << "." << std::endl;
        }

        testStrLength(0);
        testStrLength(1);
        testStrLength(2);
        testStrLength(3);
        testStrLength(4);

        testStrLength(10);
        testStrLength(1024);
        testStrLength(static_cast<std::size_t>(std::numeric_limits<int>::max()));
        testStrLength(static_cast<std::size_t>(std::numeric_limits<int>::max()) + 1);
        testStrLength(static_cast<std::size_t>(std::numeric_limits<int>::max()) + 33);
    }

    //

    inline void runAll()
    {
        std::cout << "Running all string examples..." << std::endl;
        Ex1();
        std::cout << "...done." << std::endl;
    }

} // namespace strings

#endif // EXAMPLES_STRINGS_HPP_INCLUDED

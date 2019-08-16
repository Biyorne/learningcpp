#ifndef EXAMPLES_STRINGS_HPP_INCLUDED
#define EXAMPLES_STRINGS_HPP_INCLUDED
//
// strings.hpp
//
#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

constexpr std::size_t operator"" _st(unsigned long long number)
{
    return static_cast<std::size_t>(number);
}

constexpr std::size_t operator"" _ST(unsigned long long number)
{
    return static_cast<std::size_t>(number);
}

namespace strings
{

    // Ex1

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
        testStrLength(static_cast<std::size_t>(std::numeric_limits<int>::max()) + 1);
    }

    // Ex2

    inline std::string Ex2_copyString_StdStr(const std::string & str, const std::size_t length)
    {
        // TODO
        return "";
    }

    inline const char * Ex2_copyString_Cstr(const char * cstrPtr, const std::size_t length)
    {
        // TODO
        return nullptr;
    }

    inline void Ex2()
    {
        auto makeShortString = [&](const std::string & str) {
            if (str.length() <= 20)
            {
                return ("\"" + str + "\"");
            }
            else
            {
                return ("\"" + str.substr(0, 20) + "...\"(" + std::to_string(str.length()) + ")");
            }
        };

        auto makeStdString = [&](const char * strPtr) {
            return std::string(strPtr, Ex1_getStringLength_Cstr(strPtr));
        };

        auto testResultsStdStr = [&](const std::string & functionName,
                                     const std::string & strToCopy,
                                     const std::size_t lengthLimit,
                                     const std::string & strActual) {
            if (strToCopy.substr(0, lengthLimit) == strActual)
            {
                std::cout << "PASS " << functionName << "(" << makeShortString(strToCopy) << ", "
                          << lengthLimit << ")" << std::endl;
            }
            else
            {
                std::cout << "FAIL:  " << functionName << "(" << makeShortString(strToCopy) << ", "
                          << lengthLimit << ")\n\t" << makeShortString(strToCopy) << "\n\t"
                          << makeShortString(strActual) << std::endl
                          << std::endl;

                exit(1);
            }
        };

        auto testResultsCStr = [&](const char * strToCopyPtr,
                                   const std::size_t lengthLimit,
                                   const char * strActualPtr) {
            if (strToCopyPtr == nullptr)
            {
                if (strActualPtr == nullptr)
                {
                    std::cout << "PASS testResultsCStr(nullptr, " << lengthLimit << ")"
                              << std::endl;
                }
                else
                {
                    std::cout << "FAIL:  testResultsCStr(nullptr, lenLimit=" << lengthLimit
                              << ") did not return nullptr." << std::endl;

                    exit(1);
                }
            }
            else if (*strToCopyPtr == '\0')
            {
                if (strActualPtr == nullptr)
                {
                    std::cout << "PASS testResultsCStr(\"\", " << lengthLimit << ")" << std::endl;
                }
                else if (*strActualPtr == '\0')
                {
                    std::cout << "PASS testResultsCStr(\"\", " << lengthLimit << ")" << std::endl;
                }
                else
                {
                    std::cout << "FAIL:  testResultsCStr(\"\\0\", lenLimit=" << lengthLimit
                              << ") did NOT return nullptr." << std::endl;

                    exit(1);
                }
            }
            else if (strActualPtr == nullptr)
            {
                std::cout << "FAIL:  Ex2_copyString_StdStr("
                          << makeShortString(makeStdString(strToCopyPtr))
                          << ", lenLimit=" << lengthLimit << ") returned nullptr." << std::endl;

                exit(1);
            }
            else
            {
                testResultsStdStr(
                    "Ex2_copyString_StdStr",
                    makeStdString(strToCopyPtr),
                    lengthLimit,
                    makeStdString(strActualPtr));
            }
        };

        auto testStrCopyAtLength
            = [&](const std::string & strToCopy, const std::size_t lengthLimit) {
                  testResultsStdStr(
                      "Ex2_copyString_StdStr",
                      strToCopy,
                      lengthLimit,
                      Ex2_copyString_StdStr(strToCopy, lengthLimit));

                  testResultsCStr(
                      strToCopy.c_str(),
                      lengthLimit,
                      Ex2_copyString_Cstr(strToCopy.c_str(), lengthLimit));
              };

        auto testStrCopy = [&](const std::string & strToCopy) {
            const auto length { strToCopy.length() };
            testStrCopyAtLength(strToCopy, length);

            for (std::size_t i(0); i < (length * 4);)
            {
                testStrCopyAtLength(strToCopy, i);

                if (i <= 4)
                {
                    ++i;
                }
                else
                {
                    i *= 2;
                }
            }
        };

        if (Ex2_copyString_Cstr(nullptr, 0) == nullptr)
        {
            std::cout << "PASS" << std::endl;
        }
        else
        {
            std::cout << "FAIL:  Ex2_getStringLength_Cstr(nulptr) should have returned "
                         "nullptr but "
                         "did not."
                      << std::endl;
        }

        if (Ex2_copyString_Cstr(nullptr, 10) == nullptr)
        {
            std::cout << "PASS" << std::endl;
        }
        else
        {
            std::cout << "FAIL:  Ex2_getStringLength_Cstr(nulptr, 10) should have returned "
                         "nullptr "
                         "but did not."
                      << std::endl;
        }

        testStrCopy("");
        testStrCopy("a");
        testStrCopy("ab");
        testStrCopy("abc");
        testStrCopy("abcd");

        testStrCopy(" ");
        testStrCopy("  ");
        testStrCopy("   ");
        testStrCopy("    ");

        testStrCopy(std::string(10, 'a'));

        testStrCopy(std::string(1023, 'a'));
        testStrCopy(std::string(1024, 'a'));
        testStrCopy(std::string(1025, 'a'));

        // testStrCopy(
        //    std::string((static_cast<std::size_t>(std::numeric_limits<int>::max()) + 1), 'a'));
    }

    //

    inline void runAll()
    {
        std::cout << "Running all string examples..." << std::endl;
        // Ex1();
        Ex2();
        std::cout << "...done." << std::endl;
    }

} // namespace strings

#endif // EXAMPLES_STRINGS_HPP_INCLUDED

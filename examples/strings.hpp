#ifndef EXAMPLES_STRINGS_HPP_INCLUDED
#define EXAMPLES_STRINGS_HPP_INCLUDED
//
// strings.hpp
//
#include <algorithm>
#include <cstdlib>
#include <cstring>
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

    inline std::string Ex2_copyString_StdStr(const std::string & orig, const std::size_t length)
    {
        return orig.substr(0, length);
    }

    inline const char * Ex2_copyString_Cstr(const char * origPtr, const std::size_t lengthLimit)
    {
        const std::size_t lengthFinal(std::min(lengthLimit, strlen(origPtr)));
        char * copyPtr(new char[lengthFinal + 1]);

        // hand-written loop way to AVOID #1
        // std::size_t i(0);
        // for (; i < lengthFinal; ++i)
        //{
        //    *(copyPtr + i) = *(origPtr + i);
        //}
        //*(copyPtr + i) = '\0';

        // hand-written loop way to AVOID #2
        // for (std::size_t i(0); i < lengthFinal; ++i)
        //{
        //    *copyPtr++ = *origPtr++;
        //}
        //*copyPtr = '\0';
        // return (copyPtr - lengthFinal);

        // C way of not using hand written loops
        // memcpy(copyPtr, origPtr, lengthFinal);
        //*(copyPtr + lengthFinal) = '\0';
        // return copyPtr;

        // C++ std algorithm library way
        std::copy(origPtr, origPtr + lengthFinal, copyPtr);
        *(copyPtr + lengthFinal) = '\0';
        return copyPtr;
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

        auto testResultsCStr = [&](const std::string & functionName,
                                   const char * strToCopyPtr,
                                   const std::size_t lengthLimit,
                                   const char * strActualPtr) {
            if (strToCopyPtr == nullptr)
            {
                if (strActualPtr == "")
                {
                    std::cout << "PASS " << functionName << "(nullptr, " << lengthLimit << ")"
                              << std::endl;
                }
                else
                {
                    std::cout << "FAIL:  " << functionName << "(nullptr, lenLimit=" << lengthLimit
                              << ") did not return literal." << std::endl;
                }
            }
            else if (strActualPtr == nullptr)
            {
                std::cout << "FAIL:  " << functionName << "("
                          << makeShortString(makeStdString(strToCopyPtr))
                          << ", lenLimit=" << lengthLimit << ") returned nullptr." << std::endl;
            }
            else
            {
                testResultsStdStr(
                    functionName,
                    makeStdString(strToCopyPtr),
                    lengthLimit,
                    makeStdString(strActualPtr));
            }

            delete[] strActualPtr;
        };

        auto testStrCopyAtLength
            = [&](const std::string & strToCopy, const std::size_t lengthLimit) {
                  testResultsStdStr(
                      "Ex2_copyString_StdStr",
                      strToCopy,
                      lengthLimit,
                      Ex2_copyString_StdStr(strToCopy, lengthLimit));

                  testResultsCStr(
                      "  Ex2_copyString_CStr",
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

        // if (Ex2_copyString_Cstr(nullptr, 0) == "")
        //{
        //    std::cout << "PASS Ex2_copyString_Cstr(nullptr, 0)" << std::endl;
        //}
        // else
        //{
        //    std::cout << "FAIL:  Ex2_getStringLength_Cstr(nulptr) should have returned literal."
        //              << std::endl;
        //}
        //
        // if (Ex2_copyString_Cstr(nullptr, 10) == "")
        //{
        //    std::cout << "PASS Ex2_copyString_Cstr(nullptr, 10)" << std::endl;
        //}
        // else
        //{
        //    std::cout << "FAIL:  Ex2_getStringLength_Cstr(nulptr, 10) should have returned
        //    literal."
        //              << std::endl;
        //}

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

        testStrCopy(
            std::string((static_cast<std::size_t>(std::numeric_limits<int>::max()) + 1), 'a'));
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

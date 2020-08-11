#ifndef FILE_HPP_INCLUDED
#define FILE_HPP_INCLUDED
//
// filesys.hpp
//

#include <cmath>
#include <filesystem>
#include <sstream>
#include <string>
#include <vector>

//

constexpr std::size_t operator"" _st(unsigned long long number)
{
    return static_cast<std::size_t>(number);
}

namespace fixer::util
{

    //#if (defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64) ||
    // defined(__WINDOWS__))

    struct Platform
    {
#if defined(FIXER_RUNNING_ON_WINDOWS)
        static inline constexpr bool is_windows = true;
#endif

#if defined(FIXER_RUNNING_ON_NON_WINDOWS)
        static inline constexpr bool is_windows = false;
#endif

        static inline constexpr bool is_non_windows = !is_windows;

        static_assert(is_windows || is_non_windows);
        static_assert(is_windows != is_non_windows);
    };

    //

    inline std::wstring
        ratioToPercentStr(const std::size_t numerator, const std::size_t denominator)
    {
        std::wstringstream tempSS;
        tempSS << "  (";

        if (denominator == 0)
        {
            tempSS << L"0";
        }
        else
        {
            double result { (static_cast<double>(numerator) / static_cast<double>(denominator)) };
            result *= 100.0;

            // make sure thee are at most one decimal places
            result *= 10.0;
            result = std::round(result);
            result /= 10.0;

            tempSS << result;
        }

        tempSS << L"%)";
        return tempSS.str();
    };

} // namespace fixer::util

#endif // FILESYS_HPP_INCLUDED

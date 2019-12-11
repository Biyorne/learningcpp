#ifndef METHHEADS_ERROR_HANDLING_HPP_INCLUDED
#define METHHEADS_ERROR_HANDLING_HPP_INCLUDED

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>

namespace methhead
{
    // TODO
    // Macros providing __FILE__ __func__ and __LINE__
    //  Macros providing streamable error messages
    inline void assertOrThrow(const bool expression, const std::string & message)
    {
        if (!expression)
        {
            std::cout << "assertOrThrow(" << message << ")" << std::endl;

#ifdef NDEBUG
            throw std::runtime_error(message);
#else
            assert(expression);
#endif
        }
    }
} // namespace methhead

#endif // METHHEADS_ERROR_HANDLING_HPP_INCLUDED
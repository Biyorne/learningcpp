#ifndef METH_HEAD_ERROR_HANDLING_HPP_INCLUDED
#define METH_HEAD_ERROR_HANDLING_HPP_INCLUDED

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>

namespace methhead
{
    // TODO
    // Macros providing __FILE__ __func__ and __LINE__
    // Streamable error messages
    inline void assertOrThrow(const bool expression, const std::string & message)
    {
        if (!expression)
        {
            std::cerr << "assertOrThrow(" << message << ")" << std::endl;

#ifdef NDEBUG
            throw std::runtime_error(message);
#else
            assert(expression);
#endif
        }
    }

} // namespace methhead

#endif // METH_HEAD_ERROR_HANDLING_HPP_INCLUDED

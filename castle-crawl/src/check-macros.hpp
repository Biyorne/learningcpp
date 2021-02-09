#ifndef CASTLECRAWL_CHECK_MACROS_HPP_INCLUDED
#define CASTLECRAWL_CHECK_MACROS_HPP_INCLUDED
//
// check-macros.hpp
//
#include <cassert>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

//

#if !defined(M_DISABLE_ALL_CHECK_MACROS)

//

#ifdef NDEBUG
#define M_FAIL_HANDLER(exp, desc_str) throw std::runtime_error(desc_str);
#else
#define M_FAIL_HANDLER(exp, desc_str) assert((exp));
#endif

//

#define M_CHECK_SS(exp, streamable_extra_info)                                       \
    {                                                                                \
        if (!(exp))                                                                  \
        {                                                                            \
            std::ostringstream _m_desc_ss;                                           \
                                                                                     \
            _m_desc_ss << streamable_extra_info;                                     \
            const std::string _m_extra_str{ _m_desc_ss.str() };                      \
                                                                                     \
            _m_desc_ss.str("");                                                      \
                                                                                     \
            _m_desc_ss << "ERROR:  M_CHECK(" << #exp << ") failed at:  " << __FILE__ \
                       << "::" << __func__ << "()::" << __LINE__;                    \
                                                                                     \
            if (!_m_extra_str.empty())                                               \
            {                                                                        \
                _m_desc_ss << ":  \"" << _m_extra_str << "\"";                       \
            }                                                                        \
                                                                                     \
            std::cout << _m_desc_ss.str() << std::endl;                              \
                                                                                     \
            M_FAIL_HANDLER((exp), _m_desc_ss.str());                                 \
        }                                                                            \
    }

#define M_CHECK(exp) M_CHECK_SS((exp), "");

//

#else // defined(M_DISABLE_ALL_CHECK_MACROS)

#define M_LOG(streamable_message) ;
#define M_CHECK_SS(exp, streamable_extra_info) ;
#define M_CHECK(exp) ;

#endif // defined(M_DISABLE_ALL_CHECK_MACROS)

#endif // CASTLECRAWL

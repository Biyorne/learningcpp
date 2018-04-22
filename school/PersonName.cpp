#include "PersonName.hpp"
#include <sstream>
namespace school
{

    PersonName::PersonName(
        const std::string & FIRST, const std::string & MIDDLE, const std::string & LAST)
        : m_first(FIRST)
        , m_middle(MIDDLE)
        , m_last(LAST)
    {
        ensureValid();
    }

    std::string PersonName::toString() const
    {
        std::ostringstream ss;
        ss << m_first;
        if (m_middle.empty() == false)
        {
            ss << " " << m_middle;
        }
        ss << " " << m_last;
        return ss.str();
    }

    void PersonName::ensureValid() const
    {
        if (m_first.empty() || m_last.empty())
        {
            throw std::invalid_argument("first or last name was empty");
        }
    }

    bool operator==(const PersonName & NAMEA, const PersonName & NAMEB)
    {
        return (
            (NAMEA.m_first == NAMEB.m_first) && (NAMEA.m_middle == NAMEB.m_middle)
            && (NAMEA.m_last == NAMEB.m_last));
    }

    bool operator!=(const PersonName & NAMEA, const PersonName & NAMEB)
    {
        return !(NAMEA == NAMEB);
    }

} // namespace school

#ifndef SCHOOL_NAME_HPP_INCLUDED
#define SCHOOL_NAME_HPP_INCLUDED

#include <string>

namespace school
{
    class PersonName
    {
    public:
        PersonName(const std::string & FIRST, const std::string & MIDDLE, const std::string & LAST);

        std::string first() const { return m_first; }

        std::string middle() const { return m_middle; }

        std::string last() const { return m_last; }

        std::string toString() const;

        friend bool operator==(const PersonName & NAMEA, const PersonName & NAMEB);

    private:
        void ensureValid() const;

    private:
        std::string m_first;
        std::string m_middle;
        std::string m_last;
    };

    bool operator==(const PersonName & NAMEA, const PersonName & NAMEB);

    bool operator!=(const PersonName & NAMEA, const PersonName & NAMEB);

} // namespace school
#endif // SCHOOL_NAME_HPP_INCLUDED

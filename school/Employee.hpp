#ifndef SCHOOL_EMPLOYEE_HPP_INCLUDED
#define SCHOOL_EMPLOYEE_HPP_INCLUDED

#include "Address.hpp"
#include "Date.hpp"
#include "PersonName.hpp"
#include <string>

namespace school
{

    class Employee
    {
    public:
        Employee(const PersonName & NAME, const Date & DATE, const Address & ADDRESS);

        PersonName name() const { return m_name; }
        Date date() const { return m_dateOfBirth; }
        Address address() const { return m_address; }

        std::string toString() const { return m_name.toString(); }

    private:
        PersonName m_name;
        Date m_dateOfBirth;
        Address m_address;
    };

    bool operator==(const Employee & L, const Employee & R);

    bool operator!=(const Employee & L, const Employee & R);

} // namespace school

#endif // SCHOOL_EMPLOYEE_HPP_INCLUDED

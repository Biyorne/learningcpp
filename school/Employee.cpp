#include "Employee.hpp"
#include <sstream>
namespace school
{

    Employee::Employee(const PersonName & NAME, const Date & DATE, const Address & ADDRESS)
        : m_name(NAME)
        , m_dateOfBirth(DATE)
        , m_address(ADDRESS)
    {}

    bool operator==(const Employee & L, const Employee & R)
    {
        return ((L.name() == R.name()) && (L.date() == R.date()) && (L.address() == R.address()));
    }

    bool operator!=(const Employee & L, const Employee & R) { return !(L == R); }

} // namespace school

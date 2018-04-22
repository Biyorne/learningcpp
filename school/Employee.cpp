#include "Employee.hpp"
#include <sstream>
namespace school
{

    Employee::Employee(const PersonName & NAME)
        : m_name(NAME)
    {}

    bool operator==(const Employee & L, const Employee & R) { return (L.name() == R.name()); }
    bool operator!=(const Employee & L, const Employee & R) { return !(L == R); }
} // namespace school

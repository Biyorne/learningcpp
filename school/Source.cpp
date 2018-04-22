#include "Address.hpp"
#include "Class.hpp"
#include "Date.hpp"
#include "Employee.hpp"
#include "PersonName.hpp"
#include "TimeDuration.hpp"
#include "TimeOfDay.hpp"
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>

int main(void)
{
    school::PersonName bobName("Bob", "", "Derp");
    school::Employee bobEmployee(bobName);
    school::Address bobAddress("USA", "Colorado", "Denver", "80005", "123 main");
    school::Date bobDateOfBirth(1972, 03, 1);

    school::Class bobsClass(
        "Math", bobEmployee, bobDateOfBirth, bobAddress, school::TimeOfDay(4, 0, 0));

    school::PersonName sallyName("Sally", "", "Derp");

    // test equal operators
    std::cout << "Does my PersonName equality operator work?  " << std::boolalpha
              << ((bobName == bobName) && (sallyName != bobName)) << std::endl;

    std::cout << "Do Sally and Bob have the same name?  " << std::boolalpha
              << (sallyName == bobName) << std::endl;

    char ignore;
    std::cin >> ignore;
    return 0;
}

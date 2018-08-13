#include "Address.hpp"
#include "Class.hpp"
#include "Date.hpp"
#include "Employee.hpp"
#include "PersonName.hpp"
#include "TimeDuration.hpp"
#include "TimeOfDay.hpp"
#include "file-io.hpp"
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>

int main(void)
{
    school::runFileIoTests();
    return EXIT_SUCCESS;
}

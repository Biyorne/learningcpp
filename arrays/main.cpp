#include "angry-white-wrapper.hpp"
#include "std-container-examples.hpp"
#include "test.hpp"
#include "util.hpp"

#include <cstdlib>

int main(void)
{
    test::runTests();
    util::runTests();
    wrappers::runTests();
    examples::runTests();

    return EXIT_SUCCESS;
}

// ----------------------------------------------------------------------------
// "THE BEER-WARE LICENSE" (Revision 42):
// Nel Carlson wrote this file.  As long as you retain this notice you
// can do whatever you want with this stuff. If we meet some day, and you think
// this stuff is worth it, you can buy me a beer in return.  <biyorne@gmail.com>
// ----------------------------------------------------------------------------
//
// main.cpp
//

//
// The File Fixer App
//  Features:
//      Makes all file extensions in a directory (recursive) lower case.
//      Print out the path of all files fixed.
//

#include "util-ascii.hpp"

#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>

int main()
{
    Ascii::printAll();
    Ascii::test();
    return EXIT_SUCCESS;
}

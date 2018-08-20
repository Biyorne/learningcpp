#ifndef FILE_IO_HPP_INCLUDED
#define FILE_IO_HPP_INCLUDED

#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <string>
#include <vector>

namespace school
{
    void printFileByCharacter(const std::string & FILE_NAME)
    {
        // std::fstream::in
        // You can do reading from the file
        // std::fstream::out
        // You can write to the file
        // Default is both in and out
        // std::filestream::binary
        // Grabs bits and stuffs them where we tell it to
        // std::fstream::ate
        // "At End" - Write position starts at the end of the file
        // std::fstream::app
        // "Append" - Write position is always at the end of the file
        // std::filestream::trunc
        // "Truncate" - As soon as you open, delete everything
        std::fstream fileStream(FILE_NAME);
        if (!fileStream.is_open())
        {
            std::cerr << "Failed to open file: " << FILE_NAME << std::endl;
        }
        else
        {
            // Can't loop on eof() because it's only true after you read too far.
            // Can't loop on bad() because the file could not be read.
            // Can't loop on fail() because there is a logical error.
            // Can't loop on good() because it only tells you if ! fail() or bad()
            // Unix line endings are \n which is 10, or Line Feed
            // Macintosh before OSX used \r
            // Windows uses \r\n
            // When writing c++, \n means whatever the OS is using
            char myChar(0);
            while (fileStream >> std::noskipws >> myChar)
            {
                std::cout << myChar;
            }
        }
    }

    void printFileByString(const std::string & FILE_NAME)
    {
        std::fstream fileStream(FILE_NAME);
        if (!fileStream.is_open())
        {
            std::cerr << "Failed to open file: " << FILE_NAME << std::endl;
        }
        else
        {
            std::string str;
            while (fileStream >> str)
            {
                std::cout << str << '\n';
            }
            std::cout << std::flush;
        }
    }

    void printFileByLine(const std::string & FILE_NAME)
    {
        std::fstream fileStream(FILE_NAME);
        if (!fileStream.is_open())
        {
            std::cerr << "Failed to open file: " << FILE_NAME << std::endl;
        }
        else
        {

            std::string line;
            while (std::getline(fileStream, line))
            {
                std::cout << line << '\n';
            }
            std::cout << std::flush;
        }
    }

    void fileWordTests(const std::string & FILE_NAME)
    {
        std::fstream fileStream(FILE_NAME);
        if (!fileStream.is_open())
        {
            std::cerr << "Failed to open file: " << FILE_NAME << std::endl;
        }
        else
        {
            std::set<std::string> lines;
            std::string line;
            while (std::getline(fileStream, line))
            {
                lines.insert(line);
            }
            /*
for (const auto & STREET_NAME : lines)
{
    std::cout << STREET_NAME << '\n';
}
std::cout << std::flush;
            */
            std::fstream outputStream("out.txt", std::fstream::out | std::fstream::trunc);
            for (const auto & STREET_NAME : lines)
            {
                outputStream << STREET_NAME << '\n';
            }
        }
    }

    void runFileIoTests()
    {
        fileWordTests("names-street.txt");
        // printFileByLine("names-street.txt");
        // printFileByString("names-street.txt");
        // printFileByCharacter("names-street.txt");
        // std::cout << static_cast<int>(std::numeric_limits<char>::min()) << " to "
        //          << static_cast<int>(std::numeric_limits<char>::max()) << std::endl;
    }

} // namespace school

#endif FILE_IO_HPP_INCLUDED

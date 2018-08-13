#ifndef FILE_IO_HPP_INCLUDED
#define FILE_IO_HPP_INCLUDED

#include <fstream>
#include <iostream>
#include <numeric>

namespace school
{
    void runFileIoTests()
    {
        std::cout << static_cast<int>(std::numeric_limits<char>::min()) << " to "
                  << static_cast<int>(std::numeric_limits<char>::max()) << std::endl;

        {
            const std::string FILE_NAME("names-street.txt");
            std::fstream fileStream(FILE_NAME);
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
            if (!fileStream.is_open())
            {
                std::cerr << "Failed to open file: " << FILE_NAME << std::endl;
            }
            char c(0);

            // Can't loop on eof() because it's only true after you read too far.
            // Can't loop on bad() because the file could not be read.
            // Can't loop on fail() because there is a logical error.
            // Can't loop on good() because it only tells you if ! fail() or bad()
            // Unix line endings are \n which is 10, or Line Feed
            // Macintosh before OSX used \r
            // Windows uses \r\n
            // When writing c++, \n means whatever the OS is using
            while (fileStream.get(c))
            {
                std::cout << c;
                if ('\n' == c)
                {
                    break;
                }
            }
        }
    }

} // namespace school

#endif FILE_IO_HPP_INCLUDED

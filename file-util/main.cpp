#include <cassert>
#include <cstdlib>
#include <filesystem>
#include <iostream>

// target high-tolerance error handling, but start with good use of assert()

/*
 * FEATURE #1:  Find Duplicate Names
 *  Given a directory, prints all absolute paths for any regular files that have the same name,
ignoring the extention.
 *  Start with case-sensitive
 *  Start with only supporting ascii
 *
 * Sample Output
 *
Found Duplicates:

filename.txt: (x2)
        c:\src
        c:\build

anotherfile.txt: (x2)
        c:\src
        c:\build

*/

void printAllFilesIn(const std::filesystem::path & dirPath = std::filesystem::path("."));

std::size_t regularFileCount(0);
std::size_t regularFileCountPerStatus(10'000);

int main()
{
    try
    {
        printAllFilesIn("D:/");
    }
    catch (const std::exception & ex)
    {
        std::cerr << "Error after " << regularFileCount
                  << " files:  Unhandled std::exception:  " << ex.what() << '\n';
    }

    return EXIT_SUCCESS;
}

void printAllFilesIn(const std::filesystem::path & dirPathOrig)
{
    namespace fs = std::filesystem;

    const fs::path dirPathCann(fs::canonical(dirPathOrig));

    assert(fs::exists(dirPathCann));
    assert(fs::is_directory(dirPathCann));

    for (const fs::directory_entry & entry : fs::recursive_directory_iterator(
             dirPathCann, fs::directory_options::skip_permission_denied))
    {
        if (!entry.is_regular_file())
        {
            continue;
        }

        ++regularFileCount;

        if ((regularFileCount % regularFileCountPerStatus) == 0)
        {
            std::cout << "#" << regularFileCount << "\n";
            regularFileCountPerStatus += 1000;
        }

        // std::cout << entry.path().string() << '\n';
    }

    std::cout << "Had access to " << regularFileCount << " regular files.\n";
}

#include <cassert>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <map>

// All kinds of permission/access/privilages/etc errors are logged, not included, but the app keeps
// going.

// Error Handling:
//  Start with non-tolerant, (i.e. assert/exceptions), because it won't take so much time write.
//  Use std::cerr for all error output.
//  Prefer using throwing version of std::filesystem functions.

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
        printAllFilesIn("C:/");
    }
    catch (const std::exception & ex)
    {
        std::cerr << "Error after " << regularFileCount
                  << " files:  Unhandled std::exception:  what=\'" << ex.what() << "\'"
                  << std::endl;

        assert(false);
    }

    return EXIT_SUCCESS;
}

void printAllFilesIn(const std::filesystem::path & dirPathOrig)
{
    namespace fs = std::filesystem;

    const fs::path dirPathCann(fs::canonical(dirPathOrig));

    assert(fs::exists(dirPathCann));
    assert(fs::is_directory(dirPathCann));

    using Map_t = std::map<std::string, std::size_t>;
    using MapPair_t = std::pair<std::string, std::size_t>;
    Map_t fileNamesToCount;

    for (const fs::directory_entry & entry : fs::recursive_directory_iterator(
             dirPathCann, fs::directory_options::skip_permission_denied))
    {

        try
        {
            const fs::file_status status(entry.status());
        }
        catch (const std::exception & ex)
        {
            std::cerr << "Error:  printAllFilesIn(" << dirPathCann
                      << ") threw exception trying to call status() on:  what=\'" << ex.what()
                      << "\'.  Continuing..." << std::endl;

            continue;
        }

        if (!entry.is_regular_file())
        {
            continue;
        }

        fileNamesToCount[entry.path().filename().string()]++;

        // temp way to make sure it is not hung with status updates
        ++regularFileCount;
        if ((regularFileCount % regularFileCountPerStatus) == 0)
        {
            std::cout << "#" << regularFileCount << '\n';
            regularFileCountPerStatus += regularFileCount;
        }
    }

    std::cout << "Finished searching..." << std::endl;

    std::vector<MapPair_t> nameCountDups;
    nameCountDups.reserve(fileNamesToCount.size());

    for (const MapPair_t & nameCountPair : fileNamesToCount)
    {
        if (nameCountPair.second <= 1)
        {
            continue;
        }

        nameCountDups.push_back(nameCountPair);
    }

    std::sort(
        std::begin(nameCountDups),
        std::end(nameCountDups),
        [](const MapPair_t & left, const MapPair_t & right) {
            return (left.second < right.second);
        });

    for (const auto & [filename, count] : nameCountDups)
    {
        if (count <= 2)
        {
            continue;
        }

        std::cout << filename << ": " << count << '\n';
    }

    std::cout << "Found " << fileNamesToCount.size() << " regular files with unique names, but "
              << nameCountDups.size() << " were duplicates." << std::endl;

    fileNamesToCount.clear();
}

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




// shorthand type names
namespace fs = std::filesystem;

using Map_t = std::map<std::string, std::size_t>;
using MapPair_t = std::pair<std::string, std::size_t>;
using VecPair_t = std::vector<MapPair_t>;


// function declarations defined below main
VecPair_t findDuplicatesIn(const std::filesystem::path & dirPath = std::filesystem::path("."));
void feature_findDuplicates();
void feature_findNonAscii(const std::filesystem::path & dirPathOrig);

// Responsible for periodically printing how many files have been found,
// since this app could run for whole minutes without showing anything,
// and the user will think the app has hung.  And guess what happens to
// your giant BlueRay files when the user thinks it's hung and interrupts
// a copy/move/rename/etc... 
struct Status
{
    static void reset()
    { 
        regularFileCount = 0; 
        regularFileCountPerStatus = 10000;
    }

    static void printIfNeeded()
    {
        ++regularFileCount;

        if ((regularFileCount % regularFileCountPerStatus) == 0)
        {
            std::cout << "#" << regularFileCount << '\n';
            regularFileCountPerStatus += regularFileCount;
        }
    }


    static inline std::size_t regularFileCount { 0 };
    static inline std::size_t regularFileCountPerStatus { 10000 };
};

//

//

int main()
{
    try
    {
        feature_findNonAscii("N:/My Stuff/Music/Ace Attorney/Phoenix Wright - Trials and Tribulations");
    }
    catch (const std::exception & ex)
    {
        std::cerr << "Error after " << Status::regularFileCount
                  << " files:  Unhandled std::exception:  what=\'" << std::flush << ex.what() << "\'"
                  << std::endl;

        assert(false);
    }

    std::cout << "(reached end of main)\n";
    return EXIT_SUCCESS;
}

/*
 * FEATURE #1:  Find Duplicate Names
 *  Given a directory, prints all absolute paths for any regular files that have
    the same name, ignoring the extention.
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

--
TODO: 
    don't consider extensions when comparing filenames.
    write the function that prints the final results.

*/
void feature_findDuplicates() {
    Status::reset();
    VecPair_t dupPairs(findDuplicatesIn("C:/")); 
}

void feature_findNonAscii(const std::filesystem::path & dirPathCann)
{
    // Status::reset();

    // const fs::path dirPathCann(fs::canonical(dirPathOrig));

    // assert(fs::exists(dirPathCann));
    // assert(fs::is_directory(dirPathCann))
     for (const fs::directory_entry & entry : fs::recursive_directory_iterator(
             dirPathCann, fs::directory_options::skip_permission_denied))
    {
        //try
        //{
        //    const fs::file_status status(entry.status());
        //}
        //catch (const std::exception & ex)
        //{
        //    std::cerr << "Error!"  << std::endl;
        //    continue;
        //}
        //
        //if (!entry.is_regular_file())
        //{
        //    continue;
        //}

        //TODO If entry contains non-ascii characters, print

         std::cout << ".\n";
         //std::wcout << entry.path().filename().wstring() << std::endl;

        //fileNamesToCount[entry.path().filename().string()]++;

        //Status::printIfNeeded();
    }
}

VecPair_t findDuplicatesIn(const std::filesystem::path & dirPathOrig)
{

    const fs::path dirPathCann(fs::canonical(dirPathOrig));

    assert(fs::exists(dirPathCann));
    assert(fs::is_directory(dirPathCann));


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

        Status::printIfNeeded();
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


    std::cout << "Found " << fileNamesToCount.size() << " regular files with unique names, but "
              << nameCountDups.size() << " were duplicates." << std::endl;

    return nameCountDups;
}

//

/*

#include "acme-refrigerator--model-1234.hpp"

namespace acme
{
    const unsigned char * motorStartPtr(1);
    const unsigned char * motorStopPtr(2);

    const unsigned char * sensorFullPtr(3);
    const unsigned char * sensorEmptyPtr(4);
}

*/

 //   std::sort(
//       std::begin(nameCountDups),
//       std::end(nameCountDups),
//       [](const MapPair_t & left, const MapPair_t & right) {
//           return (left.second < right.second);
//       });

//  for (const auto & [filename, count] : nameCountDups)
//  {
//      if (count <= 2)
//      {
//          continue;
//      }
//
//      std::cout << filename << ": " << count << '\n';
//  }







//#include "acme-refrigerator--model-1234.hpp"


namespace acme
{
    const unsigned char onValue(1);
    const unsigned char offValue(0);

    unsigned char * const motorStartPtr('\0');
    unsigned char * const motorStopPtr('\0');

    unsigned char * const sensorFullPtr('\0');
    unsigned char * const sensorEmptyPtr('\0');

    void callbackTrayStateSet(const bool willExecute) { }

    void callbackTrayStateChanged() 
    {
        if (onValue == *sensorEmptyPtr)
        {
            *motorStartPtr = onValue;
        }

        if (onValue == *sensorFullPtr)
        {
            *motorStopPtr = onValue;
        }
    }

}


int main()
{
    acme::callbackTrayStateSet(true);
    return EXIT_SUCCESS; 
}
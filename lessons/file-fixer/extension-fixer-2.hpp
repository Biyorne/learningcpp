#ifndef EXTENSION_FIXER_2_HPP_INCLUDED
#define EXTENSION_FIXER_2_HPP_INCLUDED
//
// extension-fixer-2.hpp
//
#include <filesystem>

namespace fixer::ext2
{
    struct Extension
    {
        void makeLowerCase(const std::filesystem::path & directoryPath);
        void makeLowerCase(const std::filesystem::directory_entry & entry);
    };
} // namespace fixer::ext2

#endif // EXTENSION_FIXER_2_HPP_INCLUDED

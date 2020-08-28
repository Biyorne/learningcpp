#ifndef EXTENSION_FIXER_HPP_INCLUDED
#define EXTENSION_FIXER_HPP_INCLUDED
//
// extension-fixer.hpp
//
#include <filesystem>

namespace fixer::extension
{
    struct Extension
    {
        void makeLowerCase(const std::filesystem::path & directoryPath);
        bool makeLowerCase(const std::filesystem::directory_entry & entry);
    };
} // namespace fixer::extension

#endif // EXTENSION_FIXER_HPP_INCLUDED

#ifndef EXTENSION_FIXER_1_HPP_INCLUDED
#define EXTENSION_FIXER_1_HPP_INCLUDED
//
// extension-fixer-1.hpp
//
#include <filesystem>

namespace fixer::ext1
{
    struct Extension
    {
        void makeLowerCase(const std::filesystem::path & directoryPath);
    };
} // namespace fixer::ext1

#endif // EXTENSION_FIXER_1_HPP_INCLUDED

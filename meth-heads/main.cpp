// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "simulator.hpp"

#include <cstddef>

int main()
{
    //auto getSizes = [&](const std::string & str) {
    //    try
    //    {
    //        const auto size(str.size());
    //        const auto dashIndex(str.rfind('-'));
    //        const auto xIndex(str.rfind('x'));
    //
    //        if ((xIndex <= dashIndex) || (xIndex >= size) || (dashIndex >= size))
    //        {
    //            std::cout << " FAIL \"" << str << "\",  dash_index=" << dashIndex
    //                      << ", x_index=" << xIndex << ", error=(dash <= x)" << std::endl;
    //
    //            return;
    //        }
    //
    //        const std::string widthStr(str.substr((dashIndex + 1), (size - xIndex - 1)));
    //        const std::string heightStr(str.substr(xIndex + 1));
    //
    //        const int width(std::stoi(widthStr));
    //        const int height((heightStr.empty()) ? width : std::stoi(heightStr));
    //
    //        std::cout << " PASS \t\t\"" << str << "\": \t w=\"" << widthStr << "\""
    //                  << ", h=\"" << heightStr << "\",  " << width << 'x' << height << std::endl;
    //    }
    //    catch (...)
    //    {
    //        std::cout << " FAIL \"" << str << "\": exception" << std::endl;
    //        return;
    //    }
    //};
    //
    //getSizes("");
    //getSizes("i");
    //getSizes("x");
    //getSizes("x1");
    //getSizes("1x");
    //getSizes("2x1");
    //getSizes("3x2x1");
    //
    //std::cout << std::endl;
    //
    //getSizes("-");
    //getSizes("-i");
    //getSizes("-x");
    //getSizes("-x1");
    //getSizes("-1x");
    //getSizes("-2x1");
    //getSizes("-3x2x1");
    //
    //std::cout << std::endl;
    //
    //getSizes("-");
    //getSizes("i-");
    //getSizes("x-");
    //getSizes("x1-");
    //getSizes("1x-");
    //getSizes("2x1-");
    //getSizes("3x2x1-");
    //
    //std::cout << std::endl;
    //
    //getSizes("-");
    //getSizes("-i");
    //getSizes("-x");
    //getSizes("-x9999999999999999999");
    //getSizes("-9999999999999999999x");
    //getSizes("-9999999999999999999x9999999999999999999");
    //getSizes("-9999999999999999999x9999999999999999999x9999999999999999999");
    //
    //std::cout << std::endl;
    //
    //getSizes("inferno-128x9999999999999999999");
    //getSizes("inferno-9999999999999999999x128");
    //getSizes("inferno-9999999999999999999x9999999999999999999");
    //getSizes("inferno-9999999999999999999x");
    //
    //std::cout << std::endl;
    //
    //getSizes("inferno-128x256");
    //getSizes("inferno-7x");
    //
    //std::cout << std::endl;
    //std::cout << std::endl;
    //std::cout << "npos=" << std::string::npos << ", npos+1=" << (std::string::npos + 1)
    //          << std::endl;

    using namespace methhead;

    Simulator simulator(Mode::Normal);
    simulator.run();

    return EXIT_SUCCESS;
}

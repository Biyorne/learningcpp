#include <cstdlib>
#include <iostream>

int main()
{
    std::cout << "You are in a jungle. Do you go (l)eft or (r)ight?" << std::endl;

    char direction { '\0' };
    std::cin >> direction;

    if ('l' == direction)
    {
        std::cout << "You escape and win!" << std::endl;
    }
    else
    {
        std::cout << "You are eaten by a grue. You lose." << std::endl;
    }

    return EXIT_SUCCESS;
}

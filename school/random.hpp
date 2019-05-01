#ifndef SCHOOL_RANDOM_HPP_INCLUDED
#define SCHOOL_RANDOM_HPP_INCLUDED

// main
// int seed(osSeed());
// for int i(0); i < 10; ++i
////std::cout << prevNumber << std::endl;
//
//

// Randomness - A lack of pattern and predictability.
// Entropy - The randomness of a closed system.
// True Randomness - Infinite entropy.
// Pseudo Randomness - Zero entropy, but difficult for humans to predict.
// Pseudo Random Number Generator - An algorithm that takes one number(seed) and gives you a
// pseudo random result. It's limited by the number of bits in the values(size) and the number of
// bits in the statespace.

// List of PRNG Algorithms (In order from lowest to highest quality)

// The C Standard Library's rand() function.
// Good enough that casual human users won't notice the pattern
// So bad that no professional uses it anymore
// Not good enough for encryption or statistical work

// The Linear Congruential Generator (LCG)
// The simplest, oldest and most well known
// The quality varies depending on how you configure it
// But still bad enough to be second on this list
// Very fast
// Still nowhere near good enough for encryption or statistical work

// Subtract-With-Carry (SWC)
// Just an improved version of LCG (but not a big one)
// Not good enough for encryption or statistical work

// The Mersenne Twister
// It has great quality with a few nasty "gotchas"
// Great speed
// The biggest "gotcha" is that it's unreasonably bad with only slightly bad seeds. It NEEDS a good
// seed.
// What professionals use when randomness is not critical.
// Still not good enough for encryption or statistics

// Four Ways to get a seed (In order from worst to best)

// Time (First used and worst possible)
// Using time to generate a seed is tolerable for casual use (OS can give time in nanoseconds)

// Human Interfaces
// At best, high entropy but at worst, low entropy

// Hardware
// Specialized chunks of transistors that leverage unpredictable electron behavior

// Operating Systems
// Really just a combination of the three above
// In general, combining multiple random sources does not add the entropy together. Usually, it
// cancels them out. Picks from the best available options in the computer

// Includes required to use the c standard library prng
#include <cstdlib> //This gives us rand() and srand()
#include <ctime>

#include <limits>

void outputRandomNumbersRand()
{
    for (int counter(0); counter < 50; ++counter)
    {
        std::cout << rollD6() << std::endl;
    }
}

int rollD6() { return rand(); }

void runRngTests()
{
    srand(time(0));
    outputRandomNumbersRand();
}

#endif // SCHOOL_RANDOM_HPP_INCLUDED

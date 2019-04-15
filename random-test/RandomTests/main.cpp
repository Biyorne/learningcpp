#include <algorithm>
#include <array>
#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>

#include "random.hpp"

// Randomness - A lack of pattern and predictability.
// Entropy - The randomness of a closed system.
// True Randomness - Infinite entropy.
// Pseudo Randomness - Zero entropy, but difficult for humans to predict.
// Pseudo Random Number Generator - An algorithm that takes one number(seed) and gives you a
// pseudo random result. It's limited by the number of bits in the values(size) and the number of
// bits in the seed.

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

// Distribution
// How likely some numbers are vs others to come out of the prng

// Non-Uniformity
//
//  True Randomness means that every number that can possibly be generated has the same chance of
//  being generated, but this is not always what you want.  The way to define non-truly-random
//  behavior is with a probability distribution.  Here are the most common examples:
//
//      Uniform Distribution    - Every number that can possibly be generated has an equal chance of
//                                being generated.
//
//     x Normal Distribution     - Values in the center of the interval are more likely, creating a
//                                classic bell-curve probability distribution.  This is how you
//                                could generate on the interval [1,100] where 50 is the most
//                                likely.
//
//      Exponential Distribution- If you count the number of times a coin toss lands 'heads' before
//                                the first 'tails', then the chances of each count are: 0=50%,
//                                1=25%, 2=12.5%, and so on.  Ths is an Exponential Distribution.
//
//      (dozens of others...)
//
// The point:
//   - True Randomness is rarely what you want.
//   - If you want to eliminate duplicates or runs, then use a batch and remove process.
//   - All other situations where you want non-truly-random behavior can and should be defined with
//     a probability distribution.

// The old C rand() way.
#include <cstdlib>
#include <ctime>
void tenRandomNumbers_theOldCrandWay()
{
    srand(static_cast<unsigned int>(time(nullptr)));
    for (int i(0); i < 10; ++i)
    {
        std::cout << (1 + (rand() % 4)) << std::endl;
    }
}

// Quick and dirty c++11 way
// Not good enough for: encryption, statistics, unpredictability, if you need too many numbers or
// need them too quickly
#include <random>
void tenRandomNumbers_quickAndDirtyCpp11Way()
{
    // Decides where to get the seed from and gives us as many uints as we ask from it.
    std::random_device randomDevice;

    // Takes a small number of seeds and stretches it into however many the prng needs.
    //(note:  this is the worst way to use seed_seq, but good enough for casual apps.)
    std::seed_seq seedSequence { randomDevice() };

    // make the mt19937 PRNG and give it the seed  (seed_seq)
    std::mt19937 engine;
    engine.seed(seedSequence);

    // this is both the distribution and the range limited over [1, 4]
    std::uniform_int_distribution<int> distribution(1, 4);

    for (int i(0); i < 10; ++i)
    {
        std::cout << distribution(engine) << std::endl;
    }
}

// Best possible use of mt19937
// yes we lose some entropy but this is still a kick ass mt19937
#include <array>
#include <random>

void tenRandomNumber_bestCpp11Way()
{
    std::mt19937 engine;

    // create the the full state/seed from best seed sources
    std::random_device seedSource;
    std::array<unsigned int, engine.state_size> seedArray;
    std::generate(std::begin(seedArray), std::end(seedArray), std::ref(seedSource));

    // put our perfect seed into seed_seq which makes no sense and loses entropy but we have to
    // because the engine only takes a seed_seq
    std::seed_seq seedSequence(std::begin(seedArray), std::end(seedArray));
    engine.seed(seedSequence);

    std::uniform_int_distribution<int> dist { 1, 4 };

    for (std::size_t i(0); i < 10; ++i)
    {
        std::cout << dist(engine) << std::endl;
    }
}

void diceRoller()
{
    while (true)
    {
        std::cout << "How many sides?" << std::endl;
        int dieSideCount(0);
        std::cin >> dieSideCount;

        if (dieSideCount < 0)
        {
            return;
        }

        std::cout << "How many times?" << std::endl;
        std::size_t count(0);
        std::cin >> count;

        std::mt19937 engine;

        std::random_device seedSource;
        std::array<unsigned int, engine.state_size> seedArray;
        std::generate(std::begin(seedArray), std::end(seedArray), std::ref(seedSource));

        std::seed_seq seedSequence(std::begin(seedArray), std::end(seedArray));
        engine.seed(seedSequence);

        std::uniform_int_distribution<int> dist { 1, dieSideCount };

        for (std::size_t i(0); i < count; ++i)
        {
            std::cout << count << "d" << dieSideCount << ": " << dist(engine) << std::endl;
        }
    }
}

void randomRollExamples()
{
    Random random;
    std::cout << "1d6: " << random.rollInteger(1, 6) << std::endl;
    std::cout << "1d6: " << random.rollInteger(6, 1) << std::endl;
    std::cout << "1d6: " << random.rollInteger(6, 6) << std::endl;

    std::cout << "1d6: " << random.rollReal(1.0, 6.0) << std::endl;
}

int main(void)
{

    // diceRoller();
    // tenRandomNumbers_theOldCrandWay();
    // tenRandomNumber_bestCpp11Way()
    // randomRollExamples();

    Random random;

    for (int i(0); i < 10; ++i)
    {
        std::cout << random.rollInteger(10) << std::endl;
        std::cout << random.rollReal(10.0f) << std::endl;
    }

    char ignore('a');
    std::cin >> ignore;
    return EXIT_SUCCESS;
}

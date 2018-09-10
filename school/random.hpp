#ifndef SCHOOL_RANDOM_HPP_INCLUDED
#define SCHOOL_RANDOM_HPP_INCLUDED

#include <functional>
#include <iostream>

// Randomness
//  The lack of pattern or predictability.

// Entropy
//
// Definitions:
//
//  (1) The typical meaning of Entropy comes from thermo dynamics and chemistry, where it is the
// amount of heat that cannot do work, or the heat that you inherently lose in a system,
// process, or conversion of energy.
//
//  (2) The information lost in a transmition or conversion.  (Think packet loss during internet
//  downloads, or the blurry edges that result from scaling a raster image.)
//
//  (3) The randomness in a closed system.
//
//  In the world of randomness in software, we will use definition (3).  We will call some
//  function that returns (generates) a random number.  A function is a closed system, so
//  Entropy is the measure of how random the generated number is.  So the function, or
//  algorithm, has some level of Entropy.  The more random, the higher the Entropy.  If the
//  Entropy is zero, then the generated number is not random at all, and is completely
//  predictable.

// True Randomness (infinite Entropy)
//  Most consider this to be impossible to achieve in a computer, because they are designed to
//  behave in a perfectly deterministic way.  However, recent advances in modern computer chip
//  hardware with specialized random features are changing the game...

// Pseudo-Randomn
//  Zero Entropy, completely predictable and not random at all, but too complex for humans to
//  predict.
//
// Pseudo-Randomn Number Generators (PRNGs)
//  A function or algorithm that changes a given starting number, called the seed, into a
//  resulting (generated) number.  The changes are so complex that humans cannot predict what
//  the generated number will be.  To humans, the generated number APPEARS to be truly random,
//  but it is actually the opposite.
//
//  To get another pseudo-random number, you re-run the PRNG using the previously generated
//  number as the new seed.  If you don't change the seed, you will get the same numbers every
//  time.
//
//  So if you use a Pseudo-Randomn Number Generator for your game, and use the same seed every
//  time, then all your random game elements will turn out the same every time it is played.  A
//  Poker game written this way will have the same shuffles and hands every time.  Similarly, an
//  RPG will always have the same random damage for every attack.  Etc.
//
//  This is actually a very handy feature for game developers.  During game testing, every run
//  of the game saves the seed it started with.  That way developers can re-run the game with
//  that same random behavior to better re-create the situation where a bug occurred.
//
//  So if your app only needs to APPEAR random to a human, such as a game, then all you need is
//  a seed that humans cannot predict, and then a PRNG can create as many more pseudo-random
//  numbers as you need.
//
//  However, if your app needs as close to truly random numbers as possible, as close to
//  infinite Entropy as possible, then that means your seed must be of extremely high Entropy.  This
//  is what you need for encryption and intense math such as statistical modeling, etc.
//
//  Also be aware that every PRNG will eventually generate the seed that it started with, after
//  that, it is simply repeating the numbers it has already generated.  The number of times that a
//  PRNG can be called before it resets is called the "length of cycle" or "state space". Obviously,
//  the bigger the better.
//
//  So using PRNGs is all about choosing the PRNG ALGORITHM and the SEED.

// PRNG Algorithms (in worst-to-best order of quality)
//
//  The C Standard Library's rand() Function
//      - Good enough that casual human users won't notice patterns.
//      - Bad enough to have fallen out of fashion, and not used much anymore.
//      - Bad enough that if you used it in a complex game, serious players might notice patterns.
//      - Not good enough for mission critical (encryption, statistical math, etc.)
//
//  Linear Congruential Generator (LCG)
//      - One of the simplest, oldest, and most well known.
//      - Quality ranges depending on how you configure it, but still second-worst in this list.
//      - very fast
//      - Included in the C++11 <random> library.
//      - Not good enough for mission critical use (encryption, statistical math, etc.)
//
//  Subtract-With-Carry
//      - Created as an improvement to LCG, but it wasn't a huge improvement in quality or speed.
//      - Included in the C++11 <random> library.
//      - Not good enough for mission critical use (encryption, statistical math, etc.)
//
//  Mersenne Twister
//      - Great quality (but there are a few gotchas...)
//      - Great Speed
//      - Don't pick the Mersenne Twister if you don't have good seeds.
//      - Included in the new C++ <random> library. (in fact it is the default PRNG)
//      * This is what everyone is using for typical apps and games.
//      - Not good enough for mission critical use (encryption, statistical math, etc.)
//
//  Mission critical uses (encryption, statistical math, etc.) requires researching to find the
// algoriths currently used by the community and approved by the NSA.

// Seeds (in worst-to-best order of quality)
//  Time
//      This is what everyone was using twenty years ago.  Every operating system provides a
//      function that returns a number representing the current time that includes tiny
//      fractions of a second.  That number will almost always be different every time an app
//      runs, and those tiny fractions of a second will be impossible for humans to predict,
//      thus, making it a tolerable seed for causal use.
//
//  Human-Interfaces
//      Some human-interfaces have very high Entropy, such as mouse positions and speeds, the
//      timing between keystrokes, hard drive delay times, etc.  Modern operating systems
//      collect these numbers and provide functions that will use all of it to seed a PRNG and
//      give you the result.
//
// Hardware
//      Modern computer chips are made of parts so tiny that they deal with very high Entropy
//      temperature changes and finiky elecrtons governed by quantum mechanics.  Such a chip can
//      create situations where the temperature or electron behavior is as close to impossible
//      to predict as you can get.  These random number generating features in modern chips have
//      extremely high Entropy, but they often take several seconds, which is often prohibitive.
//
// Combinations
//      So Hardware seeds are the best, but are too slow for most situations.  So the current
//      solution everyone uses is to have the operating system provide seeds based on Time and
//      Human-Interfaces.  Your app then feeds these seeds into whatever PRNG algorithm you
//      need.

// Review:
//  * A PRNG algorithm takes a seed and generates a pseudo-random number.
//  * Every time the PRNG is executed, the seed is replaced by the number it just generated.
//  * If the seed is the same, then a PRNG generates the same numbers in the same order -every time.
//  * PRNGs are not random at all (zero Entropy), and their quality varies.
//  * Low Entropy seeds result in patterns that clever code (and even some humans) can predict.
//  * Modern OSs use a combination of high Entropy sources to give you high Entropy seeds.

// Old School
//  Before we go any further, lets look at how everyone was getting random numbers in C twenty
//  years ago.  This is useful because it is still the quickest and simplest way to get a
//  pseudo-random number in C and even C++.  (also the worst quality though)  It is not good enough
//  to use in an app that people pay for, but most of the time you don't care and just want a quick
//  and dirty number that is "random-enough".  That means you will see this code everywhere, so you
//  should recognize it and understand it.  I'll call this the "Old School" version.  It uses three
//  functions from the C Standard Library:
//
//  - time()    returns the current system time as a number (used as the seed)
//  - srand()   sets the seed
//  - rand()    the PRNG function/algorithm returning an int
//
// Below is the code that makes it all work.

#include <cstdlib> //for srand() and rand()
#include <ctime> //for time()

// this function sets the seed for rand() using srand() and time(), usually this function would be
// called only once at the very start of main()
void seedRandomNumberGenerator_OldSchoolVersion_TutorialVersion()
{
    // get the current system time (to be used as the seed)
    const time_t CURRENT_TIME(time(nullptr));

    // Note that time() actually takes a pointer to a time_t object in memory, but that is only
    // if you want the time() function to do something with a pre-existing time, that is not
    // what we want, we just want time() to give us the current system time, so we just give it
    // a null pointer

    // right now CURRENT_TIME is of type "time_t", which is usually a "long long", but it varies
    // depending on the platform (OS/hardware), the srand() function we will pass CURRENT_TIME
    // to takes an "unsigned int", so CURRENT_TIME cannot be used as the seed until we cast it
    // to an unsigned int
    const unsigned int SEED(static_cast<unsigned int>(CURRENT_TIME));

    // here is where we actually set the seed
    srand(SEED);
}

// usually you will see all of the code above written as this one hackish line of C, and now that it
// is just one line there is no real need to put it in a function at all, so you usually just see
// this line at the top of main()
void seedRandomNumberGenerator_OldSchoolVersion_ShortVersion() { srand((unsigned int)time(0)); }

// this function uses rand() to generate a pseudo-random int over [0, RAND_MAX], RAND_MAX is a
// #define that can vary with the platform, but it is usually set to 32767
//
// since this is just a one-liner you would not really wrap it in its own function
int randomNumber_OldSchoolVersion_ZeroToRandMax() { return rand(); }

// The problem with the function above is that you don't want numbers over [0, RAND_MAX], you want
// numbers from 0 to a MAX that you specify [0, MAX]
//
// note the clever use of the mod (modulus) operator, which (basically) gives you the remainder
// of division.  (any_rand_number % number_to_divide_by) returns numbers over [0,
// number_to_divide_by).  Notice that is a half-closed interval, so if you want results over [0,
// MAX] then you need to use MAX+1 as your number_to_divide_by, like so: (any_rand_number %
// (MAX+1)), which is what you see below
//
// Since we are using the % (mod) operator we need to be aware that we are dividing, remember this
// very important C++ rule: "If the second operand of / or % is zero then the behavior is
// undefined", that means if we do this: randomNumber_OldSchoolVersion_ZeroTo(-1) then we will end
// up dividing by zero and all hell breaks lose, remember this whenever working with code that {
// rand() % X } because you have to be sure X is not zero.  See the SafeRandom() exercise below.
//
// you will see this everywhere in real code
//
// since this is just a one-liner you would not really wrap it in its own function
int randomNumber_OldSchoolVersion_ZeroTo(const int MAX) { return (rand() % (MAX + 1)); }

// here is a handy version that lets you pick both [MIN, MAX]
//
// you will see this everywhere in real code
//
// since this is just a one-liner you would not really wrap it in its own function
int randomNumber_OldSchoolVersion_MinToMax(const int MIN, const int MAX)
{
    return (MIN + (rand() % ((MAX - MIN) + 1)));
}

// Exercise
//  Implement a better version of the function above that never results in undefined behavior, and
//  always returns a number that makes sense.
//
// Here is a specification to help you get started:
//  - If MIN==MAX return MIN
//  - If MIN>MAX return SafeRandom(MAX, MIN)
//  - Either or both MIN/MAX can be negative
//  - Never results in undefined behavior
//  - Does not throw exceptions
//
int randomNumber_OldSchoolVersion_MinToMax_Safe(const int MIN, const int MAX)
{
    if (MIN == MAX)
    {
        return MIN;
    }
    else if (MIN > MAX)
    {
        return randomNumber_OldSchoolVersion_MinToMax_Safe(MAX, MIN);
    }
    else
    {
        return (MIN + (rand() % ((MAX - MIN) + 1)));
    }
}

// Okay, no more Old School.  But to understand what everyone is writing in C++ today you need to
// understand the problem with true randomness, and distributions.

// True Randomness
//  A true random number generator (with infinite Entropy) is not always what you want.  There are
//  two basic categories of non-truly-random behavior that you want:
//
//  Run Prevention
//      Truly random numbers in the interval [1, 3], could result in many repeats:
//      (1, 1, 1, 1, 1, 1, 1, 1, 1, 1)
//
//      If you can't tolerate that in your app, then generate random numbers ahead of time in
//      batches, and then loop over the whole batch removing the duplicates.  (or removing whatever
//      you can't tolerate) A common example is shuffling a song playlist.  One clever way to solve
//      that kind of problem is to simply randomize the order.
//
// Non-Uniformity
//
//  True Randomness means that every number that can possibly be generated has the same chance of
//  being generated, but this is not always what you want.  The way to define non-truly-random
//  behavior is with a probability distribution.  Here are the most common examples:
//
//      Uniform Distribution    - Every number that can possibly be generated has an equal chance of
//                                being generated.
//
//      Normal Distribution     - Values in the center of the interval are more likely, creating a
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

// Review:
//  * A PRNG algorithm takes a seed and generates a pseudo-random number.
//  * Every time the PRNG is executed, the seed is replaced by the number it just generated.
//  * If the seed is the same, then a PRNG generates the same numbers in the same order -every time.
//  * PRNGs are not random at all (zero Entropy), and their quality varies.
//  * Low Entropy seeds result in patterns that clever code (and even some humans) can predict.
//  * Modern OSs use a combination of high Entropy sources to give you high Entropy seeds.
//  * True Randomness can create runs or patterns that you usually can not tolerate in your app.
//  * When you want somthing other than true-randomness, you describe it with a distribution.

// So what does it all mean?

// What you need to generate random numbers:
//  - Seed          (a small amount of data with as high Entropy as possible)
//  - PRNG          (zero Entropy algorithm that changes the seed, complex but still deterministic)
//  - Distribution  (a description of the chance each possible generated number will occur)

// For general apps that only need randomness that fools the casual observer
//
//  Seed    - Time is a good enough seed for things like randomized chart colors, etc.
//
//  PRNG    - Just use the Mersenne Twister.  It's already built into almost every library, and even
//            if that is not true for your enviornment, open-source implementations in almost every
//            language can be found on the first page of google search results.  The quality is
//            better than you will need, and it is more than fast enough.  The memory footprint is
//            larger than most other algorithms, but is still only 2.5k, which is really nothing.

// For games, playlist shufflers, or other apps where predictability or patterns is embarrasing
//
//  Seed    - The helper functions provided by most modern operating systems are good enough.
//
//  PRNG    - The Mersenne Twister will be good enough, or at least the easiest and best place to
//            start.  If you want something better (less predictible, faster, uses less memory, etc)
//            there are options out there if you look for them.

// For apps where avoiding predictibility or patterns is mission critical (encryption, etc.)
//
//  Seed    - The highest Entropy you can get.  The helper functions provided by most modern
//            operating systems are a good enough starting point.  Hardware features take forever
//            but if you have the time, take the time.
//
//  PRNG    - Research an open-source, high-quality, proven, PRNG algorithm.  Open-source is
//            critical because you want the security community to have scrutinized it.  Proven means
//            that major players are using it, and they won't do that without testing it ruthlessly.
//            No, the Mersenne Twister is NOT good enough.

// A good random number library will separate the PRNG algorithms from the distributions, and let
// you mix and match to get the quality/speed tradeoffs that are right for your app.  If you are
// lucky, it will also provide one function that gives you the best your OS can offer in terms of a
// small amount of high Entropy data you can use as a Seed.

// Okay, so now you know WHAT you need.  How do you get it quickly and easily in C++?  The C++11
// Random Number Library.  It has everything you need for non-mission-critical random numbers, yes,
// even games.  Here is are examples of how to use it.

// here are the includes we will need
#include <algorithm>
#include <array>
#include <limits>
#include <random>

// This library calls PRNG Algorithms "Engines".  It has about a dozen of them, but the Mersenne
// Twister 19937 is generally regarded as being the best, and it is the default.  ("19937" is a
// specific variant of the Mersenne Twister that is highly regarded)
//
// This function takes one of these "std::mt19937" engines and seeds it properly.  If you use a
// std::mt19937 engine (or any other engine) without seeding it, the seed will be some defaut value,
// and the engine will generate the same numbers in the same order every time
//
// the other seed functions you have seen only need to be called once and so they are found at the
// top of main(), but notice that in this case we are seeding an instance of an engine class, so
// this seeding function will need to be run once for every engine we instantiate
void seedRandomNumberGenerator_TutorialVersion(std::mt19937 & engine)
{
    // first, we need to make a std container to hold the seed data
    //
    // all the engines in the <random> library use some number of ints for the seed, every engine
    // has a different size of seed, so we need to know what the seed size is for our engine which
    // is the std::mt19937, every engine in the <random> library has the same std::size_t member
    // variable that holds this size called "state_size", it holds the number of ints in the seed
    // NOT the number of bytes
    //
    // so we need a std container to hold our ints, this is one of those rare situations where we
    // need a container of a known size that will never change, so we can use the fastest std STL
    // container there is: std::array
    std::array<int, engine.state_size> seedArray;

    // second, we need to fill our seedArray with the highest Entropy data that we can get
    //
    // That means we need to find out what OS we are running on, and then write code that will only
    // work on that OS, including it's gigantic set of libraries, then research to find what random
    // helper functions it has and how they work...but the <random> library has down this for us!
    // All of that is wrapped it up in a cross-platform easy-to-use helper class called
    // std::random_device.
    std::random_device randomDevice;

    // All std::random_device does is give us the highest Entropy ints we can get, one at a
    // time, so std::random_device is a functor that implements the function call operator(), so you
    // use it like a function to get one int at a time:
    for (auto & seedInt : seedArray)
    {
        seedInt = randomDevice();
    }

    // so now we are ready to give our engine its seed, but wait...

    // you might be wondering why bother with the Mersenne Twister or any other PRNG engines if
    // std::random_device gives us random numbers with significantly higher Entropy, the answer is
    // because std::random_device only has access to a tiny amount of data, remember that
    // std::random_device is getting numbers from time consuming hardware features or keystroke
    // timings (etc...), these sources of random numbers are very high Entropy but also very small,
    // another way to put it is that std::random_device has a tiny state_space, it is only meant to
    // provide a small amount of data to seed an engine
    //
    // in fact, with an engine like std::mt19937 which has a state_size of 624 ints you can bet that
    // std::random_device will not have enough data to fill the seed and will end up repeating
    // itself several times over before the seed is fully populated
    //
    // this is a fundamental problem that is important to understand:  on the one hand, engines
    // (PRNGs) are designed to need a large seed so that they can provide a large state space, but
    // on the other hand, the OS only has a tiny amount of high Entropy data for us to make the seed
    // with!
    //
    // so we have just populated a std container with high Entropy seed data, but we have every
    // reason to expect that it is full of repeated values, and that absolutely kills our seed's
    // Entropy, so right now our seed data is terrible, we need to fix that before we use it to seed
    // our engine
    //
    // this is a fundamental problem that programers have been struggling to deal with for a
    // long time, so the authors of the <random> library created (yet another) helper class to solve
    // the problem: std::seed_seq
    //
    // std::seed_seq takes imperfect seed data and mixes it up, spreading it out over the full range
    // of values, this is how you can take any data you want and turn it into as good a seed as it
    // can be (having the highest Entropy possible)
    //
    // now that we have std::seed_seq we can use anything as a source of seed data and we will get
    // the most Entropy bang for our buck, the best way to make a std::seed_seq is the way I show
    // here, but you could also just make one with the current system time -or any other way you
    // wanted.
    //
    // We will create our std::seed_seq with our seedArray:
    std::seed_seq seedSequence(seedArray.begin(), seedArray.end());

    // all the engines in the <random> library have seed() functions that take std::seed_seq, so
    // now, setting the seed is trivial (a lot of helper classes to get here though...)
    engine.seed(seedSequence);
}

// this next function is the same as the one above, only it doesn't have all the comments so you can
// see it more clearly
//
// I also replaced the loop with a std library function called std::generate_n(), it takes a
// destination iterator, a functor to generate values, and a std::size_t for how many times to call
// that function and put the value generated in the destination
void seedRandomNumberGenerator_RealVersion(std::mt19937 & engine)
{
    std::random_device randomDevice;

    std::array<int, engine.state_size> seedArray;
    std::generate_n(std::begin(seedArray), seedArray.size(), std::ref(randomDevice));

    std::seed_seq seedSequence(seedArray.begin(), seedArray.end());

    engine.seed(seedSequence);
}

// this function creates a random int over the interval [MIN, MAX], and shows how engines work with
// distributions to make random numbers over a specific range
//
// note that the MIN and MAX you provide to uniform_int_distribution defines a closed-interval,
// so this function could return either MIN or MAX
//
// NOTE that this function is NOT SAFE, providing MIN/MAX values that do not make sense results in
// undefined behavior!
int randomNumber_CPPRandomLibraryVersion_MT19937_Int(
    std::mt19937 & engine, const int MIN, const int MAX)
{
    std::uniform_int_distribution<int> uniformDistribution(MIN, MAX);
    return uniformDistribution(engine);
}

// here is another (fancy) version of the function above with the following handy modifications:
//  - if() statements added to prevent undefined behavior
//  - more if() statements added so that strange/incorrect input is handled gracefully
//  - added a template type allowing the use of any engine
//  - added a template type allowing the use of any fundamental arithmetic type (except for bool)
//  - std::uniform_int_distribution does not work with char or unsigned char so casts have been
//    added to get around that limitation and allow the function to work anyway
//  - std::uniform_real_distribution's interval is half-closed [MIN, MAX) so the std::nextafter()
//    function is used to close the interval so it works the same as the non-real types
template <typename Engine_t, typename Number_t>
Number_t randomNumber_CPPRandomLibraryVersion_Fancy(
    Engine_t & engine, const Number_t MIN, const Number_t MAX)
{
    // these lines will cause a compile error if Number_t is not a number
    static_assert(std::is_arithmetic<Number_t>::value);

    // std::is_arithmetic<bool>==true, so we also need this line
    static_assert(std::is_same<Number_t, bool>::value == false);

    if (MIN == MAX)
    {
        return MIN;
    }
    else if (MIN > MAX)
    {
        return randomNumber_CPPRandomLibraryVersion_Fancy(engine, MAX, MIN);
    }
    else
    {
        if constexpr (std::is_floating_point<Number_t>::value)
        {
            // this forces the use of the uniform_real_distribution if Number_t is real
            std::uniform_real_distribution<Number_t> uniformRealDistribution(
                MIN, std::nextafter(MAX, std::numeric_limits<Number_t>::max()));

            return uniformRealDistribution(engine);
        }
        else if constexpr (
            std::is_same<Number_t, char>::value || std::is_same<Number_t, unsigned char>::value)
        {
            // this is a work-around allowing chars and unsigned chars to work right even though
            // uniform_int_distribution does not support them
            return static_cast<Number_t>(randomNumber_CPPRandomLibraryVersion_Fancy(
                engine, static_cast<int>(MIN), static_cast<int>(MAX)));
        }
        else
        {
            // the if constexpr statements above combined with the static_asserts before them means
            // that at this point in the code Number_t must be one of the types supported by
            // std::uniform_int_distribution
            std::uniform_int_distribution<Number_t> uniformIntDistribution(MIN, MAX);
            return uniformIntDistribution(engine);
        }
    }
}

// you can use both seedRandomNumberGenerator_RealVersion() and
// randomNumber_CPPRandomLibraryVersion_Fancy() to get all the random numbers you need, but it would
// be better if we had a class that would wrap all of this functionality into one place and use the
// constructor to seed the engine so we don't have to remember to seed it ourselves
class Random
{
public:
    Random()
        : m_engine()
    {
        seed();
    }

    // these lines tell the compiler to make default versions of the: copy-constructor,
    // move-constructor, copy-assignment operator, and move-assignment operator, basically this
    // means that instances of our Random class have value semantics
    Random(const Random &) = default;
    Random(Random &&) = default;
    Random & operator=(const Random &) = default;
    Random & operator=(Random &&) = default;

    template <typename Number_t>
    static constexpr bool isValidNumberType()
    {
        if constexpr (std::is_arithmetic<Number_t>::value && !std::is_same<Number_t, bool>::value)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    // this is the same as randomNumber_CPPRandomLibraryVersion_Fancy() above
    template <typename Number_t>
    Number_t range(const Number_t MIN, const Number_t MAX) const
    {
        static_assert(
            isValidNumberType<Number_t>()
            && "Number_t must be both std::is_arithmetic and not bool");

        if (MIN == MAX)
        {
            return MIN;
        }
        else if (MIN > MAX)
        {
            return range(MAX, MIN);
        }
        else
        {
            if constexpr (std::is_floating_point<Number_t>::value)
            {
                std::uniform_real_distribution<Number_t> uniformRealDistribution(
                    MIN, std::nextafter(MAX, std::numeric_limits<Number_t>::max()));

                return uniformRealDistribution(m_engine);
            }
            else if constexpr (
                std::is_same<Number_t, char>::value || std::is_same<Number_t, unsigned char>::value)
            {
                return static_cast<Number_t>(range(static_cast<int>(MIN), static_cast<int>(MAX)));
            }
            else
            {
                std::uniform_int_distribution<Number_t> uniformIntDistribution(MIN, MAX);
                return uniformIntDistribution(m_engine);
            }
        }
    }

    // everything below this comment is just a few functions i thought would come in handy

    template <typename Number_t>
    Number_t zeroTo(const Number_t MAX) const
    {
        static_assert(
            isValidNumberType<Number_t>()
            && "Number_t must be both std::is_arithmetic and not bool");

        return range(Number_t(0), MAX);
    }

    template <typename Number_t>
    Number_t oneTo(const Number_t MAX) const
    {
        static_assert(
            isValidNumberType<Number_t>()
            && "Number_t must be both std::is_arithmetic and not bool");

        return range(Number_t(1), MAX);
    }

    bool boolean() const { return (zeroTo(1) == 0); }

    // TODO this does not work for real types yet...not sure why...yet...
    template <typename Number_t>
    Number_t range() const
    {
        static_assert(
            isValidNumberType<Number_t>()
            && "Number_t must be both std::is_arithmetic and not bool");

        return range(std::numeric_limits<Number_t>::min(), std::numeric_limits<Number_t>::max());
    }

private:
    void seed()
    {
        std::random_device randomDevice;

        std::array<int, this->m_engine.state_size> seedArray;
        std::generate_n(std::begin(seedArray), seedArray.size(), std::ref(randomDevice));

        std::seed_seq seedSequence(seedArray.begin(), seedArray.end());
        m_engine.seed(seedSequence);
    }

    // using the engine to generate random numbers changes the engine because the seed gets updated,
    // but we want this Random class to be const-able and to behave as if generating random numbers
    // does NOT change anything, so we cheat here and make the m_engine mutable which allows
    // functions that are const to modify it anyway.
    mutable std::mt19937 m_engine;
};

// Everything below this comment is code that tests everything above, so you can ignore it.
//
// WARNING - Attempting to understand the code below may cause:
//  * an existential crisis
//  * dizziness
//  * projectile vomiting
//  * severe head pain
//  * loss of bladder control
//  * suicidal thoughts
//  * rage
//  * hate crimes
//  * death
//  * war crimes (as defined by the Hague Conventions of 1899 and 1907)

void printTestStringBegin(const char * const FUNCTION_NAME)
{
    std::cout << "--- " << FUNCTION_NAME << "() Tests --- " << std::endl;
}

void printTestStringEnd(const char * const FUNCTION_NAME)
{
    printTestStringBegin(FUNCTION_NAME);
    std::cout << std::endl << std::endl;
}

#include <sstream>
#include <typeinfo>

void makeAStringFromVariadicArgs(std::ostringstream &) {}

template <typename NextArg_t, typename... Args>
void makeAStringFromVariadicArgs(
    std::ostringstream & ss, const NextArg_t & MAYBE_A_NUMBER, Args... args)
{
    if constexpr (Random::isValidNumberType<NextArg_t>())
    {
        if (ss.str().empty() == false)
        {
            ss << ", ";
        }

        if constexpr (
            std::is_same<NextArg_t, char>::value || std::is_same<NextArg_t, unsigned char>::value)
        {
            ss << static_cast<int>(MAYBE_A_NUMBER);
        }
        else
        {
            ss << MAYBE_A_NUMBER;
        }
    }

    makeAStringFromVariadicArgs(ss, args...);
}

template <typename Function_t, typename... Args>
void repeatTenTimes(Function_t foo, Args &&... args)
{
    std::ostringstream ss;
    makeAStringFromVariadicArgs(ss, args...);

    for (int i(0); i < 10; ++i)
    {
        std::cout << "(" << ss.str() << ")=";

        const auto REUSLT { foo(std::forward<Args>(args)...) };

        if constexpr (
            std::is_same<std::remove_cv<decltype(REUSLT)>::type, char>::value
            || std::is_same<std::remove_cv<decltype(REUSLT)>::type, unsigned char>::value)
        {
            std::cout << static_cast<int>(REUSLT);
        }
        else
        {
            std::cout << REUSLT;
        }
        std::cout << std::endl;
    }
}

template <typename Number_t>
const std::vector<Number_t> makeTestValuesForOneParam()
{
    std::vector<Number_t> vec;

    if constexpr (std::is_signed<Number_t>::value)
    {
        vec = { -5, -1, 0, 1, 5, 100 };
    }
    else
    {
        vec = { 0, 1, 5, 100 };
    }

    if constexpr (std::is_floating_point<Number_t>::value)
    {
        for (auto & number : vec)
        {
            if (Number_t(0.0) != number)
            {
                number += Number_t(0.5);
            }
        }
    }

    return vec;
}

template <typename Number_t>
const std::vector<Number_t> makeTestValuesForTwoParams()
{
    std::vector<Number_t> vec;

    if constexpr (std::is_signed<Number_t>::value)
    {
        vec = { -1, -1, 0, 0, 1, 1, -1, 0, 0, 1, 5, 6, 5, 7, 1, 100 };
    }
    else
    {
        vec = { 0, 0, 1, 1, 0, 1, 5, 6, 5, 7, 1, 100 };
    }

    if constexpr (std::is_floating_point<Number_t>::value)
    {
        for (auto & number : vec)
        {
            if (Number_t(0.0) != number)
            {
                number += Number_t(0.5);
            }
        }
    }

    return vec;
}

template <typename Number_t, typename Function_t>
void runTestsOneParam(const char * const FUNCTION_NAME, Function_t foo)
{
    printTestStringBegin(FUNCTION_NAME);

    const auto VALUES_TO_TEST { makeTestValuesForOneParam<Number_t>() };

    for (std::size_t numbersIndex(0); numbersIndex < VALUES_TO_TEST.size(); ++numbersIndex)
    {
        repeatTenTimes(foo, VALUES_TO_TEST.at(numbersIndex));
    }

    printTestStringEnd(FUNCTION_NAME);
}

template <typename Number_t, typename Function_t>
void runTestsTwoParams(const char * const FUNCTION_NAME, Function_t foo)
{
    printTestStringBegin(FUNCTION_NAME);

    const auto VALUES_TO_TEST { makeTestValuesForTwoParams<Number_t>() };

    for (std::size_t numbersIndex(0); numbersIndex < VALUES_TO_TEST.size(); numbersIndex += 2)
    {
        repeatTenTimes(foo, VALUES_TO_TEST.at(numbersIndex), VALUES_TO_TEST.at(numbersIndex + 1));
    }

    printTestStringEnd(FUNCTION_NAME);
}

template <typename Number_t, typename Engine_t, typename Function_t>
void runTestsTwoParamsAndEngine(const char * const FUNCTION_NAME, Engine_t & engine, Function_t foo)
{
    printTestStringBegin(FUNCTION_NAME);

    const auto VALUES_TO_TEST { makeTestValuesForTwoParams<Number_t>() };

    for (std::size_t numbersIndex(0); numbersIndex < VALUES_TO_TEST.size(); numbersIndex += 2)
    {
        repeatTenTimes(
            foo, engine, VALUES_TO_TEST.at(numbersIndex), VALUES_TO_TEST.at(numbersIndex + 1));
    }

    printTestStringEnd(FUNCTION_NAME);
}

void runOldSchoolTests()
{
    seedRandomNumberGenerator_OldSchoolVersion_TutorialVersion();
    // seedRandomNumberGenerator_OldSchoolVersion_ShortVersion();

    runTestsTwoParams<int>(
        "randomNumber_OldSchoolVersion_MinToMax_Safe", [](const auto MIN, const auto MAX) -> auto {
            return randomNumber_OldSchoolVersion_MinToMax_Safe(MIN, MAX);
        });
}

void runCPPRandomLibraryTests()
{
    std::mt19937 engine;

    seedRandomNumberGenerator_TutorialVersion(engine);
    // seedRandomNumberGenerator_RealVersion(engine);

    runTestsTwoParamsAndEngine<int>("randomNumber_CPPRandomLibraryVersion_Fancy", engine, [
    ](std::mt19937 & engine, const auto MIN, const auto MAX) -> auto {
        return randomNumber_CPPRandomLibraryVersion_Fancy(engine, MIN, MAX);
    });

    runTestsTwoParamsAndEngine<unsigned char>(
        "randomNumber_CPPRandomLibraryVersion_Fancy", engine, [
        ](std::mt19937 & engine, const auto MIN, const auto MAX) -> auto {
            return randomNumber_CPPRandomLibraryVersion_Fancy(engine, MIN, MAX);
        });

    runTestsTwoParamsAndEngine<long double>("randomNumber_CPPRandomLibraryVersion_Fancy", engine, [
    ](std::mt19937 & engine, const auto MIN, const auto MAX) -> auto {
        return randomNumber_CPPRandomLibraryVersion_Fancy(engine, MIN, MAX);
    });

    // compilation tests
    {
        // none of these should compile
        // std::cout << randomNumber_CPPRandomLibraryVersion_Fancy(engine, false, true) <<
        // std::endl;

        // std::cout << randomNumber_CPPRandomLibraryVersion_Fancy(engine, "abc", "def") <<
        // std::endl;

        // std::cout << randomNumber_CPPRandomLibraryVersion_Fancy(
        //                 engine, std::string("abc"), std::string("def"))
        //          << std::endl;
    }
}

void runRandomTests_Boolean(Random & randomHelper)
{
    printTestStringBegin(__FUNCTION__);

    for (int i(0); i < 10; ++i)
    {
        std::cout << std::boolalpha << randomHelper.boolean() << std::endl;
    }

    printTestStringEnd(__FUNCTION__);
}

template <typename Number_t, typename Printable_t = long long>
void runRandomTests_FullRangeOfType(Random & randomHelper)
{
    std::ostringstream ss;
    ss << "runRandomTests_FullRangeOfType<" << typeid(Number_t).name() << ">";

    printTestStringBegin(ss.str().c_str());

    for (int i(0); i < 10; ++i)
    {
        std::cout << typeid(Number_t).name() << "="
                  << static_cast<Printable_t>(randomHelper.range<Number_t>()) << std::endl;
    }

    printTestStringEnd(ss.str().c_str());
}

void runRandomHelperTests()
{
    Random randomHelper;

    runTestsTwoParams<int>("randomHelper.range", [&](const auto MIN, const auto MAX) -> auto {
        return randomHelper.range(MIN, MAX);
    });

    runTestsTwoParams<float>("randomHelper.range", [&](const auto MIN, const auto MAX) -> auto {
        return randomHelper.range(MIN, MAX);
    });

    runTestsOneParam<int>(
        "randomHelper.zeroTo", [&](const auto MAX) -> auto { return randomHelper.zeroTo(MAX); });

    runTestsOneParam<double>(
        "randomHelper.zeroTo", [&](const auto MAX) -> auto { return randomHelper.zeroTo(MAX); });

    runTestsOneParam<int>(
        "randomHelper.oneTo", [&](const auto MAX) -> auto { return randomHelper.oneTo(MAX); });

    runTestsOneParam<long double>(
        "randomHelper.oneTo", [&](const auto MAX) -> auto { return randomHelper.oneTo(MAX); });

    runRandomTests_Boolean(randomHelper);

    runRandomTests_FullRangeOfType<unsigned char>(randomHelper);
    runRandomTests_FullRangeOfType<char>(randomHelper);
    runRandomTests_FullRangeOfType<unsigned short>(randomHelper);
    runRandomTests_FullRangeOfType<short>(randomHelper);
    runRandomTests_FullRangeOfType<unsigned int>(randomHelper);
    runRandomTests_FullRangeOfType<int>(randomHelper);
    runRandomTests_FullRangeOfType<unsigned long long>(randomHelper);
    runRandomTests_FullRangeOfType<long long>(randomHelper);
    runRandomTests_FullRangeOfType<float>(randomHelper);
    runRandomTests_FullRangeOfType<double>(randomHelper);
    runRandomTests_FullRangeOfType<long double>(randomHelper);

    // compilation tests
    {
        // basically, any type that std::is_arithmetic() and !std::is_same<T, bool> should compile

        // using different types should NOT compile
        // std::cout << randomHelper.range(0, 1.1) << std::endl;

        // using bools should NOT compile
        // std::cout << randomHelper.range(false, true) << std::endl;

        // pointer types should NOT compile
        // std::cout << randomHelper.range(nullptr, nullptr) << std::endl;
        // int ignored(0);
        // std::cout << randomHelper.range(&ignored, &ignored) << std::endl;

        // std::cout << randomHelper.range("a", "b") << std::endl;
        // std::cout << randomHelper.range(std::string("a"), std::string("b")) << std::endl;

        // enum types should NOT compile
        // enum Enum1 : int
        //{
        //    A0 = 0,
        //    B1 = 1
        //};
        // std::cout << randomHelper.range(A0, B1) << std::endl;
        //
        // enum class Enum2 : int
        //{
        //    C0 = 0,
        //    D1 = 1
        //};
        // std::cout << randomHelper.range(Enum2::C0, Enum2::D1) << std::endl;

        // NOTE that NULL is a #define 0 so this SHOULD compile and call range<int>(0, 0)
        // this compiles but it is commented out so that no random garbage is cout
        // std::cout << randomHelper.range(NULL, NULL) << std::endl;

        // NOTE that char and unsigned char are a valid arithmetic types so this SHOULD compile
        // std::cout << randomHelper.range('a', 'b') << std::endl;
    }
}

namespace school
{

    void runRandomTests()
    {
        runOldSchoolTests();
        runCPPRandomLibraryTests();
        runRandomHelperTests();
    }

} // namespace school

#endif // SCHOOL_RANDOM_HPP_INCLUDED

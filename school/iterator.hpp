#ifndef ITERATOR_HPP_INCLUDED
#define ITERATOR_HPP_INCLUDED

namespace school
{
    void printVector(const std::vector<int> & VECTOR)
    {
        for (std::size_t index(0); index < VECTOR.size(); ++index)
        {
            std::cout << VECTOR[index];

            if (index < (VECTOR.size() - 1))
            {
                std::cout << ", ";
            }
        }
        std::cout << std::endl;
    }

    // Don't forget, standard containers have value semantics.
    // Copying is simple.
    void copyVector1(const std::vector<int> & SOURCE, std::vector<int> & dest) { dest = SOURCE; }

    // This copy is based on for loops and the indicie operator.
    // This is the oldschool C way.
    void copyVector2(const std::vector<int> & SOURCE, std::vector<int> & dest)
    {
        for (std::size_t index(0); index < SOURCE.size(); ++index)
        {
            dest[index] = SOURCE[index];
        }
    }

    // This version uses iterators but it only works for two vectors.
    // It doesn't work for other containers.
    // It isn't formatter in the way it needs to be.
    // It should take two iterators defining a range to work on and an iterator specifying the
    // destination.
    void copyVector3(const std::vector<int> & SOURCE, std::vector<int> & dest)
    {
        auto sourceIter(std::begin(SOURCE));
        auto destIter(std::begin(dest));
        while (sourceIter != std::end(SOURCE))
        {
            *(destIter++) = *(sourceIter++);
        }
    }

    // Finally, the perameters are right, but if we use iterators as an abstraction,
    // we can get rid of the vector type and the int type.
    // See below.
    void copyVector4(
        const std::vector<int>::iterator & SOURCE_BEGIN_ITER,
        const std::vector<int>::iterator & SOURCE_END_ITER,
        std::vector<int>::iterator & destBeginIter)
    {
        auto sourceIter(SOURCE_BEGIN_ITER);
        auto destIter(destBeginIter);
        while (sourceIter != SOURCE_END_ITER)
        {
            *(destIter++) = *(sourceIter++);
        }
    }

    // Now, using the iterator abstraction, we have a function that does only what it says on
    // anything.
    template <typename Iterator_t>
    void copy(
        const Iterator_t & SOURCE_BEGIN_ITER,
        const Iterator_t & SOURCE_END_ITER,
        Iterator_t & destBeginIter)
    {
        auto sourceIter(SOURCE_BEGIN_ITER);
        auto destIter(destBeginIter);
        while (sourceIter != SOURCE_END_ITER)
        {
            *(destIter++) = *(sourceIter++);
        }
    }

    template <typename Iterator_t, typename Functor_t>
    void doSomethingToEveryElement(
        const Iterator_t & SOURCE_BEGIN_ITER,
        const Iterator_t & SOURCE_END_ITER,
        Functor_t & doSomethingFunctor)
    {
        auto sourceIter(SOURCE_BEGIN_ITER);
        auto destIter(destBeginIter);
        while (sourceIter != SOURCE_END_ITER)
        {
            doSomethingFunctor(*(sourceIter++));
        }
    }

    void runIteratorTests()
    {
        // Iterator
        // An iterator is a container position that:
        // - Knows what type it refers to
        // - Can be moved (forward/backward)
        // - Can be dereferenced, to get a reference, to the element referred to.
        // - Can be compared with other iterators.
        // -
        //
        // Uninitialized iterators cannot be used.
        // Iterators come from containers (both the type and the value).

        // Iterator categories
        //	Input			- Read only, move forward only (really only for istream)
        //	Output			- Write only, move forward only
        //	Forward			- Read/Write, move forward only
        //	Bidirectional	- Read/Write, forward/backward
        //	Random Access	- Read/Write, forward/backward (move more than one step at a time)

        // std::begin() returns an iterator to the first element in the container.
        // std::end() returns an iterator REPRESENTING to the element PAST the last element in the
        // container.
        //  - Whenever moving ONE step past the last element it becomes the "end" iterator
        //	- The end iterator cannot be used. (you can use it to compare, but that's it)

        // If iterator mistakes are just as bad as pointer mistakes, what the fuck good are
        // iterators?
        //
        // (1) Actually, to make an iterator safe,
        // you have to fill it with if statements that check for error conditions.
        // All of those if checks are already in there, but macros are turning them off.
        //
        // (2) The abstraction makes code simple and powerful (see copy above).

        // use the const keyword to prevent an iterator from moving
        // use the const_ name to prevent an iterator from changing what element it refers to

        // Iterators cannot change the size of their container.  (no instert/ no erase)
        // Why not? Changing a container can easily invalidate the iterator, and it would duplicate
        // tons of code, and it would make iterators huge and slow.

        std::vector<int> numbers = { 0, 1, 2, 3 };
        printVector(numbers);

        // The type and value come from the container.
        std::vector<int>::iterator iter(numbers.begin());
        std::cout << *iter << std::endl;
        ++iter;
        std::cout << *iter << std::endl;
        *iter = 69;
        printVector(numbers);
        std::cout << std::boolalpha << (numbers.begin() == iter) << std::endl;
        iter = numbers.begin();
        std::cout << std::boolalpha << (numbers.begin() == iter) << std::endl;

        std::vector<int> sameNumbers = { 0, 1, 2, 3 };
        std::cout << std::boolalpha << (sameNumbers.begin() == iter) << std::endl;
        std::cout << std::boolalpha << (*sameNumbers.begin() == *iter) << std::endl;

        iter += 2;
        std::cout << *iter << std::endl;
        iter = std::begin(numbers);
        std::advance(iter, 3);
        std::cout << *iter << std::endl;

        // std::begin() returns an iterator to the first element in the container.
        // std::end() returns an iterator REPRESENTING to the element PAST the last element in the
        // container.
        //  - Whenever moving ONE step past the last element it becomes the "end" iterator
        //	- The end iterator cannot be used. (you can use it to compare, but that's it)

        std::cout << std::boolalpha << ((std::begin(numbers) + numbers.size()) == std::end(numbers))
                  << std::endl;

        // Intervals
        //	[0, 3] = 0-3 - Fully closed
        //  (0, 3] = 1-3 - half closed
        //  (0, 3) = 1-2 - not closed
        //  [0, 3)       - half closed interval - REALLY HANDY

        // What happens if you make a mistake with an iterator?
        //	--std::end(...) = Undefined behavior
        //  --std::begin(...) = Also undefined behavior
        // *std::end(...) same
        // *std::begin(empty container) = gives end iterator, does nothing
        // If iterator mistakes are just as bad as pointer mistakes, what the fuck good are
        // iterators?
        //
        // (1) Actually, to make an iterator safe,
        // you have to fill it with if statements that check for error conditions.
        // All of those if checks are already in there, but macros are turning them off.
        //
        // (2) The abstraction makes code simple and powerful (see copy above).
        std::vector<int> backwardNumbers = { 3, 2, 1, 0 };
        copyVector1(backwardNumbers, numbers);
        printVector(numbers);

        copyVector2(sameNumbers, numbers);
        printVector(numbers);

        copyVector3(backwardNumbers, numbers);
        printVector(numbers);

        copyVector4(std::begin(sameNumbers), std::end(sameNumbers), std::begin(numbers));
        printVector(numbers);

        copy(std::begin(backwardNumbers), std::end(backwardNumbers), std::begin(numbers));
        printVector(numbers);

        std::copy(std::rbegin(sameNumbers), std::rend(sameNumbers), std::begin(numbers));
        printVector(numbers);

        // this is evil (undefined behavior)
        // When you call std::copy() you have to be CERTAIN the destination is big enough.
        // std::vector<int> testNumbers = { 123, 456 };
        // std::copy(std::begin(numbers), std::end(numbers), std::begin(testNumbers));
        // printVector(testNumbers);

        // Iterators can vbe invalidated
        // After changing the numbers vector myNumbersIterAtBegin iterator could now be invalidated.
        // auto myNumbersIterAtBegin(std::begin(numbers));
        // for (int index(0); index < 9999; ++index)
        //{
        //	numbers.push_back(123);
        //}
    }

} // namespace school

#endif ITERATOR_HPP_INCLUDED

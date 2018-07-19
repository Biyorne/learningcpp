#include <cstdlib>
#include <iostream>
#include <string>

#include "test.hpp"
#include "util.hpp"

template <typename T>
class ArrayWrapper
{
public:
    // default constructor
    ArrayWrapper() = delete;

    // custom constructor
    explicit ArrayWrapper(const std::size_t SIZE)
        : m_arrayPtr(new T[SIZE])
        , m_size(SIZE)
    {}

    // copy constructor
    explicit ArrayWrapper(const ArrayWrapper & AW)
        : m_arrayPtr(new T[AW.size()])
        , m_size(AW.size())
    {
        for (std::size_t i(0); i < m_size; ++i)
        {
            m_arrayPtr[i] = AW[i];
        }
    }

    // destructor
    ~ArrayWrapper() { delete[] m_arrayPtr; }

    T & operator[](const std::size_t INDEX) { return m_arrayPtr[INDEX]; }

    const T & operator[](const std::size_t INDEX) const { return m_arrayPtr[INDEX]; }

    std::size_t size() const { return m_size; }

private:
    T * m_arrayPtr;
    std::size_t m_size;
};

int main(void)
{

    const std::size_t ARRAY_SIZE(4);

    ArrayWrapper<int> wArray1(ARRAY_SIZE);
    for (std::size_t i(0); i < ARRAY_SIZE; ++i)
        wArray1[i] = static_cast<int>(i + 1); //{ 1, 2, 3, 4 }

    ArrayWrapper<int> wArray2(ARRAY_SIZE);
    for (std::size_t i(0); i < ARRAY_SIZE; ++i)
        wArray2[i] = static_cast<int>((i + 1) * 10); //{ 10, 20, 30, 40}

    // test(wArray1.size() == ARRAY_SIZE);

    ArrayWrapper<int> wArray2Copy(wArray2);
    // test(wArray2[2] == wArray2Copy[2]);

    // test(wArray1 != wArray2);
    // wArray1 = wArray2;
    // test(wArray1 == wArray2);

    int intArray[ARRAY_SIZE];
    util::fillCounting(intArray, ARRAY_SIZE);
    util::print(intArray, ARRAY_SIZE);

    int intArray2[ARRAY_SIZE];
    util::fillCounting(intArray2, ARRAY_SIZE);
    util::print(intArray2, ARRAY_SIZE);

    test::printResult(test::isEqual(intArray, ARRAY_SIZE, intArray2, ARRAY_SIZE));

    int intArray3[ARRAY_SIZE] = { 0, 1, 2, 3333 };
    util::print(intArray3, ARRAY_SIZE);
    test::printResult(test::isEqual(intArray, ARRAY_SIZE, intArray3, ARRAY_SIZE) == false);
    return EXIT_SUCCESS;
}

/*
short=[byte,byte] [00000000 00000000]
std::size_t=[byte,byte,byte,byte] [11111111111111...1] (32,000,000,000)

                                            intArray[0]             intArray[1]
pointer to testArraySum() Stack Of Main[byte,byte,byte,byte,byte,byte,(byte,byte,byte,byte) ,
(byte,byte,byte,byte), byte,byte,byte..., byte,byte,byte,byte]

                sumArray()                 a1
Heap[byte,byte,(byte,byte,byte,(byte, ... (byte,byte,byte,byte) , (byte,byte,byte,byte),
byte,byte,byte]
*/

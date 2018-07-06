#include <cstdlib>
#include <iostream>
#include <string>

template <typename T, typename U>
T sumBoth(const T A, const U B)
{
	return A + B;
}

template <typename T>
T sumArray(const T ARRAY[], const std::size_t SIZE)
{
	T sum(0);

	for (std::size_t i(0); i < SIZE; ++i)
	{
		sum += ARRAY[i];
	}

	return sum;
}


template<typename T>
void testArray(const std::string & NAME, const T ARRAY[], const std::size_t SIZE, const T SUM_EXPECTED)
{
	const T SUM_ACTUAL(sumArray(ARRAY, SIZE));
	std::cout << NAME << " sum = " << SUM_ACTUAL;

	if (SUM_ACTUAL == SUM_EXPECTED)
	{
		std::cout << " PASS" << std::endl;
	}
	else
	{
		std::cout << " FAIL (" << "expected " << SUM_EXPECTED << ")" << std::endl;
	}
}

void test(const bool B)
{
	if (B)
	{
		std::cout << "PASS" << std::endl;
	}
	else
	{
		std::cout << "FAIL" << std::endl;
	}
}

/*
template <typename T>
T sumArray(const ArrayWrapper<T> & ARRAY)
{
	T sum(0);

	for (std::size_t i(0); i < ARRAY.size(); ++i)
	{
		sum += ARRAY[i];
	}

	return sum;
}
*/

template <typename T>
class ArrayWrapper
{
public:
	//default constructor
	ArrayWrapper() = delete;

	//custom constructor
	ArrayWrapper(const std::size_t SIZE)
		:m_arrayPtr(new T[SIZE]),
		m_size(SIZE)
	{}

	//copy constructor
	explicit ArrayWrapper(const ArrayWrapper & AW)
		:m_arrayPtr(new T[AW.size()])
		, m_size(AW.size())
	{
		for (std::size_t i(0); i < m_size; ++i)
		{
			m_arrayPtr[i] = AW[i];
		}
	}

	//destructor
	~ArrayWrapper()
	{
		delete[] m_arrayPtr;
	}

	T & operator[](const std::size_t INDEX) { return m_arrayPtr[INDEX]; }

	const T & operator[](const std::size_t INDEX) const { return m_arrayPtr[INDEX]; }

	std::size_t size() const
	{
		return m_size;
	}

private:
	T * m_arrayPtr;
	std::size_t m_size;
};


template <typename T>
T giveMeWhatIsAtIndex(const T ARRAY[], const std::size_t INDEX)
{
	return ARRAY[INDEX];
}

int main(void)
{
	const std::size_t  ARRAY_SIZE(4);

	ArrayWrapper<int> wArray1(ARRAY_SIZE);
	for (std::size_t i(0); i < ARRAY_SIZE; ++i) wArray1[i] = static_cast<int>(i + 1); //{ 1, 2, 3, 4 }

	ArrayWrapper<int> wArray2(ARRAY_SIZE);
	for (std::size_t i(0); i < ARRAY_SIZE; ++i) wArray2[i] = static_cast<int>((i + 1) * 10);//{ 10, 20, 30, 40}
	
	test(wArray1.size() == ARRAY_SIZE);

	ArrayWrapper<int> wArray2Copy(wArray2);
	test(wArray2[2] == wArray2Copy[2]);
	
	//test(wArray1 != wArray2);
	//wArray1 = wArray2;
	//test(wArray1 == wArray2);

	//value semantics
	short a(10);
	float b(0.9f);
	short c(a);
	c = a;

	std::cout << "The sum of a and b is " << sumBoth(a, b) << "." << std::endl;

	//pointer semantics
	short array1[ARRAY_SIZE] = { 0, 1, 2, 3};
	testArray("array1", array1, ARRAY_SIZE, static_cast<short>(6));
	
	return EXIT_SUCCESS;
}



/*
short=[byte,byte] [00000000 00000000]
std::size_t=[byte,byte,byte,byte] [11111111111111...1] (32,000,000,000)

                                            intArray[0]             intArray[1]							      pointer to testArray()
Stack Of Main[byte,byte,byte,byte,byte,byte,(byte,byte,byte,byte) , (byte,byte,byte,byte), byte,byte,byte..., byte,byte,byte,byte]

                sumArray()                 a1
Heap[byte,byte,(byte,byte,byte,(byte, ... (byte,byte,byte,byte) , (byte,byte,byte,byte), byte,byte,byte]
*/

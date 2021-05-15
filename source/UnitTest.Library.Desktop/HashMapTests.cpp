#include "pch.h"
#include <crtdbg.h>
#include <CppUnitTest.h>
#include <exception>
#include <stdexcept>
#include <functional>
#include "Foo.h"
#include "Vector.h"
#include "HashMap.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<Vector<Foo>>(const Vector<Foo>& t)
	{
		RETURN_WIDE_STRING(t.Size());
	}

	template<>
	inline std::wstring ToString<Foo>(const Foo& t)
	{
		RETURN_WIDE_STRING(t.Data());
	}

	template<>
	inline std::wstring ToString<Vector<Foo>::Iterator>(const Vector<Foo>::Iterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (...)
		{
			return L"end()";
		}
	}

	template<>
	inline std::wstring ToString<Vector<Foo>::ConstIterator>(const Vector<Foo>::ConstIterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (...)
		{
			return L"end()";
		}
	}
}

namespace FieaGameEngine
{
	template<>
	struct DefaultHash<Foo>
	{
		inline size_t operator()(const Foo& key) const
		{
			return key.Data();
		}
	};

	template<>
	struct DefaultHash<const Foo>
	{
		inline size_t operator()(const Foo& key) const
		{
			return key.Data();
		}
	};
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(HashMapTests)
	{
	public:
		//	Runs before every Test_Method
		TEST_METHOD_INITIALIZE(Initialize)
		{
#ifdef _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
		}

		//	Runs after every Test_Method
		TEST_METHOD_CLEANUP(Cleanup)
		{
#ifdef _DEBUG
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &_startMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

#pragma region DefaultHash and Default Equality Functor Testing
		TEST_METHOD(TestEqualityFunctor)
		{
			DefaultEquality<int> eqTestInt;
			DefaultEquality<float> eqTestFloat;
			DefaultEquality<char> eqTestChar;
			DefaultEquality<Foo> eqTestFoo;

			//	Equality Int Testing
			{
				int a = 10;
				int b = 20;
				int c = 10;

				bool result;

				result = eqTestInt(10, 10);
				Assert::AreEqual(true, result);
				result = eqTestInt(a, 10);
				Assert::AreEqual(true, result);
				result = eqTestInt(c, 10);
				Assert::AreEqual(true, result);
				result = eqTestInt(a, c);
				Assert::AreEqual(true, result);

				result = eqTestInt(10, 20);
				Assert::AreEqual(false, result);
				result = eqTestInt(b, 10);
				Assert::AreEqual(false, result);
				result = eqTestInt(c, b);
				Assert::AreEqual(false, result);
				result = eqTestInt(a, b);
				Assert::AreEqual(false, result);
			}

			//	Equality float Testing
			{
				float a = 10.0f;
				float b = 20.0f;
				float c = 10.0f;

				bool result;

				result = eqTestFloat(10.0f, 10.0f);
				Assert::AreEqual(true, result);
				result = eqTestFloat(a, 10.0f);
				Assert::AreEqual(true, result);
				result = eqTestFloat(c, 10.0f);
				Assert::AreEqual(true, result);
				result = eqTestFloat(a, c);
				Assert::AreEqual(true, result);

				result = eqTestFloat(10.0f, 20.0f);
				Assert::AreEqual(false, result);
				result = eqTestFloat(b, 10.0f);
				Assert::AreEqual(false, result);
				result = eqTestFloat(c, b);
				Assert::AreEqual(false, result);
				result = eqTestFloat(a, b);
				Assert::AreEqual(false, result);
			}

			//	Equality Char Testing
			{
				char a = 10;
				char b = 20;
				char c = 10;

				bool result;

				result = eqTestChar(10, 10);
				Assert::AreEqual(true, result);
				result = eqTestChar(a, 10);
				Assert::AreEqual(true, result);
				result = eqTestChar(c, 10);
				Assert::AreEqual(true, result);
				result = eqTestChar(a, c);
				Assert::AreEqual(true, result);

				result = eqTestChar(10, 20);
				Assert::AreEqual(false, result);
				result = eqTestChar(b, 10);
				Assert::AreEqual(false, result);
				result = eqTestChar(c, b);
				Assert::AreEqual(false, result);
				result = eqTestChar(a, b);
				Assert::AreEqual(false, result);
			}

			//	Equality Foo Testing
			{
				Foo a(10);
				Foo b(20);
				Foo c(10);

				bool result;

				result = eqTestFoo(Foo(10), Foo(10));
				Assert::AreEqual(true, result);
				result = eqTestFoo(a, Foo(10));
				Assert::AreEqual(true, result);
				result = eqTestFoo(c, Foo(10));
				Assert::AreEqual(true, result);
				result = eqTestFoo(a, c);
				Assert::AreEqual(true, result);

				result = eqTestFoo(Foo(10), Foo(20));
				Assert::AreEqual(false, result);
				result = eqTestFoo(b, Foo(10));
				Assert::AreEqual(false, result);
				result = eqTestFoo(c, b);
				Assert::AreEqual(false, result);
				result = eqTestFoo(a, b);
				Assert::AreEqual(false, result);
			}
		}

		TEST_METHOD(TestDefaultIntHash)
		{
			DefaultHash<int> hfTestInt;

			size_t resultA = 0;
			size_t resultB = 0;
			size_t resultC = 0;

			//	Int Hash Test
			{
				int a = 10;
				int b = 20;
				int c = 10;

				resultA = hfTestInt(a);
				resultB = hfTestInt(b);
				resultC = hfTestInt(c);

				Assert::AreNotEqual(resultA, resultB);
				Assert::AreNotEqual(resultB, resultC);
				Assert::AreEqual(resultA, resultC);
			}
		}

		TEST_METHOD(TestDefaultCharHash)
		{
			DefaultHash<char> hfTestChar;

			size_t resultA = 0;
			size_t resultB = 0;
			size_t resultC = 0;

			//	Char Hash Test
			{
				char a = 10;
				char b = 20;
				char c = 10;

				resultA = hfTestChar(a);
				resultB = hfTestChar(b);
				resultC = hfTestChar(c);

				Assert::AreNotEqual(resultA, resultB);
				Assert::AreNotEqual(resultB, resultC);
				Assert::AreEqual(resultA, resultC);
			}
		}

		TEST_METHOD(TestDefaultFloatHash)
		{
			DefaultHash<float> hfTestFloat;

			size_t resultA = 0;
			size_t resultB = 0;
			size_t resultC = 0;

			//	Float Hash Test
			{
				float a = 10.0f;
				float b = 20.0f;
				float c = 10.0f;

				resultA = hfTestFloat(a);
				resultB = hfTestFloat(b);
				resultC = hfTestFloat(c);

				Assert::AreNotEqual(resultA, resultB);
				Assert::AreNotEqual(resultB, resultC);
				Assert::AreEqual(resultA, resultC);
			}
		}

		TEST_METHOD(TestDefaultStringHash)
		{
			DefaultHash<std::string> hfTestString;

			size_t resultA = 0;
			size_t resultB = 0;
			size_t resultC = 0;

			//	String Hash Test
			{
				string a = "Hello";
				string b = "Not Hello";
				string c = "Hello";

				resultA = hfTestString(a);
				resultB = hfTestString(b);
				resultC = hfTestString(c);

				Assert::AreNotEqual(resultA, resultB);
				Assert::AreNotEqual(resultB, resultC);
				Assert::AreEqual(resultA, resultC);
			}
		}
#pragma endregion

#pragma region HashMap Method Testing
		TEST_METHOD(TestConstructor)
		{
			HashMap<int, Foo> testZero(0);
			HashMap<int, Foo> testHundred(100);
			HashMap<int, Foo> testDefault;

			Assert::AreEqual(0_z, testZero.Size());
			Assert::AreEqual(true, testZero.IsEmpty());

			Assert::AreEqual(0_z, testHundred.Size());
			Assert::AreEqual(true, testHundred.IsEmpty());

			Assert::AreEqual(0_z, testDefault.Size());
			Assert::AreEqual(true, testDefault.IsEmpty());
		}

		TEST_METHOD(TestInsert)
		{
			HashMap<int, float> testMap;
			HashMap<int, float>::PairType entry = make_pair(10, 20.0f);
			HashMap<int, float>::PairType entryOverwrite = make_pair(10, 30.0f);

			//	L-Value Reference
			auto [insIt, tfVal] = testMap.Insert(entry);
			Assert::IsTrue(tfVal);
			//	Testing Initial insert
			Assert::AreEqual((testMap.Find(10))->second, 20.0f);

			//	R-Value Reference
			testMap.Insert(HashMap<int, float>::PairType(20, 30.0f));
			//	Testing initial insert
			Assert::AreEqual((testMap.Find(20))->second, 30.0f);

			//	Attempting Overwriting both keys
			auto [owIt, owVal] = testMap.Insert(entryOverwrite);
			testMap.Insert(HashMap<int, float>::PairType(20, 40.0f));

			//	The original Key values should not be, size should still be 2
			Assert::IsFalse(owVal);
			Assert::AreEqual((testMap.Find(10))->second, 20.0f);
			Assert::AreEqual((testMap.Find(20))->second, 30.0f);
			Assert::AreEqual(2_z, testMap.Size());
			Assert::AreEqual(false, testMap.IsEmpty());
			Assert::AreEqual(true, testMap.ContainsKey(10));
		}

		TEST_METHOD(TestCopySemantics)
		{
			HashMap<int, float> initMap;
			HashMap<int, float> copyAMap;

			for (int i = 0; i < 10; ++i)
			{
				float result = 10.0f * i;
				initMap.Insert(HashMap<int, float>::PairType(i, result));
			}

			HashMap<int, float> copyCMap = initMap;
			copyAMap = initMap;

			for (int i = 0; i < 10; ++i)
			{
				float resultA = initMap.At(i);
				float resultB = copyCMap.At(i);
				float resultC = copyAMap.At(i);
				
				Assert::AreEqual(resultA, resultB);
				Assert::AreEqual(resultB, resultC);
			}
			Assert::AreEqual(initMap.Size(), copyCMap.Size());
			Assert::AreEqual(copyAMap.Size(), copyCMap.Size());
		}

		TEST_METHOD(TestMoveSemantics)
		{
			HashMap<int, float> initMap;
			HashMap<int, float> moveAMap;
			float results[10];

			for (int i = 0; i < 10; ++i)
			{
				float result = 10.0f * i;
				initMap.Insert(HashMap<int, float>::PairType(i, result));
			}

			for (int i = 0; i < 10; ++i)
			{
				results[i] = initMap.At(i);
			}

			HashMap<int, float> moveCMap = std::move(initMap);

			for (int i = 0; i < 10; ++i)
			{
				Assert::AreEqual(results[i], moveCMap.At(i));
			}

			Assert::AreEqual(0_z, initMap.Size());
			Assert::AreEqual(true, initMap.IsEmpty());
			Assert::AreEqual(10_z, moveCMap.Size());
			Assert::AreEqual(false, moveCMap.IsEmpty());

			Assert::AreEqual(0_z, moveAMap.Size());
			Assert::AreEqual(true, moveAMap.IsEmpty());

			moveAMap = std::move(moveCMap);

			Assert::AreEqual(10_z, moveAMap.Size());
			Assert::AreEqual(false, moveAMap.IsEmpty());
			
			for (int i = 0; i < 10; ++i)
			{
				Assert::AreEqual(results[i], moveAMap.At(i));
			}

			Assert::AreEqual(0_z, moveCMap.Size());
			Assert::AreEqual(true, moveCMap.IsEmpty());
		}

		TEST_METHOD(TestDeletionMethods)
		{
			HashMap<int, float> initMap;

			for (int i = 0; i < 10; ++i)
			{
				float result = 10.0f * i;
				initMap.Insert(HashMap<int, float>::PairType(i, result));
			}

			// Should not find the value
			Assert::AreEqual(false, initMap.Remove(20));

			HashMap<int, float>::Iterator fakeIterator;
			Assert::ExpectException<std::runtime_error>([&fakeIterator, &initMap] { initMap.Remove(fakeIterator); });
			
			Assert::AreEqual(10_z, initMap.Size());
			Assert::AreEqual(true, initMap.ContainsKey(5));

			initMap.Remove(5);

			Assert::AreEqual(9_z, initMap.Size());
			bool result = initMap.ContainsKey(5);
			Assert::AreEqual(false, result);

			initMap.Clear();

			Assert::AreEqual(0_z, initMap.Size());
			Assert::AreEqual(true, initMap.IsEmpty());
		}

		TEST_METHOD(TestConstIndexOperator)
		{
			HashMap<int, float> testMap;
			HashMap<int, float>::PairType entry = make_pair(10, 20.0f);

			testMap.Insert(entry);

			const HashMap<int, float>& constTestMapR = testMap;

			float ref = constTestMapR[10];
			Assert::AreEqual(20.0f, ref);
		}

		TEST_METHOD(TestAccessorMethods)
		{
			HashMap<int, float> initMap;

			for (int i = 0; i < 10; ++i)
			{
				float result = 10.0f * i;
				initMap.Insert(HashMap<int, float>::PairType(i, result));
			}

			const HashMap<int, float> constMap = initMap;

			//	Testing out of bounds At call for coverage.
			Assert::ExpectException<std::runtime_error>([&initMap] { initMap.At(20); });
			Assert::ExpectException<std::runtime_error>([&constMap] { constMap.At(20); });

			//	Testing operator[]
			float result = initMap[5];
			Assert::AreEqual(50.0f, result);

			initMap[5] = 250.0f;

			Assert::AreEqual(250.0f, initMap.At(5));
		}

		TEST_METHOD(TestBegins)
		{
			HashMap<int, float> initMap;
			HashMap<int, float> emptyMap;
			const HashMap<int, float> constEmptyMap;

			for (int i = 0; i < 10; ++i)
			{
				float result = 10.0f * i;
				initMap.Insert(HashMap<int, float>::PairType(i, result));
			}

			const HashMap<int, float> constMap = initMap;

			HashMap<int, float>::Iterator it;
			HashMap<int, float>::ConstIterator cit;
			HashMap<int, float>::ConstIterator citTwo;

			//	Testing begins on empty maps.
			it = emptyMap.begin();
			cit = constEmptyMap.begin();
			citTwo = emptyMap.cbegin();

			bool result = it == emptyMap.end();
			Assert::AreEqual(true, result);

			result = cit == constEmptyMap.end();
			Assert::AreEqual(true, result);

			result = citTwo == emptyMap.cend();
			Assert::AreEqual(true, result);

			//	Testing on non-empty maps.
			it = initMap.begin();
			cit = constMap.begin();
			citTwo = initMap.cbegin();

			result = it != initMap.end();
			Assert::AreEqual(true, result);

			result = cit != constMap.end();
			Assert::AreEqual(true, result);

			result = citTwo != initMap.cend();
			Assert::AreEqual(true, result);

			//	Increment the iterator 10 times and they should be at the end.
			for (int i = 0; i < 10; ++i)
			{
				if (i % 2 == 1)
				{
					++it;
					++cit;
					++citTwo;
				}
				else
				{
					it++;
					cit++;
					citTwo++;
				}
			}
			result = it == initMap.end();
			Assert::AreEqual(true, result);

			result = cit == constMap.cend();
			Assert::AreEqual(true, result);

			result = citTwo == initMap.cend();
			Assert::AreEqual(true, result);
		}

		TEST_METHOD(TestResize)
		{
			HashMap<int, float> testMap;
			HashMap<int, float>::PairType entry = make_pair(10, 20.0f);
			HashMap<int, float>::PairType entryOverwrite = make_pair(10, 30.0f);

			//	L-Value Reference
			auto [insIt, tfVal] = testMap.Insert(entry);
			Assert::IsTrue(tfVal);
			//	Testing Initial insert
			Assert::AreEqual((testMap.Find(10))->second, 20.0f);

			//	R-Value Reference
			testMap.Insert(HashMap<int, float>::PairType(20, 30.0f));
			//	Testing initial insert
			Assert::AreEqual((testMap.Find(20))->second, 30.0f);

			//	Attempting Overwriting both keys
			auto [owIt, owVal] = testMap.Insert(entryOverwrite);
			testMap.Insert(HashMap<int, float>::PairType(20, 40.0f));

			//	The original Key values should not be, size should still be 2
			Assert::IsFalse(owVal);
			Assert::AreEqual((testMap.Find(10))->second, 20.0f);
			Assert::AreEqual((testMap.Find(20))->second, 30.0f);
			Assert::AreEqual(2_z, testMap.Size());
			Assert::AreEqual(false, testMap.IsEmpty());
			Assert::AreEqual(true, testMap.ContainsKey(10));

			testMap.Resize(20);

			Assert::AreEqual((testMap.Find(10))->second, 20.0f);
			Assert::AreEqual((testMap.Find(20))->second, 30.0f);
			Assert::AreEqual(2_z, testMap.Size());
			Assert::AreEqual(false, testMap.IsEmpty());
			Assert::AreEqual(true, testMap.ContainsKey(10));
		}

		TEST_METHOD(TestIteratorMethods)
		{
			HashMap<int, float> initMap;
			HashMap<int, float> emptyMap;
			const HashMap<int, float> constEmptyMap;

			for (int i = 0; i < 10; ++i)
			{
				float result = 10.0f * i;
				initMap.Insert(HashMap<int, float>::PairType(i, result));
			}

			HashMap<int, float>::Iterator it;
			HashMap<int, float>::ConstIterator cit;

			//	Testing Incrementing on unowned iterators
			Assert::ExpectException<std::runtime_error>([&it] {++it; });
			Assert::ExpectException<std::runtime_error>([&cit] {++cit; });

			//	Testing incrementing on empty Maps (should just do nothing)
			it = emptyMap.begin();
			cit = emptyMap.cbegin();

			bool result = it == emptyMap.end();
			Assert::AreEqual(true, result);
			result = cit == emptyMap.end();
			Assert::AreEqual(true, result);

			++it;
			++cit;

			result = it == emptyMap.end();
			Assert::AreEqual(true, result);
			result = cit == emptyMap.end();
			Assert::AreEqual(true, result);

			//	Testing Operator*() on end
			Assert::ExpectException<std::runtime_error>([&it] {*it; UNREFERENCED_LOCAL(it); });
			Assert::ExpectException<std::runtime_error>([&cit] {*cit; UNREFERENCED_LOCAL(cit); });
			
			//	Testing Operator->() on end
			Assert::ExpectException<std::runtime_error>([&it] {it->first; UNREFERENCED_LOCAL(it); });
			Assert::ExpectException<std::runtime_error>([&cit] {cit->second; UNREFERENCED_LOCAL(cit); });
			
			//	Testing Operator*() normally
			it = initMap.begin();
			cit = initMap.cbegin();

			HashMap<int, float>::PairType itResult = *it;
			const HashMap<int, float>::PairType citResult = *cit;
			Assert::AreEqual(itResult.first, citResult.first);
			Assert::AreEqual(itResult.second, citResult.second);

			//	Testing iterator -> constiterator operator=()
			HashMap<int, float>::ConstIterator constructorTest;
			constructorTest = it;
		}
#pragma endregion
	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState HashMapTests::_startMemState;
}

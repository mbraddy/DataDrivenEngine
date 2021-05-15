#include "pch.h"
#include <crtdbg.h>
#include <CppUnitTest.h>
#include <exception>
#include <stdexcept>
#include "Foo.h"
#include "Vector.h"

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

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(VectorTests)
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

		TEST_METHOD(TestVector)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			Vector<int> vi;
			Vector<Foo> vf;

			Vector<Foo> vfoo{a, b, c};

			vf.PushBack(a);

			Assert::AreEqual(a, vfoo[0]);
			Assert::AreEqual(b, vfoo.At(1));
			Assert::AreEqual(c, vfoo.At(2));

			const Foo d = vfoo.At(1);
			const Foo e = vfoo[2];

			Assert::AreEqual(b, d);
			Assert::AreEqual(c, e);
			
			Vector<int> viHundred(100);

			// Testing Accessing no capacity Vector
			Assert::ExpectException<runtime_error>([&vi] { int i = vi[5]; i; });
			Assert::ExpectException<runtime_error>([&vi] { const int i = vi[5]; i; });

			// Testing Accessing empty Vector
			Assert::ExpectException<runtime_error>([&viHundred] { int i = viHundred[5]; i; });
			Assert::ExpectException<runtime_error>([&viHundred] { const int i = viHundred[5]; i; });

			// Testing Back() function on empty vector
			Assert::ExpectException<runtime_error>([&viHundred] { const int i = viHundred.Back(); i; });
			Assert::ExpectException<runtime_error>([&viHundred] { int i = viHundred.Back(); i; });

			// Testing Back() function on no capcity vector
			Assert::ExpectException<runtime_error>([&vi] { const int i = vi.Back(); i; });
			Assert::ExpectException<runtime_error>([&vi] { int i = vi.Back(); i; });

			Assert::AreEqual(0_z, vi.Size());
			Assert::AreEqual(0_z, vi.Capacity());
			Assert::AreEqual(100_z, viHundred.Capacity());
			Assert::AreEqual(0_z, viHundred.Size());

			//	Pushback correct value testing
			vi.PushBack(100);
			viHundred.PushBack(100);

			Assert::AreEqual(vi[0], viHundred[0]);
			Assert::AreEqual(vi.Back(), viHundred.Back());
			Assert::AreEqual(100, vi[0]);
			Assert::AreEqual(1_z, vi.Size());
			Assert::AreEqual(vi.Size(), viHundred.Size());
			Assert::AreEqual(2_z, vi.Capacity());

			//	Pushback + capacity incrementation testing
			int test = 10;
			for (int i = 0; i < 20; i++)
			{
				vi.PushBack(test);
			}

			//	Testing PopBack()
			vfoo.PopBack();
			Assert::AreEqual(2_z, vfoo.Size());
			Assert::AreEqual(3_z, vfoo.Capacity());

			//	Testing PopBack() on empty Vector
			Vector<int> emptyV;
			Assert::ExpectException<runtime_error>([&emptyV] { emptyV.PopBack(); });

			//	Testing PushBack(), Remove(), operator[], Size(), Capacity()
			vfoo.PushBack(c);
			Assert::AreEqual(3_z, vfoo.Size());
			Assert::AreEqual(3_z, vfoo.Capacity());
			vfoo.Remove(b);
			Assert::AreEqual(2_z, vfoo.Size());
			Assert::AreEqual(3_z, vfoo.Capacity());
			Assert::AreEqual(c, vfoo[1]);
			vfoo.Remove(Foo(10));
			Assert::AreEqual(1_z, vfoo.Size());
			Assert::AreEqual(3_z, vfoo.Capacity());
			Assert::AreEqual(c, vfoo[0]);
			vfoo.Remove(c);
			Assert::AreEqual(0_z, vfoo.Size());
			Assert::AreEqual(3_z, vfoo.Capacity());
			Assert::ExpectException<runtime_error>([&vfoo] { const Foo q = vfoo[0]; });
			Assert::ExpectException<runtime_error>([&vfoo] { const Foo q = vfoo[1]; });
			Assert::ExpectException<runtime_error>([&vfoo] { const Foo q = vfoo[2]; });
			vfoo.PushBack(a);
			vfoo.PushBack(b);
			vfoo.PushBack(c);
			vfoo.Remove(c);
			Assert::AreEqual(2_z, vfoo.Size());
			Assert::AreEqual(3_z, vfoo.Capacity());
			Assert::AreEqual(b, vfoo[1]);
			Foo notInVector(500);
			vfoo.Remove(notInVector);

			//	Testing Remove with wrong iterator
			Vector<Foo>::Iterator testIt = vf.begin();
			Assert::ExpectException<runtime_error>([&vfoo, &testIt] { vfoo.Remove(testIt); });

			//	Testing ShrinkToFit() Clear()
			vi.Clear();
			vi.ShrinkToFit();
			Assert::AreEqual(0_z, vi.Size());
			Assert::AreEqual(0_z, vi.Capacity());

			//	Testing Front() Back() IsEmpty() ShrinkToFit() Clear()
			Vector<size_t> testBackFrontv{ 0_z, 1_z, 2_z, 3_z };
			Assert::AreEqual(3_z, testBackFrontv.Back());
			Assert::AreEqual(0_z, testBackFrontv.Front());
			testBackFrontv.Clear();
			Assert::ExpectException<runtime_error>([&testBackFrontv] { testBackFrontv.Back(); });
			Assert::ExpectException<runtime_error>([&testBackFrontv] { testBackFrontv.Front(); });
			testBackFrontv.ShrinkToFit();
			Assert::ExpectException<runtime_error>([&testBackFrontv] { testBackFrontv.Back(); });
			Assert::ExpectException<runtime_error>([&testBackFrontv] { testBackFrontv.Front(); });
			Assert::AreEqual(true, testBackFrontv.IsEmpty());

			Vector<Foo> copiedVector;
			copiedVector = vfoo;
			Vector<Foo> copiedVector2(vfoo);
			Assert::AreEqual(*(copiedVector.begin()), *(vfoo.begin()));
			Assert::AreEqual(*(copiedVector2.begin()), *(vfoo.begin()));
			Assert::AreEqual(copiedVector.Size(), vfoo.Size());
			Assert::AreEqual(copiedVector2.Size(), vfoo.Size());
			Assert::AreEqual(copiedVector.Size(), vfoo.Size());
			Assert::AreEqual(copiedVector2.Size(), vfoo.Size());

			Vector<Foo> emptyVector;
			
			Vector<Foo>::Iterator derp = emptyVector.end();
			Vector<Foo>::ConstIterator derp2 = emptyVector.end();
			Vector<Foo>::ConstIterator derp3 = emptyVector.cend();

			Assert::ExpectException<runtime_error>([&emptyVector] { emptyVector.At(2); });
			emptyVector.Reserve(5);
			Assert::ExpectException<runtime_error>([&emptyVector] { emptyVector.At(22); });

			const Vector<Foo> emptyVectorC;
			Assert::ExpectException<runtime_error>([&emptyVectorC] { emptyVectorC.At(2); });
			Assert::ExpectException<runtime_error>([&emptyVectorC] { emptyVectorC[2]; });
			Assert::ExpectException<runtime_error>([&emptyVectorC] { emptyVectorC.Back(); });
			Assert::ExpectException<runtime_error>([&emptyVectorC] { emptyVectorC.Front(); });
			const Vector<Foo> emptyVectorC2 = emptyVector;
			Assert::ExpectException<runtime_error>([&emptyVectorC2] { emptyVectorC2.At(20); });
			Assert::ExpectException<runtime_error>([&emptyVectorC2] { emptyVectorC2[20]; });
			Assert::ExpectException<runtime_error>([&emptyVectorC2] { emptyVectorC2.Back(); });
			Assert::ExpectException<runtime_error>([&emptyVectorC2] { emptyVectorC2.Front(); });
			const Vector<Foo> vfooC = vfoo;
			Assert::AreEqual(vfooC.At(0), vfooC[0]);
			Assert::AreEqual(vfooC.Back(), vfoo.Back());
			Assert::AreEqual(vfooC.Front(), vfoo.Front());
		}

		TEST_METHOD(TestIterator)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			Vector<Foo> v{ a,b,c };

			Vector<Foo>::Iterator garbageIt;
			Vector<Foo>::ConstIterator garbageItC;

			Assert::ExpectException<runtime_error>([&garbageIt] { garbageIt++; });
			Assert::ExpectException<runtime_error>([&garbageIt] { ++garbageIt; });
			Assert::ExpectException<runtime_error>([&garbageItC] { garbageItC++; });
			Assert::ExpectException<runtime_error>([&garbageItC] { ++garbageItC; });

			Vector<Foo>::Iterator it = v.begin();
			Vector<Foo>::ConstIterator cit = v.begin();

			Assert::AreEqual(a, *it);
			Assert::AreEqual(a, *cit);
			it++;
			cit++;
			Assert::AreEqual(b, *it);
			Assert::AreEqual(b, *cit);
			++it;
			++cit;
			Assert::AreEqual(c, *it);
			Assert::AreEqual(c, *cit);

			cit = v.cbegin();
			Assert::AreEqual(a, *cit);
			//Assert::ExpectException<runtime_error>([&cit] { (*cit) = Foo(100); });

			const Vector<Foo> cv{ a, b, c };
			cit = cv.begin();
			Assert::AreEqual(a, *cit);
			const Vector<Foo> garbcv;
			cit = cv.end();

			Vector<Foo>::Iterator copy = it;
			assert(copy == it);
			Vector<Foo>::ConstIterator copyc = cit;
			assert(copyc == cit);
			Vector<Foo>::ConstIterator copyc2 = it;

			Vector<Foo>::ConstIterator derpy = cv.Find(b);
			Assert::AreEqual(cv[1], *derpy);

			Vector<Foo>::ConstIterator doie;
			doie = it;
		}

		TEST_METHOD(TestBigRemove)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);
			const Foo d(40);
			const Foo e(50);
			const Foo f(60);
			const Foo g(70);
			const Foo h(80);
			const Foo i(90);
			const Foo j(100);

			{
				Vector<Foo> v{ a, b, c, d, e, f, g, h, i, j };
				Vector<Foo>::Iterator start = v.begin();
				Vector<Foo>::Iterator end = v.begin();
				Vector<Foo>::Iterator fake;

				//	Place start on index 3 - d
				++start;
				++start;
				++start;

				//	Place end on index 7 - h
				++end;
				++end;
				++end;
				++end;
				++end;
				++end;
				++end;

				Assert::AreEqual(d, *start);
				Assert::AreEqual(h, *end);

				Assert::ExpectException<runtime_error>([&fake, &start, &v] { v.Remove(start, fake); });

				v.Remove(start, end);

				Assert::AreEqual(10_z, v.Capacity());
				Assert::AreEqual(5_z, v.Size());
				Assert::AreEqual(i, v[3]);
			}

			{
				Vector<Foo> v{ a, b, c, d, e, f, g, h, i, j };
				Vector<Foo>::Iterator start = v.begin();
				Vector<Foo>::Iterator end = v.begin();

				//	Place start on index 3 - d
				++start;
				++start;
				++start;

				//	Place end on index 9 - j
				++end;
				++end;
				++end;
				++end;
				++end;
				++end;
				++end;
				++end;
				++end;

				Assert::AreEqual(d, *start);
				Assert::AreEqual(j, *end);

				v.Remove(start, end);

				Assert::AreEqual(10_z, v.Capacity());
				Assert::AreEqual(3_z, v.Size());
				Assert::AreEqual(c, v.Back());
			}

			{
				Vector<Foo> v{ a, b, c, d, e, f, g, h, i, j };
				Vector<Foo>::Iterator start = v.begin();
				Vector<Foo>::Iterator end = v.begin();

				//	Place start on index 0 - a

				//	Place end on index 9 - j
				++end;
				++end;
				++end;
				++end;
				++end;
				++end;
				++end;
				++end;
				++end;

				Assert::AreEqual(a, *start);
				Assert::AreEqual(j, *end);

				v.Remove(start, end);

				Assert::AreEqual(10_z, v.Capacity());
				Assert::AreEqual(0_z, v.Size());
			}

			{
				Vector<Foo> v{ a, b, c, d, e, f, g, h, i, j };
				Vector<Foo>::Iterator start = v.begin();
				Vector<Foo>::Iterator end = v.begin();

				//	Place start on index 0 - a

				//	Place end on index 5 - f
				++end;
				++end;
				++end;
				++end;
				++end;

				Assert::AreEqual(a, *start);
				Assert::AreEqual(f, *end);

				v.Remove(start, end);

				Assert::AreEqual(10_z, v.Capacity());
				Assert::AreEqual(4_z, v.Size());
				Assert::AreEqual(g, v.Front());
			}

		}

		TEST_METHOD(TestMoveSemantics)
		{
			const Foo a(10);
			const Foo b(10);
			const Foo c(10);

			Vector<Foo> original{ a, b, c };

			Vector<Foo> thief(std::forward<Vector<Foo>>(original));
			Assert::ExpectException<runtime_error>([&original] { original[1]; });
			Assert::AreEqual(b, thief[1]);
			Assert::AreEqual(0_z, original.Size());
			Assert::AreEqual(0_z, original.Capacity());

			Vector<Foo> thiefOfThieves;
			thiefOfThieves = std::forward<Vector<Foo>>(thief);
			Assert::ExpectException<runtime_error>([&thief] { thief[1]; });
			Assert::AreEqual(b, thiefOfThieves[1]);
			Assert::AreEqual(0_z, thief.Size());
			Assert::AreEqual(0_z, thief.Capacity());
		}

		TEST_METHOD(TestShrinkToFitAndResize)
		{
			Vector<Foo> test(10_z);

			test.PushBack(Foo(20));
			test.ShrinkToFit();
			Assert::AreEqual(1_z, test.Size());

			Vector<Foo> testTwo(10_z);
			testTwo.Resize(10_z);

			for (int i = 0; i < 10; ++i)
			{
				testTwo.PushBack(Foo(i));
			}
			testTwo.Resize(5_z);
			Assert::AreEqual(5_z, testTwo.Size());
			Assert::AreEqual(5_z, testTwo.Capacity());
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState VectorTests::_startMemState;
}

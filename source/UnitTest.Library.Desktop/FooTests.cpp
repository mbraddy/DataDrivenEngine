#include "pch.h"
#include <crtdbg.h>
#include <CppUnitTest.h>
#include <exception>
#include "Foo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<> 
	inline std::wstring ToString<Foo>(const Foo& t) {
		RETURN_WIDE_STRING(t.Data());
	}
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(FooTests)
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
		
		TEST_METHOD(TestConstructor)
		{
			Foo a;
			Assert::AreEqual(0, a.Data());

			const int value = 10;
			Foo b(value);
			Assert::AreEqual(value, b.Data());
		}

		TEST_METHOD(TestCopySemantics)
		{
			const int value = 10;
			Foo a(value);
			Assert::AreEqual(value, a.Data());

			Foo b(a);
			Assert::AreEqual(a, b);

			Foo c;
			c = a;
			Assert::AreEqual(a, c);
		}

		TEST_METHOD(TestMoveSemantics)
		{
			const int value = 10;
			Foo a(value);

			Foo b(std::move(a));
			Assert::AreEqual(value, b.Data());
			Assert::ExpectException<std::exception>([&a] { a.Data(); }), L"Expected an exception but none was thrown.";

			Foo c;
			Assert::AreEqual(0, c.Data());
			c = std::move(b);
			Assert::AreEqual(value, c.Data());
			Assert::ExpectException<std::exception>([&b] { b.Data(); }), L"Expected an exception but none was thrown.";
		}

		TEST_METHOD(TestComparisons)
		{
			const int value = 10;
			Foo a(value);

			Foo b;
			Assert::IsTrue(a != b);

			b = a;
			Assert::AreEqual(a, b);

			if (a == b)
			{
				if (a != b)
				{
				}
			}

			b = std::move(a);
			Assert::ExpectException<std::runtime_error>([&a, &b] { a == b; });
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState FooTests::_startMemState;
}

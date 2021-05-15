#include "pch.h"
#include <crtdbg.h>
#include <CppUnitTest.h>
#include <exception>
#include <stdexcept>
#include "Foo.h"
#include "SList.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<SList<Foo>>(const SList<Foo>& t) 
	{
		RETURN_WIDE_STRING(t.Size());
	}

	template<>
	inline std::wstring ToString<Foo>(const Foo& t) 
	{
		RETURN_WIDE_STRING(t.Data());
	}

	template<>
	inline std::wstring ToString<SList<Foo>::Iterator>(const SList<Foo>::Iterator& t) 
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
	inline std::wstring ToString<SList<Foo>::ConstIterator>(const SList<Foo>::ConstIterator& t) 
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
	TEST_CLASS(SListTests)
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
			Foo f;
			SList<Foo> list;
			list.PushFront(f);

			Assert::AreEqual((size_t)1, list.Size());
		}

		TEST_METHOD(TestIterator)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			SList<Foo> list;
			SList<Foo> listNotTheOwner;
			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(c);
			listNotTheOwner.PushBack(a);
			listNotTheOwner.PushBack(b);

			SList<Foo>::Iterator it = list.begin();
			Assert::AreEqual(*(it), a);
			++(it);
			Assert::AreEqual(*(it), b);
			it = list.end();
			Assert::ExpectException<runtime_error>([&it] { *(it); });

			it = list.Find(c);
			Assert::AreEqual(*(it), c);
			list.Remove(c);
			Assert::AreEqual(list.Size(), 2_z);

			SList<Foo>::Iterator it2 = list.Find(b);
			list.InsertAfter(it2, c);
			Assert::AreEqual(c, list.Back());

			SList<Foo>::Iterator emptyIt;
			Assert::ExpectException<runtime_error>([&emptyIt, &list] { list.InsertAfter(emptyIt, Foo(20)); });
			Assert::ExpectException<runtime_error>([&emptyIt, &list] { list.InsertAfter(emptyIt, std::move(Foo(20))); });
			Assert::ExpectException<runtime_error>([&emptyIt, &list] { list.InsertAfter(emptyIt, std::move(Foo(20))); });
			Assert::ExpectException<runtime_error>([&it, &listNotTheOwner] { listNotTheOwner.InsertAfter(it, std::move(Foo(20))); });
			Assert::ExpectException<runtime_error>([&it, &listNotTheOwner, &b] { listNotTheOwner.InsertAfter(it, b); });
			Assert::ExpectException<runtime_error>([&emptyIt, &listNotTheOwner, &b] { listNotTheOwner.InsertAfter(emptyIt, b); });

			Assert::ExpectException<runtime_error>([&it, &listNotTheOwner] { listNotTheOwner.Remove(it); });

			it = list.begin();
			it2 = list.begin();
			Assert::AreEqual(it, it2);
			Assert::IsTrue(it == it2);
			++it2;
			Assert::IsFalse(it == it2);

			SList<Foo>::Iterator unownedIt;
			Assert::ExpectException<runtime_error>([&unownedIt] { ++unownedIt; });
			it++;
			Assert::AreEqual(it, it2);

			SList<Foo> listIABackTest;
			listIABackTest.PushBack(a);
			listIABackTest.PushBack(b);
			it = listIABackTest.Find(b);
			listIABackTest.InsertAfter(it, c);

			SList<Foo> listIABackTest2;
			listIABackTest2.PushBack(a);
			listIABackTest2.PushBack(b);
			it = listIABackTest2.Find(b);
			listIABackTest2.InsertAfter(it, std::move(Foo(20)));

			SList<Foo> removeTests;
			removeTests.PushBack(a);
			removeTests.PushBack(b);
			removeTests.PushBack(c);

			SList<Foo>::Iterator notMine = list.begin();
			SList<Foo>::Iterator mine = removeTests.begin();

			Assert::ExpectException<runtime_error>([&notMine, &removeTests] { removeTests.Remove(notMine); });
			removeTests.Remove(c);
			Assert::AreEqual(2_z, removeTests.Size());
			removeTests.PushBack(c);
			removeTests.Remove(b);


			SList<Foo> listIAtest;
			listIAtest.PushBack(a);
			listIAtest.PushBack(b);
			listIAtest.PushBack(c);

			SList<Foo>::Iterator IAtest = listIAtest.Find(b);
			listIAtest.InsertAfter(IAtest, a);
			listIAtest.InsertAfter(IAtest, std::move(Foo(40)));

		}

		TEST_METHOD(TestConstIterator)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			SList<Foo> list;
			SList<Foo> listNotTheOwner;
			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(c);
			listNotTheOwner.PushBack(a);
			listNotTheOwner.PushBack(b);

			SList<Foo>::ConstIterator it = list.begin();
			Assert::AreEqual(*(it), a);
			++(it);
			Assert::AreEqual(*(it), b);
			it = list.end();
			Assert::ExpectException<runtime_error>([&it] { *(it); });

			it = list.Find(c);
			Assert::AreEqual(*(it), c);
			list.Remove(c);
			Assert::AreEqual(list.Size(), 2_z);

			SList<Foo>::ConstIterator it2 = list.Find(b);
			Assert::AreEqual(b, *(it2));

			it = list.begin();
			it2 = list.begin();
			Assert::AreEqual(it, it2);
			Assert::IsTrue(it == it2);
			++it2;
			Assert::IsFalse(it == it2);

			SList<Foo>::ConstIterator unownedIt;
			Assert::ExpectException<runtime_error>([&unownedIt] { ++unownedIt; });
			it++;
			Assert::AreEqual(it, it2);

			const SList<Foo> constList = list;
			SList<Foo>::ConstIterator constListIt;

			constListIt = constList.cbegin();
			Assert::AreEqual(a, *(constListIt));
			constListIt = constList.begin();
			Assert::AreEqual(a, *(constListIt));
			constListIt = constList.cend();
			Assert::ExpectException<runtime_error>([&constListIt] { *(constListIt); });
			constListIt = constList.end();
			Assert::ExpectException<runtime_error>([&constListIt] { *(constListIt); });

			constListIt = constList.Find(a);
			Assert::AreEqual(a, *(constListIt));
			constListIt = constList.Find(b);
			Assert::AreEqual(b, *(constListIt));
			constListIt = constList.Find(c);
			Assert::ExpectException<runtime_error>([&constListIt, &c] { Assert::AreEqual(c, *(constListIt)); });

		}

		TEST_METHOD(TestCopySemantics)
		{
			const Foo a(10);
			const Foo b(10);
			const Foo c(10);

			SList<Foo> list;
			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(c);
			Assert::AreEqual(3_z, list.Size());

			SList<Foo> copyOfList(list);
			SList<Foo> copyOfListTwo;
			copyOfListTwo = list;

			Assert::AreEqual(3_z, copyOfList.Size());
			Assert::AreEqual(list.Front(), copyOfList.Front());
			Assert::AreEqual(list.Back(), copyOfList.Back());

			Assert::AreEqual(3_z, copyOfListTwo.Size());
			Assert::AreEqual(list.Front(), copyOfListTwo.Front());
			Assert::AreEqual(list.Back(), copyOfListTwo.Back());

			list.PopFront();
			copyOfList.PopFront();
			copyOfListTwo.PopFront();
			Assert::AreEqual(list.Front(), copyOfList.Front());
			Assert::AreEqual(list.Front(), copyOfListTwo.Front());
		}

		TEST_METHOD(TestMoveSemantics)
		{
			const Foo a(10);
			const Foo b(10);
			const Foo c(10);

			SList<Foo> list;
			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(c);

			SList<Foo> otherList(std::move(list));

			Assert::AreEqual(3_z, otherList.Size());
			Assert::AreEqual(a, otherList.Front());
			Assert::AreEqual(c, otherList.Back());
			Assert::ExpectException<std::exception>([&list] { list.Front(); }), L"Expected an exception but none was thrown.";

			SList<Foo> otherOtherList;
			Assert::AreEqual(0_z, otherOtherList.Size());
			otherOtherList = std::move(otherList);
			Assert::AreEqual(3_z, otherOtherList.Size());
			Assert::ExpectException<std::exception>([&otherList] { otherList.Front(); }), L"Expected an exception but none was thrown.";
		}

		TEST_METHOD(TestBackFunctions)
		{
			SList<Foo> list;

			list.PopBack();

			Assert::ExpectException<runtime_error>([&list] { list.Back(); });

			Foo a(10);
			Foo b(20);
			Foo c(30);

			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(c);

			Assert::AreEqual(c, list.Back());

			const SList<Foo> listCons = list;

			const SList<Foo> consEmpty;
			Assert::ExpectException<runtime_error>([&consEmpty] { consEmpty.Back(); });

			Assert::AreEqual(c ,listCons.Back());

			list.PopBack();
			Assert::AreEqual(b, list.Back());

		}

		TEST_METHOD(TestFrontFunctions)
		{
			SList<Foo> list;
			Assert::ExpectException<runtime_error>([&list] { list.Front(); });

			list.PopFront();

			Foo a(10);
			Foo b(20);
			Foo c(30);

			list.PushFront(a);
			list.PushFront(b);
			list.PushFront(c);

			SList<Foo> otherList;

			Foo d = a;

			otherList.PushFront(std::move(d));

			Assert::AreEqual(c, list.Front());

			const SList<Foo> listCons = list;

			const SList<Foo> consEmpty;

			Assert::ExpectException<runtime_error>([&consEmpty] { consEmpty.Front(); });

			Assert::AreEqual(c, listCons.Front());

			list.PopFront();
			Assert::AreEqual(b, list.Front());
		}

		TEST_METHOD(TestEmptySizeAndClear)
		{
			SList<Foo> list;
			bool test = list.IsEmpty();
			Assert::IsTrue(test);

			list.PushBack(Foo(10));

			Assert::IsFalse(list.IsEmpty());

			for (int i = 0; i < 50; i++)
			{
				list.PushBack(Foo(10));
				list.PushFront(Foo(20));
			}

			Assert::AreEqual((size_t)101, list.Size());

			list.Clear();

			Assert::AreEqual((size_t)0, list.Size());

		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState SListTests::_startMemState;
}

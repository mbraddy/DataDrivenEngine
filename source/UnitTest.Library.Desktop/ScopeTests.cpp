#include "pch.h"
#include <crtdbg.h>
#include <CppUnitTest.h>
#include <exception>
#include <stdexcept>
#include <functional>
#include "Foo.h"
#include "Datum.h"
#include "Scope.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{

	template<>
	inline std::wstring ToString<Foo>(const Foo& t)
	{
		RETURN_WIDE_STRING(t.Data());
	}
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(ScopeTests)
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

#pragma region Scope Testing
		TEST_METHOD(TestConstructor)
		{
			Scope s;
			Scope sTen(10);

			Assert::AreEqual(s.Size(), 0_z);
		}

		TEST_METHOD(TestAppend)
		{
			Scope s;

			Assert::AreEqual(s.Size(), 0_z);

			Datum* d = &s.Append("TestOne");
			*d = 10;
			
			Assert::AreEqual(s.Size(), 1_z);
			Assert::IsTrue(s[0].Type() == Datum::DatumType::Integer);
			Assert::AreEqual((s[0]).Get<int>(0), 10);

			s.Append("TestOne");

			//	Expect no changes with second append call
			Assert::AreEqual(s.Size(), 1_z);
			Assert::IsTrue(s[0].Type() == Datum::DatumType::Integer);
			Assert::AreEqual((s[0]).Get<int>(0), 10);

			Assert::ExpectException<invalid_argument>([&s] {s.Append(""); });

		}

		TEST_METHOD(TestAppendScope)
		{
			Scope s;

			Assert::AreEqual(s.Size(), 0_z);

			Scope* test = &s.AppendScope("TestOne");
			
			Datum* d = &(test->Append("NestedTestOne"));
			*d = 10;

			Assert::AreEqual(s.Size(), 1_z);
			Assert::IsTrue(s[0][0][0].Type() == Datum::DatumType::Integer);
			Assert::AreEqual((s[0][0][0]).Get<int>(0), 10);
			
			//	This should Append another scope to the datum paired with key TestOne       Scope -> Datum(Table) -> Scope -> Datum (int) 10
			Scope* testTwo = &s.AppendScope("TestOne");									//	    				  -> Scope -> Datum (float) 10

			//	Expect no changes with the first scope in the second append call
			Assert::AreEqual(s.Size(), 1_z);
			Assert::IsTrue(s[0][0][0].Type() == Datum::DatumType::Integer);
			Assert::AreEqual((s[0][0][0]).Get<int>(0), 10);

			//	The size of the root scope's datum should be size 2 now
			Assert::AreEqual(s[0].Size(), 2_z);

			Datum* dTwo = &(testTwo->Append("NestedTestTwo"));
			*dTwo = 10.0f;

			Assert::IsTrue(s[0][1][0].Type() == Datum::DatumType::Float);
			Assert::AreEqual((s[0][1][0]).Get<float>(0), 10.0f);

			Scope q(s);

			Scope c(*testTwo);

			Assert::IsTrue(q == s);
			Assert::IsTrue(c == *testTwo);

			Datum& di = s.Append("IntTest");
			di = 2;

			Assert::ExpectException<runtime_error>([&s] {s.AppendScope("IntTest"); });
		}

		TEST_METHOD(TestAdopt)
		{
			Scope s;

			Assert::AreEqual(s.Size(), 0_z);

			Scope* test = &s.AppendScope("TestOne");

			Datum* d = &(test->Append("NestedTestOne"));
			*d = 10;

			Assert::AreEqual(s.Size(), 1_z);
			Assert::IsTrue(s[0][0][0].Type() == Datum::DatumType::Integer);
			Assert::AreEqual((s[0][0][0]).Get<int>(0), 10);

			Scope r;
			Datum* dTwo = &r.Append("NestedTest");
			*dTwo = 10.0f;
			
			//	This should Adopt another scope to the datum paired with key TestOne        Scope -> Datum(Table) -> Scope -> Datum (int) 10
			s.Adopt(r, "TestOne");														//	    				  -> Scope -> Datum (float) 10

			//	Expect no changes with the first scope or it's child
			Assert::AreEqual(s.Size(), 1_z);
			Assert::IsTrue(s[0][0][0].Type() == Datum::DatumType::Integer);
			Assert::AreEqual((s[0][0][0]).Get<int>(0), 10);

			//	The size of the root scope's datum should be size 2 now
			Assert::AreEqual(s[0].Size(), 2_z);

			Assert::IsTrue(s[0][1][0].Type() == Datum::DatumType::Float);
			Assert::AreEqual((s[0][1][0]).Get<float>(0), 10.0f);

			Datum* dInt = &s.Append("IntTest");
			*dInt = 10;

			Assert::ExpectException<runtime_error>([&s] {Scope t;  s.Adopt(t, "IntTest"); });

			Scope q;
			s.Adopt(q, "TestTwo");

			Scope t(std::move(q));

			Scope j;
			j = std::move(t);
		}

		TEST_METHOD(TestSearchAndFindContainedScope)
		{
			Scope s;

			Assert::AreEqual(s.Size(), 0_z);

			Scope* test = &s.AppendScope("TestOne");

			Datum* d = &(test->Append("NestedTestOne"));
			*d = 10;

			Assert::AreEqual(s.Size(), 1_z);
			Assert::IsTrue(s[0][0][0].Type() == Datum::DatumType::Integer);
			Assert::AreEqual((s[0][0][0]).Get<int>(0), 10);

			//	This should Append another scope to the datum paired with key TestOne       Scope -> Datum(Table) -> Scope -> Datum (int) 10
			Scope* testTwo = &s.AppendScope("TestOne");									//	    				  -> Scope -> Datum (float) 10

			//	Expect no changes with the first scope in the second append call
			Assert::AreEqual(s.Size(), 1_z);
			Assert::IsTrue(s[0][0][0].Type() == Datum::DatumType::Integer);
			Assert::AreEqual((s[0][0][0]).Get<int>(0), 10);

			//	The size of the root scope's datum should be size 2 now
			Assert::AreEqual(s[0].Size(), 2_z);

			Datum* dTwo = &(testTwo->Append("NestedTestTwo"));
			*dTwo = 10.0f;

			Assert::IsTrue(s[0][1][0].Type() == Datum::DatumType::Float);
			Assert::AreEqual((s[0][1][0]).Get<float>(0), 10.0f);

			Scope q(s);

			Scope c(*testTwo);

			Assert::IsTrue(q == s);
			Assert::IsTrue(c == *testTwo);

			Scope notIn;
			size_t index = 0;

			Datum* g = s.FindContainedScope(&notIn, index);

			Assert::IsNull(g);
			Assert::AreEqual(0_z, index);

			Datum* qz = testTwo->Search("TestOne");

			Assert::IsTrue(qz == &s[0]);

			Datum* qq = testTwo->Search("NotInHere");

			Assert::IsNull(qq);

			Scope** testDouble;
			Scope* testSingle = &s;
			testDouble = &testSingle;

			const Scope cs = s;

			cs.Find("TestOne");

			size_t ind = 0;
			cs.FindContainedScope(test, ind);
			cs.FindContainedScope(testTwo, ind);

			Scope* testThree = &c;

			cs.FindContainedScope(testThree, ind);

			Scope notPresent;

			const Scope childNP;
			
			notPresent.Adopt(const_cast<Scope&>(childNP), "np'sChild"s);

			cs.FindContainedScope(&notPresent, ind);

			childNP.Search("JustForCoverage");

			cs.Search("TestOne");
			cs.Search("NestedTestOne");
			cs.Search("NotPresent");
		}

		TEST_METHOD(TestCopySemantics)
		{
			Scope s;

			Assert::AreEqual(s.Size(), 0_z);

			Scope* test = &s.AppendScope("TestOne");

			Datum* d = &(test->Append("NestedTestOne"));
			*d = 10;

			Assert::AreEqual(s.Size(), 1_z);
			Assert::IsTrue(s[0][0][0].Type() == Datum::DatumType::Integer);
			Assert::AreEqual((s[0][0][0]).Get<int>(0), 10);

			//	This should Append another scope to the datum paired with key TestOne       Scope -> Datum(Table) -> Scope -> Datum (int) 10
			Scope* testTwo = &s.AppendScope("TestOne");									//	    				  -> Scope -> Datum (float) 10

			//	Expect no changes with the first scope in the second append call
			Assert::AreEqual(s.Size(), 1_z);
			Assert::IsTrue(s[0][0][0].Type() == Datum::DatumType::Integer);
			Assert::AreEqual((s[0][0][0]).Get<int>(0), 10);

			//	The size of the root scope's datum should be size 2 now
			Assert::AreEqual(s[0].Size(), 2_z);

			Datum* dTwo = &(testTwo->Append("NestedTestTwo"));
			*dTwo = 10.0f;

			Assert::IsTrue(s[0][1][0].Type() == Datum::DatumType::Float);
			Assert::AreEqual((s[0][1][0]).Get<float>(0), 10.0f);

			Scope q(s);

			Scope c(*testTwo);

			Assert::IsTrue(q == s);
			Assert::IsTrue(c == *testTwo);

			Scope qc;
			qc = q;

			Scope cc;
			cc = c;

			Assert::IsTrue(qc == q);
			Assert::IsTrue(cc == c);
		}

		TEST_METHOD(TestMoveSemantics)
		{
			Scope s;

			Assert::AreEqual(s.Size(), 0_z);

			Scope* test = &s.AppendScope("TestOne");

			Datum* d = &(test->Append("NestedTestOne"));
			*d = 10;

			Assert::AreEqual(s.Size(), 1_z);
			Assert::IsTrue(s[0][0][0].Type() == Datum::DatumType::Integer);
			Assert::AreEqual((s[0][0][0]).Get<int>(0), 10);

			//	This should Append another scope to the datum paired with key TestOne       Scope -> Datum(Table) -> Scope -> Datum (int) 10
			Scope* testTwo = &s.AppendScope("TestOne");									//	    				  -> Scope -> Datum (float) 10

			//	Expect no changes with the first scope in the second append call
			Assert::AreEqual(s.Size(), 1_z);
			Assert::IsTrue(s[0][0][0].Type() == Datum::DatumType::Integer);
			Assert::AreEqual((s[0][0][0]).Get<int>(0), 10);

			//	The size of the root scope's datum should be size 2 now
			Assert::AreEqual(s[0].Size(), 2_z);

			Datum* dTwo = &(testTwo->Append("NestedTestTwo"));
			*dTwo = 10.0f;

			Assert::IsTrue(s[0][1][0].Type() == Datum::DatumType::Float);
			Assert::AreEqual((s[0][1][0]).Get<float>(0), 10.0f);

			Scope r(s);
			Scope z(r);
			Scope q(std::move(s));

			Assert::IsTrue(q == r);
			Assert::AreEqual(0_z, s.Size());

			Scope qc;
			qc = std::move(q);

			Assert::IsTrue(qc == z);
			Assert::AreEqual(0_z, q.Size());
		}

		TEST_METHOD(TestOperatorEquality)
		{
			Scope s;

			Assert::AreEqual(s.Size(), 0_z);

			Scope* test = &s.AppendScope("TestOne");

			Datum* d = &(test->Append("NestedTestOne"));
			*d = 10;

			Assert::AreEqual(s.Size(), 1_z);
			Assert::IsTrue(s[0][0][0].Type() == Datum::DatumType::Integer);
			Assert::AreEqual((s[0][0][0]).Get<int>(0), 10);

			//	This should Append another scope to the datum paired with key TestOne       Scope -> Datum(Table) -> Scope -> Datum (int) 10
			Scope* testTwo = &s.AppendScope("TestOne");									//	    				  -> Scope -> Datum (float) 10

			//	Expect no changes with the first scope in the second append call
			Assert::AreEqual(s.Size(), 1_z);
			Assert::IsTrue(s[0][0][0].Type() == Datum::DatumType::Integer);
			Assert::AreEqual((s[0][0][0]).Get<int>(0), 10);

			//	The size of the root scope's datum should be size 2 now
			Assert::AreEqual(s[0].Size(), 2_z);

			Datum* dTwo = &(testTwo->Append("NestedTestTwo"));
			*dTwo = 10.0f;

			Assert::IsTrue(s[0][1][0].Type() == Datum::DatumType::Float);
			Assert::AreEqual((s[0][1][0]).Get<float>(0), 10.0f);

			////////////////////////////////////////////////////////////////////////////
			
			Scope r;

			Assert::AreEqual(r.Size(), 0_z);

			Scope* testr = &r.AppendScope("TestOne");

			Datum* dr = &(testr->Append("NestedTestOne"));
			*dr = 10;

			Assert::AreEqual(r.Size(), 1_z);
			Assert::IsTrue(r[0][0][0].Type() == Datum::DatumType::Integer);
			Assert::AreEqual((r[0][0][0]).Get<int>(0), 10);

			//	This should Append another scope to the datum paired with key TestOne       Scope -> Datum(Table) -> Scope -> Datum (int) 10
			Scope* testTwor = &r.AppendScope("TestOne");									//	    				  -> Scope -> Datum (float) 10

			//	Expect no changes with the first scope in the second append call
			Assert::AreEqual(r.Size(), 1_z);
			Assert::IsTrue(r[0][0][0].Type() == Datum::DatumType::Integer);
			Assert::AreEqual((r[0][0][0]).Get<int>(0), 10);

			//	The size of the root scope's datum should be size 2 now
			Assert::AreEqual(r[0].Size(), 2_z);

			Datum* dTwor = &(testTwor->Append("NestedTestTwo"));
			*dTwor = 10.0f;

			Assert::IsTrue(r[0][1][0].Type() == Datum::DatumType::Float);
			Assert::AreEqual((r[0][1][0]).Get<float>(0), 10.0f);

			////////////////////////////////////////////////////////////////////////////

			Scope t;

			Assert::AreEqual(t.Size(), 0_z);

			Scope* testt = &t.AppendScope("TestOne");

			Datum* dt = &(testt->Append("NestedTestOne"));
			*dt = 20;

			Assert::AreEqual(t.Size(), 1_z);
			Assert::IsTrue(t[0][0][0].Type() == Datum::DatumType::Integer);
			Assert::AreEqual((t[0][0][0]).Get<int>(0), 20);

			//	This should Append another scope to the datum paired with key TestOne       Scope -> Datum(Table) -> Scope -> Datum (int) 10
			Scope* testTwot = &t.AppendScope("TestOne");									//	    				  -> Scope -> Datum (float) 10

			//	Expect no changes with the first scope in the second append call
			Assert::AreEqual(t.Size(), 1_z);
			Assert::IsTrue(t[0][0][0].Type() == Datum::DatumType::Integer);
			Assert::AreEqual((t[0][0][0]).Get<int>(0), 20);

			//	The size of the root scope's datum should be size 2 now
			Assert::AreEqual(t[0].Size(), 2_z);

			Datum* dTwot = &(testTwot->Append("NestedTestTwo"));
			*dTwot = 20.0f;

			Assert::IsTrue(t[0][1][0].Type() == Datum::DatumType::Float);
			Assert::AreEqual((t[0][1][0]).Get<float>(0), 20.0f);

			//	s and r are both Scopes containing the same structure and keys
			Assert::IsTrue(s == r);
			Assert::IsFalse(s.operator!=(r));

			//	s and t have the same structure but different values associated with their keys so should not be equal
			Assert::IsFalse(s == t);

			Scope q;

			//	Completely different scopes
			Assert::IsFalse(s == q);

		}

		TEST_METHOD(ExtraCoverage)
		{
			{
				Datum d;
				int arr[5] = { 0, 1, 2, 3, 4 };

				d.SetStorage(arr, 5);

				Scope s;
				Scope* const z = &s;

				Assert::ExpectException<runtime_error>([&s, &d] {d.PushBack(&s); });
				Assert::ExpectException<runtime_error>([&z, &d] {d.PushBack(z); });
			}

			{
				Scope s;

				Assert::AreEqual(s.Size(), 0_z);

				Scope* test = &s.AppendScope("TestOne");

				Datum* d = &(test->operator[]("NestedTestOne"));
				*d = 10;

				Assert::AreEqual(s.Size(), 1_z);
				Assert::IsTrue(s[0][0][0].Type() == Datum::DatumType::Integer);
				Assert::AreEqual((s[0][0][0]).Get<int>(0), 10);

				//	This should Append another scope to the datum paired with key TestOne       Scope -> Datum(Table) -> Scope -> Datum (int) 10
				Scope* testTwo = &s.AppendScope("TestOne");									//	    				  -> Scope -> Datum (float) 10

				//	Expect no changes with the first scope in the second append call
				Assert::AreEqual(s.Size(), 1_z);
				Assert::IsTrue(s[0][0][0].Type() == Datum::DatumType::Integer);
				Assert::AreEqual((s[0][0][0]).Get<int>(0), 10);

				//	The size of the root scope's datum should be size 2 now
				Assert::AreEqual(s[0].Size(), 2_z);

				Datum* dTwo = &(testTwo->Append("NestedTestTwo"));
				*dTwo = 10.0f;

				Assert::IsTrue(s[0][1][0].Type() == Datum::DatumType::Float);
				Assert::AreEqual((s[0][1][0]).Get<float>(0), 10.0f);
				Assert::ExpectException<runtime_error>([&s] {s[20].Size(); });

				Scope q(s);

				Scope c(*testTwo);

				Assert::IsTrue(q == s);
				Assert::IsTrue(c == *testTwo);

				Assert::IsNull(s.GetParent());
			}

			{
				Datum t(Datum::DatumType::Integer);

				Scope s;
				Scope r;
				Scope u;

				Scope* const sp = &s;

				t = sp;

				t.PushBack(&r);
				t.PushBack(&u);

				t.Remove(&r);

				Datum dInt;
				dInt.SetType(Datum::DatumType::Integer);

				dInt.PushBack(10);
				dInt.PushBack(20);

				Assert::ExpectException<runtime_error>([&dInt, &r] {dInt.Remove(&r); });
			}
		}


#pragma endregion
	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState ScopeTests::_startMemState;
}

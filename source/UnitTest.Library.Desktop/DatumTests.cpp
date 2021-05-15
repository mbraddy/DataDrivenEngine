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
	TEST_CLASS(DatumTests)
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

#pragma region Datum Testing
		TEST_METHOD(TestConstructorAndTypeSetting)
		{
			//	Test Defaulting it to Unknown
			Datum d;
			Assert::IsTrue(Datum::DatumType::Unknown == d.Type());

			//	Test Setting type
			d.SetType(Datum::DatumType::Integer);
			Assert::IsTrue(Datum::DatumType::Integer == d.Type());

			//	Test Calling SetType with the same value - Nothing should Change
			d.SetType(Datum::DatumType::Integer);
			Assert::IsTrue(Datum::DatumType::Integer == d.Type());

			//	Attempting to change the type after it has already been set should throw exception
			Assert::ExpectException<runtime_error>([&d] {d.SetType(Datum::DatumType::Float); });

			//	Should default it to type float instead of Unknown
			Datum dFloat(Datum::DatumType::Float);
			Assert::IsTrue(Datum::DatumType::Float == dFloat.Type());

			//	Attempting to change the type after it has already been set should throw exception
			Assert::ExpectException<runtime_error>([&dFloat] {dFloat.SetType(Datum::DatumType::Integer); });

			{
				Datum dInt;
				Datum dString;
				Datum dVec4;
				Datum dMat4;
				Datum dRTTI;

				dInt.SetType(Datum::DatumType::Integer);
				dString.SetType(Datum::DatumType::String);
				dVec4.SetType(Datum::DatumType::Vector);
				dMat4.SetType(Datum::DatumType::Matrix);
				dRTTI.SetType(Datum::DatumType::Pointer);

				//	Testing all Type Set Failures
				Assert::ExpectException<runtime_error>([&dInt] {dInt.SetType(Datum::DatumType::Float); });
				Assert::ExpectException<runtime_error>([&dString] {dString.SetType(Datum::DatumType::Integer); });
				Assert::ExpectException<runtime_error>([&dVec4] {dVec4.SetType(Datum::DatumType::Integer); });
				Assert::ExpectException<runtime_error>([&dMat4] {dMat4.SetType(Datum::DatumType::Integer); });
				Assert::ExpectException<runtime_error>([&dRTTI] {dRTTI.SetType(Datum::DatumType::Integer); });
			}
		}

		TEST_METHOD(TestReserveResizeAndClear)
		{
			Datum dInt;
			Datum dFloat;
			Datum dString;
			Datum dVec4;
			Datum dMat4;
			Datum dRTTI;

			//	Calling Reserve/Resize on an un-typed datum
			Assert::ExpectException<runtime_error>([&dInt] {dInt.Reserve(10); });
			Assert::ExpectException<runtime_error>([&dInt] {dInt.Resize(10); });

			dInt.SetType(Datum::DatumType::Integer);
			dFloat.SetType(Datum::DatumType::Float);
			dString.SetType(Datum::DatumType::String);
			dVec4.SetType(Datum::DatumType::Vector);
			dMat4.SetType(Datum::DatumType::Matrix);
			dRTTI.SetType(Datum::DatumType::Pointer);

			//	Testing Int
			{
				dInt.Resize(5);
				Assert::AreEqual(5_z, dInt.Capacity());
				Assert::AreEqual(5_z, dInt.Size());
				dInt.Resize(15);
				Assert::AreEqual(15_z, dInt.Capacity());
				Assert::AreEqual(15_z, dInt.Size());
				dInt.Resize(10);
				Assert::AreEqual(10_z, dInt.Capacity());
				Assert::AreEqual(10_z, dInt.Size());
				dInt.Clear();
				Assert::AreEqual(10_z, dInt.Capacity());
				Assert::AreEqual(0_z, dInt.Size());
				dInt.Reserve(25);
				Assert::AreEqual(25_z, dInt.Capacity());
				Assert::AreEqual(0_z, dInt.Size());
			}

			//	Testing Float
			{
				dFloat.Resize(5);
				Assert::AreEqual(5_z, dFloat.Capacity());
				Assert::AreEqual(5_z, dFloat.Size());
				dFloat.Resize(15);
				Assert::AreEqual(15_z, dFloat.Capacity());
				Assert::AreEqual(15_z, dFloat.Size());
				dFloat.Resize(10);
				Assert::AreEqual(10_z, dFloat.Capacity());
				Assert::AreEqual(10_z, dFloat.Size());
				dFloat.Clear();
				Assert::AreEqual(10_z, dFloat.Capacity());
				Assert::AreEqual(0_z, dFloat.Size());
				dFloat.Reserve(25);
				Assert::AreEqual(25_z, dFloat.Capacity());
				Assert::AreEqual(0_z, dFloat.Size());
			}

			//	Testing String
			{
				dString.Resize(5);
				Assert::AreEqual(5_z, dString.Capacity());
				Assert::AreEqual(5_z, dString.Size());
				dString.Resize(15);
				Assert::AreEqual(15_z, dString.Capacity());
				Assert::AreEqual(15_z, dString.Size());
				dString.Resize(10);
				Assert::AreEqual(10_z, dString.Capacity());
				Assert::AreEqual(10_z, dString.Size());
				dString.Clear();
				Assert::AreEqual(10_z, dString.Capacity());
				Assert::AreEqual(0_z, dString.Size());
				dString.Reserve(25);
				Assert::AreEqual(25_z, dString.Capacity());
				Assert::AreEqual(0_z, dString.Size());
			}

			//	Testing Vector
			{
				dVec4.Resize(5);
				Assert::AreEqual(5_z, dVec4.Capacity());
				Assert::AreEqual(5_z, dVec4.Size());
				dVec4.Resize(15);
				Assert::AreEqual(15_z, dVec4.Capacity());
				Assert::AreEqual(15_z, dVec4.Size());
				dVec4.Resize(10);
				Assert::AreEqual(10_z, dVec4.Capacity());
				Assert::AreEqual(10_z, dVec4.Size());
				dVec4.Clear();
				Assert::AreEqual(10_z, dVec4.Capacity());
				Assert::AreEqual(0_z, dVec4.Size());
				dVec4.Reserve(25);
				Assert::AreEqual(25_z, dVec4.Capacity());
				Assert::AreEqual(0_z, dVec4.Size());
			}

			//	Testing Matrix
			{
				dMat4.Resize(5);
				Assert::AreEqual(5_z, dMat4.Capacity());
				Assert::AreEqual(5_z, dMat4.Size());
				dMat4.Resize(15);
				Assert::AreEqual(15_z, dMat4.Capacity());
				Assert::AreEqual(15_z, dMat4.Size());
				dMat4.Resize(10);
				Assert::AreEqual(10_z, dMat4.Capacity());
				Assert::AreEqual(10_z, dMat4.Size());
				dMat4.Clear();
				Assert::AreEqual(10_z, dMat4.Capacity());
				Assert::AreEqual(0_z, dMat4.Size());
				dMat4.Reserve(25);
				Assert::AreEqual(25_z, dMat4.Capacity());
				Assert::AreEqual(0_z, dMat4.Size());
			}

			//	Testing RTTI*
			{
				dRTTI.Resize(5);
				Assert::AreEqual(5_z, dRTTI.Capacity());
				Assert::AreEqual(5_z, dRTTI.Size());
				dRTTI.Resize(15);
				Assert::AreEqual(15_z, dRTTI.Capacity());
				Assert::AreEqual(15_z, dRTTI.Size());
				dRTTI.Resize(10);
				Assert::AreEqual(10_z, dRTTI.Capacity());
				Assert::AreEqual(10_z, dRTTI.Size());
				dRTTI.Clear();
				Assert::AreEqual(10_z, dRTTI.Capacity());
				Assert::AreEqual(0_z, dRTTI.Size());
				dRTTI.Reserve(25);
				Assert::AreEqual(25_z, dRTTI.Capacity());
				Assert::AreEqual(0_z, dRTTI.Size());
			}
		}

		TEST_METHOD(TestPushBack)
		{
			Datum dInt(Datum::DatumType::Integer);
			Datum dFloat(Datum::DatumType::Float);
			Datum dString(Datum::DatumType::String);
			Datum dVec4(Datum::DatumType::Vector);
			Datum dMat4(Datum::DatumType::Matrix);
			Datum dRTTI(Datum::DatumType::Pointer);

			//	Int Testing
			{
				const int testEntry = 10;
				const int testEntryTwo = 20;

				Assert::AreEqual(0_z, dInt.Capacity());
				Assert::AreEqual(0_z, dInt.Size());

				dInt.PushBack(testEntry);
				dInt.PushBack(testEntryTwo);
				Assert::AreEqual(2_z, dInt.Capacity());
				Assert::AreEqual(20, dInt.Back<int>());

				dInt.PushBack(30);
				Assert::AreEqual(3_z, dInt.Size());
				Assert::AreEqual(30, dInt.Back<int>());

				Assert::IsTrue(dInt.OwnsData());
			}

			//	Float Testing
			{
				const float testEntry = 10.0f;
				const float testEntryTwo = 20.0f;

				Assert::AreEqual(0_z, dFloat.Capacity());
				Assert::AreEqual(0_z, dFloat.Size());

				dFloat.PushBack(testEntry);
				dFloat.PushBack(testEntryTwo);
				Assert::AreEqual(2_z, dFloat.Capacity());
				Assert::AreEqual(20.0f, dFloat.Back<float>());

				dFloat.PushBack(30.0f);
				Assert::AreEqual(3_z, dFloat.Size());
				Assert::AreEqual(30.0f, dFloat.Back<float>());
			}

			//	String Testing
			{
				const string testEntry = "one";
				const string testEntryTwo = "two";

				Assert::AreEqual(0_z, dString.Capacity());
				Assert::AreEqual(0_z, dString.Size());

				dString.PushBack(testEntry);
				dString.PushBack(testEntryTwo);
				Assert::AreEqual(2_z, dString.Capacity());
				Assert::AreEqual("two"s, dString.Back<string>());

				dString.PushBack("three"s);
				Assert::AreEqual(3_z, dString.Size());
				Assert::AreEqual("three"s, dString.Back<string>());
			}

			//	Matrix Testing
			{
				const mat4x4 testEntry = 
				{ 10 , 10 , 10 , 10,
				  10 , 10 , 10 , 10,
				  10 , 10 , 10 , 10,
				  10 , 10 , 10 , 10 };

				const mat4x4 testEntryTwo =
				{ 20 , 20 , 20 , 20,
				  20 , 20 , 20 , 20,
				  20 , 20 , 20 , 20,
				  20 , 20 , 20 , 20 };

				mat4x4 comparison(30);

				Assert::AreEqual(0_z, dMat4.Capacity());
				Assert::AreEqual(0_z, dMat4.Size());

				dMat4.PushBack(testEntry);
				dMat4.PushBack(testEntryTwo);
				Assert::AreEqual(2_z, dMat4.Capacity());
				Assert::AreEqual(to_string(testEntryTwo), to_string(dMat4.Back<mat4x4>()));

				dMat4.PushBack(static_cast<mat4>(30));
				Assert::AreEqual(3_z, dMat4.Size());
				Assert::AreEqual(to_string(comparison), to_string(dMat4.Back<mat4x4>()));

				dMat4.PushBackFromString("mat4x4((1, 1, 1, 1), (1, 1, 1, 1), (1, 1, 1, 1), (1, 1, 1, 1))"s);
				Assert::AreEqual(4_z, dMat4.Size());
			}
			
			//	Vec4 Testing
			{
				const vec4 testEntry(10.0f);
				const vec4 testEntryTwo(20.0f);

				Assert::AreEqual(0_z, dVec4.Capacity());
				Assert::AreEqual(0_z, dVec4.Size());

				dVec4.PushBack(testEntry);
				dVec4.PushBack(testEntryTwo);
				Assert::AreEqual(2_z, dVec4.Capacity());
				Assert::AreEqual(to_string(vec4(20.0f)), to_string(dVec4.Back<vec4>()));

				dVec4.PushBack(vec4(30.0f));
				Assert::AreEqual(3_z, dVec4.Size());
				Assert::AreEqual(to_string(vec4(30.0f)), to_string(dVec4.Back<vec4>()));
			}

			//	RTTI Testing
			{
				Foo testEntry(10);
				Foo testEntryTwo(20);

				RTTI* rOne = &testEntry;
				RTTI* rTwo = &testEntryTwo;

				Assert::AreEqual(0_z, dRTTI.Capacity());
				Assert::AreEqual(0_z, dRTTI.Size());

				dRTTI.PushBack(rOne);
				dRTTI.PushBack(rTwo);
				Assert::AreEqual(2_z, dRTTI.Capacity());
				Assert::AreEqual(testEntryTwo.ToString(), dRTTI.Back<RTTI*>()->ToString());

				Foo testEntryThree(30);

				dRTTI.PushBack(&testEntryThree);
				Assert::AreEqual(3_z, dRTTI.Size());
				Assert::AreEqual(testEntryThree.ToString(), dRTTI.Back<RTTI*>()->ToString());
			}
		}

		TEST_METHOD(TestRemoves)
		{
			Datum dInt(Datum::DatumType::Integer);
			Datum dFloat(Datum::DatumType::Float);
			Datum dString(Datum::DatumType::String);
			Datum dVec4(Datum::DatumType::Vector);
			Datum dMat4(Datum::DatumType::Matrix);
			Datum dRTTI(Datum::DatumType::Pointer);

			//	Int Testing
			{
				for (int i = 0; i < 10; ++i)
				{
					dInt.PushBack(i * 10);
				}
				
				Assert::AreEqual(10_z, dInt.Size());
				Assert::ExpectException<runtime_error>([&dInt] {dInt.Remove(10.0f); });
				Assert::IsFalse(dInt.RemoveAt(20_z));

				dInt.Remove(50);
				Assert::AreEqual(9_z, dInt.Size());
				dInt.Remove(90);
				Assert::AreEqual(8_z, dInt.Size());
			}

			//	Float Testing
			{
				for (int i = 0; i < 10; ++i)
				{
					dFloat.PushBack(i * 10.0f);
				}

				Assert::AreEqual(10_z, dFloat.Size());
				Assert::ExpectException<runtime_error>([&dFloat] {dFloat.Remove(10); });
				Assert::IsFalse(dFloat.RemoveAt(20_z));

				dFloat.Remove(50.0f);
				Assert::AreEqual(9_z, dFloat.Size());
				dFloat.Remove(90.0f);
				Assert::AreEqual(8_z, dFloat.Size());
			}

			//	String Testing
			{
				for (int i = 0; i < 10; ++i)
				{
					dString.PushBack(to_string(i));
				}

				Assert::AreEqual(10_z, dString.Size());
				Assert::ExpectException<runtime_error>([&dString] {dString.Remove(mat4x4(10)); });
				Assert::IsFalse(dString.RemoveAt(20_z));

				dString.Remove(to_string(static_cast<int>(5)));
				Assert::AreEqual(9_z, dString.Size());
				dString.Remove(to_string(static_cast<int>(9)));
				Assert::AreEqual(8_z, dString.Size());
			}

			//	Matrix Testing
			{
				for (int i = 0; i < 10; ++i)
				{
					dMat4.PushBack(mat4x4(static_cast<float>(i)));
				}

				Assert::AreEqual(10_z, dMat4.Size());
				Assert::ExpectException<runtime_error>([&dMat4] {dMat4.Remove("garbage"s); });
				Assert::IsFalse(dMat4.RemoveAt(20_z));

				dMat4.Remove(mat4x4(static_cast<float>(5)));
				Assert::AreEqual(9_z, dMat4.Size());
				dMat4.Remove(mat4x4(static_cast<float>(9)));
				Assert::AreEqual(8_z, dMat4.Size());
			}

			//	Vec4 Testing
			{
				for (int i = 0; i < 10; ++i)
				{
					dVec4.PushBack(vec4(static_cast<float>(i)));
				}

				Foo f(5);
				RTTI* rFoo = &f;

				Assert::AreEqual(10_z, dVec4.Size());
				Assert::ExpectException<runtime_error>([&dVec4, &rFoo] {dVec4.Remove(rFoo); });
				Assert::IsFalse(dVec4.RemoveAt(20_z));

				dVec4.Remove(vec4(static_cast<float>(5)));
				Assert::AreEqual(9_z, dVec4.Size());
				dVec4.Remove(vec4(static_cast<float>(9)));
				Assert::AreEqual(8_z, dVec4.Size());
			}

			//	RTTI Testing
			{
				Foo testEntry(10);
				Foo testEntryTwo(20);
				Foo testEntryThree(30);

				RTTI* rOne = &testEntry;
				RTTI* rTwo = &testEntryTwo;
				RTTI* rThree = &testEntryThree;

				dRTTI.PushBack(rOne);
				dRTTI.PushBack(rTwo);
				dRTTI.PushBack(rThree);
				Assert::AreEqual(3_z, dRTTI.Size());

				Assert::ExpectException<runtime_error>([&dRTTI] {dRTTI.Remove(vec4(1)); });
				Assert::IsFalse(dRTTI.RemoveAt(20_z));

				dRTTI.Remove(rTwo);
				Assert::AreEqual(2_z, dRTTI.Size());
				dRTTI.Remove(rThree);
				Assert::AreEqual(1_z, dRTTI.Size());

			}
		}

		TEST_METHOD(TestSetStorage)
		{
			Datum dInt(Datum::DatumType::Integer);
			Datum dFloat(Datum::DatumType::Float);
			Datum dString(Datum::DatumType::String);
			Datum dVec4(Datum::DatumType::Vector);
			Datum dMat4(Datum::DatumType::Matrix);
			Datum dRTTI(Datum::DatumType::Pointer);
			Datum d;

			Foo f(0);
			Foo f1(1);
			Foo f2(2);
			Foo f3(3);
			Foo f4(4);

			RTTI* const r = &f;
			RTTI* const r1 = &f1;
			RTTI* const r2 = &f2;
			RTTI* const r3 = &f3;
			RTTI* const r4 = &f4;

			//	For errors to get coverage
			const int int1 = 10;
			const float float1 = 10.0f;
			const string str1 = "ten"s;
			const vec4 vec1 = vec4(10.0f);
			const mat4x4 mat1 = mat4x4(10.0f);

			int intArr[5] = { 0, 1, 2, 3, 4 };
			float floatArr[5] = { 0.0f, 1.0f, 2.0f, 3.0f, 4.0f };
			string strArr[5] = { "0"s, "1"s, "2"s, "3"s, "4"s };
			vec4 vecArr[5] = { vec4(0.0f), vec4(1.0f), vec4(2.0f), vec4(3.0f), vec4(4.0f) };
			mat4x4 matArr[5] = { mat4x4(0.0f), mat4x4(1.0f), mat4x4(2.0f), mat4x4(3.0f), mat4x4(4.0f) };
			RTTI* rttiArr[5] = { r, r1, r2, r3, r4 };

			int* ip = intArr;
			float* fp = floatArr;
			string* sp = strArr;
			vec4* vp = vecArr;
			mat4x4* mp = matArr;
			RTTI** rp = rttiArr;

			Assert::ExpectException<runtime_error>([&dInt, &ip] {dInt.SetStorage(ip, 0); });
			Assert::ExpectException<runtime_error>([&dFloat, &fp] {dFloat.SetStorage(fp, 0); });
			Assert::ExpectException<runtime_error>([&dString, &sp] {dString.SetStorage(sp, 0); });
			Assert::ExpectException<runtime_error>([&dVec4, &vp] {dVec4.SetStorage(vp, 0); });
			Assert::ExpectException<runtime_error>([&dMat4, &mp] {dMat4.SetStorage(mp, 0); });
			Assert::ExpectException<runtime_error>([&dRTTI, &rp] {dRTTI.SetStorage(rp, 0); });

			Assert::ExpectException<runtime_error>([&dInt, &fp] {dInt.SetStorage(fp, 0); });
			Assert::ExpectException<runtime_error>([&dFloat, &ip] {dFloat.SetStorage(ip, 0); });
			Assert::ExpectException<runtime_error>([&dString, &mp] {dString.SetStorage(mp, 0); });
			Assert::ExpectException<runtime_error>([&dVec4, &sp] {dVec4.SetStorage(sp, 0); });
			Assert::ExpectException<runtime_error>([&dMat4, &rp] {dMat4.SetStorage(rp, 0); });
			Assert::ExpectException<runtime_error>([&dRTTI, &vp] {dRTTI.SetStorage(vp, 0); });

			dInt.SetStorage(ip, 5);
			dFloat.SetStorage(fp, 5);
			dString.SetStorage(sp, 5);
			dVec4.SetStorage(vp, 5);
			dMat4.SetStorage(mp, 5);
			dRTTI.SetStorage(rp, 5);

			Assert::IsFalse(dInt.OwnsData());

			//	Testing on untyped
			d.SetStorage(ip, 5);

			Assert::IsTrue(static_cast<int>(d.Type()) == static_cast<int>(Datum::DatumType::Integer));
			Assert::IsTrue(dInt == d);

			//	First all the error calls on methods that modify the unowned data
			Assert::ExpectException<runtime_error>([&dInt] {dInt.Reserve(20_z); });
			Assert::ExpectException<runtime_error>([&dFloat] {dFloat.Reserve(20_z); });
			Assert::ExpectException<runtime_error>([&dString] {dString.Reserve(20_z); });
			Assert::ExpectException<runtime_error>([&dVec4] {dVec4.Reserve(20_z); });
			Assert::ExpectException<runtime_error>([&dMat4] {dMat4.Reserve(20_z); });
			Assert::ExpectException<runtime_error>([&dRTTI] {dRTTI.Reserve(20_z); });

			Assert::ExpectException<runtime_error>([&dInt] {dInt.Resize(20_z); });
			Assert::ExpectException<runtime_error>([&dFloat] {dFloat.Resize(20_z); });
			Assert::ExpectException<runtime_error>([&dString] {dString.Resize(20_z); });
			Assert::ExpectException<runtime_error>([&dVec4] {dVec4.Resize(20_z); });
			Assert::ExpectException<runtime_error>([&dMat4] {dMat4.Resize(20_z); });
			Assert::ExpectException<runtime_error>([&dRTTI] {dRTTI.Resize(20_z); });

			Assert::ExpectException<runtime_error>([&dInt] {dInt.RemoveAt(20_z); });
			Assert::ExpectException<runtime_error>([&dFloat] {dFloat.RemoveAt(20_z); });
			Assert::ExpectException<runtime_error>([&dString] {dString.RemoveAt(20_z); });
			Assert::ExpectException<runtime_error>([&dVec4] {dVec4.RemoveAt(20_z); });
			Assert::ExpectException<runtime_error>([&dMat4] {dMat4.RemoveAt(20_z); });
			Assert::ExpectException<runtime_error>([&dRTTI] {dRTTI.RemoveAt(20_z); });

			Assert::ExpectException<runtime_error>([&dInt] {dInt.PopBack(); });
			Assert::ExpectException<runtime_error>([&dFloat] {dFloat.PopBack(); });
			Assert::ExpectException<runtime_error>([&dString] {dString.PopBack(); });
			Assert::ExpectException<runtime_error>([&dVec4] {dVec4.PopBack(); });
			Assert::ExpectException<runtime_error>([&dMat4] {dMat4.PopBack(); });
			Assert::ExpectException<runtime_error>([&dRTTI] {dRTTI.PopBack(); });

			Assert::ExpectException<runtime_error>([&dInt] {dInt.PushBack(10); });
			Assert::ExpectException<runtime_error>([&dFloat] {dFloat.PushBack(10.0f); });
			Assert::ExpectException<runtime_error>([&dString] {dString.PushBack("garbage"s); });
			Assert::ExpectException<runtime_error>([&dVec4] {dVec4.PushBack(vec4(10.0f)); });
			Assert::ExpectException<runtime_error>([&dMat4] {dMat4.PushBack(mat4x4(10.0f)); });
			Assert::ExpectException<runtime_error>([&dRTTI] {Foo fg(4); RTTI* rg = &fg;  dRTTI.PushBack(std::move(rg)); });

			Assert::ExpectException<runtime_error>([&dInt, &int1] {dInt.PushBack(int1); });
			Assert::ExpectException<runtime_error>([&dFloat, &float1] {dFloat.PushBack(float1); });
			Assert::ExpectException<runtime_error>([&dString, &str1] {dString.PushBack(str1); });
			Assert::ExpectException<runtime_error>([&dVec4, &vec1] {dVec4.PushBack(vec1); });
			Assert::ExpectException<runtime_error>([&dMat4, &mat1] {dMat4.PushBack(mat1); });
			Assert::ExpectException<runtime_error>([&dRTTI, &r4] {dRTTI.PushBack(r4); });

			//	Too big index (Set Coverage while we are here)
			Assert::ExpectException<runtime_error>([&dInt] {dInt.Set(10, 15); });
			Assert::ExpectException<runtime_error>([&dFloat] {dFloat.Set(10.0f, 15); });
			Assert::ExpectException<runtime_error>([&dString] {dString.Set("garbage"s, 15); });
			Assert::ExpectException<runtime_error>([&dVec4] {dVec4.Set(vec4(10.0f), 15); });
			Assert::ExpectException<runtime_error>([&dMat4] {dMat4.Set(mat4x4(10.0f), 15); });
			Assert::ExpectException<runtime_error>([&dRTTI] {Foo fg(4); RTTI* rg = &fg;  dRTTI.Set(std::move(rg), 15); });

			Assert::ExpectException<runtime_error>([&dInt, &int1] {dInt.Set(int1, 15); });
			Assert::ExpectException<runtime_error>([&dFloat, &float1] {dFloat.Set(float1, 15); });
			Assert::ExpectException<runtime_error>([&dString, &str1] {dString.Set(str1, 15); });
			Assert::ExpectException<runtime_error>([&dVec4, &vec1] {dVec4.Set(vec1, 15); });
			Assert::ExpectException<runtime_error>([&dMat4, &mat1] {dMat4.Set(mat1, 15); });
			Assert::ExpectException<runtime_error>([&dRTTI, &r4] {dRTTI.Set(r4, 15); });

			//	Incorrect Typing
			Assert::ExpectException<runtime_error>([&dInt] {dInt.Set(10.0f, 0); });
			Assert::ExpectException<runtime_error>([&dFloat] {dFloat.Set(10, 0); });
			Assert::ExpectException<runtime_error>([&dString] {dString.Set(vec4(10.0f), 0); });
			Assert::ExpectException<runtime_error>([&dVec4] {dVec4.Set("garbage"s, 0); });
			Assert::ExpectException<runtime_error>([&dMat4] {Foo fg(4); RTTI* rg = &fg; dMat4.Set(std::move(rg), 0); });
			Assert::ExpectException<runtime_error>([&dRTTI] {dRTTI.Set(mat4x4(10.0f), 0); });

			Assert::ExpectException<runtime_error>([&dInt, &float1] {dInt.Set(float1, 0); });
			Assert::ExpectException<runtime_error>([&dFloat, &int1] {dFloat.Set(int1, 0); });
			Assert::ExpectException<runtime_error>([&dString, &vec1] {dString.Set(vec1, 0); });
			Assert::ExpectException<runtime_error>([&dVec4, &str1] {dVec4.Set(str1, 0); });
			Assert::ExpectException<runtime_error>([&dMat4, &r4] {dMat4.Set(r4, 0); });
			Assert::ExpectException<runtime_error>([&dRTTI, &mat1] {dRTTI.Set(mat1, 0); });

			dInt.SetStorage(ip, 5);
			dFloat.SetStorage(fp, 5);
			dString.SetStorage(sp, 5);
			dVec4.SetStorage(vp, 5);
			dMat4.SetStorage(mp, 5);
			dRTTI.SetStorage(rp, 5);

			Datum dIntCopy(dInt);
			Datum dFloatCopy(dFloat);
			Datum dStringCopy(dString);
			Datum dVecCopy(dVec4);
			Datum dMatCopy(dMat4);
			Datum dRTTICopy(dRTTI);

			Assert::IsTrue(dInt == dIntCopy);
			Assert::IsTrue(dFloat == dFloatCopy);
			Assert::IsTrue(dString == dStringCopy);
			Assert::IsTrue(dVec4 == dVecCopy);
			Assert::IsTrue(dMat4 == dMatCopy);
			Assert::IsTrue(dRTTI == dRTTICopy);

			Datum dIntOwns(Datum::DatumType::Integer);
			Datum dFloatOwns(Datum::DatumType::Float);
			Datum dStringOwns(Datum::DatumType::String);
			Datum dVecOwns(Datum::DatumType::Vector);
			Datum dMatOwns(Datum::DatumType::Matrix);
			Datum dPointOwns(Datum::DatumType::Pointer);

			dIntOwns.Resize(20);
			dFloatOwns.Resize(20);
			dStringOwns.Resize(20);
			dVecOwns.Resize(20);
			dMatOwns.Resize(20);
			dPointOwns.Resize(20);

			dIntCopy = dIntOwns;
			dFloatCopy = dFloatOwns;
			dStringCopy = dStringOwns;
			dVecCopy = dVecOwns;
			dMatCopy = dMatOwns;
			dRTTICopy = dPointOwns;

			dIntOwns = dInt;
			dFloatOwns = dFloat;
			dStringOwns = dString;
			dVecOwns = dVec4;
			dMatOwns = dMat4;
			dPointOwns = dRTTI;

			dIntCopy = dIntOwns;
			dFloatCopy = dFloatOwns;
			dStringCopy = dStringOwns;
			dVecCopy = dVecOwns;
			dMatCopy = dMatOwns;
			dRTTICopy = dPointOwns;

			//	Neither own the data
			{
				Datum dIntTwo(Datum::DatumType::Integer);
				int intArrTwo[5] = { 4, 2, 6, 4, 7 };
				int* ipTwo = intArrTwo;

				dIntTwo.SetStorage(ipTwo, 5);

				dIntTwo = dInt;

				Assert::IsTrue(dInt == dIntTwo);
			}

		}

		TEST_METHOD(TestCopySemantics)
		{
			//	Testing Copying Ints
			{
				Datum d;
				d.SetType(Datum::DatumType::Integer);
				d.PushBack(10);
				d.PushBack(20);
				d.PushBack(30);

				//	Testing copy constructor (owns storage)
				Datum dCopy(d);

				Assert::IsTrue(d == dCopy);
			}

			//	Testing Copying Strings
			{
				Datum d;
				d.SetType(Datum::DatumType::String);
				d.PushBack("First"s);
				d.PushBack("Second"s);
				d.PushBack("Third"s);

				//	Testing copy constructor (owns storage)
				Datum dCopy(d);

				Assert::IsTrue(d == dCopy);
			}

			//	Some code coverage stuff
			{
				Datum dfloat;
				Datum dfloatTwo;

				dfloat.SetType(Datum::DatumType::Float);
				dfloat.Reserve(5);
				dfloatTwo = dfloat;
				dfloat.ShrinkToFit();
			}

			{
				Datum d(Datum::DatumType::String);
				d.Resize(10);

				Datum dInt(Datum::DatumType::Integer);
				
				for (int i = 0; i < 10; ++i)
				{
					dInt.PushBack(i);
				}

				d = dInt;

				Assert::IsTrue(d == dInt);
			}
		}

		TEST_METHOD(TestMoveSemantics)
		{
			Datum dInt(Datum::DatumType::Integer);
			dInt.PushBack(10);
			dInt.PushBack(20);
			dInt.PushBack(30);

			Datum d(std::move(dInt));

			Assert::AreEqual(0_z, dInt.Size());
			Assert::AreEqual(0_z, dInt.Capacity());
			Assert::AreEqual(static_cast<int>(Datum::DatumType::Unknown), static_cast<int>(dInt.Type()));

			Assert::AreEqual(3_z, d.Size());
			Assert::AreEqual(10, d.Front<int>());
			Assert::AreEqual(30, d.Back<int>());
			Assert::AreEqual(static_cast<int>(Datum::DatumType::Integer), static_cast<int>(d.Type()));

			Datum dTwo;

			dTwo = std::move(d);

			Assert::AreEqual(0_z, d.Size());
			Assert::AreEqual(0_z, d.Capacity());
			Assert::AreEqual(static_cast<int>(Datum::DatumType::Unknown), static_cast<int>(d.Type()));

			Assert::AreEqual(3_z, dTwo.Size());
			Assert::AreEqual(10, dTwo.Front<int>());
			Assert::AreEqual(30, dTwo.Back<int>());
			Assert::AreEqual(static_cast<int>(Datum::DatumType::Integer), static_cast<int>(dTwo.Type()));

		}

		TEST_METHOD(TestOperatorEqualityAndPopBack)
		{
			Datum dIntinit(Datum::DatumType::Integer);
			Datum dIntTwo(Datum::DatumType::Integer);
			Datum dfloat(Datum::DatumType::Float);
			Datum dStringinit(Datum::DatumType::String);

			//	Popback returns false if nothing to pop
			Assert::IsFalse(dIntinit.PopBack());

			dIntinit.PushBack(10);
			dIntinit.PushBack(20);
			dIntinit.PushBack(30);

			dIntTwo.PushBack(30);
			dIntTwo.PushBack(20);
			dIntTwo.PushBack(10);

			Assert::IsFalse(dIntinit == dIntTwo);
			Assert::IsTrue(dIntinit != dIntTwo);
			Assert::IsTrue(dIntinit != dfloat);

			Assert::AreEqual(0_z, dStringinit.Size());
			dStringinit.PushBack("One"s);
			dStringinit.PushBack("Two"s);
			Assert::AreEqual(2_z, dStringinit.Size());
			dStringinit.PopBack();
			Assert::AreEqual(1_z, dStringinit.Size());
			Assert::AreEqual("One"s, dStringinit.Back<string>());

			//	Operator Equality for all Types
			Datum dInt(Datum::DatumType::Integer);
			Datum dFloat(Datum::DatumType::Float);
			Datum dString(Datum::DatumType::String);
			Datum dVec4(Datum::DatumType::Vector);
			Datum dMat4(Datum::DatumType::Matrix);
			Datum dRTTI(Datum::DatumType::Pointer);
			Datum dIntc(Datum::DatumType::Integer);
			Datum dFloatc(Datum::DatumType::Float);
			Datum dStringc(Datum::DatumType::String);
			Datum dVec4c(Datum::DatumType::Vector);
			Datum dMat4c(Datum::DatumType::Matrix);
			Datum dRTTIc(Datum::DatumType::Pointer);

			//	Int Testing
			{
				for (int i = 0; i < 10; ++i)
				{
					dInt.PushBack(i * 10);
				}

				dIntc = dInt;

				Assert::IsTrue(dIntc == dInt);
				Assert::IsFalse(dIntc != dInt);
				Assert::IsTrue(dInt == 0);
				Assert::IsFalse(dInt != 0);
				Assert::IsFalse(dInt.operator==(10.0f));
			}

			//	Float Testing
			{
				for (int i = 0; i < 10; ++i)
				{
					dFloat.PushBack(i * 10.0f);
				}

				dFloatc = dFloat;

				Assert::IsTrue(dFloatc == dFloat);
				Assert::IsFalse(dFloatc != dFloat);
				Assert::IsTrue(dFloat == 0.0f);
				Assert::IsFalse(dFloat != 0.0f);
				Assert::IsFalse(dFloat.operator==(10));
			}

			//	String Testing
			{
				for (int i = 0; i < 10; ++i)
				{
					dString.PushBack(to_string(i));
				}

				dStringc = dString;

				Assert::IsTrue(dStringc == dString);
				Assert::IsFalse(dStringc != dString);
				Assert::IsTrue(dString == to_string(0));
				Assert::IsFalse(dString != to_string(0));
				Assert::IsFalse(dString.operator==(mat4x4(10.0f)));
			}

			//	Matrix Testing
			{
				for (int i = 0; i < 10; ++i)
				{
					dMat4.PushBack(mat4x4(static_cast<float>(i)));
				}

				dMat4c = dMat4;

				Assert::IsTrue(dMat4c == dMat4);
				Assert::IsFalse(dMat4c != dMat4);
				Assert::IsTrue(dMat4 == mat4x4(0.0f));
				Assert::IsFalse(dMat4 != mat4x4(0.0f));
				Assert::IsFalse(dMat4.operator==("garbage"s));
			}

			//	Vec4 Testing
			{
				for (int i = 0; i < 10; ++i)
				{
					dVec4.PushBack(vec4(static_cast<float>(i)));
				}

				dVec4c = dVec4;

				Assert::IsTrue(dVec4c == dVec4);
				Assert::IsFalse(dVec4c != dVec4);
				Assert::IsTrue(dVec4 == vec4(0.0f));
				Assert::IsFalse(dVec4 != vec4(0.0f));

				Foo f(4); 
				RTTI* rf = &f;
				Assert::IsFalse(dVec4.operator==(rf));
			}

			//	RTTI Testing
			{
				Foo testEntry(10);
				Foo testEntryTwo(20);
				Foo testEntryThree(30);

				RTTI* rOne = &testEntry;
				RTTI* rTwo = &testEntryTwo;
				RTTI* rThree = &testEntryThree;

				dRTTI.PushBack(rOne);
				dRTTI.PushBack(rTwo);
				dRTTI.PushBack(rThree);
				Assert::AreEqual(3_z, dRTTI.Size());

				dRTTIc = dRTTI;

				Assert::IsTrue(dRTTIc == dRTTI);
				Assert::IsFalse(dRTTIc != dRTTI);
				Assert::IsTrue(dRTTI == rOne);
				Assert::IsFalse(dRTTI != rOne);
				Assert::IsFalse(dRTTI.operator==(vec4(10.0f)));

				dRTTI = rTwo;
				Assert::AreEqual(1_z, dRTTI.Size());
				Assert::ExpectException<runtime_error>([&dRTTI] {dRTTI.ToString<vec4>(); });
				Assert::ExpectException<runtime_error>([&dRTTI] {dRTTI.ToString<RTTI*>(24); });
				Assert::AreEqual(rTwo->ToString(), dRTTI.ToString<RTTI*>());
			}

			dInt = 5000;
			dFloat = 5000.0f;
			dString = "arbitrary"s;
			dVec4 = vec4(20.0f);
			dMat4 = mat4x4(20.0f);

			Assert::AreEqual(1_z, dInt.Size());
			Assert::AreEqual(1_z, dFloat.Size());
			Assert::AreEqual(1_z, dString.Size());
			Assert::AreEqual(1_z, dMat4.Size());
			Assert::AreEqual(1_z, dVec4.Size());

			Assert::ExpectException<runtime_error>([&dInt] {dInt.ToString<float>(); });
			Assert::ExpectException<runtime_error>([&dFloat] {dFloat.ToString<int>(); });
			Assert::ExpectException<runtime_error>([&dString] {dString.ToString<mat4x4>(); });
			Assert::ExpectException<runtime_error>([&dVec4] {dVec4.ToString<RTTI*>(); });
			Assert::ExpectException<runtime_error>([&dMat4] {dMat4.ToString<string>(); });

			Assert::ExpectException<runtime_error>([&dInt] {dInt.ToString<int>(24); });
			Assert::ExpectException<runtime_error>([&dFloat] {dFloat.ToString<float>(24); });
			Assert::ExpectException<runtime_error>([&dString] {dString.ToString<string>(24); });
			Assert::ExpectException<runtime_error>([&dVec4] {dVec4.ToString<vec4>(24); });
			Assert::ExpectException<runtime_error>([&dMat4] {dMat4.ToString<mat4x4>(24); });

			Assert::AreEqual("5000"s, dInt.ToString<int>());
			Assert::AreEqual(to_string(5000.0f), dFloat.ToString<float>());
			Assert::AreEqual("arbitrary"s, dString.ToString<string>());
			Assert::AreEqual(to_string(mat4x4(20.0f)), dMat4.ToString<mat4x4>());
			Assert::AreEqual(to_string(vec4(20.0f)), dVec4.ToString<vec4>());
		}

		TEST_METHOD(TestStringMethods)
		{
			Datum dInt(Datum::DatumType::Integer);
			Datum dFloat(Datum::DatumType::Float);
			Datum dString(Datum::DatumType::String);
			Datum dVec4(Datum::DatumType::Vector);
			Datum dMat4(Datum::DatumType::Matrix);

			Datum d;

			dInt.Resize(1);
			dFloat.Resize(1);
			dString.Resize(1);
			dVec4.Resize(1);
			dMat4.Resize(1);

			mat4x4 testm(10.0f);
			vec4 testv(10.0f);

			string sm = to_string(testm);
			string sv = to_string(testv);

			dInt.SetFromString("10"s, 0);
			dFloat.SetFromString("10.0"s, 0);
			dVec4.SetFromString(sv, 0);
			dMat4.SetFromString(sm, 0);
			dString.SetFromString("10"s, 0);

			Assert::ExpectException<runtime_error>([&d] {d.SetFromString("fails"s, 0);  });

			Assert::AreEqual(10, dInt.Get<int>(0));
			Assert::AreEqual(10.0f, dFloat.Get<float>(0));
			Assert::AreEqual(to_string(testv), to_string(dVec4.Get<vec4>(0)));
			Assert::AreEqual(to_string(testm), to_string(dMat4.Get<mat4x4>(0)));
			Assert::AreEqual("10"s, dString.Get<string>(0));

		}

		TEST_METHOD(TestGetSet)
		{
			Datum dInt(Datum::DatumType::Integer);
			Datum dFloat(Datum::DatumType::Float);
			Datum dString(Datum::DatumType::String);
			Datum dVec4(Datum::DatumType::Vector);
			Datum dMat4(Datum::DatumType::Matrix);
			Datum dRTTI(Datum::DatumType::Pointer);

			const size_t outOfBounds = 20;
			

			//	Test Int
			{
				const int test = 1;

				dInt.Resize(5);
				Assert::ExpectException<runtime_error>([&dInt] {dInt.Set(20, 20); });
				Assert::ExpectException<runtime_error>([&dInt] {dInt.Set(20.0f, 20); });
				Assert::ExpectException<runtime_error>([&dInt, &outOfBounds] {dInt.Set(20, outOfBounds); });
				dInt.Set(test, 4);
				dInt.Set(2, 3);
				dInt.Set(3, 2);
				dInt.Set(4, 1);
				dInt.Set(5, 0);
				
				Assert::AreEqual(5_z, dInt.Size());
				Assert::AreEqual(5_z, dInt.Capacity());

				Assert::AreEqual(1, dInt.Get<int>(4_z));
				Assert::AreEqual(2, dInt.Get<int>(3_z));
				Assert::AreEqual(3, dInt.Get<int>(2_z));
				Assert::AreEqual(4, dInt.Get<int>(1_z));
				Assert::AreEqual(5, dInt.Get<int>(0_z));
				Assert::ExpectException<runtime_error>([&dInt, &outOfBounds] {dInt.Get<int>(outOfBounds); });
				Assert::ExpectException<runtime_error>([&dInt] {dInt.Get<float>(0_z); });

				const Datum dIntc = dInt;

				Assert::AreEqual(1, dIntc.Get<int>(4_z));
				Assert::AreEqual(2, dIntc.Get<int>(3_z));
				Assert::AreEqual(3, dIntc.Get<int>(2_z));
				Assert::AreEqual(4, dIntc.Get<int>(1_z));
				Assert::AreEqual(5, dIntc.Get<int>(0_z));
				Assert::ExpectException<runtime_error>([&dIntc, &outOfBounds] {dIntc.Get<int>(outOfBounds); });
				Assert::ExpectException<runtime_error>([&dIntc] {dIntc.Get<float>(0_z); });
			}

			//	Test float
			{
				const float test = 1.0f;

				dFloat.Resize(5);
				Assert::ExpectException<runtime_error>([&dFloat] {dFloat.Set(20, 20); });
				Assert::ExpectException<runtime_error>([&dFloat] {dFloat.Set(20.0f, 20); });
				Assert::ExpectException<runtime_error>([&dFloat, &outOfBounds] {dFloat.Set(20, outOfBounds); });
				dFloat.Set(test, 4);
				dFloat.Set(2.0f, 3);
				dFloat.Set(3.0f, 2);
				dFloat.Set(4.0f, 1);
				dFloat.Set(5.0f, 0);

				Assert::AreEqual(5_z, dFloat.Size());
				Assert::AreEqual(5_z, dFloat.Capacity());

				Assert::AreEqual(1.0f, dFloat.Get<float>(4_z));
				Assert::AreEqual(2.0f, dFloat.Get<float>(3_z));
				Assert::AreEqual(3.0f, dFloat.Get<float>(2_z));
				Assert::AreEqual(4.0f, dFloat.Get<float>(1_z));
				Assert::AreEqual(5.0f, dFloat.Get<float>(0_z));
				Assert::ExpectException<runtime_error>([&dFloat, &outOfBounds] {dFloat.Get<float>(outOfBounds); });
				Assert::ExpectException<runtime_error>([&dFloat] {dFloat.Get<int>(0_z); });

				const Datum dFloatc = dFloat;

				Assert::AreEqual(1.0f, dFloatc.Get<float>(4_z));
				Assert::AreEqual(2.0f, dFloatc.Get<float>(3_z));
				Assert::AreEqual(3.0f, dFloatc.Get<float>(2_z));
				Assert::AreEqual(4.0f, dFloatc.Get<float>(1_z));
				Assert::AreEqual(5.0f, dFloatc.Get<float>(0_z));
				Assert::ExpectException<runtime_error>([&dFloatc, &outOfBounds] {dFloatc.Get<float>(outOfBounds); });
				Assert::ExpectException<runtime_error>([&dFloatc] {dFloatc.Get<int>(0_z); });
			}

			//	Test string
			{
				const string test = "one";

				dString.Resize(5);
				Assert::ExpectException<runtime_error>([&dString] {dString.Set(20, 20); });
				Assert::ExpectException<runtime_error>([&dString] {dString.Set("twenty"s, 20); });
				Assert::ExpectException<runtime_error>([&dString, &outOfBounds] {dString.Set(20, outOfBounds); });
				dString.Set(test, 4);
				dString.Set("two"s, 3);
				dString.Set("three"s, 2);
				dString.Set("four"s, 1);
				dString.Set("five"s, 0);

				Assert::AreEqual(5_z, dString.Size());
				Assert::AreEqual(5_z, dString.Capacity());

				Assert::AreEqual("one"s, dString.Get<string>(4_z));
				Assert::AreEqual("two"s, dString.Get<string>(3_z));
				Assert::AreEqual("three"s, dString.Get<string>(2_z));
				Assert::AreEqual("four"s, dString.Get<string>(1_z));
				Assert::AreEqual("five"s, dString.Get<string>(0_z));
				Assert::ExpectException<runtime_error>([&dString, &outOfBounds] {dString.Get<string>(outOfBounds); });
				Assert::ExpectException<runtime_error>([&dString] {dString.Get<vec4>(0_z); });

				const Datum dStringc = dString;

				Assert::AreEqual("one"s, dStringc.Get<string>(4_z));
				Assert::AreEqual("two"s, dStringc.Get<string>(3_z));
				Assert::AreEqual("three"s, dStringc.Get<string>(2_z));
				Assert::AreEqual("four"s, dStringc.Get<string>(1_z));
				Assert::AreEqual("five"s, dStringc.Get<string>(0_z));
				Assert::ExpectException<runtime_error>([&dStringc, &outOfBounds] {dStringc.Get<string>(outOfBounds); });
				Assert::ExpectException<runtime_error>([&dStringc] {dStringc.Get<vec4>(0_z); });
			}

			//	Test Vector
			{
				const vec4 test(1);

				dVec4.Resize(5);
				Assert::ExpectException<runtime_error>([&dVec4] {dVec4.Set(20, 20); });
				Assert::ExpectException<runtime_error>([&dVec4] {dVec4.Set(vec4(20), 20); });
				Assert::ExpectException<runtime_error>([&dVec4, &outOfBounds] {dVec4.Set(20, outOfBounds); });
				dVec4.Set(test, 4);
				dVec4.Set(vec4(2), 3);
				dVec4.Set(vec4(3), 2);
				dVec4.Set(vec4(4), 1);
				dVec4.Set(vec4(5), 0);

				Assert::AreEqual(5_z, dVec4.Size());
				Assert::AreEqual(5_z, dVec4.Capacity());

				Assert::AreEqual(to_string(vec4(1)), to_string(dVec4.Get<vec4>(4_z)));
				Assert::AreEqual(to_string(vec4(2)), to_string(dVec4.Get<vec4>(3_z)));
				Assert::AreEqual(to_string(vec4(3)), to_string(dVec4.Get<vec4>(2_z)));
				Assert::AreEqual(to_string(vec4(4)), to_string(dVec4.Get<vec4>(1_z)));
				Assert::AreEqual(to_string(vec4(5)), to_string(dVec4.Get<vec4>(0_z)));
				Assert::ExpectException<runtime_error>([&dVec4, &outOfBounds] {dVec4.Get<vec4>(outOfBounds); });
				Assert::ExpectException<runtime_error>([&dVec4] {dVec4.Get<string>(0_z); });

				const Datum dVec4c = dVec4;

				Assert::AreEqual(to_string(vec4(1)), to_string(dVec4c.Get<vec4>(4_z)));
				Assert::AreEqual(to_string(vec4(2)), to_string(dVec4c.Get<vec4>(3_z)));
				Assert::AreEqual(to_string(vec4(3)), to_string(dVec4c.Get<vec4>(2_z)));
				Assert::AreEqual(to_string(vec4(4)), to_string(dVec4c.Get<vec4>(1_z)));
				Assert::AreEqual(to_string(vec4(5)), to_string(dVec4c.Get<vec4>(0_z)));
				Assert::ExpectException<runtime_error>([&dVec4c, &outOfBounds] {dVec4c.Get<vec4>(outOfBounds); });
				Assert::ExpectException<runtime_error>([&dVec4c] {dVec4c.Get<string>(0_z); });
			}

			//	Test Matrix
			{
				const mat4x4 test(1);

				dMat4.Resize(5);
				Assert::ExpectException<runtime_error>([&dMat4] {dMat4.Set(20, 20); });
				Assert::ExpectException<runtime_error>([&dMat4] {dMat4.Set(mat4x4(20), 20); });
				Assert::ExpectException<runtime_error>([&dMat4, &outOfBounds] {dMat4.Set(20, outOfBounds); });
				dMat4.Set(test, 4);
				dMat4.Set(mat4x4(2), 3);
				dMat4.Set(mat4x4(3), 2);
				dMat4.Set(mat4x4(4), 1);
				dMat4.Set(mat4x4(5), 0);

				Assert::AreEqual(5_z, dMat4.Size());
				Assert::AreEqual(5_z, dMat4.Capacity());

				Assert::AreEqual(to_string(mat4x4(1)), to_string(dMat4.Get<mat4x4>(4_z)));
				Assert::AreEqual(to_string(mat4x4(2)), to_string(dMat4.Get<mat4x4>(3_z)));
				Assert::AreEqual(to_string(mat4x4(3)), to_string(dMat4.Get<mat4x4>(2_z)));
				Assert::AreEqual(to_string(mat4x4(4)), to_string(dMat4.Get<mat4x4>(1_z)));
				Assert::AreEqual(to_string(mat4x4(5)), to_string(dMat4.Get<mat4x4>(0_z)));
				Assert::ExpectException<runtime_error>([&dMat4, &outOfBounds] {dMat4.Get<mat4x4>(outOfBounds); });
				Assert::ExpectException<runtime_error>([&dMat4] {dMat4.Get<RTTI*>(0_z); });

				const Datum dMat4c = dMat4;

				Assert::AreEqual(to_string(mat4x4(1)), to_string(dMat4c.Get<mat4x4>(4_z)));
				Assert::AreEqual(to_string(mat4x4(2)), to_string(dMat4c.Get<mat4x4>(3_z)));
				Assert::AreEqual(to_string(mat4x4(3)), to_string(dMat4c.Get<mat4x4>(2_z)));
				Assert::AreEqual(to_string(mat4x4(4)), to_string(dMat4c.Get<mat4x4>(1_z)));
				Assert::AreEqual(to_string(mat4x4(5)), to_string(dMat4c.Get<mat4x4>(0_z)));
				Assert::ExpectException<runtime_error>([&dMat4c, &outOfBounds] {dMat4c.Get<mat4x4>(outOfBounds); });
				Assert::ExpectException<runtime_error>([&dMat4c] {dMat4c.Get<RTTI*>(0_z); });
			}

			//	Test Pointer
			{
				Foo testFoo(1);
				Foo testFoo2(2);
				Foo testFoo3(3);
				Foo testFoo4(4);
				Foo testFoo5(5);

				RTTI* const rFoo = &testFoo;
				RTTI* const rFoo2 = &testFoo2;
				RTTI* const rFoo3 = &testFoo3;
				RTTI* const rFoo4 = &testFoo4;

				RTTI* rFooMove = &testFoo5;

				dRTTI.Resize(5);
				Assert::ExpectException<runtime_error>([&dRTTI] {dRTTI.Set(20, 20); });
				Assert::ExpectException<runtime_error>([&dRTTI, &rFoo] {dRTTI.Set(rFoo, 20); });
				Assert::ExpectException<runtime_error>([&dRTTI, &outOfBounds] {dRTTI.Set(20, outOfBounds); });
				dRTTI.Set(rFoo, 4);
				dRTTI.Set(rFoo2, 3);
				dRTTI.Set(rFoo3, 2);
				dRTTI.Set(rFoo4, 1);
				dRTTI.Set(std::move(rFooMove), 0);

				Assert::AreEqual(5_z, dRTTI.Size());
				Assert::AreEqual(5_z, dRTTI.Capacity());

				Assert::AreEqual(rFoo->ToString(), dRTTI.Get<RTTI*>(4_z)->ToString());
				Assert::AreEqual(rFoo2->ToString(), dRTTI.Get<RTTI*>(3_z)->ToString());
				Assert::AreEqual(rFoo3->ToString(), dRTTI.Get<RTTI*>(2_z)->ToString());
				Assert::AreEqual(rFoo4->ToString(), dRTTI.Get<RTTI*>(1_z)->ToString());
				Assert::AreEqual(testFoo5.ToString(), dRTTI.Get<RTTI*>(0_z)->ToString());
				Assert::ExpectException<runtime_error>([&dRTTI, &outOfBounds] {dRTTI.Get<RTTI*>(outOfBounds); });
				Assert::ExpectException<runtime_error>([&dRTTI] {dRTTI.Get<mat4x4>(0_z); });

				const Datum dRTTIc = dRTTI;

				Assert::AreEqual(rFoo->ToString(), dRTTIc.Get<RTTI*>(4_z)->ToString());
				Assert::AreEqual(rFoo2->ToString(), dRTTIc.Get<RTTI*>(3_z)->ToString());
				Assert::AreEqual(rFoo3->ToString(), dRTTIc.Get<RTTI*>(2_z)->ToString());
				Assert::AreEqual(rFoo4->ToString(), dRTTIc.Get<RTTI*>(1_z)->ToString());
				Assert::AreEqual(testFoo5.ToString(), dRTTIc.Get<RTTI*>(0_z)->ToString());
				Assert::ExpectException<runtime_error>([&dRTTIc, &outOfBounds] {dRTTIc.Get<RTTI*>(outOfBounds); });
				Assert::ExpectException<runtime_error>([&dRTTIc] {dRTTIc.Get<mat4x4>(0_z); });
			}
		}

		TEST_METHOD(TestIndexOfMethods)
		{
			{
				Datum dInt(Datum::DatumType::Integer);
				Datum dFloat(Datum::DatumType::Float);
				Datum dString(Datum::DatumType::String);
				Datum dVec4(Datum::DatumType::Vector);
				Datum dMat4(Datum::DatumType::Matrix);
				Datum dRTTI(Datum::DatumType::Pointer);

				Foo testFoo(10);
				RTTI* rFoo = &testFoo;

				Foo testFooTwo(20);
				RTTI* rFooTwo = &testFooTwo;

				Foo testFooThree(30);
				RTTI* rFooThree = &testFooThree;

				dInt.PushBack(10);
				dFloat.PushBack(10.0f);
				dString.PushBack("ten"s);
				dVec4.PushBack(vec4(10));
				dMat4.PushBack(mat4x4(10));
				dRTTI.PushBack(rFoo);

				dInt.PushBack(20);
				dFloat.PushBack(20.0f);
				dString.PushBack("twenty"s);
				dVec4.PushBack(vec4(20));
				dMat4.PushBack(mat4x4(20));
				dRTTI.PushBack(rFooTwo);

				Assert::AreEqual(0_z, dInt.IndexOf(10));
				Assert::AreEqual(0_z, dFloat.IndexOf(10.0f));
				Assert::AreEqual(0_z, dString.IndexOf("ten"s));
				Assert::AreEqual(0_z, dVec4.IndexOf(vec4(10)));
				Assert::AreEqual(0_z, dMat4.IndexOf(mat4x4(10)));
				Assert::AreEqual(0_z, dRTTI.IndexOf(&testFoo));

				Assert::ExpectException<runtime_error>([&dInt] { dInt.IndexOf(10.0f); });
				Assert::ExpectException<runtime_error>([&dFloat] { dFloat.IndexOf(10); });
				Assert::ExpectException<runtime_error>([&dString, &rFooTwo] { dString.IndexOf(rFooTwo); });
				Assert::ExpectException<runtime_error>([&dVec4] { dVec4.IndexOf("twenty"s); });
				Assert::ExpectException<runtime_error>([&dMat4] { dMat4.IndexOf(vec4(10)); });
				Assert::ExpectException<runtime_error>([&dRTTI] { dRTTI.IndexOf(mat4x4(10)); });

				Assert::AreEqual(dInt.Size(), dInt.IndexOf(100));
				Assert::AreEqual(dFloat.Size(), dFloat.IndexOf(100.0f));
				Assert::AreEqual(dString.Size(), dString.IndexOf("tenten"s));
				Assert::AreEqual(dVec4.Size(), dVec4.IndexOf(vec4(100)));
				Assert::AreEqual(dMat4.Size(), dMat4.IndexOf(mat4x4(100)));
				Assert::AreEqual(dRTTI.Size(), dRTTI.IndexOf(rFooThree));
			}

			//	Testing const indexOf().
			{
				Datum dI(Datum::DatumType::Integer);
				Datum dF(Datum::DatumType::Float);
				Datum dS(Datum::DatumType::String);
				Datum dV(Datum::DatumType::Vector);
				Datum dM(Datum::DatumType::Matrix);
				Datum dR(Datum::DatumType::Pointer);

				Foo testFoo(10);
				RTTI* rFoo = &testFoo;

				Foo testFooTwo(20);
				RTTI* rFooTwo = &testFooTwo;

				Foo testFooThree(30);
				RTTI* rFooThree = &testFooThree;

				dI.PushBack(10);
				dF.PushBack(10.0f);
				dS.PushBack("ten"s);
				dV.PushBack(vec4(10));
				dM.PushBack(mat4x4(10));
				dR.PushBack(rFoo);

				dI.PushBack(20);
				dF.PushBack(20.0f);
				dS.PushBack("twenty"s);
				dV.PushBack(vec4(20));
				dM.PushBack(mat4x4(20));
				dR.PushBack(rFooTwo);

				const Datum dInt = dI;
				const Datum dFloat = dF;
				const Datum dString = dS;
				const Datum dVec4 = dV;
				const Datum dMat4 = dM;
				const Datum dRTTI = dR;

				Assert::AreEqual(0_z, dInt.IndexOf(10));
				Assert::AreEqual(0_z, dFloat.IndexOf(10.0f));
				Assert::AreEqual(0_z, dString.IndexOf("ten"s));
				Assert::AreEqual(0_z, dVec4.IndexOf(vec4(10)));
				Assert::AreEqual(0_z, dMat4.IndexOf(mat4x4(10)));
				Assert::AreEqual(0_z, dRTTI.IndexOf(&testFoo));

				Assert::ExpectException<runtime_error>([&dInt] { dInt.IndexOf(10.0f); });
				Assert::ExpectException<runtime_error>([&dFloat] { dFloat.IndexOf(10); });
				Assert::ExpectException<runtime_error>([&dString, &rFooTwo] { dString.IndexOf(rFooTwo); });
				Assert::ExpectException<runtime_error>([&dVec4] { dVec4.IndexOf("twenty"s); });
				Assert::ExpectException<runtime_error>([&dMat4] { dMat4.IndexOf(vec4(10)); });
				Assert::ExpectException<runtime_error>([&dRTTI] { dRTTI.IndexOf(mat4x4(10)); });

				Assert::AreEqual(dInt.Size(), dInt.IndexOf(100));
				Assert::AreEqual(dFloat.Size(), dFloat.IndexOf(100.0f));
				Assert::AreEqual(dString.Size(), dString.IndexOf("tenten"s));
				Assert::AreEqual(dVec4.Size(), dVec4.IndexOf(vec4(100)));
				Assert::AreEqual(dMat4.Size(), dMat4.IndexOf(mat4x4(100)));
				Assert::AreEqual(dRTTI.Size(), dRTTI.IndexOf(rFooThree));
			}
		}

		TEST_METHOD(TestFrontMethods)
		{
			//	Testing Calling Front on empty Datums
			{
				const Datum dInt(Datum::DatumType::Integer);
				const Datum dFloat(Datum::DatumType::Float);
				const Datum dString(Datum::DatumType::String);
				const Datum dVec4(Datum::DatumType::Vector);
				const Datum dMat4(Datum::DatumType::Matrix);
				const Datum dRTTI(Datum::DatumType::Pointer);

				Assert::ExpectException<runtime_error>([&dInt] { dInt.Front<int>(); });
				Assert::ExpectException<runtime_error>([&dFloat] { dFloat.Front<float>(); });
				Assert::ExpectException<runtime_error>([&dString] { dString.Front<string>(); });
				Assert::ExpectException<runtime_error>([&dVec4] { dVec4.Front<vec4>(); });
				Assert::ExpectException<runtime_error>([&dMat4] { dMat4.Front<mat4x4>(); });
				Assert::ExpectException<runtime_error>([&dRTTI] { dRTTI.Front<RTTI*>(); });

			}

			//	Testing Calling Front on empty Datums
			{
				Datum dInt(Datum::DatumType::Integer);
				Datum dFloat(Datum::DatumType::Float);
				Datum dString(Datum::DatumType::String);
				Datum dVec4(Datum::DatumType::Vector);
				Datum dMat4(Datum::DatumType::Matrix);
				Datum dRTTI(Datum::DatumType::Pointer);

				Assert::ExpectException<runtime_error>([&dInt] { dInt.Front<int>(); });
				Assert::ExpectException<runtime_error>([&dFloat] { dFloat.Front<float>(); });
				Assert::ExpectException<runtime_error>([&dString] { dString.Front<string>(); });
				Assert::ExpectException<runtime_error>([&dVec4] { dVec4.Front<vec4>(); });
				Assert::ExpectException<runtime_error>([&dMat4] { dMat4.Front<mat4x4>(); });
				Assert::ExpectException<runtime_error>([&dRTTI] { dRTTI.Front<RTTI*>(); });

			}

			//	Testing Front() after pushback.
			{
				Datum dInt(Datum::DatumType::Integer);
				Datum dFloat(Datum::DatumType::Float);
				Datum dString(Datum::DatumType::String);
				Datum dVec4(Datum::DatumType::Vector);
				Datum dMat4(Datum::DatumType::Matrix);
				Datum dRTTI(Datum::DatumType::Pointer);

				Foo testFoo(10);
				RTTI* rFoo = &testFoo;

				Foo testFooTwo(20);
				RTTI* rFooTwo = &testFooTwo;

				dInt.PushBack(10);
				dFloat.PushBack(10.0f);
				dString.PushBack("ten"s);
				dVec4.PushBack(vec4(10));
				dMat4.PushBack(mat4x4(10));
				dRTTI.PushBack(rFoo);

				dInt.PushBack(20);
				dFloat.PushBack(20.0f);
				dString.PushBack("twenty");
				dVec4.PushBack(vec4(20));
				dMat4.PushBack(mat4x4(20));
				dRTTI.PushBack(rFooTwo);

				Assert::AreEqual(to_string(10), to_string(dInt.Front<int>()));
				Assert::AreEqual(to_string(10.0f), to_string(dFloat.Front<float>()));
				Assert::AreEqual("ten"s, dString.Front<string>());
				Assert::AreEqual(to_string(vec4(10)), to_string(dVec4.Front<vec4>()));
				Assert::AreEqual(to_string(mat4x4(10)), to_string(dMat4.Front<mat4x4>()));
				Assert::AreEqual(rFoo->ToString(), dRTTI.Front<RTTI*>()->ToString());

				Assert::ExpectException<runtime_error>([&dInt] { dInt.Front<float>(); });
				Assert::ExpectException<runtime_error>([&dFloat] { dFloat.Front<int>(); });
				Assert::ExpectException<runtime_error>([&dString] { dString.Front<RTTI*>(); });
				Assert::ExpectException<runtime_error>([&dVec4] { dVec4.Front<string>(); });
				Assert::ExpectException<runtime_error>([&dMat4] { dMat4.Front<vec4>(); });
				Assert::ExpectException<runtime_error>([&dRTTI] { dRTTI.Front<mat4x4>(); });
			}

			//	Testing Front() after pushback.
			{
				Datum dI(Datum::DatumType::Integer);
				Datum dF(Datum::DatumType::Float);
				Datum dS(Datum::DatumType::String);
				Datum dV(Datum::DatumType::Vector);
				Datum dM(Datum::DatumType::Matrix);
				Datum dR(Datum::DatumType::Pointer);

				Foo testFoo(10);
				RTTI* rFoo = &testFoo;

				Foo testFooTwo(20);
				RTTI* rFooTwo = &testFooTwo;

				dI.PushBack(10);
				dF.PushBack(10.0f);
				dS.PushBack("ten"s);
				dV.PushBack(vec4(10));
				dM.PushBack(mat4x4(10));
				dR.PushBack(rFoo);

				dI.PushBack(20);
				dF.PushBack(20.0f);
				dS.PushBack("twenty"s);
				dV.PushBack(vec4(20));
				dM.PushBack(mat4x4(20));
				dR.PushBack(rFooTwo);

				const Datum dInt = dI;
				const Datum dFloat = dF;
				const Datum dString = dS;
				const Datum dVec4 = dV;
				const Datum dMat4 = dM;
				const Datum dRTTI = dR;

				Assert::AreEqual(to_string(10), to_string(dInt.Front<int>()));
				Assert::AreEqual(to_string(10.0f), to_string(dFloat.Front<float>()));
				Assert::AreEqual("ten"s, dString.Front<string>());
				Assert::AreEqual(to_string(vec4(10)), to_string(dVec4.Front<vec4>()));
				Assert::AreEqual(to_string(mat4x4(10)), to_string(dMat4.Front<mat4x4>()));
				Assert::AreEqual(rFoo->ToString(), dRTTI.Front<RTTI*>()->ToString());

				Assert::ExpectException<runtime_error>([&dInt] { dInt.Front<float>(); });
				Assert::ExpectException<runtime_error>([&dFloat] { dFloat.Front<int>(); });
				Assert::ExpectException<runtime_error>([&dString] { dString.Front<RTTI*>(); });
				Assert::ExpectException<runtime_error>([&dVec4] { dVec4.Front<string>(); });
				Assert::ExpectException<runtime_error>([&dMat4] { dMat4.Front<vec4>(); });
				Assert::ExpectException<runtime_error>([&dRTTI] { dRTTI.Front<mat4x4>(); });
			}
		}

		TEST_METHOD(TestBackMethods)
		{
			//	Testing Calling Back on empty Datums
			{
				const Datum dInt(Datum::DatumType::Integer);
				const Datum dFloat(Datum::DatumType::Float);
				const Datum dString(Datum::DatumType::String);
				const Datum dVec4(Datum::DatumType::Vector);
				const Datum dMat4(Datum::DatumType::Matrix);
				const Datum dRTTI(Datum::DatumType::Pointer);

				Assert::ExpectException<runtime_error>([&dInt] { dInt.Back<int>(); });
				Assert::ExpectException<runtime_error>([&dFloat] { dFloat.Back<float>(); });
				Assert::ExpectException<runtime_error>([&dString] { dString.Back<string>(); });
				Assert::ExpectException<runtime_error>([&dVec4] { dVec4.Back<vec4>(); });
				Assert::ExpectException<runtime_error>([&dMat4] { dMat4.Back<mat4x4>(); });
				Assert::ExpectException<runtime_error>([&dRTTI] { dRTTI.Back<RTTI*>(); });

			}

			//	Testing Calling Back on empty Datums
			{
				Datum dInt(Datum::DatumType::Integer);
				Datum dFloat(Datum::DatumType::Float);
				Datum dString(Datum::DatumType::String);
				Datum dVec4(Datum::DatumType::Vector);
				Datum dMat4(Datum::DatumType::Matrix);
				Datum dRTTI(Datum::DatumType::Pointer);

				Assert::ExpectException<runtime_error>([&dInt] { dInt.Back<int>(); });
				Assert::ExpectException<runtime_error>([&dFloat] { dFloat.Back<float>(); });
				Assert::ExpectException<runtime_error>([&dString] { dString.Back<string>(); });
				Assert::ExpectException<runtime_error>([&dVec4] { dVec4.Back<vec4>(); });
				Assert::ExpectException<runtime_error>([&dMat4] { dMat4.Back<mat4x4>(); });
				Assert::ExpectException<runtime_error>([&dRTTI] { dRTTI.Back<RTTI*>(); });
				
			}

			//	Testing Back() after pushback.
			{
				Datum dInt(Datum::DatumType::Integer);
				Datum dFloat(Datum::DatumType::Float);
				Datum dString(Datum::DatumType::String);
				Datum dVec4(Datum::DatumType::Vector);
				Datum dMat4(Datum::DatumType::Matrix);
				Datum dRTTI(Datum::DatumType::Pointer);

				Foo testFoo(10);
				RTTI* rFoo = &testFoo;

				dInt.PushBack(10);
				dFloat.PushBack(10.0f);
				dString.PushBack("ten");
				dVec4.PushBack(vec4(10));
				dMat4.PushBack(mat4x4(10));
				dRTTI.PushBack(rFoo);

				Assert::AreEqual(to_string(10), to_string(dInt.Back<int>()));
				Assert::AreEqual(to_string(10.0f), to_string(dFloat.Back<float>()));
				Assert::AreEqual("ten"s, dString.Back<string>());
				Assert::AreEqual(to_string(vec4(10)), to_string(dVec4.Back<vec4>()));
				Assert::AreEqual(to_string(mat4x4(10)), to_string(dMat4.Back<mat4x4>()));
				Assert::AreEqual(rFoo->ToString(), dRTTI.Back<RTTI*>()->ToString());
				
				Assert::ExpectException<runtime_error>([&dInt] { dInt.Back<float>(); });
				Assert::ExpectException<runtime_error>([&dFloat] { dFloat.Back<int>(); });
				Assert::ExpectException<runtime_error>([&dString] { dString.Back<RTTI*>(); });
				Assert::ExpectException<runtime_error>([&dVec4] { dVec4.Back<string>(); });
				Assert::ExpectException<runtime_error>([&dMat4] { dMat4.Back<vec4>(); });
				Assert::ExpectException<runtime_error>([&dRTTI] { dRTTI.Back<mat4x4>(); });
			}

			//	Testing Back() after pushback.
			{
				Datum dI(Datum::DatumType::Integer);
				Datum dF(Datum::DatumType::Float);
				Datum dS(Datum::DatumType::String);
				Datum dV(Datum::DatumType::Vector);
				Datum dM(Datum::DatumType::Matrix);
				Datum dR(Datum::DatumType::Pointer);

				Foo testFoo(10);
				RTTI* rFoo = &testFoo;

				dI.PushBack(10);
				dF.PushBack(10.0f);
				dS.PushBack("ten"s);
				dV.PushBack(vec4(10));
				dM.PushBack(mat4x4(10));
				dR.PushBack(rFoo);

				const Datum dInt = dI;
				const Datum dFloat = dF;
				const Datum dString = dS;
				const Datum dVec4 = dV;
				const Datum dMat4 = dM;
				const Datum dRTTI = dR;

				Assert::AreEqual(to_string(10), to_string(dInt.Back<int>()));
				Assert::AreEqual(to_string(10.0f), to_string(dFloat.Back<float>()));
				Assert::AreEqual("ten"s, dString.Back<string>());
				Assert::AreEqual(to_string(vec4(10)), to_string(dVec4.Back<vec4>()));
				Assert::AreEqual(to_string(mat4x4(10)), to_string(dMat4.Back<mat4x4>()));
				Assert::AreEqual(rFoo->ToString(), dRTTI.Back<RTTI*>()->ToString());

				Assert::ExpectException<runtime_error>([&dInt] { dInt.Back<float>(); });
				Assert::ExpectException<runtime_error>([&dFloat] { dFloat.Back<int>(); });
				Assert::ExpectException<runtime_error>([&dString] { dString.Back<RTTI*>(); });
				Assert::ExpectException<runtime_error>([&dVec4] { dVec4.Back<string>(); });
				Assert::ExpectException<runtime_error>([&dMat4] { dMat4.Back<vec4>(); });
				Assert::ExpectException<runtime_error>([&dRTTI] { dRTTI.Back<mat4x4>(); });
			}
		}

		TEST_METHOD(RTTIcoverageCompletion)
		{
			Bar b;

			const string s = "Foo"s;

			b.QueryInterface(0);
			b.Is(0);
			b.Is(s);

			RTTI* const q = &b;

			q->Is(s);
			q->ToString();
			q->Equals(q);

			Datum d;
			d = 10;

			Datum f;
			f = 10.0f;

			Assert::ExpectException<runtime_error>([&f] {f.PushBack(10); });
			Assert::ExpectException<runtime_error>([&d] {d.PushBack(10.0f); });
			Assert::ExpectException<runtime_error>([&d] {mat4 m;  d.PushBack(m); });
			Assert::ExpectException<runtime_error>([&d] {vec4 v;  d.PushBack(v); });
			Assert::ExpectException<runtime_error>([&d] {string s; d.PushBack(s); });
			Assert::ExpectException<runtime_error>([&d] {string s; d.PushBack(std::move(s)); });

		}

		TEST_METHOD(TypeSafetyChecks)
		{
			Datum dRTTI;
			Datum dScope;
			Datum dInt;
			Datum dFloat;
			Datum dVec;
			Datum dMat;

			dRTTI.SetType(Datum::DatumType::Pointer);
			dScope.SetType(Datum::DatumType::Table);
			dInt.SetType(Datum::DatumType::Integer);
			dFloat.SetType(Datum::DatumType::Float);
			dVec.SetType(Datum::DatumType::Vector);
			dMat.SetType(Datum::DatumType::Matrix);

			Scope scopeLValue;
			RTTI* rttiValue = &scopeLValue;
			int intLValue = 10;
			float floatLValue = 10.0f;
			vec4 vecLValue = { 10,10,10,10 };
			mat4x4 matLValue = { 10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10 };
			int intArr[5] = { 10, 10, 10, 10, 10 };

			const Scope t;

			{
				Assert::ExpectException<std::runtime_error>([&dRTTI, &scopeLValue] { dRTTI.PushBack(scopeLValue); });
				Assert::ExpectException<std::runtime_error>([&dInt, &rttiValue] { dInt.PushBack(rttiValue); });
				Assert::ExpectException<std::runtime_error>([&dScope, &intLValue] { dScope.PushBack(intLValue); });
				Assert::ExpectException<std::runtime_error>([&dInt, &floatLValue] { dInt.PushBack(floatLValue); });
				Assert::ExpectException<std::runtime_error>([&dFloat, &intLValue] { dFloat.PushBack(intLValue); });
				Assert::ExpectException<std::runtime_error>([&dVec, &intLValue] { dVec.PushBack(intLValue); });
				Assert::ExpectException<std::runtime_error>([&dMat, &intLValue] { dMat.PushBack(intLValue); });
			}

			Datum externalStorage;
			externalStorage.SetStorage(intArr, 5);

			{
				Assert::ExpectException<std::runtime_error>([&scopeLValue, &externalStorage] { externalStorage.PushBack(scopeLValue); });
			}

			{
				Scope temp;
				size_t index = dScope.IndexOf(&scopeLValue);
				Assert::IsTrue(index == dScope.Size());

				Assert::ExpectException<std::runtime_error>([&dRTTI, &scopeLValue] { dRTTI.IndexOf(&scopeLValue); });

				dScope.PushBack(scopeLValue);
				index = dScope.IndexOf(&scopeLValue);
				Assert::IsTrue(index == 0_z);
				Assert::IsFalse(index == dScope.Size());

				index = dScope.IndexOf(&temp);
				Assert::IsTrue(index == dScope.Size());

				Assert::ExpectException<std::runtime_error>([&dRTTI] { dRTTI.Get<Scope*>(10); });

				dInt.PushBack(10);
				Assert::ExpectException<std::runtime_error>([&dInt] { dInt.Get<Scope*>(0); });

				const Datum dIntCopy(dInt);
				const Datum dScopeCopy(dScope);

				Assert::ExpectException<std::runtime_error>([&dIntCopy] { dIntCopy.Get<Scope*>(10); });
				Assert::ExpectException<std::runtime_error>([&dIntCopy] { dIntCopy.Get<Scope*>(0); });

				dScopeCopy.Get<Scope*>(0);
				
				Assert::ExpectException<std::runtime_error>([&dInt, &t] { dInt.Set(t, 10); });
				Assert::ExpectException<std::runtime_error>([&dInt, &t] { dInt.Set(t, 0); });

				Datum dScopeTest;
				dScopeTest.SetType(Datum::DatumType::Table);
				dScopeTest.Resize(5);

				dScopeTest.Set(t, 0);

				Assert::ExpectException<std::runtime_error>([&dInt, &scopeLValue] { dInt.Remove(scopeLValue); });
				dScope.Remove(scopeLValue);
			}

			{
				Datum scopeRemoveAtTest;

				Scope s1;
				Scope s2;
				Scope s3;

				scopeRemoveAtTest.PushBack(s1);
				scopeRemoveAtTest.PushBack(s2);
				scopeRemoveAtTest.PushBack(s3);

				scopeRemoveAtTest.RemoveAt(1);

				Scope* sRetrieval = scopeRemoveAtTest.Get<Scope*>(1);
				Assert::IsTrue(*sRetrieval == s3);
			}

			{
				Datum scopeResizeTest;

				scopeResizeTest.SetType(Datum::DatumType::Table);
				scopeResizeTest.Resize(5);

				Assert::IsTrue(scopeResizeTest.Size() == 5);
			}

			{
				Datum unknownType;
				Datum stringType(Datum::DatumType::String);

				Assert::ExpectException<std::runtime_error>([&unknownType] { unknownType.PushBackFromString("test"); });
				stringType.PushBackFromString("test");
				Assert::IsTrue("test"s == stringType.Get<string>(0));
			}
		}

#pragma endregion
	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState DatumTests::_startMemState;
}

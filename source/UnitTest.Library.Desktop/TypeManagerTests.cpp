#include "pch.h"
#include <crtdbg.h>
#include <CppUnitTest.h>
#include <exception>
#include <stdexcept>
#include "TypeManager.h"
#include "RTTI.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std;

namespace UnitTestLibraryDesktop
{
	class TestMonster : public RTTI
	{
		RTTI_DECLARATIONS(TestMonster, RTTI);

	public:
		std::string Name;
		int HitPoints;
		float Dps;

		static const Vector<Signature> Signatures()
		{
			return Vector<Signature>
			{
				{ "Name"s, Datum::DatumType::String, 1, offsetof(TestMonster, Name) },
				{ "HitPoints"s, Datum::DatumType::Integer, 1, offsetof(TestMonster, HitPoints) },
				{ "Dps"s, Datum::DatumType::Float, 1, offsetof(TestMonster, Dps) }
			};
		};
	};

	RTTI_DEFINITIONS(TestMonster);

	class TestMeanMonster : public TestMonster
	{
		RTTI_DECLARATIONS(TestMeanMonster, TestMonster);

	public:
		int Grumpiness;

		static const Vector<Signature> Signatures()
		{
			return Vector<Signature>
			{
				{ "Grumpiness"s, Datum::DatumType::Integer, 1, offsetof(TestMeanMonster, Grumpiness) }
			};
		};
	};

	RTTI_DEFINITIONS(TestMeanMonster);


	TEST_CLASS(TypeManagerTests)
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
			TypeManager::Clear();
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &_startMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(TestTypeManager)
		{
			Assert::IsFalse(TypeManager::ContainsType(TestMonster::TypeIdClass()));
			TypeManager::AddType(TestMonster::TypeIdClass(), TestMonster::Signatures());
			Assert::AreEqual(TypeManager::Size(), 1_z);
			Assert::IsTrue(TypeManager::ContainsType(TestMonster::TypeIdClass()));

			auto testMonsterSignatures = TypeManager::GetSignaturesForType(TestMonster::TypeIdClass());
			Assert::AreEqual(3_z, testMonsterSignatures.Size());
			
			TypeManager::RemoveType(TestMonster::TypeIdClass());
			Assert::AreEqual(0_z, TypeManager::Size());
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState TypeManagerTests::_startMemState;
}

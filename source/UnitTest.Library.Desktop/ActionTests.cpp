#include "pch.h"
#include <crtdbg.h>
#include <CppUnitTest.h>
#include <exception>
#include <stdexcept>
#include <functional>
#include "Foo.h"
#include "JsonParseCoordinator.h"
#include "JsonTableParseHelper.h"
#include "TypeManager.h"
#include "IFactory.h"
#include "GameObject.h"
#include "Avatar.h"
#include "ActionListIf.h"
#include "ActionTestDamage.h"
#include "ActionTestHealing.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std;

namespace UnitTestLibraryDesktop
{
	ConcreteFactory(GameObject, Scope)

	ConcreteFactory(Avatar, Scope)

	TEST_CLASS(ActionTests)
	{
	public:
		//	Runs before every Test_Method
		TEST_METHOD_INITIALIZE(Initialize)
		{
#ifdef _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
			TypeManager::AddType(GameObject::TypeIdClass(), GameObject::Signatures());
			TypeManager::AddType(Avatar::TypeIdClass(), Avatar::Signatures());
			TypeManager::AddType(ActionListIf::TypeIdClass(), ActionListIf::Signatures());
			TypeManager::AddType(ActionTestHealing::TypeIdClass(), ActionTestHealing::Signatures());
			TypeManager::AddType(ActionTestDamage::TypeIdClass(), ActionTestDamage::Signatures());
		}

		//	Runs after every Test_Method
		TEST_METHOD_CLEANUP(Cleanup)
		{
			TypeManager::Clear();
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

		TEST_METHOD(TestParsing)
		{
			GameObject world;
			SharedTableData tData(world);
			tData.SetRootScope(world); // Redundant, just for coverage.
			JsonParseCoordinator parseMaster(tData);
			JsonTableParseHelper tHelper;
			GameState state;
			GameTime time;

			//	Register the Factories
			GameObjectFactory gameObjectFactory;
			AvatarFactory avatarFactory;
			ActionListIfFactory afFactory;
			ActionTestDamageFactory dFactory;
			ActionTestHealingFactory hFactory;


			parseMaster.AddHelper(tHelper);

			std::string fileName = "Content/JsonTestAction.json";

			parseMaster.ParseFromFile(fileName);	//	Parses correctly

			Scope* sp;
			Datum* d;

			d = world.Find("Children");

			sp = &(*d)[0];

			d = sp->Find("Children");

			Scope* spTwo;

			sp = &(*d)[0];
			spTwo = &(*d)[1];

			Datum* dTwo;

			//	Checking initial values
			d = sp->Find("Health");
			dTwo = spTwo->Find("Health");

			Assert::IsTrue(80 == *(d));
			Assert::IsTrue(80 == *(dTwo));

			//	Calling update should set sp's health value to 10 and spTwo's to 1000
			world.Update(state);

			//	Checking final values
			d = sp->Find("Health");
			dTwo = spTwo->Find("Health");

			Assert::IsTrue(10 == *(d));
			Assert::IsTrue(1000 == *(dTwo));
		}

		TEST_METHOD(TestGameState)
		{
			GameState state;
			GameTime time;

			state.SetGameTime(time);
			Assert::IsTrue(&time == &state.GetGameTime());
		}

		TEST_METHOD(ExtraCodeCoverageTests)
		{
			GameObject object;
			Avatar avatar;

			AvatarFactory avatarFactory;

			Vector<Signature> test = avatar.Signatures();
			Assert::IsTrue(test.Size() == 7_z);
		}

		TEST_METHOD(RTTIMacroCoverage)
		{
			GameObject gameObject;
			Avatar avatar;

			{
				RTTI* rtti = &gameObject;
				Assert::IsFalse(rtti->Is("Bar"s));
				Assert::IsTrue(rtti->Is("GameObject"s));
				Assert::IsFalse(rtti->Is(Bar::TypeIdClass()));
				Assert::IsTrue(rtti->Is(GameObject::TypeIdClass()));
				Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
				Assert::IsTrue(GameObject::TypeIdClass() == rtti->TypeIdInstance());

				Bar* b = rtti->As<Bar>();
				Assert::IsNull(b);

				GameObject* h = rtti->As<GameObject>();
				Assert::IsNotNull(h);
				Assert::IsTrue(&gameObject == h);
				RTTI* r = rtti->QueryInterface(Bar::TypeIdClass());
				Assert::IsNull(r);
				r = rtti->QueryInterface(GameObject::TypeIdClass());
				Assert::IsNotNull(r);

				r = rtti->QueryInterface(Bar::TypeIdClass());
				Assert::IsNull(r);

				r = rtti->QueryInterface(GameObject::TypeIdClass());
				Assert::IsNotNull(r);
			}

			{
				RTTI* rtti = &avatar;
				Assert::IsFalse(rtti->Is("Bar"s));
				Assert::IsTrue(rtti->Is("GameObject"s));
				Assert::IsTrue(rtti->Is("Avatar"s));
				Assert::IsFalse(rtti->Is(Bar::TypeIdClass()));
				Assert::IsTrue(rtti->Is(Avatar::TypeIdClass()));
				Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
				Assert::IsTrue(Avatar::TypeIdClass() == rtti->TypeIdInstance());

				Bar* b = rtti->As<Bar>();
				Assert::IsNull(b);

				Avatar* h = rtti->As<Avatar>();
				Assert::IsNotNull(h);
				Assert::IsTrue(&avatar == h);
				RTTI* r = rtti->QueryInterface(Bar::TypeIdClass());
				Assert::IsNull(r);
				r = rtti->QueryInterface(Avatar::TypeIdClass());
				Assert::IsNotNull(r);

				r = rtti->QueryInterface(Bar::TypeIdClass());
				Assert::IsNull(r);

				r = rtti->QueryInterface(Avatar::TypeIdClass());
				Assert::IsNotNull(r);
			}
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState ActionTests::_startMemState;
}

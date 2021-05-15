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

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std;

namespace UnitTestLibraryDesktop
{
	ConcreteFactory(GameObject, Scope)

	ConcreteFactory(Avatar, Scope)

	TEST_CLASS(GameObjectTests)
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

			parseMaster.AddHelper(tHelper);

			std::string fileName = "Content/JsonGameObjectInputTest.json";

			parseMaster.ParseFromFile(fileName);	//	Parses correctly

			Assert::AreEqual(5_z, world.Size());

			Datum* d = world.Find("Children"s);

			//	d[0] = this
			//	d[1] = _name
			//	d[2] = _transform
			//	d[3] = _actions
			//	d[4] = Parsed Members Begin
			Scope* sp = &(*d)[0];
			
			Assert::AreEqual(5_z, sp->Size());
			Assert::IsTrue(sp->Is(GameObject::TypeIdClass()));

			d = sp->Find("Children"s);
			sp = &(*d)[0];
			//Assert::AreEqual(5_z, sp->Size());
			Assert::IsTrue(sp->Is(GameObject::TypeIdClass()));

			Scope* spTwo;

			sp = &(*d)[0];
			spTwo = &(*d)[1];

			Assert::IsTrue(sp->Is(Avatar::TypeIdClass()));
			Assert::IsTrue(spTwo->Is(Avatar::TypeIdClass()));

			d = sp->Find("Name");
			Assert::IsTrue("Michael"s == *d);
			d = sp->Find("Dps");
			Assert::IsTrue(1.5f == *d);
			d = sp->Find("Velocity");
			Assert::IsTrue(vec4(1, 1, 1, 1) == *d);
			d = sp->Find("Transform");
			Assert::IsTrue(mat4x4(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1) == *d);

			d = spTwo->Find("Name");
			Assert::IsTrue("Paul"s == *d);
			d = spTwo->Find("Dps");
			Assert::IsTrue(2.5f == *d);
			d = spTwo->Find("Velocity");
			Assert::IsTrue(vec4(2, 2, 2, 2) == *d);
			d = spTwo->Find("Transform");
			Assert::IsTrue(mat4x4(2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2) == *d);

			//	Update Test - Should call down to the avatars and reduce their health from 100 to 50
			Assert::AreEqual(80, reinterpret_cast<Avatar*>(sp)->GetHealth());
			//	Update modified and tested further in Action
			world.Update(state);

			//	Test Name Setting
			world.SetName("World"s);
			Assert::IsTrue("World"s == world.Name());

			const std::string name = "WorldLValue";
			world.SetName(name);
			Assert::IsTrue("WorldLValue"s == world.Name());

			//	Clone Testing
			GameObject* clone = world.Clone();
			delete clone;

			//	Test Transform Setting
			world.SetTransform(mat4({1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}));
			Assert::IsTrue(mat4({ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }) == world.Transform());

			const mat4 transform = mat4({ 0,1,1,0,1,1,1,0,1,1,1,0,1,1,1,1 });
			world.SetTransform(transform);
			Assert::IsTrue(mat4({ 0,1,1,0,1,1,1,0,1,1,1,0,1,1,1,1 }) == world.Transform());

			//	Test GameObjects()
			Datum& goTest = world.Children();
			d = world.Find("Children"s);
			Assert::IsTrue(d == &goTest);

			//	Test CreateGameObject
			world.CreateGameObject("GameObject", "Universes");
			d = world.Find("Universes"s);
			Assert::IsTrue(6_z == world.Size());
			Assert::IsNotNull(d);

			Assert::ExpectException<std::runtime_error>([&world] {world.CreateGameObject("notAClass", "instanceName"); });
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

	_CrtMemState GameObjectTests::_startMemState;
}

#include "pch.h"
#include <crtdbg.h>
#include <CppUnitTest.h>
#include <exception>
#include <stdexcept>
#include "IFactory.h"
#include "Foo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std;

namespace UnitTestLibraryDesktop
{

	ConcreteFactory(Foo, RTTI)

	ConcreteFactory(Bar, RTTI)

	TEST_CLASS(FactoryTests)
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
			Assert::AreEqual(0_z, IFactory<RTTI>::Size());
		}

		TEST_METHOD(AddRemoveFactory)
		{
			const IFactory<RTTI>* foundFactory = IFactory<RTTI>::Find("Foo"s);
			Assert::IsNull(foundFactory);
			foundFactory = IFactory<RTTI>::Find("Bar"s);
			Assert::IsNull(foundFactory);

			{
				const FooFactory fooFactory;
				Assert::AreEqual(1_z, IFactory<RTTI>::Size());
				foundFactory = IFactory<RTTI>::Find("Foo"s);
				Assert::IsTrue(&fooFactory == reinterpret_cast<const FooFactory*>(foundFactory));

				Assert::ExpectException<std::runtime_error>([] { const FooFactory alreadyAddedFooFactory; });

				const BarFactory barFactory;
				Assert::AreEqual(2_z, IFactory<RTTI>::Size());
				foundFactory = IFactory<RTTI>::Find("Bar"s);
				Assert::IsTrue(&barFactory == reinterpret_cast<const BarFactory*>(foundFactory));
			}

			Assert::AreEqual(0_z, IFactory<RTTI>::Size());
			foundFactory = IFactory<RTTI>::Find("Foo"s);
			Assert::IsNull(foundFactory);
			foundFactory = IFactory<RTTI>::Find("Bar"s);
			Assert::IsNull(foundFactory);
		}

		TEST_METHOD(ProductCreation)
		{
			RTTI* rtti = IFactory<RTTI>::Create("Foo");
			Assert::IsNull(rtti);

			{
				FooFactory fooFactory;

				rtti = IFactory<RTTI>::Create("Foo");
				Assert::IsNotNull(rtti);

				Foo* foo = rtti->As<Foo>();
				Assert::IsNotNull(foo);

				delete foo;
			}

			rtti = IFactory<RTTI>::Create("Foo");
			Assert::IsNull(rtti);
		}

		
	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState FactoryTests::_startMemState;
}

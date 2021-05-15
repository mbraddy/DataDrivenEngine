#include "pch.h"
#include <crtdbg.h>
#include <CppUnitTest.h>
#include <exception>
#include <stdexcept>
#include <functional>
#include "Foo.h"
#include "Event.h"
#include "EventQueue.h"
#include "FooSubscriber.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std;
using namespace std::chrono_literals;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(EventTests)
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

		TEST_METHOD(TestSend)
		{
			Foo foo;
			FooSubscriber fooSubscriber;
			GameState gameState;

			EventQueue eventQueue(gameState);

			Assert::IsFalse(fooSubscriber.wasNotified);
			Assert::IsTrue(eventQueue.IsEmpty());

			shared_ptr<Event<Foo>> e = make_shared<Event<Foo>>(foo);

			Assert::IsTrue(foo == e->Message());
			
			eventQueue.Send(e);
			Assert::IsFalse(fooSubscriber.wasNotified);

			Event<Foo>::Subscribe(fooSubscriber);
			eventQueue.Send(e);
			Assert::IsTrue(fooSubscriber.wasNotified);
			Event<Foo>::UnsubscribeAll();
		}

		TEST_METHOD(TestQueue)
		{
			Foo foo;
			FooSubscriber fooSubscriber;
			GameState gameState;
			GameTime gameTime;

			std::chrono::high_resolution_clock::time_point timePoint(0ms);

			gameTime.SetCurrentTime(timePoint);

			gameState.SetGameTime(gameTime);

			EventQueue eventQueue(gameState);

			Assert::IsFalse(fooSubscriber.wasNotified);
			Assert::IsTrue(eventQueue.IsEmpty());

			shared_ptr<Event<Foo>> e = make_shared<Event<Foo>>(foo);

			Assert::IsTrue(foo == e->Message());

			eventQueue.Send(e);
			Assert::IsFalse(fooSubscriber.wasNotified);

			Event<Foo>::Subscribe(fooSubscriber);
			eventQueue.Send(e);
			Assert::IsTrue(fooSubscriber.wasNotified);
			Event<Foo>::UnsubscribeAll();
		}

		TEST_METHOD(RTTIMacroCoverage)
		{
			FooSubscriber fooSub;

			{
				RTTI* rtti = &fooSub;
				Assert::IsFalse(rtti->Is("Bar"s));
				Assert::IsTrue(rtti->Is("FooSubscriber"s));
				Assert::IsFalse(rtti->Is(Bar::TypeIdClass()));
				Assert::IsTrue(rtti->Is(FooSubscriber::TypeIdClass()));
				Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
				Assert::IsTrue(FooSubscriber::TypeIdClass() == rtti->TypeIdInstance());

				Bar* b = rtti->As<Bar>();
				Assert::IsNull(b);

				FooSubscriber* h = rtti->As<FooSubscriber>();
				Assert::IsNotNull(h);
				Assert::IsTrue(&fooSub == h);
				RTTI* r = rtti->QueryInterface(Bar::TypeIdClass());
				Assert::IsNull(r);
				r = rtti->QueryInterface(FooSubscriber::TypeIdClass());
				Assert::IsNotNull(r);

				r = rtti->QueryInterface(Bar::TypeIdClass());
				Assert::IsNull(r);

				r = rtti->QueryInterface(FooSubscriber::TypeIdClass());
				Assert::IsNotNull(r);
			}

		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState EventTests::_startMemState;
}

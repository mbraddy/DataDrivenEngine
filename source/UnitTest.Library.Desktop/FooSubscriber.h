#pragma once
#include "Foo.h"
#include "IEventSubscriber.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Test class FooSubscriber
	/// </summary>
	class FooSubscriber :
		public Foo, public IEventSubscriber
	{
		RTTI_DECLARATIONS(FooSubscriber, Foo)

	public:
		/// <summary>
		/// Virtual Destructor
		/// </summary>
		virtual ~FooSubscriber() = default;

		/// <summary>
		/// Notify - Overloaded to set wasNotified to true if the subscriber is fired.
		/// </summary>
		/// <param name="publisher">Event publisher that is notifying this event subscriber</param>
		virtual void Notify(const EventPublisher&);

		/// <summary>
		/// wasNotified - Boolean used for testing
		/// </summary>
		bool wasNotified = false;
	};
}


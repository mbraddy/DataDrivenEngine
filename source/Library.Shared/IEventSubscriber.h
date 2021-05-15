#pragma once

namespace FieaGameEngine
{
	class EventPublisher;

	class IEventSubscriber
	{
	public:
		/// <summary>
		/// IEventSubscriber Destructor - Made Virtual as this is a pure abstract class - Defaulted.
		/// </summary>
		virtual ~IEventSubscriber() = default;

		/// <summary>
		/// Notify - Method that is called by Deliver in Event Publisher. This is meant to "catch" the messages and implementation of Notify
		/// will be subject to change based on the implementing code's usage case of said message.
		/// </summary>
		/// <param name="publisher">Reference to the event publisher that is delivering it's message.</param>
		virtual void Notify(const EventPublisher&) = 0;
	};
}


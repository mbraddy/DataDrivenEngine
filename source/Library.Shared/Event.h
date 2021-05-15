#pragma once
#include "EventPublisher.h"
#include "IEventSubscriber.h"

namespace FieaGameEngine
{
	template <typename T>
	class Event : public EventPublisher
	{
		RTTI_DECLARATIONS(Event<T>, EventPublisher)

	public:
		/// <summary>
		/// Event Default Constructor - Deleted to prevent instantiation without a message to wrap
		/// </summary>
		Event() = delete;

		/// <summary>
		/// Event Constructor - Wraps the Message payload passed in with an event wrapper.
		/// </summary>
		/// <param name="message">L-Value reference to an event payload.</param>
		explicit Event(const T& message);

		/// <summary>
		/// Event Constructor - Wraps the Message payload passed in with an event wrapper.
		/// </summary>
		/// <param name="message">R-Value reference to an event payload.</param>
		explicit Event(T&& message);

		/// <summary>
		/// Event Copy Constructor - Defaulted
		/// </summary>
		/// <param name="other">Event being copied.</param>
		Event(const Event&) = default;

		/// <summary>
		/// Event Move Constructor - Defaulted
		/// </summary>
		/// <param name="other">Event being Moved.</param>
		Event(Event&&) = default;

		/// <summary>
		/// Event Copy Assignment Operator - Defaulted
		/// </summary>
		/// <param name="other">Event being copied</param>
		/// <returns>Reference to the copy</returns>
		Event& operator=(const Event&) = default;

		/// <summary>
		/// Event Movement Assignment Operator - Defaulted
		/// </summary>
		/// <param name="other">Event being moved</param>
		/// <returns>Reference to the Event being moved</returns>
		Event& operator=(Event&&) = default;

		/// <summary>
		/// Event Destructor - Virtual due to inheritance - Defaulted.
		/// </summary>
		virtual ~Event() = default;

		/// <summary>
		/// Subscribe - Adds an Event subscriber to the list of subscribers.
		/// </summary>
		/// <param name="subscriber">The IEventSubscriber that is being added to the list.</param>
		static void Subscribe(class IEventSubscriber&);

		/// <summary>
		/// Unsubscribe - Removes an Event subscriber from the list of subscribers.
		/// </summary>
		/// <param name="subscriber">The IEventSubscriber that is being removed from the list.</param>
		static void Unsubscribe(class IEventSubscriber&);

		/// <summary>
		/// UnsubscribeAll - Removes all Events subscribed from the list of subscribers (calls Vector::Clear() on the list)
		/// </summary>
		static void UnsubscribeAll();

		/// <summary>
		/// Message - Returns a constant reference to the message payload of the event.
		/// </summary>
		/// <returns>Constant reference to the message payload of the event (T)</returns>
		const T& Message() const;

	private:
		/// <summary>
		/// _subscribers - Vector of all event subscribers that are subscribed to the event.
		/// </summary>
		inline static Vector<IEventSubscriber*> _subscribers;

		/// <summary>
		/// _message - Message payload of the event that is to be delivered to subscribers when notified.
		/// </summary>
		T _message;
	};

}

#include "Event.inl"
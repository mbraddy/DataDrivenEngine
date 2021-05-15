#pragma once
#include "EventPublisher.h"
#include "GameObject.h"
#include <memory>

namespace FieaGameEngine
{
	struct QueueEntry 
	{
		/// <summary>
		/// _event - Shared pointer to an event that will be placed in the queue
		/// </summary>
		std::shared_ptr<EventPublisher> _event;

		/// <summary>
		/// _expirationTime - The time that this event entry will expire
		/// </summary>
		std::chrono::high_resolution_clock::time_point _expirationTime;

		/// <summary>
		/// IsExpired - Returns whether or not the event has expired (and thus should be delivered)
		/// </summary>
		/// <param name="currentTime">Current Game time (to be compared against the expiration time)</param>
		/// <returns>True if the event has expired, false if not.</returns>
		bool IsExpired(std::chrono::high_resolution_clock::time_point currentTime);
	};

	class EventQueue final
	{
	public:

		/// <summary>
		/// EventQueue Default Constructor - Deleted to prevent instantiation without a Timer
		/// </summary>
		EventQueue() = delete;

		/// <summary>
		/// EventQueue Constructor - Takes a Gamestate which wraps a GameTime and constructs the Queue with the current time.
		/// </summary>
		/// <param name="gameState">GameState wrapping a GameTime used by the queue</param>
		EventQueue(GameState& gameState);

		/// <summary>
		/// EventQueue Copy Constructor - Defaulted
		/// </summary>
		/// <param name="other">Queue being copied.</param>
		EventQueue(const EventQueue& other) = default;

		/// <summary>
		/// EventQueue Move Constructor - Defaulted
		/// </summary>
		/// <param name="other">Queue being moved.</param>
		EventQueue(EventQueue&& other) = default;

		/// <summary>
		/// EventQueue Copy Assignment Operator - Defaulted
		/// </summary>
		/// <param name="other">Queue being copied.</param>
		/// <returns>Reference to the copy.</returns>
		EventQueue& operator=(const EventQueue& other) = default;

		/// <summary>
		/// EventQueue Move Assignment Operator - Defaulted
		/// </summary>
		/// <param name="other">Queue being moved.</param>
		/// <returns>Reference to the newly created Queue.</returns>
		EventQueue& operator=(EventQueue&& other) = default;

		/// <summary>
		/// EventQueue Destructor - Defaulted
		/// </summary>
		~EventQueue() = default;

		/// <summary>
		/// Size - returns the number of QueueEntries in the queue.
		/// </summary>
		/// <returns>size_t representing the number of QueueEntries registered.</returns>
		size_t Size();

		/// <summary>
		/// Clear - clears any events in the queue.
		/// </summary>
		void Clear();

		/// <summary>
		/// Reserve - Reserves a specified size for the event queue.
		/// </summary>
		/// <param name="size">Number of events to reserve space for</param>
		void Reserve(size_t size);

		/// <summary>
		/// ShrinkToFit - Shrinks the event queue to fit the number of events currently in it.
		/// </summary>
		void ShrinkToFit();

		/// <summary>
		/// Enqueue - Takes a shared event pointer and delay to form a queue entry and emplaces it within the queue.
		/// </summary>
		/// <param name="eventPointer">Shared pointer to an event publisher</param>
		/// <param name="delay">How long before the event should fire</param>
		void Enqueue(std::shared_ptr<EventPublisher> eventPointer, std::chrono::milliseconds delay);

		/// <summary>
		/// IsEmpty - Returns a boolean indicating if the queue is empty or not.
		/// </summary>
		/// <returns>bool - True if the queue is empty, else false.</returns>
		bool IsEmpty();

		/// <summary>
		/// Update - Calls Deliver() on any events that have expired within the queue then deletes any expired events from the queue.
		/// </summary>
		/// <param name="gameState">GameState wrapping the current gameTime to be used in the Update parameter. Choosing to force passing in a GameState
		/// for consistency with the other Update functions in the engine - even though the Update method could just use _gameTime.</param>
		void Update(GameState& gameState);

		/// <summary>
		/// Send - Calls Deliver() on a specific event immediately.
		/// </summary>
		/// <param name="e">The event to call deliver on.</param>
		void Send(EventPublisher* e);

		/// <summary>
		/// Send - Calls Deliver() on a specific event immediately.
		/// </summary>
		/// <param name="e">The event to call deliver on.</param>
		void Send(const shared_ptr<EventPublisher>& e);

		/// <summary>
		/// Send - Calls Deliver() on a specific event immediately.
		/// </summary>
		/// <param name="e">The event to call deliver on.</param>
		void Send(const unique_ptr<EventPublisher>& e);

	private:

		/// <summary>
		/// _events - Vector storing any event entries that have been enqueued
		/// </summary>
		Vector<QueueEntry> _events;

		/// <summary>
		/// _gameTime - Alias to the gameTime wrapped by the GameState passed at the time of construction.
		/// </summary>
		GameTime& _gameTime;
	};
}


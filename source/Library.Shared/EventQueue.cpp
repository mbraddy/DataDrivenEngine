#include "pch.h"
#include "EventQueue.h"

using namespace std::chrono_literals;

namespace FieaGameEngine
{
#pragma region QueueEntry Methods

	bool QueueEntry::IsExpired(std::chrono::high_resolution_clock::time_point currentTime)
	{
		return ( _expirationTime <= currentTime );
	}

#pragma endregion

	EventQueue::EventQueue(GameState& gameState) :
		_gameTime(gameState.GetGameTime())
	{
	}

	void EventQueue::Enqueue(std::shared_ptr<EventPublisher> eventPointer, std::chrono::milliseconds delay = 0ms)
	{
		QueueEntry entry{ std::move(eventPointer), (_gameTime.CurrentTime() + delay) };
		_events.PushBack(std::move(entry));
	}

	void EventQueue::Clear()
	{
		_events.Clear();
	}

	bool EventQueue::IsEmpty()
	{
		return _events.IsEmpty();
	}

	void EventQueue::Reserve(size_t size)
	{
		_events.Reserve(size);
	}

	void EventQueue::ShrinkToFit()
	{
		_events.ShrinkToFit();
	}

	size_t EventQueue::Size()
	{
		return _events.Size();
	}

	void EventQueue::Update(GameState& gameState)
	{
		std::chrono::high_resolution_clock::time_point currentTime = gameState.GetGameTime().CurrentTime();

		//	Sort Expired events to the back, fire off their Deliver while doing it.
		auto it = std::partition(_events.begin(), _events.end(), [&currentTime](QueueEntry e) {
			return !(e.IsExpired(currentTime));
		});

		for (auto expiredStart = it; expiredStart != _events.end(); ++expiredStart)
		{
			(*expiredStart)._event->Deliver();
		}

		//	Remove expired events
		_events.Remove(it, _events.end());
	}

	void EventQueue::Send(EventPublisher* e)
	{
		e->Deliver();
	}

	void EventQueue::Send(const shared_ptr<EventPublisher>& e)
	{
		e->Deliver();
	}

	void EventQueue::Send(const unique_ptr<EventPublisher>& e)
	{
		e->Deliver();
	}
}
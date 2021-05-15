#include "Event.h"

namespace FieaGameEngine
{
	template <typename T>
	RTTI_DEFINITIONS(Event<T>)

	template<typename T>
	inline Event<T>::Event(const T& message) :
		EventPublisher(_subscribers), _message(message)
	{
	}

	template<typename T>
	inline Event<T>::Event(T&& message) :
		EventPublisher(_subscribers), _message(std::forward(message))
	{
	}

	template<typename T>
	inline void Event<T>::Subscribe(IEventSubscriber& subscriber)
	{
		auto iterator = _subscribers.Find(&subscriber);
		if (iterator == _subscribers.end())
		{
			_subscribers.PushBack(&subscriber);
		}
	}

	template<typename T>
	inline void Event<T>::Unsubscribe(IEventSubscriber& subscriber)
	{
		auto iterator = _subscribers.Find(&subscriber);
		if (iterator != _subscribers.end())
		{
			_subscribers.Remove(iterator);
		}
	}

	template<typename T>
	inline void Event<T>::UnsubscribeAll()
	{
		_subscribers.Clear();
		_subscribers.ShrinkToFit();
	}

	template<typename T>
	inline const T& Event<T>::Message() const
	{
		return _message;
	}
}
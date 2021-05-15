#include "pch.h"
#include "EventPublisher.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(EventPublisher)

	EventPublisher::EventPublisher(const Vector<IEventSubscriber*>& subscribers) :
		_subscribers(&subscribers)
	{
	}

	void EventPublisher::Deliver() const
	{
		for (IEventSubscriber* subscriber : *(_subscribers))
		{
			assert(subscriber != nullptr);
			subscriber->Notify(*this);
		}
	}

}
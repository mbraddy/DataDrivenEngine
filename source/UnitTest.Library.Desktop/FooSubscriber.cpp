#include "pch.h"
#include "FooSubscriber.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(FooSubscriber)

	void FieaGameEngine::FooSubscriber::Notify(const EventPublisher&)
	{
		wasNotified = true;
	}
}
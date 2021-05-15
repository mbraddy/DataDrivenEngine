#include "pch.h"
#include "ReactionAttributed.h"

namespace FieaGameEngine
{
	const Vector<Signature> ReactionAttributed::Signatures()
	{
		FieaGameEngine::Vector<Signature> signatureVector =
		{
			{"Subtype", Datum::DatumType::String, 1, offsetof(ReactionAttributed, _subtype)}
		};

		return signatureVector;
	}

	ReactionAttributed::ReactionAttributed()
	{
		_subscribedEventPublisher = nullptr;
	}

	ReactionAttributed::ReactionAttributed(const Event<EventMessageAttributed>& publisher)
	{
		publisher.Subscribe(*this);
		_subscribedEventPublisher = &publisher;
	}

	ReactionAttributed::~ReactionAttributed()
	{
		if (_subscribedEventPublisher != nullptr)
		{
			_subscribedEventPublisher->Unsubscribe(*this);
		}
	}

	void ReactionAttributed::Notify(const EventPublisher& publisher)
	{
		assert(publisher.Is(EventMessageAttributed::TypeIdClass()));
		auto eventReference = reinterpret_cast<const EventMessageAttributed&>(publisher);
		if (eventReference.GetSubType() == _subtype)
		{
			for (auto attribute : AuxiliaryAttributes())
			{
				Datum& d = eventReference.Append(attribute.name);
				d.SetType(attribute.type);
			}

			for (size_t i = 0; i < Size(); ++i)
			{
				auto& [key, datum] = *(_orderList[i]);
				if (!eventReference.IsPrescribedAttribute(key))
				{
					eventReference[key] = datum;
				}
			}

			GameState state;
			state.SetCurrentGameObject(*(eventReference.GetGameObject()));
			ActionList::Update(state);
		}
	}

	void ReactionAttributed::SubscribeToAttributedEvent(const Event<EventMessageAttributed>& publisher)
	{
		publisher.Subscribe(*this);
		_subscribedEventPublisher = &publisher;
	}
}
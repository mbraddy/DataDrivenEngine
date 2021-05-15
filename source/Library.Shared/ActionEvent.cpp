#include "pch.h"
#include "ActionEvent.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionEvent)

	const Vector<Signature> ActionEvent::Signatures()
	{
		FieaGameEngine::Vector<Signature> signatureVector =
		{
			{"Name", Datum::DatumType::String, 1, offsetof(ActionEvent, _name)},
			{"Subtype", Datum::DatumType::String, 1, offsetof(ActionEvent, _subtype)},
			{"Delay", Datum::DatumType::Integer, 1, offsetof(ActionEvent, _delay)}
		};

		return signatureVector;
	}

	ActionEvent::ActionEvent() :
		Action(ActionEvent::TypeIdClass())
	{
	}

	void ActionEvent::Update(GameState& gameState)
	{
		EventMessageAttributed eventMessage;
		eventMessage.SetGameObject(*gameState.GetCurrentGameObject());
		eventMessage.SetSubType(_subtype);

		for (auto attribute : AuxiliaryAttributes())
		{
			Datum& d = eventMessage.Append(attribute.name);
			d.SetType(attribute.type);
		}

		for (size_t i = 0; i < Size(); ++i)
		{
			auto& [key, datum] = *(_orderList[i]);
			if (!eventMessage.IsPrescribedAttribute(key))
			{
				eventMessage[key] = datum;
			}
		}

		shared_ptr<Event<EventMessageAttributed>> newEvent = make_shared<Event<EventMessageAttributed>>(eventMessage);

		EventQueue* queue = gameState.GetEventQueue();
		queue->Enqueue(newEvent, std::chrono::milliseconds(_delay));
	}

	gsl::owner<ActionEvent*> ActionEvent::Clone() const
	{
		return new ActionEvent(*this);
	}
}
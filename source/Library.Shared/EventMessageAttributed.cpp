#include "pch.h"
#include "EventMessageAttributed.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(EventMessageAttributed)

	const Vector<Signature> EventMessageAttributed::Signatures()
	{
		FieaGameEngine::Vector<Signature> signatureVector =
		{
			{"Subtype", Datum::DatumType::String, 1, offsetof(EventMessageAttributed, _subType)},
			{"GameObject", Datum::DatumType::Table, 0, 0}
		};

		return signatureVector;
	}

	EventMessageAttributed::EventMessageAttributed() :
		Attributed(EventMessageAttributed::TypeIdClass())
	{
	}

	void EventMessageAttributed::SetSubType(const std::string& subType)
	{
		_subType = subType;
	}

	void EventMessageAttributed::SetSubType(std::string&& subType)
	{
		_subType = std::move(subType);
	}

	void EventMessageAttributed::SetGameObject(const GameObject& gameObject)
	{
		assert(gameObject.Is(GameObject::TypeIdClass()));
		Datum& d = _orderList[_gameObjectIndex]->second;
		if (d.Size() == 0)
		{
			d.PushBack(gameObject);
		}
		else
		{
			d.Set(gameObject, 0);
		}
	}

	GameObject* EventMessageAttributed::GetGameObject()
	{
		Datum& d = _orderList[_gameObjectIndex]->second;
		return (d.Size() != 0 ? static_cast<GameObject*>(&d[0]) : nullptr);
	}

	const std::string& EventMessageAttributed::GetSubType() const
	{
		return _subType;
	}

	gsl::owner<EventMessageAttributed*> EventMessageAttributed::Clone() const
	{
		return new EventMessageAttributed(*this);
	}
}
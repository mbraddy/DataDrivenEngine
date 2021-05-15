#include "pch.h"
#include "Action.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Action)

	Action::Action(RTTI::IdType id) :
		Attributed(id)
	{
	}

	void Action::SetName(const std::string& name)
	{
		_name = name;
	}

	void Action::SetName(std::string&& name)
	{
		_name = std::move(name);
	}

	const std::string& Action::GetName() const
	{
		return _name;
	}

	const Vector<Signature> Action::Signatures()
	{
		FieaGameEngine::Vector<Signature> signatureVector =
		{
			{"Name", Datum::DatumType::String, 1, offsetof(Action, _name)}
		};

		return signatureVector;
	}
}
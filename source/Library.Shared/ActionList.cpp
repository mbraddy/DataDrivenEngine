#include "pch.h"
#include "ActionList.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionList)

	ActionList::ActionList() :
		Action(ActionList::TypeIdClass())
	{
		assert(_orderList.At(_actionIndex)->first == "Actions");
		assert(_orderList.At(_actionIndex)->second.Type() == Datum::DatumType::Table);
	}

	ActionList::ActionList(RTTI::IdType id) :
		Action(id)
	{
	}

	gsl::owner<ActionList*> ActionList::Clone() const
	{
		return new ActionList(*this);
	}

	const Vector<Signature> ActionList::Signatures()
	{
		FieaGameEngine::Vector<Signature> signatureVector =
		{
			{"Name", Datum::DatumType::String, 1, offsetof(ActionList, _name)},
			{"Actions", Datum::DatumType::Table, 0, 0}
		};

		return signatureVector;
	}

	void ActionList::Update(GameState& gameState)
	{
		gameState.SetCurrentAction(*this);

		Datum& actionsDatum = _orderList[_actionIndex]->second;
		for (size_t i = 0; i < actionsDatum.Size(); ++i)
		{
			Scope& currentAction = actionsDatum[i];
			assert(currentAction.Is(Action::TypeIdClass()));
			static_cast<Action&>(currentAction).Update(gameState);
		}
	}
}
#include "pch.h"
#include "ActionListIf.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionListIf)

	ActionListIf::ActionListIf() :
		ActionList(ActionListIf::TypeIdClass())
	{
	}

	const Vector<Signature> ActionListIf::Signatures()
	{
		FieaGameEngine::Vector<Signature> signatureVector =
		{
			{"Condition", Datum::DatumType::Integer, 1, offsetof(ActionListIf, _condition)},
			{"Then", Datum::DatumType::Table, 0, 0},
			{"Else", Datum::DatumType::Table, 0, 0}
		};

		return signatureVector;
	}

	void ActionListIf::Update(GameState& gameState)
	{
		gameState.SetCurrentAction(*this);

		if (_condition == 1)
		{
			Scope* actionScope = &(_orderList[_thenIndex]->second[0]);
			assert(actionScope->Is(Action::TypeIdClass()));
			static_cast<Action*>(actionScope)->Update(gameState);
		}
		else
		{
			Scope* actionScope = &(_orderList[_elseIndex]->second[0]);
			assert(actionScope->Is(Action::TypeIdClass()));
			static_cast<Action*>(actionScope)->Update(gameState);
		}
	}

	gsl::owner<ActionListIf*> FieaGameEngine::ActionListIf::Clone() const
	{
		return new ActionListIf(*this);
	}
}
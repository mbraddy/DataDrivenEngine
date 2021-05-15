#include "pch.h"
#include "ActionTestHealing.h"

namespace FieaGameEngine
{
	ActionTestHealing::ActionTestHealing() :
		Action(ActionTestHealing::TypeIdClass())
	{
	}

	void ActionTestHealing::Update(GameState& gameState)
	{
		gameState.SetCurrentAction(*this);

		Scope* sp = GetParent();

		while (!sp->Is(Avatar::TypeIdClass()))
		{
			sp = sp->GetParent();
		}

		static_cast<Avatar*>(sp)->SetHealth(1000);
	}

	gsl::owner<ActionTestHealing*> ActionTestHealing::Clone() const
	{
		return new ActionTestHealing(*this);
	}

	const Vector<Signature> ActionTestHealing::Signatures()
	{
		return Vector<Signature>();
	}
}
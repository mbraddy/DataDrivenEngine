#include "pch.h"
#include "ActionTestDamage.h"

namespace FieaGameEngine
{
	ActionTestDamage::ActionTestDamage() :
		Action(ActionTestDamage::TypeIdClass())
	{
	}

	void ActionTestDamage::Update(GameState& gameState)
	{
		gameState.SetCurrentAction(*this);

		Scope* sp = GetParent();

		while (!sp->Is(Avatar::TypeIdClass()))
		{
			sp = sp->GetParent();
		}

		static_cast<Avatar*>(sp)->SetHealth(10);
	}

	gsl::owner<ActionTestDamage*> ActionTestDamage::Clone() const
	{
		return new ActionTestDamage(*this);
	}

	const Vector<Signature> ActionTestDamage::Signatures()
	{
		return Vector<Signature>();
	}
}
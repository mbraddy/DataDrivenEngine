#pragma once
#include "Action.h"
#include "Avatar.h"

namespace FieaGameEngine
{
	class ActionTestHealing final : public Action
	{
	public:
		ActionTestHealing();
		ActionTestHealing(const ActionTestHealing& other) = default;
		ActionTestHealing(ActionTestHealing&& other) = default;
		ActionTestHealing& operator=(const ActionTestHealing& other) = default;
		ActionTestHealing& operator=(ActionTestHealing&& other) = default;
		virtual ~ActionTestHealing() = default;

		virtual void Update(GameState& gameState) override;
		virtual gsl::owner<ActionTestHealing*> Clone() const override;

		static const Vector<Signature> Signatures();
	};

	ConcreteFactory(ActionTestHealing, Scope)
}

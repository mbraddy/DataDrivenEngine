#pragma once
#include "Action.h"
#include "Avatar.h"

namespace FieaGameEngine
{
	class ActionTestDamage final : public Action
	{
	public:
		ActionTestDamage();
		ActionTestDamage(const ActionTestDamage& other) = default;
		ActionTestDamage(ActionTestDamage&& other) = default;
		ActionTestDamage& operator=(const ActionTestDamage& other) = default;
		ActionTestDamage& operator=(ActionTestDamage&& other) = default;
		virtual ~ActionTestDamage() = default;

		virtual void Update(GameState& gameState) override;
		virtual gsl::owner<ActionTestDamage*> Clone() const override;

		static const Vector<Signature> Signatures();

	};

	ConcreteFactory(ActionTestDamage, Scope)
}


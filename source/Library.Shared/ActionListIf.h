#pragma once
#include "ActionList.h"

namespace FieaGameEngine
{
	class ActionListIf final : public ActionList
	{
		RTTI_DECLARATIONS(ActionListIf, ActionList)

	public:
		ActionListIf();
		ActionListIf(const ActionListIf& other) = default;
		ActionListIf(ActionListIf&& other) = default;
		ActionListIf& operator=(const ActionListIf& other) = default;
		ActionListIf& operator=(ActionListIf&& other) = default;
		virtual ~ActionListIf() = default;

		virtual void Update(GameState& gameState) override;
		virtual gsl::owner<ActionListIf*> Clone() const override;

		static const Vector<Signature> Signatures();

	private:
		int _condition;
		static const size_t _thenIndex = 2;
		static const size_t _elseIndex = 3;
	};

	ConcreteFactory(ActionListIf, Scope)
}

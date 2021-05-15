#pragma once
#include "Action.h"
#include "Vector.h"
#include "IFactory.h"


namespace FieaGameEngine
{
	class ActionList : public Action
	{
		RTTI_DECLARATIONS(ActionList, Action)

	public:
		ActionList();
		ActionList(const ActionList& other) = default;
		ActionList(ActionList&& other) = default;
		ActionList& operator=(const ActionList& other) = default;
		ActionList& operator=(ActionList&& other) = default;
		virtual ~ActionList() = default;

		virtual void Update(GameState& gameState) override;
		virtual gsl::owner<ActionList*> Clone() const override;

		static const Vector<Signature> Signatures();

	protected:
		ActionList(RTTI::IdType id);

	private:
		static const size_t _actionIndex = 2;

	};

	ConcreteFactory(ActionList, Scope)
}
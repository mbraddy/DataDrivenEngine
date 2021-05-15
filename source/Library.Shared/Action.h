#pragma once
#include "Attributed.h"
#include "GameObject.h"

namespace FieaGameEngine
{
	class GameState;

	class Action : public Attributed
	{
		RTTI_DECLARATIONS(Action, Attributed)

	public:
		Action() = delete;
		Action(const Action& other) = default;
		Action(Action&& other) = default;
		Action& operator=(const Action& other) = default;
		Action& operator=(Action&& other) = default;
		virtual ~Action() = default;

		void SetName(const std::string& name);
		void SetName(std::string&& name);
		const std::string& GetName() const;

		static const Vector<Signature> Signatures();

		virtual void Update(GameState& gameState) = 0;
		virtual gsl::owner<Action*> Clone() const = 0;

	protected:
		explicit Action(RTTI::IdType id);

		std::string _name;
	};
}


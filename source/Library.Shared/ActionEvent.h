#pragma once
#include "Action.h"
#include "Event.h"
#include "EventMessageAttributed.h"
#include "EventQueue.h"
#include <memory>

namespace FieaGameEngine
{
	class ActionEvent : public Action
	{
		RTTI_DECLARATIONS(ActionEvent, Action)

	public:
		/// <summary>
		/// ActionEvent Default Constructor
		/// </summary>
		ActionEvent();

		/// <summary>
		/// ActionEvent Copy constructor - defaulted
		/// </summary>
		/// <param name="other">ActionEvent to copy</param>
		ActionEvent(const ActionEvent& other) = default;

		/// <summary>
		/// ActionEvent Move Constructor - defaulted
		/// </summary>
		/// <param name="other">ActionEvent to move</param>
		ActionEvent(ActionEvent&& other) = default;

		/// <summary>
		/// ActionEvent copy assignment operator - defaulted
		/// </summary>
		/// <param name="other">ActionEvent to copy</param>
		/// <returns>Reference to the copy</returns>
		ActionEvent& operator=(const ActionEvent& other) = default;

		/// <summary>
		/// ActionEvent Move Assignment Operator - defaulted
		/// </summary>
		/// <param name="other">ActionEvent to move</param>
		/// <returns>Reference to the moved ActionEvent</returns>
		ActionEvent& operator=(ActionEvent&& other) = default;

		/// <summary>
		/// Virtual ActionEvent Destructor - defaulted.
		/// </summary>
		virtual ~ActionEvent() = default;

		/// <summary>
		/// Signatures - Returns the static signature vector for all prescribed attributes. Required for Attributed.
		/// </summary>
		/// <returns>Returns the static signature vector for all prescribed attributes.</returns>
		static const Vector<Signature> Signatures();

		/// <summary>
		/// Update - Creates an attributed event, assigns its GameObject pointer (from the gamestate) and subtype, copies all auxilliary attributes to it,
		/// and enqueues it into the EventQueue of the gameState with the specified _delay (in milliseconds).
		/// </summary>
		/// <param name="gameState">Gamestate pointer that is pointing to the GameObject referenced by the event that stores a 
		/// pointer to the event queue</param>
		virtual void Update(GameState& gameState) override;

		/// <summary>
		/// Clone - Creates a copy of this ActionEvent.
		/// </summary>
		/// <returns>Pointer to the copy. Note: This copy must be deleted by the caller when done otherwise memory leaks.</returns>
		virtual gsl::owner<ActionEvent*> Clone() const override;

	private:
		/// <summary>
		/// _subtype - The Subtype of events to be created in the Update method. 
		/// To be populated by Json with the Attribute name "Subtype".
		/// </summary>
		std::string _subtype;

		/// <summary>
		/// _delay - The delay (in milliseconds) to be applied to the events that are created by this ActionEvent's update method.
		/// To be populated by Json with the Attribute name "Delay"
		/// </summary>
		int _delay;
	};

	ConcreteFactory(ActionEvent, Action)

}


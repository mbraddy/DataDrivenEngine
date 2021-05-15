#pragma once
#include "Attributed.h"
#include "GameObject.h"

namespace FieaGameEngine
{
	class EventMessageAttributed : public Attributed
	{
		RTTI_DECLARATIONS(EventMessageAttributed, Attributed)

	public:
		/// <summary>
		/// EventMessageAttributed - Default Constructor
		/// </summary>
		EventMessageAttributed();

		/// <summary>
		/// EventMessageAttributed - Copy Constructor
		/// </summary>
		/// <param name="other">EventMessageAttributed to be copied</param>
		EventMessageAttributed(const EventMessageAttributed& other) = default;

		/// <summary>
		/// EventMessageAttributed - Move Constructor
		/// </summary>
		/// <param name="other">EventMessageAttributed to be moved</param>
		EventMessageAttributed(EventMessageAttributed&& other) = default;

		/// <summary>
		/// EventMessageAttributed - Copy assignment operator
		/// </summary>
		/// <param name="other">EventMessageAttributed to be copied</param>
		/// <returns>Reference to the copy</returns>
		EventMessageAttributed& operator=(const EventMessageAttributed& other) = default;

		/// <summary>
		/// EventMessageAttributed - Move assignment operator
		/// </summary>
		/// <param name="other">EventMessageAttributed to be moved</param>
		/// <returns>Reference to the moved EventMessageAttributed</returns>
		EventMessageAttributed& operator=(EventMessageAttributed&& other) = default;

		/// <summary>
		/// Virtual EventMessageAttributed Destructor - defaulted.
		/// </summary>
		virtual ~EventMessageAttributed() = default;

		/// <summary>
		/// Signatures - Returns the static signature vector for all prescribed attributes. Required for Attributed.
		/// </summary>
		/// <returns>Returns the static signature vector for all prescribed attributes.</returns>
		const Vector<Signature> Signatures();

		/// <summary>
		/// GetSubType - Returns the subtype of the EventMessageAttributed.
		/// </summary>
		/// <returns>Const string reference storing the subtype of the event message payload.</returns>
		const std::string& GetSubType() const;

		/// <summary>
		/// GetGameObject - Returns a pointer to the game object to be effected by this event message attributed's payload
		/// </summary>
		/// <returns>Pointer to the game object to be effected by this event message attributed's payload</returns>
		GameObject* GetGameObject();

		/// <summary>
		/// SetSubType - sets the subtype of the event message attributed
		/// </summary>
		/// <param name="subType">L-value string that represents the subtype</param>
		void SetSubType(const std::string& subType);

		/// <summary>
		/// SetSubType - sets the subtype of the event message attributed
		/// </summary>
		/// <param name="subType">R-value string that represents the subtype</param>
		void SetSubType(std::string&& subType);

		/// <summary>
		/// SetGameObject - Sets the game object pointer of the event message attributed to the address of the L value reference passed in
		/// </summary>
		/// <param name="gameObject">L-value reference to the game object that this event message attributed should point to</param>
		void SetGameObject(const GameObject& gameObject);

		/// <summary>
		/// Clone - Creates a copy of this EventMessageAttributed.
		/// </summary>
		/// <returns>Pointer to the copy. Note: This copy must be deleted by the invoker of this method elsewise Memory leaks.</returns>
		virtual gsl::owner<EventMessageAttributed*> Clone() const override;

	private:
		/// <summary>
		/// _subtype - string representing the subtype of the event message attributed
		/// </summary>
		std::string _subType;

		/// <summary>
		/// _gameObjectIndex - index of the GameObject prescribed attribute in the _orderList of this scope.
		/// </summary>
		const static size_t _gameObjectIndex = 2;
	};

}
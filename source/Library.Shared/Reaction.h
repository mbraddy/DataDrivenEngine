#pragma once
#include "ActionList.h"
#include "IEventSubscriber.h"
#include "EventPublisher.h"

namespace FieaGameEngine
{
	class Reaction : public ActionList, public IEventSubscriber
	{
	public:
		/// <summary>
		/// Reaction Default constructor
		/// </summary>
		Reaction() = default;

		/// <summary>
		/// Reaction Copy Constructor - defaulted
		/// </summary>
		/// <param name="other">Reaction to copy</param>
		Reaction(const Reaction& other) = default;

		/// <summary>
		/// Reaction Move Constructor - defaulted
		/// </summary>
		/// <param name="other">Reaction to move</param>
		Reaction(Reaction&& other) = default;

		/// <summary>
		/// Copy Assignment Operator - defaulted
		/// </summary>
		/// <param name="other">Reaction to copy</param>
		/// <returns>Reference to the copy</returns>
		Reaction& operator=(const Reaction& other) = default;

		/// <summary>
		/// Move Assignment Operator - defaulted
		/// </summary>
		/// <param name="other">Reaction to move</param>
		/// <returns>Reference to the moved Reaction</returns>
		Reaction& operator=(Reaction&& other) = default;

		/// <summary>
		/// Virtual Destructor - defaulted
		/// </summary>
		virtual ~Reaction() = default;

		/// <summary>
		/// Notify - Pure Virtual implementation - should compare subtypes of EventMessageAttributed to the subtype taken in from Json,
		/// If the same then should copy any arguments and then call the ActionList::Update Method.
		/// </summary>
		/// <param name="publisher">Event Publisher (that should be an EventMessageAttributed with matching subtype to successfully call update)</param>
		virtual void Notify(const EventPublisher& publisher) = 0;
	};

}

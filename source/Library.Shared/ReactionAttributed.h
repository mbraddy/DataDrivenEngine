#pragma once
#include "Reaction.h"
#include "Event.h"
#include "EventMessageAttributed.h"


namespace FieaGameEngine
{
	class ReactionAttributed : public Reaction
	{
	public:
		/// <summary>
		/// ReactionAttributed - Default Constructor
		/// </summary>
		ReactionAttributed();

		/// <summary>
		/// ReactionAttributed - Typecast constructor for passing in a publisher to subscribe to on construction
		/// </summary>
		/// <param name="publisher">Event of EventMessageAttributed to subscribe this reaction to.</param>
		explicit ReactionAttributed(const Event<EventMessageAttributed>& publisher);

		/// <summary>
		/// ReactionAttributed - Copy Constructor - defaulted.
		/// </summary>
		/// <param name="other">ReactionAttributed to copy</param>
		ReactionAttributed(const ReactionAttributed& other) = default;

		/// <summary>
		/// ReactionAttributed - Move Constructor - defaulted.
		/// </summary>
		/// <param name="other">ReactionAttributed to move</param>
		ReactionAttributed(ReactionAttributed&& other) = default;

		/// <summary>
		/// ReactionAttributed - copy assignment operator - defaulted.
		/// </summary>
		/// <param name="other">ReactionAttributed to copy</param>
		/// <returns>Reference to the copy</returns>
		ReactionAttributed& operator=(const ReactionAttributed& other) = default;

		/// <summary>
		/// ReactionAttributed - move assignment operator - defaulted.
		/// </summary>
		/// <param name="other">ReactionAttributed to move</param>
		/// <returns>Reference to the moved ReactionAttributed</returns>
		ReactionAttributed& operator=(ReactionAttributed&& other) = default;

		/// <summary>
		/// Virtual ReactionAttributed destructor - If this is subscribed to something, it unsubscribes before destruction.
		/// </summary>
		virtual ~ReactionAttributed();

		/// <summary>
		/// Notify - Accepts Attributed events, if the event subtype matches the reaction's subtype then it copies the attribute arguments
		/// to this instance of ReactionAttribute and then executes ActionList::Update().
		/// </summary>
		/// <param name="publisher">Event to check for subtypes. Should always be Attributed events.</param>
		void Notify(const EventPublisher& publisher) override;

		/// <summary>
		/// Signatures - Returns the static signature vector for all prescribed attributes. Required for Attributed.
		/// </summary>
		/// <returns>Returns the static signature vector for all prescribed attributes.</returns>
		const Vector<Signature> Signatures();

		/// <summary>
		/// SubscribeToAttributedEvent - Subscribes this reaction to an attributed event.
		/// </summary>
		/// <param name="publisher">Reference to the Event that wraps an attributed message to which this reaction should subscribe.</param>
		void SubscribeToAttributedEvent(const Event<EventMessageAttributed>& publisher);

	private:
		/// <summary>
		/// _subscribedEventPublisher - Pointer to the event publisher this event is subscribed to.
		/// </summary>
		const Event<EventMessageAttributed>* _subscribedEventPublisher;

		/// <summary>
		/// _subtype - string used for matching the subtypes of events such that any event that is passed into Notify will be checked as to
		/// *if* it should fire.
		/// </summary>
		std::string _subtype;
	};

	ConcreteFactory(ReactionAttributed, ActionList)
}


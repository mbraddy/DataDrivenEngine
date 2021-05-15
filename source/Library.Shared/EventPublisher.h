#pragma once
#include "RTTI.h"
#include "Vector.h"
#include "IEventSubscriber.h"

namespace FieaGameEngine
{

	class EventPublisher : public RTTI
	{
		RTTI_DECLARATIONS(EventPublisher, RTTI)

	public:

		/// <summary>
		/// EventPublisher Default Constructor - Deleted to prevent instantiation without being provided a list of subscribers.
		/// </summary>
		EventPublisher() = delete;

		/// <summary>
		/// EventPublisher constructor - Takes a reference to a list of subscribers and stores it's address.
		/// </summary>
		/// <param name="subscribers">Address to a list of all subscribers to this event publisher.</param>
		explicit EventPublisher(const Vector<IEventSubscriber*>& subscribers);

		/// <summary>
		/// EventPublisher Copy Constructor - defaulted since not responsible for the data of the subscriber pointer.
		/// </summary>
		/// <param name="other">Event Publisher to be copied.</param>
		EventPublisher(const EventPublisher& other) = default;

		/// <summary>
		/// EventPublisher Move Constructor - defaulted since not responsible for the data of the subscriber pointer.
		/// </summary>
		/// <param name="other">Event Publisher to be moved.</param>
		EventPublisher(EventPublisher&& other) = default;

		/// <summary>
		/// EventPublisher Copy Assignment Operator - defaulted since not responsible for the data of the subscriber pointer.
		/// </summary>
		/// <param name="other">Event Publisher to be copied.</param>
		/// <returns>Reference to the copy</returns>
		EventPublisher& operator=(const EventPublisher& other) = default;

		/// <summary>
		/// EventPublisher Move Assignment Operator - defaulted since not responsible for the data of the subscriber pointer.
		/// </summary>
		/// <param name="other">Event Publisher to be Moved.</param>
		/// <returns>Reference to the moved object.</returns>
		EventPublisher& operator=(EventPublisher&& other) = default;

		/// <summary>
		/// EventPublisher Destructor - Virtual for inheritance v-table - Defaulted.
		/// </summary>
		virtual ~EventPublisher() = default;

		/// <summary>
		/// Deliver - Loops through the list of subscribers and calls their Notify methods.
		/// </summary>
		void Deliver() const;

	protected:

		/// <summary>
		/// _subscribers - Pointer to the list of subscribers to this event publisher
		/// </summary>
		const Vector<IEventSubscriber*>* _subscribers = nullptr;
	};

}

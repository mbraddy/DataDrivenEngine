#pragma once

#include "DefaultEquality.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Singlely linked list implementation that is templated to take any objects. Also contains support for SList Iterators and ConstIterators.
	/// </summary>
	/// <typeparam name="T">Generic Objects</typeparam>
	template <typename T>
	class SList
	{
	private:
		/// <summary>
		/// Node struct - contains a piece of data (T) and a pointer to the next node.
		/// </summary>
		struct Node final
		{
			/// <summary>
			/// Node constructor for SList.
			/// </summary>
			/// <param name="data">L-Value Reference - data to be stored.</param>
			/// <param name="next">Pointer to the next node - initialized to null.</param>
			Node(const T& data, Node* next = nullptr);

			/// <summary>
			/// Node constructor for SList.
			/// </summary>
			/// <param name="data">R-Value Reference - data to be stored.</param>
			/// <param name="next">Pointer to the next node - initialized to null.</param>
			Node(T&& data, Node* next = nullptr);

			T _data;
			Node* _next;
		};

	public:
		using value_type = T;

		/// <summary>
		/// SList Iterator - Grants a way of stepping through the list
		/// </summary>
		class Iterator final
		{
			friend SList;
			friend class ConstIterator;

		public:
			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using reference = T;
			using pointer = T*;
			using iterator_category = std::forward_iterator_tag;

#pragma region Iterator Rule Of 6
			/// <summary>
			/// Defaulted Iterator Constructor
			/// </summary>
			Iterator() = default;

			/// <summary>
			/// Defaulted Iterator Copy Constructor
			/// </summary>
			/// <param name="other">Reference to the Iterator being copied.</param>
			Iterator(const Iterator& other) = default;
			
			/// <summary>
			/// Defaulted Iterator Move Constructor
			/// </summary>
			/// <param name="other">R-Value Reference to the data being moved.</param>
			/// <returns>The newly established Iterator.</returns>
			Iterator(Iterator&& other) noexcept = default;

			/// <summary>
			/// Defaulted Iterator copy assignment operator.
			/// </summary>
			/// <param name="other">Reference to the Iterator being copied.</param>
			/// <returns>The copied Iterator reference.</returns>
			Iterator& operator=(const Iterator& other) = default;

			/// <summary>
			/// Defaulted Iterator Move assignment operator
			/// </summary>
			/// <param name="other">R-Value Reference to the data being moved.</param>
			/// <returns>The newly established Iterator reference.</returns>
			Iterator& operator=(Iterator&& other) noexcept = default;

			/// <summary>
			/// Defaulted Iterator destructor.
			/// </summary>
			~Iterator() = default;
#pragma endregion

#pragma region Iterator Operator Overloads
			/// <summary>
			/// Compares two iterators for !sameness.
			/// </summary>
			/// <param name="other">A reference to an SList T ::Iterator</param>
			/// <returns>True if the iterators have different owners, or are on different nodes</returns>
			bool operator!=(const Iterator& other) const;

			/// <summary>
			/// Compares two iterators for sameness.
			/// </summary>
			/// <param name="other">A reference to an SList T ::Iterator</param>
			/// <returns>True if the iterators have the same owner and are on the same node.</returns>
			bool operator==(const Iterator& other) const;

			/// <summary>
			/// Pre-increment the Iterator.
			/// </summary>
			/// <returns>The Iterator on the next node.</returns>
			/// <exception cref="std::runtime_error">Incrementing on an unassigned Iterator (has no owner) will throw this runtime error.</exception>
			Iterator& operator++();

			/// <summary>
			/// Post-increment the Iterator.
			/// </summary>
			/// <param name="">Fake Param for Function Declaration</param>
			/// <returns>The Node the Iterator is on at the time of calling and then increments the original Iterator.</returns>
			/// <exception cref="std::runtime_error">Incrementing on an unassigned Iterator (has no owner) will throw this runtime error.</exception>
			Iterator operator++(int);

			/// <summary>
			/// Dereferences an Iterator and returns it's node's data member as a mutable reference. Not marked const because this is kinda wonky, but does not mutate the Iterator.
			/// </summary>
			/// <returns>A reference to mutable _data in the Iterator's _node->_data</returns>
			/// <exception cref="std::runtime_error">Attempting to dereference an Iterator with a _node->nullptr will throw a runtime error.</exception>
			T& operator*() const;
#pragma endregion

		private:
			/// <summary>
			/// Private Iterator constructor used for begin() and end().
			/// </summary>
			/// <param name="owner">Reference to an SList that is the Iterator's Owner.</param>
			/// <param name="node">Node Pointer that is the intended starting target of the iterator.</param>
			Iterator(const SList& owner, Node* node);

			/// <summary>
			/// Pointer to the owner of the Iterator (Owner SList is immutable through _owner). Initialized to nullptr if not given.
			/// </summary>
			const SList* _owner{ nullptr };

			/// <summary>
			/// Pointer to the current node of the iterator. Initialized to nullptr if not given.
			/// </summary>
			Node* _node{ nullptr };

		};	//	End of class Iterator

		/// <summary>
		/// SList ConstIterator - Grants a way of stepping through the list without allowing any mutability.
		/// </summary>
		class ConstIterator final
		{
			friend SList;

		public:
			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using reference = T;
			using pointer = T*;
			using iterator_category = std::forward_iterator_tag;

#pragma region ConstIterator Rule Of 6
			/// <summary>
			/// Default Constructor leaving the _owner and _node members null.
			/// </summary>
			ConstIterator() = default;

			/// <summary>
			/// Constructor for creating a ConstIterator copy of a non-const iterator.
			/// </summary>
			/// <param name="other">The non-const Iterator to be copied.</param>
			ConstIterator(const Iterator& other);

			/// <summary>
			/// Defaulted ConstIterator Copy Constructor
			/// </summary>
			/// <param name="other">Reference to the ConstIterator being copied.</param>
			ConstIterator(const ConstIterator& other) = default;

			/// <summary>
			/// Defaulted ConstIterator Move Constructor
			/// </summary>
			/// <param name="other">R-Value Reference to the data being moved.</param>
			/// <returns>The newly established ConstIterator.</returns>
			ConstIterator(ConstIterator&& other) noexcept = default;

			/// <summary>
			/// Defaulted ConstIterator copy assignment operator.
			/// </summary>
			/// <param name="other">Reference to the ConstIterator being copied.</param>
			/// <returns>The copied ConstIterator reference.</returns>
			ConstIterator& operator=(const ConstIterator& other) = default;

			/// <summary>
			/// Copy assignment operator for when you need to copy a non-const Iterator to a ConstIterator
			/// </summary>
			/// <param name="other">The non-const SList::Iterator to be copied.</param>
			/// <returns>Reference to the new ConstIterator</returns>
			ConstIterator& operator=(const Iterator& other);

			/// <summary>
			/// Defaulted ConstIterator Move assignment operator
			/// </summary>
			/// <param name="other">R-Value Reference to the data being moved.</param>
			/// <returns>The newly established ConstIterator reference.</returns>
			ConstIterator& operator=(ConstIterator&& other) noexcept = default;

			/// <summary>
			/// Defaulted ConstIterator destructor.
			/// </summary>
			~ConstIterator() = default;
#pragma endregion

#pragma region ConstIterator Operator Overloads
			/// <summary>
			/// Compares two ConstIterators for !sameness.
			/// </summary>
			/// <param name="other">A reference to an SList::ConstIterator</param>
			/// <returns>True if the ConstIterators have different owners, or are on different nodes</returns>
			bool operator!=(const ConstIterator& other) const;

			/// <summary>
			/// Compares two ConstIterators for sameness.
			/// </summary>
			/// <param name="other">A reference to an SList::ConstIterator</param>
			/// <returns>True if the ConstIterators have the same owner and are on the same node.</returns>
			bool operator==(const ConstIterator& other) const;

			/// <summary>
			/// Pre-increment the ConstIterator.
			/// </summary>
			/// <returns>The ConstIterator on the next node.</returns>
			/// <exception cref="std::runtime_error">Incrementing on an unassigned ConstIterator (has no owner) will throw this runtime error.</exception>
			ConstIterator& operator++();

			/// <summary>
			/// Post-increment the ConstIterator.
			/// </summary>
			/// <param name="">Fake Param for Function Declaration</param>
			/// <returns>The Node the ConstIterator is on at the time of calling and then increments the original ConstIterator.</returns>
			/// <exception cref="std::runtime_error">Incrementing on an unassigned ConstIterator (has no owner) will throw this runtime error.</exception>
			ConstIterator operator++(int);

			/// <summary>
			/// Dereferences an ConstIterator and returns it's node's data member as a nonmutable reference.
			/// </summary>
			/// <returns>A reference to nonmutable _data in the ConstIterator's _node->_data</returns>
			/// <exception cref="std::runtime_error">Attempting to dereference an ConstIterator with a _node->nullptr will throw a runtime error.</exception>
			const T& operator*() const;
#pragma endregion

		private:
			/// <summary>
			/// Private ConstIterator constructor used for begin(), end(), cend(), and cbegin().
			/// </summary>
			/// <param name="owner">Reference to an SList that is the ConstIterator's Owner.</param>
			/// <param name="node">Node Pointer that is the intended starting target of the ConstIterator.</param>
			ConstIterator(const SList& curOwner, Node* node);

			/// <summary>
			/// Pointer to the SList that owns the ConstIterator.
			/// </summary>
			const SList* _owner{ nullptr };

			/// <summary>
			/// Pointer to the node that the ConstIterator is currently on.
			/// </summary>
			const Node* _node{ nullptr };

		};	//	End of class ConstIterator

#pragma region SList Rule of 6
		/// <summary>
		/// Default Constructor
		/// </summary>
		SList() = default;

		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="other">Reference to the SList to be copied.</param>
		SList(const SList& other);

		/// <summary>
		/// Move Constructor
		/// </summary>
		/// <param name="other">Reference to the SList to take from.</param>
		/// <returns>Reference to the new list.</returns>
		SList(SList&& other) noexcept;

		/// <summary>
		/// Copy assignment operator overload
		/// </summary>
		/// <param name="other">Reference to the SList to be copied.</param>
		/// <returns>Reference to the copied list.</returns>
		SList& operator=(const SList& other);

		/// <summary>
		/// Move assignment operator
		/// </summary>
		/// <param name="other">Reference to the List to take from.</param>
		/// <returns>Reference to the new list.</returns>
		SList& operator=(SList&& other) noexcept;

		/// <summary>
		/// Destructor - Just calls Clear()
		/// </summary>
		~SList();
#pragma endregion
		
#pragma region SList Public Controls
		/// <summary>
		/// Push an L-value object to the back of the list.
		/// </summary>
		/// <param name="value">L-value param</param>
		Iterator PushBack(const T& value);

		/// <summary>
		/// Push an R-value object to the back of the list.
		/// </summary>
		/// <param name="value">R-value param</param>
		Iterator PushBack(T&& value);

		/// <summary>
		/// Push an L-value object to the front of the list.
		/// </summary>
		/// <param name="value">L-value param</param>
		Iterator PushFront(const T& value);

		/// <summary>
		/// Push an R-value object to the front of the list.
		/// </summary>
		/// <param name="value">R-value param</param>
		Iterator PushFront(T&& value);

		/// <summary>
		/// Delete the back of the list. Does not return the data. If you want _back, call Back() first.
		/// </summary>
		void PopBack();

		/// <summary>
		/// Delete the front of the list. Does not return the data. If you want _front, call Front() first.
		/// </summary>
		void PopFront();

		/// <summary>
		/// Delete all items on the list and reset pointers to nullptr.
		/// </summary>
		void Clear();
#pragma endregion

#pragma region SList Accessors
		/// <summary>
		/// Get the front item in the list.
		/// </summary>
		/// <returns>Reference to the _front node (mutable).</returns>
		/// <exception cref="std::runtime_error">Will throw runtime error if called on empty list.</exception>
		T& Front();

		/// <summary>
		/// Get the front item in the list.
		/// </summary>
		/// <returns>Constant reference to the _front node (non-mutable).</returns>
		/// <exception cref="std::runtime_error">Will throw runtime error if called on empty list.</exception>
		const T& Front() const;

		/// <summary>
		/// Get the back item in the list.
		/// </summary>
		/// <returns>Reference to the _back node (mutable).</returns>
		/// <exception cref="std::runtime_error">Will throw runtime error if called on empty list.</exception>
		T& Back();

		/// <summary>
		/// Get the back item in the list.
		/// </summary>
		/// <returns>Constant reference to the _back node (non-mutable)</returns>
		/// <exception cref="std::runtime_error">Will throw runtime error if called on empty list.</exception>
		const T& Back() const;
		
		/// <summary>
		/// Returns bool indicating if list is empty.
		/// </summary>
		/// <returns>bool true if _size == 0, else false</returns>
		bool IsEmpty() const;

		/// <summary>
		/// Returns the number of elements in the list.
		/// </summary>
		/// <returns>size_t _size of the list</returns>
		size_t Size() const;
#pragma endregion

#pragma region SList Iterator/ConstIterator Public Method Calls
		/// <summary>
		/// Gets the _front of the list owned by the Iterator
		/// </summary>
		/// <returns>Iterator pointing at the _front of it's owners list</returns>
		Iterator begin();

		/// <summary>
		/// Gets the node after the end of the list owned by the Iterator
		/// </summary>
		/// <returns>Always returns an Iterator owned by the list but with a node pointing to nullptr.</returns>
		Iterator end();

		/// <summary>
		/// Gets the _front of the list owned by the ConstIterator
		/// </summary>
		/// <returns>ConstIterator pointing at the _front of it's owners list</returns>
		ConstIterator begin() const;

		/// <summary>
		/// Gets the node after the end of the list owned by the ConstIterator
		/// </summary>
		/// <returns>Always returns an ConstIterator owned by the list but with a node pointing to nullptr.</returns>
		ConstIterator end() const;

		/// <summary>
		/// Gets a constant iterator pointing to the _front of the list owned by the Iterator.
		/// </summary>
		/// <returns>ConstIterator pointing at the _front of it's owners list</returns>
		ConstIterator cbegin() const;

		/// <summary>
		/// Gets the node after the end of the list owned by the Iterator.
		/// </summary>
		/// <returns>Always returns an ConstIterator owned by the list but with a node pointing to nullptr.</returns>
		ConstIterator cend() const;

		/// <summary>
		/// Insert an L-value reference to a data member into the list following a designated iterator.
		/// </summary>
		/// <param name="it">Iterator pointing prior to where you want the data inserted.</param>
		/// <param name="value">L-value reference of the data to be inserted</param>
		/// <returns>The original iterator passed in as it (with an updated _next pointer)</returns>
		/// <exception cref="runtime error">Attempting to InsertAfter on an unowned iterator or one owned by a different list will throw a runtime error.</exception>
		Iterator InsertAfter(const Iterator& it, const T& value);

		/// <summary>
		/// Insert an R-value reference to a data member into the list following a designated iterator.
		/// </summary>
		/// <param name="it">Iterator pointing prior to where you want the data inserted.</param>
		/// <param name="value">R-value reference of the data to be inserted</param>
		/// <returns>The original iterator passed in as it (with an updated _next pointer)</returns>
		/// <exception cref="runtime error">Attempting to InsertAfter on an unowned iterator or one owned by a different list will throw a runtime error.</exception>
		Iterator InsertAfter(const Iterator& it, T&& value);

		/// <summary>
		/// Find attempts to find the T value passed in within the list.
		/// </summary>
		/// <param name="value">T value that you wish to find in the list</param>
		/// <returns>Iterator pointing to the position of the first member in the list containing the desired data - Or if not in the list it will return Iterator.end()</returns>
		template <typename EqualityFunctor = DefaultEquality<T>>
		Iterator Find(const T& value);

		/// <summary>
		/// Find attempts to find the T value passed in within the list.
		/// </summary>
		/// <param name="value">T value that you wish to find in the list</param>
		/// <returns>ConstIterator pointing to the position of the first member in the list containing the desired data - Or if not in the list it will return ConstIterator.end()</returns>
		template <typename EqualityFunctor = DefaultEquality<T>>
		ConstIterator Find(const T& value) const;

		/// <summary>
		/// Remove attempts to delete a node with equilvant value from the list.
		/// </summary>
		/// <param name="value">The value you want removed from the list.</param>
		/// <returns>True if the value was found and removed from the list. False otherwise.</returns>
		template <typename EqualityFunctor = DefaultEquality<T>>
		bool Remove(const T& value);

		/// <summary>
		/// Remove attempts to delete the node pointed at by the iterator from the list.
		/// </summary>
		/// <param name="value">An iterator pointing to the value you want removed.</param>
		/// <returns>True if the value was found and removed from the list. False otherwise.</returns>
		/// <exception cref="runtime error">If attempting to use an iterator owned by a differnt list then a runtime error will be thrown.</exception>
		bool Remove(const Iterator& it);
#pragma endregion

	private:
		/// <summary>
		/// Number of elements in the list
		/// </summary>
		size_t _size{ 0 };

		/// <summary>
		/// Head of the list.
		/// </summary>
		Node* _front{ nullptr };

		/// <summary>
		/// Tail of the list.
		/// </summary>
		Node* _back{ nullptr };

	};

}

#include "SList.inl"
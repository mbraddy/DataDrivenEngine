#pragma once
#include <initializer_list>
#include "DefaultEquality.h"
#include "DefaultIncrement.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Vector class is a dynamic array that allows resizing.
	/// </summary>
	/// <typeparam name="T">Templated to store any generic data type. T is the template placeholder.</typeparam>
	template <typename T>
	class Vector
	{
	public:
		using value_type = T;
		
		/// <summary>
		/// Vector Iterator - Grants a way of iterating through the vector.
		/// </summary>
		class Iterator final
		{
			friend Vector;
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
			Iterator(const Iterator & other) = default;

			/// <summary>
			/// Defaulted Iterator Move Constructor
			/// </summary>
			/// <param name="other">R-Value Reference to the data being moved.</param>
			/// <returns>The newly established Iterator.</returns>
			Iterator(Iterator && other) noexcept = default;

			/// <summary>
			/// Defaulted Iterator copy assignment operator.
			/// </summary>
			/// <param name="other">Reference to the Iterator being copied.</param>
			/// <returns>The copied Iterator reference.</returns>
			Iterator& operator=(const Iterator & other) = default;

			/// <summary>
			/// Defaulted Iterator Move assignment operator
			/// </summary>
			/// <param name="other">R-Value Reference to the data being moved.</param>
			/// <returns>The newly established Iterator reference.</returns>
			Iterator& operator=(Iterator && other) noexcept = default;

			/// <summary>
			/// Defaulted Iterator destructor.
			/// </summary>
			~Iterator() = default;
#pragma endregion

#pragma region Iterator Operator Overloads
			/// <summary>
			/// Compares two iterators for !sameness.
			/// </summary>
			/// <param name="other">A reference to a Vector::Iterator</param>
			/// <returns>True if the iterators have different owners, or are on different Indeces</returns>
			bool operator!=(const Iterator& other) const;

			/// <summary>
			/// Compares two iterators for sameness.
			/// </summary>
			/// <param name="other">A reference to an Vector::Iterator</param>
			/// <returns>True if the iterators have the same owner and are on the same Index.</returns>
			bool operator==(const Iterator& other) const;

			/// <summary>
			/// Pre-increment the Iterator.
			/// </summary>
			/// <returns>The Iterator on the next Index.</returns>
			/// <exception cref="std::runtime_error">Incrementing on an unassigned Iterator (has no owner) will throw this runtime error.</exception>
			Iterator& operator++();

			/// <summary>
			/// Post-increment the Iterator.
			/// </summary>
			/// <param name="">Fake Param for Function Declaration</param>
			/// <returns>The Index the Iterator is on at the time of calling and then increments the original Iterator.</returns>
			/// <exception cref="std::runtime_error">Incrementing on an unassigned Iterator (has no owner) will throw this runtime error.</exception>
			Iterator operator++(int);

			/// <summary>
			/// Dereferences an Iterator and returns it's _data[_index] member as a mutable reference. Not marked const because this is kinda wonky, but does not mutate the Iterator.
			/// </summary>
			/// <returns>A reference to mutable _data in the Iterator's _owner's _data[_index]</returns>
			T& operator*() const;
#pragma endregion

		private:
			/// <summary>
			/// Constructs an Iterator with a designated owner and index. Used in begins and ends.
			/// </summary>
			/// <param name="owner">The owner of the Iterator</param>
			/// <param name="index">The initial index of the iterator.</param>
			Iterator(const Vector& owner, size_t index);

			/// <summary>
			/// A constant pointer to the owner of the iterator.
			/// </summary>
			const Vector* _owner{ nullptr };

			/// <summary>
			/// An index used for accessing the owner's _data member
			/// </summary>
			size_t _index{ 0_z };

		};	//	End of class Iterator

		/// <summary>
		/// Vector ConstIterator - Grants a way of iterating through the vector while guaranteeing const-ness.
		/// </summary>
		class ConstIterator final
		{
			friend Vector;

		public:
			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using reference = T;
			using pointer = T*;
			using iterator_category = std::forward_iterator_tag;

#pragma region ConstIterator Rule Of 6
			/// <summary>
			/// Default Constructor for ConstIterator - Defaulted.
			/// </summary>
			ConstIterator() = default;

			/// <summary>
			/// Constructor for creating a ConstIterator copy of a non-const iterator.
			/// </summary>
			/// <param name="other">The non-const Iterator to be copied.</param>
			ConstIterator(const Iterator & other);

			/// <summary>
			/// Defaulted ConstIterator Copy Constructor
			/// </summary>
			/// <param name="other">Reference to the ConstIterator being copied.</param>
			ConstIterator(const ConstIterator & other) = default;

			/// <summary>
			/// Defaulted ConstIterator Move Constructor
			/// </summary>
			/// <param name="other">R-Value Reference to the data being moved.</param>
			/// <returns>The newly established ConstIterator.</returns>
			ConstIterator(ConstIterator && other) noexcept = default;

			/// <summary>
			/// Defaulted ConstIterator copy assignment operator.
			/// </summary>
			/// <param name="other">Reference to the ConstIterator being copied.</param>
			/// <returns>The copied ConstIterator reference.</returns>
			ConstIterator& operator=(const ConstIterator & other) = default;

			/// <summary>
			/// Copy assignment operator for when you need to copy a non-const Iterator to a ConstIterator
			/// </summary>
			/// <param name="other">The non-const Iterator to be copied.</param>
			/// <returns>Reference to the new ConstIterator</returns>
			ConstIterator& operator=(const Iterator & other);

			/// <summary>
			/// Defaulted ConstIterator Move assignment operator
			/// </summary>
			/// <param name="other">R-Value Reference to the data being moved.</param>
			/// <returns>The newly established ConstIterator reference.</returns>
			ConstIterator& operator=(ConstIterator && other) noexcept = default;

			/// <summary>
			/// Defaulted ConstIterator destructor.
			/// </summary>
			~ConstIterator() = default;
#pragma endregion

#pragma region ConstIterator Operator Overloads
			/// <summary>
			/// Compares two ConstIterators for !sameness.
			/// </summary>
			/// <param name="other">A reference to a ConstIterator</param>
			/// <returns>True if the ConstIterators have different owners, or are on different indeces</returns>
			bool operator!=(const ConstIterator& other) const;

			/// <summary>
			/// Compares two ConstIterators for sameness.
			/// </summary>
			/// <param name="other">A reference to an SList::ConstIterator</param>
			/// <returns>True if the ConstIterators have the same owner and are on the same index.</returns>
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
			/// Dereferences an ConstIterator and returns it's owner's _data member at a given index as a nonmutable reference.
			/// </summary>
			/// <returns>A reference to nonmutable _data in the ConstIterator's owner's _data</returns>
			const T& operator*() const;
#pragma endregion

		private:
			/// <summary>
			/// ConstIterator constructor used in ends and begins that designates the owner and desired index.
			/// </summary>
			/// <param name="owner">Owner of the constIterator</param>
			/// <param name="index">Desired starting index.</param>
			ConstIterator(const Vector & owner, size_t index);

			/// <summary>
			/// Pointer to the vector that owns the constiterator
			/// </summary>
			const Vector* _owner{ nullptr };

			/// <summary>
			/// Index the ConstIterator is currently on. Used to access owner's _data member
			/// </summary>
			size_t _index{ 0_z };

		};	//	End of class ConstIterator

#pragma region Vector Rule Of 6
		/// <summary>
		/// Vector Constructor that provides option to designate capacity while creating.
		/// </summary>
		/// <param name="capacity">Desired capacity of the vector - defaults to 0</param>
		Vector(size_t capacity = 0_z);

		/// <summary>
		/// Vector Constructor that accepts an initializer list and sets the capacity to fit it.
		/// </summary>
		/// <param name="list">List of comma seperated objects to initialize with.</param>
		Vector(std::initializer_list<T> list);

		/// <summary>
		/// Copy Constructor - Makes a new vector copy of the old vector.
		/// </summary>
		/// <param name="other">Vector to be copied.</param>
		Vector(const Vector& other);

		/// <summary>
		/// Move Constructor - Makes a new Vector by stealing the data of the old vector. Kills the old vector.
		/// </summary>
		/// <param name="other">Vector to be taken from.</param>
		Vector(Vector&& other) noexcept;

		/// <summary>
		/// Copy Assignment Operator - Makes a new vector copy of the old vector.
		/// </summary>
		/// <param name="other">Vector to be copied.</param>
		/// <returns>Reference to the newly copied vector.</returns>
		Vector& operator=(const Vector& other);

		/// <summary>
		/// Move Assignment Operator - Makes a new vector by stealing the data of the old vector. Kills the old vector.
		/// </summary>
		/// <param name="other">The vector to be moved and invalidated.</param>
		/// <returns>Reference to the newly initialized vector. </returns>
		Vector& operator=(Vector&& other) noexcept;
		
		/// <summary>
		/// Destructor - Clears any existing data and then frees it.
		/// </summary>
		~Vector();
#pragma endregion

#pragma region Vector Push Methods
		/// <summary>
		/// Pushback function that instantiates and places the thing passed to into the back of the vector. If the capacity of the vector cannot support adding another value, then Pushback will increase the capacity using the IncrementFunctor Strategy and calling Reserve.
		/// </summary>
		/// <param name="value">L-value reference to a value to be placed in the vector</param>
		/// <typeparam name="IncrementFunctor">IncrementFunctor is "size_t operator()(size_t size, size_t capacity) const". Default strategy is return (capacity + 1) * 2 </typeparam>
		template <typename IncrementFunctor = DefaultIncrement>
		Iterator PushBack(const T& value);

		/// <summary>
		/// Pushback function that instantiates and places the thing passed to into the back of the vector. If the capacity of the vector cannot support adding another value, then Pushback will increase the capacity using the IncrementFunctor Strategy and calling Reserve.
		/// </summary>
		/// <param name="value">R-value reference to a value to be placed in the vector</param>
		/// <typeparam name="IncrementFunctor">IncrementFunctor is "size_t operator()(size_t size, size_t capacity) const". Default strategy is return (capacity + 1) * 2 </typeparam>
		template <typename IncrementFunctor = DefaultIncrement>
		Iterator PushBack(T&& value);

		/// <summary>
		/// Reserve expands the capacity of a given Vector.
		/// </summary>
		/// <param name="space">The new capacity you wish to expand the vector to.</param>
		/// <exception cref="std::runtime_error">Will throw a runtime error if attempting to reserve less space than currently allocated. Use ShrinkToFit() instead if you need to shrink a vector.</exception>
		void Reserve(size_t space);
#pragma endregion

#pragma region Vector Deletion Methods
		/// <summary>
		/// Loops through the vector and calls the destructor on all objects within. Sets size to 0 but does not effect the capacity.
		/// </summary>
		void Clear();

		/// <summary>
		/// Calls the destructor on the last item in the vector. Lowers size by one but doesn't change capacity.
		/// </summary>
		/// <exception cref="std::runtime_error">Will throw a runtime error if attempting to PopBack() an empty vector.</exception>
		void PopBack();

		/// <summary>
		/// Remove finds the first instance of the value stored in the vector and destructs it. It then memmoves the remaining objects trailing it forward to keep memory contiguous.
		/// </summary>
		/// <param name="value">The value to be removed.</param>
		/// <returns>True if successfully removed. False if not found.</returns>
		template <typename EqualityFunctor = DefaultEquality<T>>
		bool Remove(const T& value);

		/// <summary>
		/// Remove finds the _data at the Iterator's _index in the vector and destructs it. It then memmoves the remaining objects trailing it forward to keep memory contiguous.
		/// </summary>
		/// <param name="it">The iterator pointing to the data to be removed.</param>
		/// <returns>True if successfully removed. False if not found.</returns>
		/// <exception cref="std::runtime_error">Throws runtime error if trying to remove an unassociated iterator.</exception>
		bool Remove(const Iterator& it);

		/// <summary>
		/// Remove destructs all objects between the two iterators passed to it (themselves included). It then memmoves the remaining objects trailing it forward to keep memory contiguous.
		/// </summary>
		/// <param name="itStart">The iterator pointing to the first index to be removed.</param>
		/// <param name="itEnd">The iterator pointing to the last index to be removed.</param>
		/// <returns>True if successfully removed. False otherwise.</returns>
		/// <exception cref="std::runtime_error">Throws runtime error if either iterator are an unassociated iterator.</exception>
		bool Remove(const Iterator& itStart, const Iterator& itEnd);

		/// <summary>
		/// Resize the vector to the desired new size and fills the vectors with default constructed T's. (Primarily implemented for HashMap)
		/// </summary>
		/// <param name="size">The requested size of the vector.</param>
		void Resize(size_t size);

		/// <summary>
		/// Shrinks the capacity of an iterator to match it's size.
		/// </summary>
		void ShrinkToFit();
#pragma endregion

#pragma region Vector Accessor Methods
		/// <summary>
		/// Returns reference to the data at index location.
		/// </summary>
		/// <param name="index">Desired location to return in the vector.</param>
		/// <returns>Reference to the data at the index location.</returns>
		T& At(size_t index);

		/// <summary>
		/// Returns const reference to the data at index location.
		/// </summary>
		/// <param name="index">Desired location to return in the vector.</param>
		/// <returns>Const reference to the data at the index location.</returns>
		const T& At(size_t index) const;

		/// <summary>
		/// Returns reference to the data at index location size-1. (last element in container)
		/// </summary>
		/// <returns>Reference to the data at the index location size-1 (last element in container).</returns>
		/// <exception cref="std::runtime_error">Calling on an empty vector triggers a runtime error.</exception>
		/// <exception cref="std::runtime_error">Calling on a vector with no capacity triggers a runtime error.</exception>
		T& Back();

		/// <summary>
		/// Returns const reference to the data at index location size-1. (last element in container)
		/// </summary>
		/// <returns>Const reference to the data at the index location size-1 (last element in container).</returns>
		/// <exception cref="std::runtime_error">Calling on an empty vector triggers a runtime error.</exception>
		/// <exception cref="std::runtime_error">Calling on a vector with no capacity triggers a runtime error.</exception>
		const T& Back() const;
		
		/// <summary>
		/// Get the number of spaces allocated for objects.
		/// </summary>
		/// <returns>Number of objects you could fit in the allocated memory.</returns>
		size_t Capacity() const;

		/// <summary>
		/// Returns reference to the data at index location 0. (first element in container)
		/// </summary>
		/// <returns>Reference to the data at the index location 0 (first element in container).</returns>
		/// <exception cref="std::runtime_error">Calling on an empty vector triggers a runtime error.</exception>
		/// <exception cref="std::runtime_error">Calling on a vector with no capacity triggers a runtime error.</exception>
		T& Front();

		/// <summary>
		/// Returns const reference to the data at index location 0. (First element in container)
		/// </summary>
		/// <returns>Const reference to the data at the index location 0 (First element in container).</returns>
		/// <exception cref="std::runtime_error">Calling on an empty vector triggers a runtime error.</exception>
		/// <exception cref="std::runtime_error">Calling on a vector with no capacity triggers a runtime error.</exception>
		const T& Front() const;

		/// <summary>
		/// Tells you if anything is in the vector (doesn't check capacity.)
		/// </summary>
		/// <returns>true if the vector is empty</returns>
		bool IsEmpty() const;

		/// <summary>
		/// Returns reference to the data at index location.
		/// </summary>
		/// <param name="index">Desired location to return in the vector.</param>
		/// <returns>Reference to the data at the index location.</returns>
		T& operator[](size_t index);

		/// <summary>
		/// Returns const reference to the data at index location.
		/// </summary>
		/// <param name="index">Desired location to return in the vector.</param>
		/// <returns>Const reference to the data at the index location.</returns>
		const T& operator[](size_t index) const;

		/// <summary>
		/// Returns the number of items currently in the vector (not allocated space).
		/// </summary>
		/// <returns>Number of items currently in container as a size_t</returns>
		size_t Size() const;

#pragma endregion

#pragma region Vector Iterator/ConstIterator Public Method Calls
		/// <summary>
		/// Gets the 0th element of the vector owned by the Iterator
		/// </summary>
		/// <returns>Iterator pointing at the front of it's owners vector</returns>
		Iterator begin();

		/// <summary>
		/// Gets the 0th element of the vector owned by the ConstIterator
		/// </summary>
		/// <returns>ConstIterator pointing at the front of it's owners vector</returns>
		ConstIterator begin() const;

		/// <summary>
		/// Gets the 0th element of the vector owned by the Iterator
		/// </summary>
		/// <returns>ConstIterator pointing at the front of it's owners vector</returns>
		ConstIterator cbegin() const;

		/// <summary>
		/// Gets the index after the end of the vector owned by the Iterator.
		/// </summary>
		/// <returns>Always returns an ConstIterator owned by the list but with an index of _size.</returns>
		ConstIterator cend() const;

		/// <summary>
		/// Gets the index after the end of the vector owned by the Iterator.
		/// </summary>
		/// <returns>Always returns an ConstIterator owned by the list but with an index of _size.</returns>
		Iterator end();

		/// <summary>
		/// Gets the index after the end of the vector owned by the ConstIterator.
		/// </summary>
		/// <returns>Always returns an ConstIterator owned by the list but with an index of _size.</returns>
		ConstIterator end() const;

		/// <summary>
		/// Find searches for a given value within the vector and returns an iterator pointing to it if there.
		/// </summary>
		/// <param name="value">The value being searched for.</param>
		/// <returns>An iterator pointing to the index of the desired object if found or pointing to end() if not in the vector.</returns>
		template <typename EqualityFunctor = DefaultEquality<T>>
		Iterator Find(const T& value);

		/// <summary>
		/// Find searches for a given value within the vector and returns a ConstIterator pointing to it if there.
		/// </summary>
		/// <param name="value">The value being searched for.</param>
		/// <returns>A ConstIterator pointing to the index of the desired object if found or pointing to end() if not in the vector.</returns>
		template <typename EqualityFunctor = DefaultEquality<T>>
		ConstIterator Find(const T& value) const;

#pragma endregion

	private:
		/// <summary>
		/// The number of items currently in the Vector.
		/// </summary>
		size_t _size{ 0 };

		/// <summary>
		/// The number of items that have space allocated for them to fit in the vector. (Max potential members).
		/// </summary>
		size_t _capacity{ 0 };

		/// <summary>
		/// Pointer to the datablock allocated for sizeof(T) * _capacity members.
		/// </summary>
		T* _data{ nullptr };

	};

}

#include "Vector.inl"
#pragma once
#include "pch.h"
#include "DefaultEquality.h"
#include "DefaultHash.h"
#include "SList.h"
#include "Vector.h"

namespace FieaGameEngine
{
	/// <summary>
	/// HashMap Class - Unordered Map where <TKey,TData> Pairs are hashed based on TKey and inserted into the map.
	/// </summary>
	/// <typeparam name="TKey">The Key values that are being hashed into the map. These should be unique - duplicate keys overwrite previous entries.</typeparam>
	/// <typeparam name="TData">The Data associated with the unique key values.</typeparam>
	/// <typeparam name="HashFunctor">A Function Object that is used to hash TKey. See DefaultHash.inl for the default implementation.</typeparam>
	/// <typeparam name="EqualityFunctor">A Function Object that is used to test TKey equality. See DefaultEquality.inl for the default implementation.</typeparam>
	template <typename TKey, typename TData, typename HashFunctor = DefaultHash<TKey>, typename EqualityFunctor = DefaultEquality<TKey>>
	class HashMap final
	{
	public:
		using PairType = std::pair<const TKey, TData>;
		using value_type = PairType;
		using mapped_type = TData;

	private:
		using ChainType = SList<PairType>;
		using BucketType = Vector<ChainType>;
		using ChainIteratorType = typename ChainType::Iterator;
		using ConstChainIteratorType = typename ChainType::ConstIterator;

	public:
		/// <summary>
		/// Forward Iterator Class for HashMap.
		/// </summary>
		class Iterator final
		{
			friend HashMap;
			friend class ConstIterator;

		public:
			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			using value_type = PairType;
			using reference = PairType&;
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
			/// Compares two Iterators for ! equality.
			/// </summary>
			/// <param name="other">Other Iterator to be compared</param>
			/// <returns>True if any members are different. Elsewise false.</returns>
			bool operator!=(const Iterator& other) const;

			/// <summary>
			/// Compares two Iterators for equality.
			/// </summary>
			/// <param name="other">Other Iterator to be compared</param>
			/// <returns>True if all members are equivilant. Elsewise false.</returns>
			bool operator==(const Iterator& other) const;

			/// <summary>
			/// Increments the const iterator to the next element in the map.
			/// </summary>
			/// <returns>Iterator pointing to the next element, or end() if no elements left in the Map.</returns>
			/// <exception cref="std::runtime_error">Calling operator++ on an unassociated iterator will throw a runtime error.</exception>
			Iterator& operator++();

			/// <summary>
			/// Increments the const iterator to the next element in the map. But returns the unincremented Iterator.
			/// </summary>
			/// <returns>Copy of the original Iterator.</returns>
			/// <exception cref="std::runtime_error">Calling operator++ on an unassociated iterator will throw a runtime error.</exception>
			Iterator operator++(int);

			/// <summary>
			/// Dereference operator - returns the TData of the pair that the iterator is pointing to.
			/// </summary>
			/// <returns>TData reference of the data referenced by the iterator.</returns>
			/// <exception cref="std::runtime_error">Attempting to dereference an iterator that's chain iterator or bucket index is out of bounds will throw a runtime error.</exception>
			PairType& operator*() const;

			/// <summary>
			/// Dereference operator - returns the TData of the pair that the iterator pointer is pointing to.
			/// </summary>
			/// <returns>TData reference of the data referenced by the iterator.</returns>
			/// <exception cref="std::runtime_error">Attempting to dereference an iterator that's chain iterator or bucket index is out of bounds will throw a runtime error.</exception>
			PairType* operator->() const;

#pragma endregion

		private:
			/// <summary>
			/// Private Iterator constructor used within the class to instantiate Iterators.
			/// </summary>
			/// <param name="owner">Reference to the HashMap that owns the Iterator</param>
			/// <param name="bucketIndex">Current Bucket Index that the Iterator is looking at.</param>
			/// <param name="chainIt">Current Chain Iterator pointing to which link the Iterator is on.</param>
			Iterator(const HashMap& owner, const size_t& bucketIndex, const ChainIteratorType& chainIt);

			/// <summary>
			/// Pointer to the owning HashMap of the iterator. Defaulted nullptr.
			/// </summary>
			const HashMap* _owner{ nullptr };

			/// <summary>
			/// Current Bucket Index that the Iterator is pointing to. Defaulted 0_z.
			/// </summary>
			size_t _bucketIndex{ 0_z };

			/// <summary>
			/// Current ChainIterator pointing to the current link the Iterator is pointing to. Default Constructed.
			/// </summary>
			ChainIteratorType _chainIt;

		};	//	End of class Iterator

		/// <summary>
		/// Forward ConstIterator class for HashMap
		/// </summary>
		class ConstIterator final
		{
			friend HashMap;

		public:
			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			using value_type = PairType;
			using reference = PairType&;
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
			/// Compares two ConstIterators for ! equality.
			/// </summary>
			/// <param name="other">Other ConstIterator to be compared</param>
			/// <returns>True if any members are different. Elsewise false.</returns>
			bool operator!=(const ConstIterator& other) const;

			/// <summary>
			/// Compares two ConstIterators for equality.
			/// </summary>
			/// <param name="other">Other ConstIterator to be compared</param>
			/// <returns>True if all members are equivilant. Elsewise false.</returns>
			bool operator==(const ConstIterator& other) const;

			/// <summary>
			/// Increments the const iterator to the next element in the map.
			/// </summary>
			/// <returns>ConstIterator pointing to the next element, or end() if no elements left in the Map.</returns>
			/// <exception cref="std::runtime_error">Calling operator++ on an unassociated iterator will throw a runtime error.</exception>
			ConstIterator& operator++();

			/// <summary>
			/// Increments the const iterator to the next element in the map. But returns the unincremented Iterator.
			/// </summary>
			/// <returns>Copy of the original ConstIterator.</returns>
			/// <exception cref="std::runtime_error">Calling operator++ on an unassociated iterator will throw a runtime error.</exception>
			ConstIterator operator++(int);

			/// <summary>
			/// Dereference operator - returns the TData of the pair that the iterator is pointing to.
			/// </summary>
			/// <returns>TData reference of the data referenced by the iterator.</returns>
			/// <exception cref="std::runtime_error">Attempting to dereference an iterator that's chain iterator or bucket index is out of bounds will throw a runtime error.</exception>
			const PairType& operator*() const;

			/// <summary>
			/// Dereference operator - returns the TData of the pair that the iterator pointer is pointing to.
			/// </summary>
			/// <returns>TData reference of the data referenced by the iterator.</returns>
			/// <exception cref="std::runtime_error">Attempting to dereference an iterator that's chain iterator or bucket index is out of bounds will throw a runtime error.</exception>
			const PairType* operator->() const;

#pragma endregion

		private:
			/// <summary>
			/// Private ConstIterator constructor used within the class to instantiate Iterators.
			/// </summary>
			/// <param name="owner">Reference to the HashMap that owns the ConstIterator</param>
			/// <param name="bucketIndex">Current Bucket Index that the ConstIterator is looking at.</param>
			/// <param name="chainIt">Current Chain Iterator pointing to which link the ConstIterator is on.</param>
			ConstIterator(const HashMap& owner, const size_t& bucketIndex, const ChainIteratorType& chainIt);

			/// <summary>
			/// Pointer to the owning HashMap of the ConstIterator. Defaulted nullptr.
			/// </summary>
			const HashMap* _owner{ nullptr };

			/// <summary>
			/// Current Bucket Index that the ConstIterator is pointing to. Defaulted 0_z.
			/// </summary>
			size_t _bucketIndex{ 0_z };

			/// <summary>
			/// Current ChainIterator pointing to the current link the ConstIterator is pointing to. Default Constructed.
			/// </summary>
			ChainIteratorType _chainIt;

		};	//	End of class ConstIterator

		using InsertReturnPair = std::pair<typename Iterator, bool>;

#pragma region HashMap Rule Of 6

		/// <summary>
		/// HashMap Constructor.
		/// </summary>
		/// <param name="buckets">Takes in the desired number of buckets for the HashMap. Defaults to 16_z.</param>
		explicit HashMap(size_t buckets = 16_z);

		/// <summary>
		/// HashMap Constructor that takes an initializer list
		/// </summary>
		/// <param name="list">Entries to be inserted at construction.</param>
		HashMap(std::initializer_list<PairType> list);

		/// <summary>
		/// Copy Constructor.
		/// </summary>
		/// <param name="other">Reference to the HashMap to be copied.</param>
		HashMap(const HashMap& other) = default;

		/// <summary>
		/// Move Constructor.
		/// </summary>
		/// <param name="other">R-Value Reference to the HashMap to be moved.</param>
		/// <returns>Reference to the moved HashMap.</returns>
		HashMap(HashMap&& other) noexcept;

		/// <summary>
		/// Copy Assignment Operator.
		/// </summary>
		/// <param name="other">Reference to the HashMap to be copied.</param>
		/// <returns>Reference to the copied HashMap.</returns>
		HashMap& operator=(const HashMap& other) = default;

		/// <summary>
		/// Move Assignment Operator.
		/// </summary>
		/// <param name="other">R-Value Reference to the HashMap to be moved.</param>
		/// <returns>Reference to the moved HashMap.</returns>
		HashMap& operator=(HashMap&& other) noexcept;

		/// <summary>
		/// Defaulted HashMap Destructor.
		/// </summary>
		~HashMap() = default;

#pragma endregion

#pragma region HashMap Push Methods

		/// <summary>
		/// Insert Method for L-Value References.
		/// </summary>
		/// <param name="value">L-Value Reference to a pair to be put into the list.</param>
		/// <returns>std::pair(Iterator, bool) where Iterator is pointing at the location of the key in the map and bool indicates if the value was successfully placed.</returns>
		InsertReturnPair Insert(const PairType& value);

		/// <summary>
		/// Insert Method for R-Value References.
		/// </summary>
		/// <param name="value">R-Value Reference to a pair to be put into the list.</param>
		/// <returns>std::pair(Iterator, bool) where Iterator is pointing at the location of the key in the map and bool indicates if the value was successfully placed.</returns>
		InsertReturnPair Insert(PairType&& value);

#pragma endregion

#pragma region HashMap Deletion Methods
		
		/// <summary>
		/// Sets size of HashMap to zero and iterates through the chains in the buckets to destruct all pairs within - emptying the HashMap.
		/// </summary>
		void Clear();

		/// <summary>
		/// This Remove Variant calls Find on the key passed in, then calls the other variant of Remove using the iterator that was found.
		/// </summary>
		/// <param name="value">The Key of the pair to be removed.</param>
		/// <returns>True if found and removed successfully, false if not.</returns>
		bool Remove(const TKey& value);

		/// <summary>
		/// Remove deletes the item in the HashMap pointed to by the iterator passed into it.
		/// </summary>
		/// <param name="it">Iterator pointing to the item to remove.</param>
		/// <returns>True if the item was removed, false if not.</returns>
		/// <exception cref="std::runtime_error">Passing an iterator not owned by the HashMap will throw a runtime error.</exception>
		bool Remove(const Iterator& it);

		/// <summary>
		/// Resize - Resizes the hashmap.
		/// </summary>
		void Resize(const size_t& size);
#pragma endregion

#pragma region HashMap Accessor Methods

		/// <summary>
		/// Returns the TData paired with the TKey passed into At().
		/// </summary>
		/// <param name="key">The Key Value of the pair associated with the data you want to retrieve from the Map.</param>
		/// <returns>The TData value that is associated with the key provided.</returns>
		/// <exception cref="std::runtime_error">Attempting to call At on a key that is not in the Map will throw a runtime error.</exception>
		TData& At(const TKey& key);

		/// <summary>
		/// Returns the TData paired with the TKey passed into At() as a constant reference.
		/// </summary>
		/// <param name="key">The Key Value of the pair associated with the data you want to retrieve from the Map.</param>
		/// <returns>The TData value that is associated with the key provided as a constant reference.</returns>
		/// <exception cref="std::runtime_error">Attempting to call At on a key that is not in the Map will throw a runtime error.</exception>
		const TData& At(const TKey& key) const;

		/// <summary>
		/// Tells you if a key is currently within the HashMap
		/// </summary>
		/// <param name="key">The Key you are searching for.</param>
		/// <returns>True if the key is in the Map, false if not.</returns>
		bool ContainsKey(const TKey& key) const;

		/// <summary>
		/// Tells you if the HashMap currently has any entries within it.
		/// </summary>
		/// <returns>True if the Map is empty, otherwise false.</returns>
		bool IsEmpty() const;

		/// <summary>
		/// Returns the TData paired with the TKey passed into operator[]. Allows Mutation of the Data. If the key is not within the Map, default constructs a TData member
		/// and associates it with the key so that it can be mutated into whatever value is desired.
		/// </summary>
		/// <param name="key">The Key Value of the pair associated with the data you want to retrieve from the Map.</param>
		/// <returns>The TData value that is associated with the key provided.</returns>
		TData& operator[](const TKey& key);

		/// <summary>
		/// Returns the TData paired with the TKey passed into operator[]. Does not allow Mutation of the Data. If the key is not within the Map, default constructs a TData member
		/// and associates it with the key but you cannot mutate it within this same call due to const-ness.
		/// </summary>
		/// <param name="key">The Key Value of the pair associated with the data you want to retrieve from the Map.</param>
		/// <returns>The TData value that is associated with the key provided.</returns>
		const TData& operator[](const TKey& key) const;

		/// <summary>
		/// Returns the number of elements currently in the HashMap
		/// </summary>
		/// <returns>Current number of elements in the HashMap as a size_t.</returns>
		size_t Size() const;

#pragma endregion

#pragma region HashMap Iterator/ConstIterator Public Method Calls

		/// <summary>
		/// Gets the first element in the hashmap owned by the Iterator
		/// </summary>
		/// <returns>Iterator pointing to the first element in the HashMap or end() if empty</returns>
		Iterator begin();

		/// <summary>
		/// Gets the first element in the hashmap owned by the Iterator
		/// </summary>
		/// <returns>ConstIterator pointing to the first element in the HashMap or end() if empty</returns>
		ConstIterator begin() const;

		/// <summary>
		/// Gets the first element in the hashmap owned by the Iterator
		/// </summary>
		/// <returns>ConstIterator pointing to the first element in the HashMap or end() if empty</returns>
		ConstIterator cbegin() const;

		/// <summary>
		/// Gets a ConstIterator where _bucketIndex == _size and _chainIt == end().
		/// </summary>
		/// <returns>Always returns an ConstIterator pointing one past the last element in the map.</returns>
		ConstIterator cend() const;

		/// <summary>
		/// Gets a Iterator where _bucketIndex == _size and _chainIt == end().
		/// </summary>
		/// <returns>Always returns an Iterator pointing one past the last element in the map.</returns>
		Iterator end();

		/// <summary>
		/// Gets a ConstIterator where _bucketIndex == _size and _chainIt == end().
		/// </summary>
		/// <returns>Always returns an ConstIterator pointing one past the last element in the map.</returns>
		ConstIterator end() const;

		/// <summary>
		/// Find returns an iterator pointing to the location of the pair containing the key.
		/// </summary>
		/// <param name="key">The key value of the pair you are looking for.</param>
		/// <returns>Iterator pointing to the location of the key in the map - or end() if not in map.</returns>
		Iterator Find(const TKey& key);

		/// <summary>
		/// Find returns an constiterator pointing to the location of the pair containing the key.
		/// </summary>
		/// <param name="key">The key value of the pair you are looking for.</param>
		/// <returns>ConstIterator pointing to the location of the key in the map - or end() if not in map.</returns>
		ConstIterator Find(const TKey& key) const;

#pragma endregion

	private:
		/// <summary>
		/// Private overload of Find which allows for the passing of an output parameter index. (For construction of HashMap<TKey,TData>::Iterator/ConstIterator().)
		/// </summary>
		/// <param name="key">The key you are looking for.</param>
		/// <param name="index">Output parameter - Will return the index that the key hashes to for usage outside of the Find call.</param>
		/// <returns>An Iterator pointing to the key within the map.</returns>
		Iterator Find(const TKey& key, size_t& index);

		/// <summary>
		/// A Vector<SLists<std::make_pair(TKey, TData)>>. The size of the Vector is instantiated upon construction. 
		/// Holds the SList Chains that contain inserted values.
		/// </summary>
		BucketType _buckets;

		/// <summary>
		/// A counter variable that holds the current number of elements that have been inserted into the HashMap.
		/// </summary>
		size_t _size{ 0_z };

	};
	

}

#include "HashMap.inl"
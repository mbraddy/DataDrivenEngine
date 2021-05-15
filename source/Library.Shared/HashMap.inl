#include "pch.h"
#include "HashMap.h"
#include <stdexcept>
#include <cassert>

namespace FieaGameEngine
{
#pragma region HashMap Rule Of 6
	template<typename TKey, typename TData, typename HashFunctor, typename EqualityFunctor>
	inline HashMap<TKey, TData, HashFunctor, EqualityFunctor>::HashMap(size_t buckets)
	{
		_buckets.Resize(buckets);
	}

	template<typename TKey, typename TData, typename HashFunctor, typename EqualityFunctor>
	inline HashMap<TKey, TData, HashFunctor, EqualityFunctor>::HashMap(std::initializer_list<PairType> list)
	{
		_buckets.Resize(list.size());

		for (const auto& value : list)
		{
			Insert(value);
		}
	}

	template<typename TKey, typename TData, typename HashFunctor, typename EqualityFunctor>
	inline HashMap<TKey, TData, HashFunctor, EqualityFunctor>::HashMap(HashMap&& other) noexcept :
		_size(other._size), _buckets(std::forward<BucketType>(other._buckets))
	{
		other._size = 0_z;
	}

	template<typename TKey, typename TData, typename HashFunctor, typename EqualityFunctor>
	inline typename HashMap<TKey, TData, HashFunctor, EqualityFunctor>::HashMap& HashMap<TKey, TData, HashFunctor, EqualityFunctor>::operator=(HashMap&& other) noexcept
	{
		if (this != &other)
		{
			_size = other._size;
			_buckets = std::forward<BucketType>(other._buckets);

			other._size = 0_z;
		}

		return *this;
	}
#pragma endregion

#pragma region HashMap Push Methods
	template<typename TKey, typename TData, typename HashFunctor, typename EqualityFunctor>
	inline typename HashMap<TKey, TData, HashFunctor, EqualityFunctor>::InsertReturnPair HashMap<TKey, TData, HashFunctor, EqualityFunctor>::Insert(const PairType& value)
	{
		size_t index;
		Iterator it = Find(value.first, index);

		if (it == end())
		{
			ChainIteratorType cit = _buckets[index].PushFront(value);
			++_size;
			return std::make_pair(Iterator(*this, index, cit), true);
		}

		return std::make_pair(it, false);
	}

	template<typename TKey, typename TData, typename HashFunctor, typename EqualityFunctor>
	inline typename HashMap<TKey, TData, HashFunctor, EqualityFunctor>::InsertReturnPair HashMap<TKey, TData, HashFunctor, EqualityFunctor>::Insert(PairType&& value)
	{
		size_t index;
		Iterator it = Find(value.first, index);

		if (it == end())
		{
			ChainIteratorType cit = _buckets[index].PushFront(std::forward<PairType>(value));
			++_size;
			return std::make_pair(Iterator(*this, index, cit), true);
		}
		
		return std::make_pair(it, false);
	}
#pragma endregion

#pragma region HashMap Deletion Methods
	template<typename TKey, typename TData, typename HashFunctor, typename EqualityFunctor>
	inline void HashMap<TKey, TData, HashFunctor, EqualityFunctor>::Clear()
	{
		if (_size != 0)
		{
			for (size_t i = 0; i < _buckets.Size(); ++i)
			{
				_buckets[i].Clear();
			}
			_size = 0;
		}
	}

	template<typename TKey, typename TData, typename HashFunctor, typename EqualityFunctor>
	inline bool HashMap<TKey, TData, HashFunctor, EqualityFunctor>::Remove(const TKey& value)
	{
		return Remove(Find(value));
	}

	template<typename TKey, typename TData, typename HashFunctor, typename EqualityFunctor>
	inline bool HashMap<TKey, TData, HashFunctor, EqualityFunctor>::Remove(const Iterator& it)
	{
		if (it._owner != this)
		{
			throw std::runtime_error("Invalid iterator. Owned by a different container.");
		}

		bool valueFound = false;
		if (it != end())
		{
			_buckets[it._bucketIndex].Remove(it._chainIt);
			valueFound = true;
			--_size;
		}

		return valueFound;
	}
#pragma endregion

#pragma region HashMap Accessors
	template<typename TKey, typename TData, typename HashFunctor, typename EqualityFunctor>
	inline typename TData& HashMap<TKey, TData, HashFunctor, EqualityFunctor>::At(const TKey& key)
	{
		Iterator it = Find(key);
		if (it == end())
		{
			throw std::runtime_error("Key not found with At()");
		}
		return it->second;
	}

	template<typename TKey, typename TData, typename HashFunctor, typename EqualityFunctor>
	inline typename const TData& HashMap<TKey, TData, HashFunctor, EqualityFunctor>::At(const TKey& key) const
	{
		ConstIterator it = Find(key);
		if (it == end())
		{
			throw std::runtime_error("Key not found with At()");
		}
		return it->second;
	}

	template<typename TKey, typename TData, typename HashFunctor, typename EqualityFunctor>
	inline bool HashMap<TKey, TData, HashFunctor, EqualityFunctor>::ContainsKey(const TKey& key) const
	{
		ConstIterator it = Find(key);
		return (it != cend());
	}

	template<typename TKey, typename TData, typename HashFunctor, typename EqualityFunctor>
	inline bool FieaGameEngine::HashMap<TKey, TData, HashFunctor, EqualityFunctor>::IsEmpty() const
	{
		return (_size == 0_z);
	}

	template<typename TKey, typename TData, typename HashFunctor, typename EqualityFunctor>
	inline typename TData& HashMap<TKey, TData, HashFunctor, EqualityFunctor>::operator[](const TKey& key)
	{
		auto [it, insertBool] = Insert(PairType(key, TData()));
		return it->second;
	}

	template<typename TKey, typename TData, typename HashFunctor, typename EqualityFunctor>
	inline typename const TData& HashMap<TKey, TData, HashFunctor, EqualityFunctor>::operator[](const TKey& key) const
	{
		return At(key);
	}

	template<typename TKey, typename TData, typename HashFunctor, typename EqualityFunctor>
	inline size_t HashMap<TKey, TData, HashFunctor, EqualityFunctor>::Size() const
	{
		return _size;
	}

	template<typename TKey, typename TData, typename HashFunctor, typename EqualityFunctor>
	inline void HashMap<TKey, TData, HashFunctor, EqualityFunctor>::Resize(const size_t& size)
	{
		BucketType resizedBucketList;
		resizedBucketList.Resize(size);

		HashFunctor hf;
		for (size_t i = 0; i < _buckets.Size(); ++i)
		{
			ChainType bucket = _buckets[i];
			if (!bucket.IsEmpty())
			{
				for (auto it = bucket.begin(); it != bucket.end(); ++it)
				{
					PairType pair = *(it);
					size_t index = ((hf(pair.first)) % size);
					resizedBucketList[index].PushFront(pair);
				}
			}
		}
		_buckets = resizedBucketList;
	}

#pragma endregion

#pragma region HashMap Iterator/ConstIterator Public Method Calls
	template<typename TKey, typename TData, typename HashFunctor, typename EqualityFunctor>
	inline typename HashMap<TKey, TData, HashFunctor, EqualityFunctor>::Iterator HashMap<TKey, TData, HashFunctor, EqualityFunctor>::begin()
	{
		for (size_t i = 0; i < _buckets.Size(); ++i)
		{
			ChainType& chain = _buckets[i];
			if (!chain.IsEmpty())
			{
				return Iterator(*this, i, chain.begin());
			}
		}

		return end();
	}

	template<typename TKey, typename TData, typename HashFunctor, typename EqualityFunctor>
	inline typename HashMap<TKey, TData, HashFunctor, EqualityFunctor>::ConstIterator HashMap<TKey, TData, HashFunctor, EqualityFunctor>::begin() const
	{
		for (size_t i = 0; i < _buckets.Size(); ++i)
		{
			const ChainType& chain = _buckets[i];
			if (!chain.IsEmpty())
			{
				return ConstIterator(*this, i, const_cast<ChainType&>(chain).begin());
			}
		}

		return end();
	}

	template<typename TKey, typename TData, typename HashFunctor, typename EqualityFunctor>
	inline typename HashMap<TKey, TData, HashFunctor, EqualityFunctor>::ConstIterator HashMap<TKey, TData, HashFunctor, EqualityFunctor>::cbegin() const
	{
		for (size_t i = 0; i < _buckets.Size(); ++i)
		{
			const ChainType& chain = _buckets[i];
			if (!chain.IsEmpty())
			{
				return ConstIterator(*this, i, const_cast<ChainType&>(chain).begin());
			}
		}

		return end();
	}

	template<typename TKey, typename TData, typename HashFunctor, typename EqualityFunctor>
	inline typename HashMap<TKey, TData, HashFunctor, EqualityFunctor>::ConstIterator HashMap<TKey, TData, HashFunctor, EqualityFunctor>::cend() const
	{
		return ConstIterator(*this, _buckets.Size(), ChainIteratorType());
	}

	template<typename TKey, typename TData, typename HashFunctor, typename EqualityFunctor>
	inline typename HashMap<TKey, TData, HashFunctor, EqualityFunctor>::Iterator HashMap<TKey, TData, HashFunctor, EqualityFunctor>::end()
	{
		return Iterator(*this, _buckets.Size(), ChainIteratorType());
	}

	template<typename TKey, typename TData, typename HashFunctor, typename EqualityFunctor>
	inline typename HashMap<TKey, TData, HashFunctor, EqualityFunctor>::ConstIterator HashMap<TKey, TData, HashFunctor, EqualityFunctor>::end() const
	{
		return ConstIterator(*this, _buckets.Size(), ChainIteratorType());
	}

	template<typename TKey, typename TData, typename HashFunctor, typename EqualityFunctor>
	inline typename HashMap<TKey, TData, HashFunctor, EqualityFunctor>::Iterator HashMap<TKey, TData, HashFunctor, EqualityFunctor>::Find(const TKey& key)
	{
		size_t index;
		return Find(key, index);
	}

	template<typename TKey, typename TData, typename HashFunctor, typename EqualityFunctor>
	inline typename HashMap<TKey, TData, HashFunctor, EqualityFunctor>::Iterator HashMap<TKey, TData, HashFunctor, EqualityFunctor>::Find(const TKey& key, size_t& index)
	{
		HashFunctor hf;
		index = ((hf(key)) % _buckets.Size());

		if (_size != 0)
		{
			EqualityFunctor eq;

			for (ChainIteratorType it = _buckets[index].begin(); it != _buckets[index].end(); ++it)
			{
				if (eq((*it).first, key))
				{
					return Iterator(*this, index, it);
				}
			}
		}

		return end();
	}

	template<typename TKey, typename TData, typename HashFunctor, typename EqualityFunctor>
	inline typename HashMap<TKey, TData, HashFunctor, EqualityFunctor>::ConstIterator HashMap<TKey, TData, HashFunctor, EqualityFunctor>::Find(const TKey& key) const
	{
		Iterator it = const_cast<HashMap<TKey, TData, HashFunctor, EqualityFunctor>* const>(this)->Find(key);
		return ConstIterator(it);
	}
#pragma endregion

#pragma region HashMap::Iterator Methods
	template<typename TKey, typename TData, typename HashFunctor, typename EqualityFunctor>
	inline HashMap<TKey, TData, HashFunctor, EqualityFunctor>::Iterator::Iterator(const HashMap& owner, const size_t& bucketIndex, const ChainIteratorType& chainIt) :
		_owner(&owner), _bucketIndex(bucketIndex), _chainIt(chainIt)
	{
	}

	template<typename TKey, typename TData, typename HashFunctor, typename EqualityFunctor>
	inline bool HashMap<TKey, TData, HashFunctor, EqualityFunctor>::Iterator::operator!=(const Iterator& other) const
	{
		return _owner != other._owner || _bucketIndex != other._bucketIndex || _chainIt != other._chainIt;
	}

	template<typename TKey, typename TData, typename HashFunctor, typename EqualityFunctor>
	inline bool HashMap<TKey, TData, HashFunctor, EqualityFunctor>::Iterator::operator==(const Iterator& other) const
	{
		return !(operator!=(other));
	}

	template<typename TKey, typename TData, typename HashFunctor, typename EqualityFunctor>
	typename HashMap<TKey, TData, HashFunctor, EqualityFunctor>::Iterator& HashMap<TKey, TData, HashFunctor, EqualityFunctor>::Iterator::operator++()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Attempting to iterate on an unassociated iterator.");
		}

		if (_owner->_size == 0)
		{
			*this = const_cast<HashMap<TKey, TData, HashFunctor, EqualityFunctor>*const>(_owner)->end();
			return *this;
		}

		bool nextNodeFound = false;

		ChainIteratorType currentLink = _chainIt;
		++currentLink;

		do 
		{
			//	Increment the iterator - If it's on an empty chain or at the end of the chain already, it will do nothing.
			if (currentLink != _owner->_buckets[_bucketIndex].end())
			{
				nextNodeFound = true;
				_chainIt = currentLink;
			}
			else
			{
				//	If at the end of a chain, increment the bucket index, check you aren't out of buckets, then move chain it to the begin() of the next chain in the bucket.
				++_bucketIndex;
				if (_bucketIndex == _owner->_buckets.Size())
				{
					*this = const_cast<HashMap<TKey, TData, HashFunctor, EqualityFunctor>*const>(_owner)->end();
					return *this;
				}
				ChainIteratorType startOfNextChain = const_cast<HashMap<TKey, TData, HashFunctor, EqualityFunctor>*const>(_owner)->_buckets[_bucketIndex].begin();
				currentLink = startOfNextChain;
			}
			//	Will either break on finding the next node or return end when out of buckets.
		} while (nextNodeFound == false);

		return *this;
	}

	template<typename TKey, typename TData, typename HashFunctor, typename EqualityFunctor>
	inline typename HashMap<TKey, TData, HashFunctor, EqualityFunctor>::Iterator HashMap<TKey, TData, HashFunctor, EqualityFunctor>::Iterator::operator++(int)
	{
		Iterator unIncrementedIterator(*this);
		operator++();

		return unIncrementedIterator;
	}

	template<typename TKey, typename TData, typename HashFunctor, typename EqualityFunctor>
	inline typename HashMap<TKey, TData, HashFunctor, EqualityFunctor>::PairType& HashMap<TKey, TData, HashFunctor, EqualityFunctor>::Iterator::operator*() const
	{
		if ( (_bucketIndex >= _owner->_buckets.Size()) || (_chainIt == _owner->_buckets[_bucketIndex].end()) )
		{
			throw std::runtime_error("Attempting to dereference outside of bounds in HashMap::Iterator operator*()");
		}

		return (*_chainIt);
	}

	template<typename TKey, typename TData, typename HashFunctor, typename EqualityFunctor>
	inline typename HashMap<TKey, TData, HashFunctor, EqualityFunctor>::PairType* HashMap<TKey, TData, HashFunctor, EqualityFunctor>::Iterator::operator->() const
	{
		if ((_bucketIndex >= _owner->_buckets.Size()) || (_chainIt == _owner->_buckets[_bucketIndex].end()))
		{
			throw std::runtime_error("Attempting to dereference outside of bounds in HashMap::ConstIterator operator*()");
		}

		return &(*_chainIt);
	}
#pragma endregion

#pragma region HashMap::ConstIterator Methods
	template<typename TKey, typename TData, typename HashFunctor, typename EqualityFunctor>
	inline HashMap<TKey, TData, HashFunctor, EqualityFunctor>::ConstIterator::ConstIterator(const HashMap& owner, const size_t& bucketIndex, const ChainIteratorType& chainIt) :
		_owner(&owner), _bucketIndex(bucketIndex), _chainIt(chainIt)
	{
	}

	template<typename TKey, typename TData, typename HashFunctor, typename EqualityFunctor>
	inline HashMap<TKey, TData, HashFunctor, EqualityFunctor>::ConstIterator::ConstIterator(const Iterator& other) :
		_owner(other._owner), _bucketIndex(other._bucketIndex), _chainIt(other._chainIt)
	{
	}

	template<typename TKey, typename TData, typename HashFunctor, typename EqualityFunctor>
	inline typename HashMap<TKey, TData, HashFunctor, EqualityFunctor>::ConstIterator& HashMap<TKey, TData, HashFunctor, EqualityFunctor>::ConstIterator::operator=(const Iterator& other)
	{
		_bucketIndex = other._bucketIndex;
		_owner = other._owner;
		_chainIt = other._chainIt;
		return *this;
	}


	template<typename TKey, typename TData, typename HashFunctor, typename EqualityFunctor>
	inline bool HashMap<TKey, TData, HashFunctor, EqualityFunctor>::ConstIterator::operator!=(const ConstIterator& other) const
	{
		return _owner != other._owner || _bucketIndex != other._bucketIndex || _chainIt != other._chainIt;
	}

	template<typename TKey, typename TData, typename HashFunctor, typename EqualityFunctor>
	inline bool HashMap<TKey, TData, HashFunctor, EqualityFunctor>::ConstIterator::operator==(const ConstIterator& other) const
	{
		return !(operator!=(other));
	}

	template<typename TKey, typename TData, typename HashFunctor, typename EqualityFunctor>
	typename HashMap<TKey, TData, HashFunctor, EqualityFunctor>::ConstIterator& HashMap<TKey, TData, HashFunctor, EqualityFunctor>::ConstIterator::operator++()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Attempting to iterate on an unassociated iterator.");
		}

		if (_owner->_size == 0)
		{
			*this = _owner->end();
			return *this;
		}

		bool nextNodeFound = false;

		ChainIteratorType currentLink = _chainIt;
		++currentLink;

		do
		{
			//	Increment the iterator - If it's on an empty chain or at the end of the chain already, it will do nothing.
			if (currentLink != _owner->_buckets[_bucketIndex].end())
			{
				nextNodeFound = true;
				_chainIt = currentLink;
			}
			else
			{
				//	If at the end of a chain, increment the bucket index, check you aren't out of buckets, then move chain it to the begin() of the next chain in the bucket.
				++_bucketIndex;
				if (_bucketIndex == _owner->_buckets.Size())
				{
					*this = _owner->end();
					return *this;
				}
				ChainIteratorType startOfNextChain = const_cast<HashMap<TKey, TData, HashFunctor, EqualityFunctor>*const>(_owner)->_buckets[_bucketIndex].begin();
				currentLink = startOfNextChain;
			}
			//	Will either break on finding the next node or return end when out of buckets.
		} while (nextNodeFound == false);

		return *this;
	}

	template<typename TKey, typename TData, typename HashFunctor, typename EqualityFunctor>
	inline typename HashMap<TKey, TData, HashFunctor, EqualityFunctor>::ConstIterator HashMap<TKey, TData, HashFunctor, EqualityFunctor>::ConstIterator::operator++(int)
	{
		ConstIterator unIncrementedIterator(*this);
		operator++();

		return unIncrementedIterator;
	}

	template<typename TKey, typename TData, typename HashFunctor, typename EqualityFunctor>
	inline const typename HashMap<TKey, TData, HashFunctor, EqualityFunctor>::PairType& HashMap<TKey, TData, HashFunctor, EqualityFunctor>::ConstIterator::operator*() const
	{
		if ( (_bucketIndex >= _owner->_buckets.Size()) || (_chainIt == _owner->_buckets[_bucketIndex].end()) )
		{
			throw std::runtime_error("Attempting to dereference outside of bounds in HashMap::ConstIterator operator*()");
		}

		return *_chainIt;
	}

	template<typename TKey, typename TData, typename HashFunctor, typename EqualityFunctor>
	inline const typename HashMap<TKey, TData, HashFunctor, EqualityFunctor>::PairType* HashMap<TKey, TData, HashFunctor, EqualityFunctor>::ConstIterator::operator->() const
	{
		if ((_bucketIndex >= _owner->_buckets.Size()) || (_chainIt == _owner->_buckets[_bucketIndex].end()))
		{
			throw std::runtime_error("Attempting to dereference outside of bounds in HashMap::ConstIterator operator*()");
		}

		return &(*_chainIt);
	}
#pragma endregion
}

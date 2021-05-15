#include "pch.h"
#include "SList.h"
#include <stdexcept>

namespace FieaGameEngine
{
#pragma region Node_Constructors
	template<typename T>
	inline SList<T>::Node::Node(const T& data, Node* next) :
		_data(data), _next(next)
	{
	}

	template<typename T>
	inline SList<T>::Node::Node(T&& data, Node* next) :
		_data(std::forward<T>(data)), _next(next)
	{
	}
#pragma endregion

#pragma region SList Rule of 6
	template<typename T>
	inline SList<T>::SList(const SList& other)
	{
		Node* currentNode = other._front;
		while (currentNode != nullptr)
		{
			PushBack(currentNode->_data);
			currentNode = currentNode->_next;
		}
	}

	template<typename T>
	inline SList<T>::SList(SList&& other) noexcept :
		_front(other._front), _back(other._back), _size(other._size)
	{
		other._front = nullptr;
		other._back = nullptr;
		other._size = 0_z;
	}

	template<typename T>
	inline SList<T>& SList<T>::operator=(const SList& other)
	{
		if (this != &other)
		{
			Clear();

			Node* currentNode = other._front;
			while (currentNode != nullptr)
			{
				PushBack(currentNode->_data);
				currentNode = currentNode->_next;
			}
		}

		return *this;
	}

	template<typename T>
	inline SList<T>& SList<T>::operator=(SList&& other) noexcept
	{
		if (this != &other)
		{
			Clear();

			_front = other._front;
			_back = other._back;
			_size = other._size;

			other._front = nullptr;
			other._back = nullptr;
			other._size = 0_z;
		}
		
		return *this;
	}

	template<typename T>
	inline SList<T>::~SList()
	{
		Clear();
	}
#pragma endregion

#pragma region SList Push Methods
	template<typename T>
	typename SList<T>::Iterator SList<T>::PushBack(const T& value)
	{ 
		Node* node = new Node(value, nullptr);

		if (_back != nullptr)
		{
			_back->_next = node;
			
		}
		
		_back = node;

		if (_front == nullptr)
		{
			_front = _back;
		}

		++_size;

		return Iterator(*this, node);
	}

	template<typename T>
	typename SList<T>::Iterator SList<T>::PushBack(T&& value)
	{
		Node* node = new Node(std::forward<T>(value), nullptr);
		
		if (_back != nullptr)
		{
			_back->_next = node;

		}

		_back = node;

		if (_front == nullptr)
		{
			_front = _back;
		}

		++_size;

		return Iterator(*this, node);
	}

	template<typename T>
	typename SList<T>::Iterator SList<T>::PushFront(const T& value)
	{
		_front = new Node(value, _front);
		if (_size == 0)
		{
			_back = _front;
		}

		++_size;

		return Iterator(*this, _front);
	}

	template<typename T>
	typename SList<T>::Iterator SList<T>::PushFront(T&& value)
	{
		_front = new Node(std::forward<T>(value), _front);
		if (_size == 0)
		{
			_back = _front;
		}

		++_size;

		return Iterator(*this, _front);
	}
#pragma endregion

#pragma region SList Deletion Methods
	template<typename T>
	inline void SList<T>::Clear()
	{
		Node* currentNode = _front;
		while (currentNode != nullptr)
		{
			Node* nodeToDelete = currentNode;
			currentNode = currentNode->_next;
			delete nodeToDelete;
		}

		_size = 0_z;
		_front = _back = nullptr;
	}

	template<typename T>
	inline void SList<T>::PopBack()
	{
		if (_size != 0)
		{
			if (_front == _back)
			{
				delete _back;
				_front = nullptr;
				_back = nullptr;
			}
			else
			{
				Node* currentNode = _front;
				for (size_t i = 1; i < (_size - 1); i++)
				{
					currentNode = currentNode->_next;
				}
				delete _back;
				_back = currentNode;
				_back->_next = nullptr;
			}
			--_size;
		}
	}

	template<typename T>
	inline void SList<T>::PopFront()
	{
		if (_size != 0)
		{
			Node* nodeToDelete = _front;
			_front = _front->_next;
			delete nodeToDelete;
			--_size;
		}
	}
#pragma endregion

#pragma region SList Accessor Methods
	template<typename T>
	inline T& SList<T>::Front()
	{
		if (_front == nullptr)
		{
			throw std::runtime_error("List is empty.");
		}

		return _front->_data;
	}

	template<typename T>
	inline const T& SList<T>::Front() const
	{
		if (_front == nullptr)
		{
			throw std::runtime_error("List is empty.");
		}

		return _front->_data;
	}

	template<typename T>
	inline T& SList<T>::Back()
	{
		if (_back == nullptr)
		{
			throw std::runtime_error("List is empty.");
		}

		return _back->_data;
	}

	template<typename T>
	inline const T& SList<T>::Back() const
	{
		if (_back == nullptr)
		{
			throw std::runtime_error("List is empty.");
		}

		return _back->_data;
	}

	template<typename T>
	inline size_t SList<T>::Size() const
	{
		return _size;
	}

	template<typename T>
	inline bool SList<T>::IsEmpty() const
	{
		return (_size == 0_z);
	}
#pragma endregion

#pragma region Iterator Methods
	template<typename T>
	inline SList<T>::Iterator::Iterator(const SList& owner, Node* node) :
		_owner(&owner), _node(node)
	{
	}

	template<typename T>
	inline bool SList<T>::Iterator::operator==(const Iterator& other) const
	{
		return !(operator!=(other));
	}

	template<typename T>
	inline bool SList<T>::Iterator::operator!=(const Iterator& other) const
	{
		return _owner != other._owner || _node != other._node;
	}

	template<typename T>
	inline typename SList<T>::Iterator& SList<T>::Iterator::operator++()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Attempting to iterate on an unassociated iterator.");
		}

		if (_node != nullptr)
		{
			_node = _node->_next;
		}
		
		return *this;
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::Iterator::operator++(int)
	{
		Iterator unIncrementedIterator(*this);
		operator++();

		return unIncrementedIterator;
	}

	template<typename T>
	inline T& SList<T>::Iterator::operator*() const
	{
		if (_node == nullptr)
		{
			throw std::runtime_error("Attempting dereference a null _node in operator*().");
		}

		return _node->_data;
	}

	template<typename T>
	template<typename EqualityFunctor>
	typename SList<T>::Iterator SList<T>::Find(const T& value)
	{
		Iterator it = begin();
		EqualityFunctor eq;

		for (; it != end(); ++it)
		{
			if (eq(*it, value))
			{
				break;
			}
		}

		return it;
	}

	template<typename T>
	typename SList<T>::Iterator SList<T>::InsertAfter(const Iterator& it, const T& value)
	{
		if (it._owner != this)
		{
			throw std::runtime_error("Invalid iterator. Owned by a different container.");
		}

		if (it == end() || it._node == _back)
		{
			return PushBack(value);
		}
		
		Node* node = new Node(value, it._node->_next);
		it._node->_next = node;
		++_size;
		return Iterator(*this, node);
	}

	template<typename T>
	typename SList<T>::Iterator SList<T>::InsertAfter(const Iterator& it, T&& value)
	{
		if (it._owner != this)
		{
			throw std::runtime_error("Invalid iterator. Owned by a different container.");
		}

		if (it == end() || it._node == _back)
		{
			return PushBack(value);
		}

		Node* node = new Node(std::forward<T>(value), it._node->_next);
		it._node->_next = node;
		++_size;
		return Iterator(*this, node);
	}

	template<typename T>
	bool SList<T>::Remove(const Iterator& it)
	{
		if (it._owner != this)
		{
			throw std::runtime_error("Invalid iterator. Owned by a different container.");
		}

		bool valueFound = false;
		if (it != end())
		{
			if (it._node == _back)
			{
				PopBack();
			}
			else
			{
				Node* next = it._node->_next;

				it._node->_data.~T();
				new(&it._node->_data)T(std::move(next->_data));
				it._node->_next = next->_next;
				delete next;

				//	If _back is deleted, need to assign the new _back.
				if (it._node->_next == nullptr)
				{
					_back = it._node;
				}

				--_size;
			}

			valueFound = true;
		}


		return valueFound;
	}

	template<typename T>
	template<typename EqualityFunctor>
	inline bool SList<T>::Remove(const T& value)
	{
		return Remove(Find<EqualityFunctor>(value));
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::begin()
	{
		return Iterator(*this, _front);
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::end()
	{
		return Iterator(*this, nullptr);;
	}
#pragma endregion

#pragma region ConstIterator Methods
	template<typename T>
	inline SList<T>::ConstIterator::ConstIterator(const Iterator& other) :
		_owner(other._owner), _node(other._node)
	{
	}

	template<typename T>
	inline SList<T>::ConstIterator::ConstIterator(const SList& curOwner, Node* node) :
		_owner(&curOwner), _node(node)
	{
	}

	template<typename T>
	inline typename SList<T>::ConstIterator& SList<T>::ConstIterator::operator=(const Iterator& other)
	{	
		_node = other._node;
		_owner = other._owner;
		return *this;
	}

	template<typename T>
	inline bool SList<T>::ConstIterator::operator!=(const ConstIterator& other) const
	{
		return _owner != other._owner || _node != other._node;
	}

	template<typename T>
	inline bool SList<T>::ConstIterator::operator==(const ConstIterator& other) const
	{
		return !(operator!=(other));;
	}

	template<typename T>
	inline typename SList<T>::ConstIterator& SList<T>::ConstIterator::operator++()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Attempting to iterate on an unassociated iterator.");
		}

		if (_node != nullptr)
		{
			_node = _node->_next;
		}

		return *this;
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::ConstIterator::operator++(int)
	{
		ConstIterator unIncrementedIterator(*this);
		operator++();

		return unIncrementedIterator;
	}

	template<typename T>
	inline const T& SList<T>::ConstIterator::operator*() const
	{
		if (_node == nullptr)
		{
			throw std::runtime_error("Attempting dereference a null _node in operator*().");
		}

		return _node->_data;
	}

	template<typename T>
	template<typename EqualityFunctor>
	inline typename SList<T>::ConstIterator SList<T>::Find(const T& value) const
	{
		Iterator it = const_cast<SList* const>(this)->Find<EqualityFunctor>(value);
		return ConstIterator(it);
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::begin() const
	{
		return ConstIterator(*this, _front);
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::end() const
	{
		return ConstIterator(*this, nullptr);
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::cbegin() const
	{
		return ConstIterator(*this, _front);
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::cend() const
	{
		return ConstIterator(*this, nullptr);
	}
#pragma endregion
}
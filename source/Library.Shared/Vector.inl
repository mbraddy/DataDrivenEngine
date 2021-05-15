#include "pch.h"
#include "Vector.h"
#include <stdexcept>
#include <cassert>

namespace FieaGameEngine
{
#pragma region Vector Rule Of 6
	template<typename T>
	inline Vector<T>::Vector(const size_t capacity) :
		_size(0_z), _capacity(0_z), _data(nullptr)
	{
		if (capacity > 0_z)
		{
			Reserve(capacity);
		}
	}

	template<typename T>
	Vector<T>::Vector(std::initializer_list<T> list)
	{
		Reserve(list.size());

		for (const auto& value : list)
		{
			new(_data + _size++)T(value);
		}
	}

	template<typename T>
	Vector<T>::Vector(const Vector& other)
	{
		if (other._capacity != 0)
		{
			_size = other._size;
			Reserve(other._capacity);
			
			for (size_t i = 0; i < _size; i++)
			{
				new(_data + i)T(other._data[i]);
			}
		}
	}
	
	template<typename T>
	inline Vector<T>::Vector(Vector&& other) noexcept :
		_size(other._size), _capacity(other._capacity), _data(other._data)
	{
		other._size = 0_z;
		other._capacity = 0_z;
		other._data = nullptr;
	}

	template<typename T>
	Vector<T>& Vector<T>::operator=(const Vector& other)
	{
		if (this != &other)
		{
			Clear();
			ShrinkToFit();

			if (other._capacity != 0)
			{
				_size = other._size;
				Reserve(other._capacity);

				for (size_t i = 0; i < _size; i++)
				{
					new(_data + i)T(other._data[i]);
				}
			}
		}

		return *this;
	}

	template<typename T>
	Vector<T>& Vector<T>::operator=(Vector&& other) noexcept
	{
		if (this != &other)
		{
			Clear();
			ShrinkToFit();

			_size = other._size;
			_capacity = other._capacity;
			_data = other._data;

			other._size = 0_z;
			other._capacity = 0_z;
			other._data = nullptr;
		}

		return *this;
	}

	template<typename T>
	inline Vector<T>::~Vector()
	{
		Clear();
		free(_data);
		_data = nullptr;
	}

#pragma endregion

#pragma region Vector Operator Overloads
	template<typename T>
	T& Vector<T>::operator[](size_t index)
	{
		if (index >= _size)
		{
			throw std::runtime_error("Attempting to call Vector.At[Out_Of_Bounds].");
		}

		return (_data[index]);
	}

	template<typename T>
	const T& Vector<T>::operator[](size_t index) const
	{
		if (index >= _size)
		{
			throw std::runtime_error("Attempting to call Vector.At[Out_Of_Bounds].");
		}

		return (_data[index]);
	}

#pragma endregion

#pragma region Vector Push Methods
	template<typename T>
	template <typename IncrementFunctor>
	typename Vector<T>::Iterator Vector<T>::PushBack(const T& value)
	{
		IncrementFunctor incStrat;

		if (_size == _capacity)
		{
			size_t result = incStrat(_size, _capacity);
			assert(result != 0_z);
			size_t expandedCapacity = _capacity + result;
			Reserve(expandedCapacity);
		}

		new(_data + _size)T(value);

		return Iterator(*this, _size++);
	}

	template<typename T>
	template <typename IncrementFunctor>
	typename Vector<T>::Iterator Vector<T>::PushBack(T&& value)
	{
		IncrementFunctor incStrat;

		if (_size == _capacity)
		{
			size_t result = incStrat(_size, _capacity);
			assert(result != 0_z);
			size_t expandedCapacity = _capacity + result;
			Reserve(expandedCapacity);
		}

		new(_data + _size)T(std::forward<T>(value));
			
		return Iterator(*this, _size++);
	}

	template<typename T>
	void Vector<T>::Reserve(size_t space)
	{
		if (space > _capacity)
		{
			T* temp = reinterpret_cast<T*>(realloc(_data, (sizeof(T) * space)));

			assert(temp != nullptr);
			_data = temp;
			_capacity = space;
		}
	}
#pragma endregion

#pragma region Vector Deletion Methods
	template<typename T>
	inline void Vector<T>::Clear()
	{
		for (size_t i = 0_z; i < _size; i++)
		{
			_data[i].~T();
		}

		_size = 0;
	}

	template<typename T>
	inline void Vector<T>::PopBack()
	{
		if (_size != 0)
		{
			_data[(_size - 1)].~T();
			--_size;
		}
		else
		{
			throw std::runtime_error("Calling PopBack() on an empty Vector.");
		}
	}

	template<typename T>
	template<typename EqualityFunctor>
	inline bool Vector<T>::Remove(const T& value)
	{
		return Remove(Find<EqualityFunctor>(value));
	}

	template<typename T>
	bool Vector<T>::Remove(const Iterator& it)
	{
		if (it._owner != this)
		{
			throw std::runtime_error("Invalid iterator. Owned by a different container.");
		}

		bool valueFound = false;
		if (it != end())
		{
			if (it._index == (_size - 1))
			{
				PopBack();
			}
			else
			{
				//	Call the destructor of the data at index position
				_data[it._index].~T();

				//	Mem Move all members beyond that position up a spot in the vector
				memmove(&(_data[it._index]), &(_data[(it._index) + 1]), (sizeof(T) * ((_size) - ((it._index) + 1))));

				--_size;
			}

			valueFound = true;
		}


		return valueFound;
	}

	template<typename T>
	bool Vector<T>::Remove(const Iterator& itStart, const Iterator& itEnd)
	{
		if ((itStart._owner != this) || (itEnd._owner != this))
		{
			throw std::runtime_error("Invalid iterator. Owned by a different container.");
		}

		bool valueFound = false;
		if ((itStart != end()) && (itStart._index <= itEnd._index))
		{
			size_t indexDiff = itEnd._index - itStart._index;
			
			Iterator endCopy = itEnd;
			++endCopy;
			Iterator it = itStart;
			for (; it != endCopy; ++it)
			{
				_data[it._index].~T();
			}

			//	If removing all the way to the end, no need to move memory.
			if (itEnd._index != (_size - 1))
			{
				//	Mem Move all members beyond the end position position up the vector
				memmove(&(_data[itStart._index]), &(_data[(itEnd._index) + 1]), (sizeof(T) * (indexDiff - 1)));
			}

			_size = _size - (indexDiff + 1);

			valueFound = true;
		}


		return valueFound;
	}

	template<typename T>
	inline void Vector<T>::Resize(size_t size)
	{
		if (size < _size)
		{
			for (size_t i = size; i < _size; ++i)
			{
				_data[i].~T();
			}

			_size = size;
			ShrinkToFit();
		}
		else
		{
			Reserve(size);

			for (size_t i = _size; i < size; ++i)
			{
				new (_data + i)T();
			}
		}

		_size = size;
	}

	template<typename T>
	void Vector<T>::ShrinkToFit()
	{
		if (_capacity > _size)
		{
			if (_size == 0_z)
			{
				free(_data);
				_data = nullptr;
				_capacity = 0_z;
			}
			else
			{
				T* temp = reinterpret_cast<T*>(realloc(_data, (sizeof(T) * _size)));
				assert(temp != nullptr);
				_data = temp;
				_capacity = _size;
			}
		}
	}
#pragma endregion

#pragma region Vector Accessors
	template<typename T>
	inline T& Vector<T>::At(size_t index)
	{
		if (index >= _size)
		{
			throw std::runtime_error("Attempting to call Vector.At[Out_Of_Bounds].");
		}

		return (_data[index]);
	}

	template<typename T>
	inline const T& Vector<T>::At(size_t index) const
	{
		if (index >= _size)
		{
			throw std::runtime_error("Attempting to call Vector.At[Out_Of_Bounds].");
		}

		return (_data[index]);
	}

	template<typename T>
	inline T& Vector<T>::Back()
	{
		if (_size == 0_z)
		{
			throw std::runtime_error("Calling Back() on a Vector with no elements.");
		}

		return (_data[_size - 1]);
	}

	template<typename T>
	inline const T& Vector<T>::Back() const
	{
		if (_size == 0_z)
		{
			throw std::runtime_error("Calling Back() on a Vector with no elements.");
		}

		return (_data[_size - 1]);
	}

	template<typename T>
	inline size_t Vector<T>::Capacity() const
	{
		return _capacity;
	}

	template<typename T>
	inline T& Vector<T>::Front()
	{
		if (_size == 0_z)
		{
			throw std::runtime_error("Calling Front() on an empty Vector.");
		}

		return (*_data);
	}

	template<typename T>
	inline const T& Vector<T>::Front() const
	{
		if (_size == 0_z)
		{
			throw std::runtime_error("Calling Front() on an empty Vector.");
		}

		return (*_data);
	}

	template<typename T>
	inline bool Vector<T>::IsEmpty() const
	{
		return (_size == 0_z);
	}

	template<typename T>
	inline size_t Vector<T>::Size() const
	{
		return _size;
	}

#pragma endregion

#pragma region Vector::Iterator Methods
	template<typename T>
	inline Vector<T>::Iterator::Iterator(const Vector& owner, size_t index) :
		_owner(&owner), _index(index)
	{
	}

	template<typename T>
	inline bool Vector<T>::Iterator::operator!=(const Iterator& other) const
	{
		return _owner != other._owner || _index != other._index;
	}

	template<typename T>
	inline bool Vector<T>::Iterator::operator==(const Iterator& other) const
	{
		return !(operator!=(other));
	}

	template<typename T>
	inline typename Vector<T>::Iterator& Vector<T>::Iterator::operator++()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Attempting to iterate on an unassociated iterator.");
		}

		if (_index < _owner->_size)
		{
			++_index;
		}

		return *this;
	}

	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::Iterator::operator++(int)
	{
		Iterator unIncrementedIterator(*this);
		operator++();

		return unIncrementedIterator;
	}

	template<typename T>
	inline T& Vector<T>::Iterator::operator*() const
	{
		assert(_owner != nullptr);		
		return _owner->_data[_index];
	}

	template<typename T>
	template<typename EqualityFunctor>
	typename Vector<T>::Iterator Vector<T>::Find(const T& value)
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
	inline typename Vector<T>::Iterator Vector<T>::begin()
	{
		return Iterator(*this, 0);
	}

	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::end()
	{
		return Iterator(*this, _size);
	}

#pragma endregion

#pragma region Vector::ConstIterator Methods
	template<typename T>
	inline Vector<T>::ConstIterator::ConstIterator(const Iterator& other) :
		_owner(other._owner), _index(other._index)
	{
	}

	template<typename T>
	inline Vector<T>::ConstIterator::ConstIterator(const Vector& owner, size_t index) :
		_owner(&owner), _index(index)
	{
	}

	template<typename T>
	inline const T& Vector<T>::ConstIterator::operator*() const
	{
		assert(_owner != nullptr);
		return _owner->_data[_index];
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator=(const Iterator& other)
	{
		_index = other._index;
		_owner = other._owner;
		return *this;
	}

	template<typename T>
	inline bool Vector<T>::ConstIterator::operator!=(const ConstIterator& other) const
	{
		return _owner != other._owner || _index != other._index;
	}

	template<typename T>
	inline bool Vector<T>::ConstIterator::operator==(const ConstIterator& other) const
	{
		return !(operator!=(other));;
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator++()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Attempting to iterate on an unassociated iterator.");
		}

		if (_index < _owner->_size)
		{
			++_index;
		}

		return *this;
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator++(int)
	{
		ConstIterator unIncrementedIterator(*this);
		operator++();

		return unIncrementedIterator;
	}

	template<typename T>
	template<typename EqualityFunctor>
	inline typename Vector<T>::ConstIterator Vector<T>::Find(const T& value) const
	{
		Iterator it = const_cast<Vector* const>(this)->Find<EqualityFunctor>(value);
		return ConstIterator(it);
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::begin() const
	{
		return ConstIterator(*this, 0);
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::cbegin() const
	{
		return ConstIterator(*this, 0);
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::end() const
	{
		return ConstIterator(*this, _size);
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::cend() const
	{
		return ConstIterator(*this, _size);
	}
#pragma endregion
}
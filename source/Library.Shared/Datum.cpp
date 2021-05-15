#include "pch.h"
#include "Datum.h"
#include <stdexcept>

namespace FieaGameEngine
{

#pragma region Datum Rule of 6
	Datum::Datum(DatumType type) :
		_type(type)
	{
	}

	Datum::Datum(const Datum& other) :
		_type(other._type), _size(other._size), _ownsData(other._ownsData)
	{
		//	Case where the data is owned
		if (other._ownsData == true)
		{
			Reserve(other._capacity);

			CreateCopyFunction func = _copyFunctions[static_cast<int>(_type)];
			assert(func != nullptr);

			(this->*func)(other._data, other._size);

		}
		else
		{
			_capacity = other._capacity;
			_data = other._data;
		}
	}

	Datum::Datum(Datum&& other) noexcept :
		_type(other._type), _size(other._size), _capacity(other._capacity), _data(other._data)
	{
		other._type = DatumType::Unknown;
		other._size = 0;
		other._capacity = 0;
		other._data.vp = nullptr;
	}

	Datum& Datum::operator=(Datum&& other) noexcept
	{
		if (*this != other)
		{
			Clear();
			ShrinkToFit();

			_type = other._type;
			_size = other._size;
			_capacity = other._capacity;
			_data = other._data;

			other._type = DatumType::Unknown;
			other._size = 0;
			other._capacity = 0;
			other._data.vp = nullptr;
		}
		return *this;
	}

	Datum& Datum::operator=(const Datum& other)
	{
		if (*this != other)
		{
			//	Both are owners of the data
			if (_ownsData == true && other._ownsData == true)
			{
				if (_type != DatumType::Unknown)
				{
					Clear();
					ShrinkToFit();
				}

				_type = other._type;
				_size = other._size;

				Reserve(other._capacity);

				assert(0 <= static_cast<int>(other._type) && static_cast<int>(other._type) < static_cast<int>(DatumType::Unknown));
				CreateCopyFunction func = _copyFunctions[static_cast<int>(_type)];
				assert(func != nullptr);

				(this->*func)(other._data, other._size);
			}

			//	Have to clean up this first
			else if (_ownsData == true && other._ownsData == false)
			{
				if (_type != DatumType::Unknown)
				{
					Clear();
					ShrinkToFit();
				}

				_type = other._type;
				_size = other._size;
				
				_capacity = other._capacity;
				_ownsData = other._ownsData;
				_data = other._data;
			}

			//	this has external data - not responsible for this' _data
			else if (_ownsData == false && other._ownsData == true)
			{
				_type = other._type;
				_size = other._size;
				_ownsData = other._ownsData;
				_data.vp = nullptr;

				//	Sets our capacity and allocates
				Reserve(other._capacity);

				CreateCopyFunction func = _copyFunctions[static_cast<int>(_type)];
				assert(func != nullptr);

				(this->*func)(other._data, other._size);
			}

			//	(this->_ownsData == false && other._ownsData == false)
			else
			{
				_type = other._type;
				_size = other._size;
				_capacity = other._capacity;
				_ownsData = other._ownsData;
				_data = other._data;
			}
		}

		return *this;
	}

	Datum::~Datum()
	{
		if (_ownsData == true)
		{
			Clear();
			free(_data.vp);
		}
	}

#pragma endregion

	inline bool Datum::operator==(const Datum& other) const
	{
		if (_type == other._type && _size == other._size)
		{
			ComparisonDefaultFunction func = _comparisonFunctions[static_cast<int>(_type)];
			for (size_t i = 0; i < _size; ++i)
			{
				//	If any individual member is not equal
				if ( !((this->*func)(other, i)) )
				{
					return false;
				}
			}

			//	Get here when all members are equal.
			return true;
		}

		return false;
	}

	void Datum::Reserve(size_t capacity)
	{
		if (_type == Datum::DatumType::Unknown)
		{
			throw runtime_error("Unable to call reserve on an unknown type");
		}

		if (_ownsData == false)
		{
			throw runtime_error("Unable to modify data that Datum doesn't own.");
		}

		if (capacity > _capacity)
		{
			size_t size;
			size = _sizeMap[static_cast<int>(_type)];
			void* data = realloc(_data.vp, capacity * size);
			assert(data != nullptr);
			_data.vp = data;
			_capacity = capacity;
		}
	}

	void Datum::Resize(size_t size)
	{
		if (_type == Datum::DatumType::Unknown)
		{
			throw runtime_error("Unable to call reserve on an unknown type");
		}

		if (_ownsData == false)
		{
			throw runtime_error("Unable to modify data that Datum doesn't own.");
		}

		if (size < _size)
		{
			if (_type == DatumType::String)
			{
				for (size_t i = size; i < _size; ++i)
				{
					_data.s[i].~string();
				}

				_size = size;
				ShrinkToFit();
			}

			//	Catch for non-string types
			_capacity = size;
		}
		else
		{
			Reserve(size);

			CreateDefaultFunction func = _createFunctions[static_cast<int>(_type)];
			assert(func != nullptr);

			for (size_t i = _size; i < size; ++i)
			{
				(this->*func)(i);
			}
		}

		_size = size;
	}

	bool Datum::SetStorage(DatumType type, void* arr, size_t count)
	{
		if (_type == DatumType::Unknown)
		{
			_type = type;
		}

		if (_type != type)
		{
			throw runtime_error("Incorrect typing on the Datum calling SetStorage()");
		}

		if (count == 0 || arr == nullptr)
		{
			throw runtime_error("Invalid parameters passed to SetStorage().");
		}

		_ownsData = false;
		_data.vp = arr;
		_size = count;
		_capacity = count;

		return true;
	}

	void Datum::Clear()
	{
		//	Do nothing if external storage
		if (_ownsData != false)
		{
			if (_type == DatumType::String)
			{
				for (size_t i = 0; i < _size; ++i)
				{
					_data.s[i].~string();
				}
			}
			_size = 0;
		}
	}

	bool Datum::PopBack()
	{
		if (_ownsData == false)
		{
			throw runtime_error("Unable to modify data that Datum doesn't own.");
		}

		//	Nothing to pop
		if (_size == 0)
		{
			return false;
		}
		
		if (_type == DatumType::String)
		{
			_data.s[_size - 1].~string();
		}

		--_size;
		return true;
	}

	bool Datum::RemoveAt(size_t index)
	{
		if (_ownsData == false)
		{
			throw runtime_error("Unable to modify data that Datum doesn't own.");
		}

		if (index >= _size)
		{
			return false;
		}

		if (index == _size - 1)
		{
			return PopBack();
		}

		//	References the function table for memmove calls
		MemMoveDefaultFunction func = _memMoveFunctions[static_cast<int>(_type)];
		(this->*func)(index);

		if (_type == DatumType::String)
		{
			_data.s[_size - 1].~string();
		}

		--_size;

		return true;
	}

	void Datum::ShrinkToFit()
	{
		if (_capacity > _size)
		{
			if (_size == 0)
			{
				free(_data.vp);
				_data.vp = nullptr;
				_capacity = 0;
			}
			else
			{
				void* data = realloc(_data.vp, _size * sizeof(string));
				assert(data != nullptr);
				_data.vp = data;
				_capacity = _size;
			}
		}
	}

	//	Dont need to check s for nullptr because std::basic_string already does. index is already being checked by Set.
	bool Datum::SetFromString(string s, size_t index)
	{
		if (_type == DatumType::Unknown || _type == DatumType::Pointer || _type == DatumType::Table)
		{
			throw std::runtime_error("Cannot set to a Datum with no Type, or Set from RTTI string. - Datum::SetFromString()");
		}

		//	Just to avoid an uneccesary function call
		if (_type == DatumType::String)
		{
			return Set(s, index);
		}

		CreateParseFunc parseFunc = _parseFunctions[static_cast<int>(_type)];

		//	Parses then calls the proper Set()
		return (this->*parseFunc)(s, index);
	}

	bool Datum::PushBackFromString(string s)
	{
		if (_type == DatumType::Unknown || _type == DatumType::Pointer || _type == DatumType::Table || _type == DatumType::Integer || _type == DatumType::Float)
		{
			throw std::runtime_error("Invalid type call for PushBackFromString. This is designed to take in mat4x4, vec4, and std::strings.");
		}

		//	Just to avoid an uneccesary function call
		if (_type == DatumType::String)
		{
			return PushBack(s);
		}

		CreateParsePushBackFunc parsePBFunc = _parsePushBackFunctions[static_cast<int>(_type)];

		//	Parses then calls the proper Set()
		return (this->*parsePBFunc)(s);
	}
}
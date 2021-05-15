#include "Datum.h"

namespace FieaGameEngine
{
#pragma region Data Member Getters
	inline size_t Datum::Capacity() const
	{
		return _capacity;
	}

	inline size_t Datum::Size() const
	{
		return _size;
	}

	inline typename Datum::DatumType Datum::Type() const
	{
		return _type;
	}

	inline bool Datum::OwnsData() const
	{
		return _ownsData;
	}
#pragma endregion

#pragma region PushBack

	inline size_t Datum::PushBack(const float& value)
	{
		if (_ownsData == false)
		{
			throw runtime_error("Unable to modify data that Datum doesn't own.");
		}

		if (_size == _capacity)
		{
			size_t expandedCapacity = _capacity + ((_capacity + 1) * 2);
			Reserve(expandedCapacity);
		}

		if (_type != DatumType::Float)
		{
			throw runtime_error("Data type for argument value in pushback does not match Datum._type.");
		}

		new(_data.f + _size)float(value);

		return _size++;
	}

	inline size_t Datum::PushBack(const int& value)
	{
		if (_ownsData == false)
		{
			throw runtime_error("Unable to modify data that Datum doesn't own.");
		}

		if (_size == _capacity)
		{
			size_t expandedCapacity = _capacity + ((_capacity + 1) * 2);
			Reserve(expandedCapacity);
		}

		if (_type != DatumType::Integer)
		{
			throw runtime_error("Data type for argument value in pushback does not match Datum._type.");
		}

		new(_data.i + _size)int(value);

		return _size++;
	}

	inline size_t Datum::PushBack(const mat4x4& value)
	{
		if (_ownsData == false)
		{
			throw runtime_error("Unable to modify data that Datum doesn't own.");
		}

		if (_size == _capacity)
		{
			size_t expandedCapacity = _capacity + ((_capacity + 1) * 2);
			Reserve(expandedCapacity);
		}

		if (_type != DatumType::Matrix)
		{
			throw runtime_error("Data type for argument value in pushback does not match Datum._type.");
		}

		new(_data.m + _size)mat4x4(value);

		return _size++;
	}

	inline size_t Datum::PushBack(RTTI* const& value)
	{
		if (_ownsData == false)
		{
			throw runtime_error("Unable to modify data that Datum doesn't own.");
		}

		if (_size == _capacity)
		{
			size_t expandedCapacity = _capacity + ((_capacity + 1) * 2);
			Reserve(expandedCapacity);
		}

		if (_type != DatumType::Pointer)
		{
			throw runtime_error("Data type for argument value in pushback does not match Datum._type.");
		}

		new(_data.p + _size)RTTI* (value);

		return _size++;
	}

	inline size_t Datum::PushBack(const string& value)
	{
		if (_ownsData == false)
		{
			throw runtime_error("Unable to modify data that Datum doesn't own.");
		}

		if (_size == _capacity)
		{
			size_t expandedCapacity = _capacity + ((_capacity + 1) * 2);
			Reserve(expandedCapacity);
		}

		if (_type != DatumType::String)
		{
			throw runtime_error("Data type for argument value in pushback does not match Datum._type.");
		}

		new(_data.s + _size)string(value);

		return _size++;
	}

	inline size_t Datum::PushBack(const vec4& value)
	{
		if (_ownsData == false)
		{
			throw runtime_error("Unable to modify data that Datum doesn't own.");
		}

		if (_size == _capacity)
		{
			size_t expandedCapacity = _capacity + ((_capacity + 1) * 2);
			Reserve(expandedCapacity);
		}

		if (_type != DatumType::Vector)
		{
			throw runtime_error("Data type for argument value in pushback does not match Datum._type.");
		}

		new(_data.v + _size)vec4(value);

		return _size++;
	}

	inline size_t Datum::PushBack(const Scope& value)
	{
		if (_ownsData == false)
		{
			throw runtime_error("Unable to modify data that Datum doesn't own.");
		}

		if (_type == DatumType::Unknown)
		{
			_type = DatumType::Table;
		}

		if (_size == _capacity)
		{
			size_t expandedCapacity = _capacity + 1;
			Reserve(expandedCapacity);
		}

		if (_type != DatumType::Table)
		{
			throw runtime_error("Data type for argument value in pushback does not match Datum._type.");
		}

		new(_data.t + _size)Scope* (&const_cast<Scope&>(value));

		return _size++;
	}

	inline size_t Datum::PushBack(string&& value)
	{
		if (_ownsData == false)
		{
			throw runtime_error("Unable to modify data that Datum doesn't own.");
		}

		if (_size == _capacity)
		{
			size_t expandedCapacity = _capacity + ((_capacity + 1) * 2);
			Reserve(expandedCapacity);
		}

		if (_type != DatumType::String)
		{
			throw runtime_error("Data type for argument value in pushback does not match Datum._type.");
		}

		new(_data.s + _size)string(std::move(value));

		return _size++;
	}

#pragma endregion

#pragma region Back
	template<typename T>
	inline T& Datum::Back()
	{
		static_assert(false, "Unsupported type passed into Back().");
	}

	template<>
	inline float& Datum::Back<float>()
	{
		if (_data.vp == nullptr)
		{
			throw std::runtime_error("Attempting to access _data that is referencing nullptr.");
		}

		if (_type != DatumType::Float)
		{
			throw std::runtime_error("Attempting to retrieve incorrect data type from datum.");
		}

		return _data.f[static_cast<int>(_size) - 1];
	}

	template<>
	inline int& Datum::Back<int>()
	{
		if (_data.vp == nullptr)
		{
			throw std::runtime_error("Attempting to access _data that is referencing nullptr.");
		}

		if (_type != DatumType::Integer)
		{
			throw std::runtime_error("Attempting to retrieve incorrect data type from datum.");
		}

		return _data.i[static_cast<int>(_size) - 1];
	}

	template<>
	inline mat4x4& Datum::Back<mat4x4>()
	{
		if (_data.vp == nullptr)
		{
			throw std::runtime_error("Attempting to access _data that is referencing nullptr.");
		}

		if (_type != DatumType::Matrix)
		{
			throw std::runtime_error("Attempting to retrieve incorrect data type from datum.");
		}

		return _data.m[static_cast<int>(_size) - 1];
	}

	template<>
	inline string& Datum::Back<string>()
	{
		if (_data.vp == nullptr)
		{
			throw std::runtime_error("Attempting to access _data that is referencing nullptr.");
		}

		if (_type != DatumType::String)
		{
			throw std::runtime_error("Attempting to retrieve incorrect data type from datum.");
		}

		return _data.s[static_cast<int>(_size) - 1];
	}

	template<>
	inline RTTI*& Datum::Back<RTTI*>()
	{
		if (_data.vp == nullptr)
		{
			throw std::runtime_error("Attempting to access _data that is referencing nullptr.");
		}

		if (_type != DatumType::Pointer)
		{
			throw std::runtime_error("Attempting to retrieve incorrect data type from datum.");
		}

		return _data.p[static_cast<int>(_size) - 1];
	}

	template<>
	inline vec4& Datum::Back<vec4>()
	{
		if (_data.vp == nullptr)
		{
			throw std::runtime_error("Attempting to access _data that is referencing nullptr.");
		}

		if (_type != DatumType::Vector)
		{
			throw std::runtime_error("Attempting to retrieve incorrect data type from datum.");
		}

		return _data.v[static_cast<int>(_size) - 1];
	}

	template<typename T>
	inline const T& Datum::Back() const
	{
		static_assert(false, "Unsupported type passed into Back().");
	}

	template<>
	inline const float& Datum::Back<float>() const
	{
		if (_data.vp == nullptr)
		{
			throw std::runtime_error("Attempting to access _data that is referencing nullptr.");
		}

		if (_type != DatumType::Float)
		{
			throw std::runtime_error("Attempting to retrieve incorrect data type from datum.");
		}

		return _data.f[static_cast<int>(_size) - 1];
	}

	template<>
	inline const int& Datum::Back<int>() const
	{
		if (_data.vp == nullptr)
		{
			throw std::runtime_error("Attempting to access _data that is referencing nullptr.");
		}

		if (_type != DatumType::Integer)
		{
			throw std::runtime_error("Attempting to retrieve incorrect data type from datum.");
		}

		return _data.i[static_cast<int>(_size) - 1];
	}

	template<>
	inline const mat4x4& Datum::Back<mat4x4>() const
	{
		if (_data.vp == nullptr)
		{
			throw std::runtime_error("Attempting to access _data that is referencing nullptr.");
		}

		if (_type != DatumType::Matrix)
		{
			throw std::runtime_error("Attempting to retrieve incorrect data type from datum.");
		}

		return _data.m[static_cast<int>(_size) - 1];
	}

	template<>
	inline const string& Datum::Back<string>() const
	{
		if (_data.vp == nullptr)
		{
			throw std::runtime_error("Attempting to access _data that is referencing nullptr.");
		}

		if (_type != DatumType::String)
		{
			throw std::runtime_error("Attempting to retrieve incorrect data type from datum.");
		}

		return _data.s[static_cast<int>(_size) - 1];
	}

	template<>
	inline RTTI* const & Datum::Back<RTTI*>() const
	{
		if (_data.vp == nullptr)
		{
			throw std::runtime_error("Attempting to access _data that is referencing nullptr.");
		}

		if (_type != DatumType::Pointer)
		{
			throw std::runtime_error("Attempting to retrieve incorrect data type from datum.");
		}

		return _data.p[static_cast<int>(_size) - 1];
	}

	template<>
	inline const vec4& Datum::Back<vec4>() const
	{
		if (_data.vp == nullptr)
		{
			throw std::runtime_error("Attempting to access _data that is referencing nullptr.");
		}

		if (_type != DatumType::Vector)
		{
			throw std::runtime_error("Attempting to retrieve incorrect data type from datum.");
		}

		return _data.v[static_cast<int>(_size) - 1];
	}

#pragma endregion

#pragma region Front
	template<typename T>
	inline T& Datum::Front()
	{
		static_assert(false, "Unsupported type passed into Front().");
	}

	template<>
	inline float& Datum::Front<float>()
	{
		if (_data.vp == nullptr)
		{
			throw std::runtime_error("Attempting to access _data that is referencing nullptr.");
		}

		if (_type != DatumType::Float)
		{
			throw std::runtime_error("Attempting to retrieve incorrect data type from datum.");
		}

		return _data.f[0];
	}

	template<>
	inline int& Datum::Front<int>()
	{
		if (_data.vp == nullptr)
		{
			throw std::runtime_error("Attempting to access _data that is referencing nullptr.");
		}

		if (_type != DatumType::Integer)
		{
			throw std::runtime_error("Attempting to retrieve incorrect data type from datum.");
		}

		return _data.i[0];
	}

	template<>
	inline mat4x4& Datum::Front<mat4x4>()
	{
		if (_data.vp == nullptr)
		{
			throw std::runtime_error("Attempting to access _data that is referencing nullptr.");
		}

		if (_type != DatumType::Matrix)
		{
			throw std::runtime_error("Attempting to retrieve incorrect data type from datum.");
		}

		return _data.m[0];
	}

	template<>
	inline string& Datum::Front<string>()
	{
		if (_data.vp == nullptr)
		{
			throw std::runtime_error("Attempting to access _data that is referencing nullptr.");
		}

		if (_type != DatumType::String)
		{
			throw std::runtime_error("Attempting to retrieve incorrect data type from datum.");
		}

		return _data.s[0];
	}

	template<>
	inline RTTI*& Datum::Front<RTTI*>()
	{
		if (_data.vp == nullptr)
		{
			throw std::runtime_error("Attempting to access _data that is referencing nullptr.");
		}

		if (_type != DatumType::Pointer)
		{
			throw std::runtime_error("Attempting to retrieve incorrect data type from datum.");
		}

		return _data.p[0];
	}

	template<>
	inline vec4& Datum::Front<vec4>()
	{
		if (_data.vp == nullptr)
		{
			throw std::runtime_error("Attempting to access _data that is referencing nullptr.");
		}

		if (_type != DatumType::Vector)
		{
			throw std::runtime_error("Attempting to retrieve incorrect data type from datum.");
		}

		return _data.v[0];
	}

	template<typename T>
	inline const T& Datum::Front() const
	{
		static_assert(false, "Unsupported type passed into Front().");
	}

	template<>
	inline const float& Datum::Front<float>() const
	{
		if (_data.vp == nullptr)
		{
			throw std::runtime_error("Attempting to access _data that is referencing nullptr.");
		}

		if (_type != DatumType::Float)
		{
			throw std::runtime_error("Attempting to retrieve incorrect data type from datum.");
		}

		return _data.f[0];
	}

	template<>
	inline const int& Datum::Front<int>() const
	{
		if (_data.vp == nullptr)
		{
			throw std::runtime_error("Attempting to access _data that is referencing nullptr.");
		}

		if (_type != DatumType::Integer)
		{
			throw std::runtime_error("Attempting to retrieve incorrect data type from datum.");
		}

		return _data.i[0];
	}

	template<>
	inline const mat4x4& Datum::Front<mat4x4>() const
	{
		if (_data.vp == nullptr)
		{
			throw std::runtime_error("Attempting to access _data that is referencing nullptr.");
		}

		if (_type != DatumType::Matrix)
		{
			throw std::runtime_error("Attempting to retrieve incorrect data type from datum.");
		}

		return _data.m[0];
	}

	template<>
	inline const string& Datum::Front<string>() const
	{
		if (_data.vp == nullptr)
		{
			throw std::runtime_error("Attempting to access _data that is referencing nullptr.");
		}

		if (_type != DatumType::String)
		{
			throw std::runtime_error("Attempting to retrieve incorrect data type from datum.");
		}

		return _data.s[0];
	}

	template<>
	inline RTTI* const & Datum::Front<RTTI*>() const
	{
		if (_data.vp == nullptr)
		{
			throw std::runtime_error("Attempting to access _data that is referencing nullptr.");
		}

		if (_type != DatumType::Pointer)
		{
			throw std::runtime_error("Attempting to retrieve incorrect data type from datum.");
		}

		return _data.p[0];
	}

	template<>
	inline const vec4& Datum::Front<vec4>() const
	{
		if (_data.vp == nullptr)
		{
			throw std::runtime_error("Attempting to access _data that is referencing nullptr.");
		}

		if (_type != DatumType::Vector)
		{
			throw std::runtime_error("Attempting to retrieve incorrect data type from datum.");
		}

		return _data.v[0];
	}

#pragma endregion

#pragma region IndexOf

	inline size_t Datum::IndexOf(const float& value) const
	{
		if (_type != DatumType::Float)
		{
			throw runtime_error("Attempting to find an incompatible data type for this Datum.");
		}

		for (size_t i = 0; i < _size; ++i)
		{
			if (_data.f[i] == value)
			{
				return i;
			}
		}

		return _size;
	}

	inline size_t Datum::IndexOf(const int& value) const
	{
		if (_type != DatumType::Integer)
		{
			throw runtime_error("Attempting to find an incompatible data type for this Datum.");
		}

		for (size_t i = 0; i < _size; ++i)
		{
			if (_data.i[i] == value)
			{
				return i;
			}
		}

		return _size;
	}

	inline size_t Datum::IndexOf(const mat4x4& value) const
	{
		if (_type != DatumType::Matrix)
		{
			throw runtime_error("Attempting to find an incompatible data type for this Datum.");
		}

		for (size_t i = 0; i < _size; ++i)
		{
			if (_data.m[i] == value)
			{
				return i;
			}
		}

		return _size;
	}

	inline size_t Datum::IndexOf(const string& value) const
	{
		if (_type != DatumType::String)
		{
			throw runtime_error("Attempting to find an incompatible data type for this Datum.");
		}

		for (size_t i = 0; i < _size; ++i)
		{
			if (_data.s[i] == value)
			{
				return i;
			}
		}

		return _size;
	}

	inline size_t Datum::IndexOf(RTTI* const& value) const
	{
		if (_type != DatumType::Pointer)
		{
			throw runtime_error("Attempting to find an incompatible data type for this Datum.");
		}

		for (size_t i = 0; i < _size; ++i)
		{
			if (_data.p[i]->Equals(value))
			{
				return i;
			}
		}

		return _size;
	}

	inline size_t Datum::IndexOf(const vec4& value) const
	{
		if (_type != DatumType::Vector)
		{
			throw runtime_error("Attempting to find an incompatible data type for this Datum.");
		}

		for (size_t i = 0; i < _size; ++i)
		{
			if (_data.v[i] == value)
			{
				return i;
			}
		}

		return _size;
	}

	inline size_t Datum::IndexOf(Scope* const& value) const
	{
		if (_type != DatumType::Table)
		{
			throw runtime_error("Attempting to find an incompatible data type for this Datum.");
		}

		for (size_t i = 0; i < _size; ++i)
		{
			if (_data.t[i] == (value))
			{
				return i;
			}
		}

		return _size;
	}
#pragma endregion

#pragma region Get

	template<typename T>
	inline T& Datum::Get(size_t index)
	{
		static_assert(false, "Unsupported type passed into Get().");
	}

	template<typename T>
	inline const T& Datum::Get(size_t index) const
	{
		static_assert(false, "Unsupported type passed into Get().");
	}

	template<>
	inline float& Datum::Get<float>(size_t index)
	{
		if (index >= _size)
		{
			throw runtime_error("Attempting to access index beyond the _size.");
		}

		if (_type != DatumType::Float)
		{
			throw runtime_error("Attempting to Get an incompatible data type for this Datum.");
		}

		return _data.f[index];
	}

	template<>
	inline int& Datum::Get<int>(size_t index)
	{
		if (index >= _size)
		{
			throw runtime_error("Attempting to access index beyond the _size.");
		}

		if (_type != DatumType::Integer)
		{
			throw runtime_error("Attempting to Get an incompatible data type for this Datum.");
		}

		return _data.i[index];
	}

	template<>
	inline mat4x4& Datum::Get<mat4x4>(size_t index)
	{
		if (index >= _size)
		{
			throw runtime_error("Attempting to access index beyond the _size.");
		}

		if (_type != DatumType::Matrix)
		{
			throw runtime_error("Attempting to Get an incompatible data type for this Datum.");
		}

		return _data.m[index];
	}

	template<>
	inline Scope*& Datum::Get<Scope*>(size_t index)
	{
		if (index >= _size)
		{
			throw runtime_error("Attempting to access index beyond the _size.");
		}

		if (_type != DatumType::Table)
		{
			throw runtime_error("Attempting to Get an incompatible data type for this Datum.");
		}

		return _data.t[index];
	}

	template<>
	inline RTTI*& Datum::Get<RTTI*>(size_t index)
	{
		if (index >= _size)
		{
			throw runtime_error("Attempting to access index beyond the _size.");
		}

		if (_type != DatumType::Pointer)
		{
			throw runtime_error("Attempting to Get an incompatible data type for this Datum.");
		}

		return _data.p[index];
	}

	template<>
	inline string& Datum::Get<string>(size_t index)
	{
		if (index >= _size)
		{
			throw runtime_error("Attempting to access index beyond the _size.");
		}

		if (_type != DatumType::String)
		{
			throw runtime_error("Attempting to Get an incompatible data type for this Datum.");
		}

		return _data.s[index];
	}

	template<>
	inline vec4& Datum::Get<vec4>(size_t index)
	{
		if (index >= _size)
		{
			throw runtime_error("Attempting to access index beyond the _size.");
		}

		if (_type != DatumType::Vector)
		{
			throw runtime_error("Attempting to Get an incompatible data type for this Datum.");
		}

		return _data.v[index];
	}

	template<>
	inline const float& Datum::Get<float>(size_t index) const
	{
		if (index >= _size)
		{
			throw runtime_error("Attempting to access index beyond the _size.");
		}

		if (_type != DatumType::Float)
		{
			throw runtime_error("Attempting to Get an incompatible data type for this Datum.");
		}

		return _data.f[index];
	}

	template<>
	inline const int& Datum::Get<int>(size_t index) const
	{
		if (index >= _size)
		{
			throw runtime_error("Attempting to access index beyond the _size.");
		}

		if (_type != DatumType::Integer)
		{
			throw runtime_error("Attempting to Get an incompatible data type for this Datum.");
		}

		return _data.i[index];
	}

	template<>
	inline const mat4x4& Datum::Get<mat4x4>(size_t index) const
	{
		if (index >= _size)
		{
			throw runtime_error("Attempting to access index beyond the _size.");
		}

		if (_type != DatumType::Matrix)
		{
			throw runtime_error("Attempting to Get an incompatible data type for this Datum.");
		}

		return _data.m[index];
	}

	template<>
	inline RTTI* const & Datum::Get<RTTI*>(size_t index) const
	{
		if (index >= _size)
		{
			throw runtime_error("Attempting to access index beyond the _size.");
		}

		if (_type != DatumType::Pointer)
		{
			throw runtime_error("Attempting to Get an incompatible data type for this Datum.");
		}

		return _data.p[index];
	}

	template<>
	inline Scope* const & Datum::Get<Scope*>(size_t index) const
	{
		if (index >= _size)
		{
			throw runtime_error("Attempting to access index beyond the _size.");
		}

		if (_type != DatumType::Table)
		{
			throw runtime_error("Attempting to Get an incompatible data type for this Datum.");
		}

		return _data.t[index];
	}

	template<>
	inline const string& Datum::Get<string>(size_t index) const
	{
		if (index >= _size)
		{
			throw runtime_error("Attempting to access index beyond the _size.");
		}

		if (_type != DatumType::String)
		{
			throw runtime_error("Attempting to Get an incompatible data type for this Datum.");
		}

		return _data.s[index];
	}

	template<>
	inline const vec4& Datum::Get<vec4>(size_t index) const
	{
		if (index >= _size)
		{
			throw runtime_error("Attempting to access index beyond the _size.");
		}

		if (_type != DatumType::Vector)
		{
			throw runtime_error("Attempting to Get an incompatible data type for this Datum.");
		}

		return _data.v[index];
	}

#pragma endregion

#pragma region Set

	inline bool Datum::Set(const float& value, size_t index)
	{
		if (index >= _size)
		{
			throw runtime_error("Attempting to set at an index beyond capacity.");
		}

		if (_type != DatumType::Float)
		{
			throw runtime_error("Attempting to set an incompatible data type for this Datum.");
		}

		_data.f[index] = value;
		return true;
	}

	inline bool Datum::Set(const int& value, size_t index)
	{
		if (index >= _size)
		{
			throw runtime_error("Attempting to set at an index beyond capacity.");
		}

		if (_type != DatumType::Integer)
		{
			throw runtime_error("Attempting to set an incompatible data type for this Datum.");
		}

		_data.i[index] = value;
		return true;
	}

	inline bool Datum::Set(const mat4x4& value, size_t index)
	{
		if (index >= _size)
		{
			throw runtime_error("Attempting to set at an index beyond capacity.");
		}

		if (_type != DatumType::Matrix)
		{
			throw runtime_error("Attempting to set an incompatible data type for this Datum.");
		}

		_data.m[index] = value;
		return true;
	}

	inline bool Datum::Set(RTTI* const & value, size_t index)
	{
		if (index >= _size)
		{
			throw runtime_error("Attempting to set at an index beyond capacity.");
		}

		if (_type != DatumType::Pointer)
		{
			throw runtime_error("Attempting to set an incompatible data type for this Datum.");
		}

		_data.p[index] = value;
		return true;
	}

	inline bool Datum::Set(const string& value, size_t index)
	{
		if (index >= _size)
		{
			throw runtime_error("Attempting to set at an index beyond capacity.");
		}

		if (_type != DatumType::String)
		{
			throw runtime_error("Attempting to set an incompatible data type for this Datum.");
		}

		_data.s[index] = value;
		return true;
	}

	inline bool Datum::Set(const vec4& value, size_t index)
	{
		if (index >= _size)
		{
			throw runtime_error("Attempting to set at an index beyond capacity.");
		}

		if (_type != DatumType::Vector)
		{
			throw runtime_error("Attempting to set an incompatible data type for this Datum.");
		}

		_data.v[index] = value;
		return true;
	}

	inline bool Datum::Set(const Scope& value, size_t index)
	{
		if (index >= _size)
		{
			throw runtime_error("Attempting to set at an index beyond capacity.");
		}

		if (_type != DatumType::Table)
		{
			throw runtime_error("Attempting to set an incompatible data type for this Datum.");
		}

		Scope* s = const_cast<Scope*>(&value);

		_data.t[index] = s;
		return true;
	}

	inline bool Datum::Set(string&& value, size_t index)
	{
		if (index >= _size)
		{
			throw runtime_error("Attempting to set at an index beyond capacity.");
		}

		if (_type != DatumType::String)
		{
			throw runtime_error("Attempting to set an incompatible data type for this Datum.");
		}

		_data.s[index] = std::move(value);
		return true;
	}

#pragma endregion

#pragma region Remove

	inline bool Datum::Remove(const float& value)
	{
		if (_type != DatumType::Float)
		{
			throw runtime_error("Attempting to Remove an incompatible data type for this Datum.");
		}

		return RemoveAt(IndexOf(value));
	}

	inline bool Datum::Remove(const int& value)
	{
		if (_type != DatumType::Integer)
		{
			throw runtime_error("Attempting to Remove an incompatible data type for this Datum.");
		}

		return RemoveAt(IndexOf(value));
	}

	inline bool Datum::Remove(const mat4x4& value)
	{
		if (_type != DatumType::Matrix)
		{
			throw runtime_error("Attempting to Remove an incompatible data type for this Datum.");
		}

		return RemoveAt(IndexOf(value));
	}

	inline bool Datum::Remove(RTTI* const & value)
	{
		if (_type != DatumType::Pointer)
		{
			throw runtime_error("Attempting to Remove an incompatible data type for this Datum.");
		}

		return RemoveAt(IndexOf(value));
	}

	inline bool Datum::Remove(const string& value)
	{
		if (_type != DatumType::String)
		{
			throw runtime_error("Attempting to Remove an incompatible data type for this Datum.");
		}

		return RemoveAt(IndexOf(value));
	}

	inline bool Datum::Remove(const vec4& value)
	{
		if (_type != DatumType::Vector)
		{
			throw runtime_error("Attempting to Remove an incompatible data type for this Datum.");
		}

		return RemoveAt(IndexOf(value));
	}

	inline bool Datum::Remove(const Scope& value)
	{
		if (_type != DatumType::Table)
		{
			throw runtime_error("Attempting to Remove an incompatible data type for this Datum.");
		}

		Scope* s = const_cast<Scope*>(&value);

		return RemoveAt(IndexOf(s));
	}
#pragma endregion

#pragma region operator== (scalar)

	inline bool Datum::operator==(const float& other) const
	{
		if (_type != DatumType::Float)
		{
			return false;
		}

		return (_data.f[0] == other);
	}

	inline bool Datum::operator==(const int& other) const
	{
		if (_type != DatumType::Integer)
		{
			return false;
		}

		return (_data.i[0] == other);
	}

	inline bool Datum::operator==(const mat4x4& other) const
	{
		if (_type != DatumType::Matrix)
		{
			return false;
		}

		return (_data.m[0] == other);
	}

	inline bool Datum::operator==(RTTI* const & other) const
	{
		if (_type != DatumType::Pointer)
		{
			return false;
		}

		return (_data.p[0]->Equals(other));
	}

	inline bool Datum::operator==(const string& other) const
	{
		if (_type != DatumType::String)
		{
			return false;
		}

		return (_data.s[0] == other);
	}

	inline bool Datum::operator==(const vec4& other) const
	{
		if (_type != DatumType::Vector)
		{
			return false;
		}

		return (_data.v[0] == other);
	}

#pragma endregion

#pragma region operator!= (scalar)

	inline bool Datum::operator!=(const float& other) const
	{
		return !(operator==(other));
	}

	inline bool Datum::operator!=(const int& other) const
	{
		return !(operator==(other));
	}

	inline bool Datum::operator!=(const mat4x4& other) const
	{
		return !(operator==(other));
	}

	inline bool Datum::operator!=(RTTI* const& other) const
	{
		return !(operator==(other));
	}

	inline bool Datum::operator!=(const string& other) const
	{
		return !(operator==(other));
	}

	inline bool Datum::operator!=(const vec4& other) const
	{
		return !(operator==(other));
	}

#pragma endregion

#pragma region operator=(scalar)

	inline Datum& Datum::operator=(const float& other)
	{
		if (_type != DatumType::Unknown && _ownsData == true)
		{
			Clear();
			ShrinkToFit();
		}

		_type = DatumType::Float;
		PushBack(other);

		return *this;
	}

	inline Datum& Datum::operator=(const int& other)
	{
		if (_type != DatumType::Unknown && _ownsData == true)
		{
			Clear();
			ShrinkToFit();
		}

		_type = DatumType::Integer;
		PushBack(other);

		return *this;
	}

	inline Datum& Datum::operator=(const mat4x4& other)
	{
		if (_type != DatumType::Unknown && _ownsData == true)
		{
			Clear();
			ShrinkToFit();
		}

		_type = DatumType::Matrix;
		PushBack(other);

		return *this;
	}

	inline Datum& Datum::operator=(RTTI* const & other)
	{
		if (_type != DatumType::Unknown && _ownsData == true)
		{
			Clear();
			ShrinkToFit();
		}

		_type = DatumType::Pointer;
		PushBack(other);

		return *this;
	}

	inline Datum& Datum::operator=(const string& other)
	{
		if (_type != DatumType::Unknown && _ownsData == true)
		{
			Clear();
			ShrinkToFit();
		}

		_type = DatumType::String;
		PushBack(other);

		return *this;
	}

	inline Datum& Datum::operator=(const vec4& other)
	{
		if (_type != DatumType::Unknown && _ownsData == true)
		{
			Clear();
			ShrinkToFit();
		}

		_type = DatumType::Vector;
		PushBack(other);

		return *this;
	}

	inline Datum& Datum::operator=(const Scope& other)
	{
		if (_type != DatumType::Unknown && _ownsData == true)
		{
			Clear();
		}

		_type = DatumType::Table;
		PushBack(const_cast<Scope&>(other));

		return *this;
	}

#pragma endregion

#pragma region operator[]
	inline Scope& Datum::operator[](size_t index)
	{
		return *Get<Scope*>(index);
	}
#pragma endregion

#pragma region ToString

	template<typename T>
	inline string Datum::ToString(size_t index)
	{
		static_assert(false, "Unsupported type passed into ToString().");
	}

	template<>
	inline string Datum::ToString<int>(size_t index)
	{
		if (index >= _size)
		{
			throw runtime_error("Attempting to access index outside of _size in ToString()");
		}

		if (_type != DatumType::Integer)
		{
			throw runtime_error("Incorrect template specialization for Datum::ToString()");
		}

		return std::to_string(_data.i[index]);
	}

	template<>
	inline string Datum::ToString<float>(size_t index)
	{
		if (index >= _size)
		{
			throw runtime_error("Attempting to access index outside of _size in ToString()");
		}

		if (_type != DatumType::Float)
		{
			throw runtime_error("Incorrect template specialization for Datum::ToString()");
		}

		return std::to_string(_data.f[index]);
	}

	template<>
	inline string Datum::ToString<mat4x4>(size_t index)
	{
		if (index >= _size)
		{
			throw runtime_error("Attempting to access index outside of _size in ToString()");
		}

		if (_type != DatumType::Matrix)
		{
			throw runtime_error("Incorrect template specialization for Datum::ToString()");
		}

		return glm::to_string(_data.m[index]);
	}

	template<>
	inline string Datum::ToString<RTTI*>(size_t index)
	{
		if (index >= _size)
		{
			throw runtime_error("Attempting to access index outside of _size in ToString()");
		}

		if (_type != DatumType::Pointer)
		{
			throw runtime_error("Incorrect template specialization for Datum::ToString()");
		}

		return _data.p[index]->ToString();
	}

	template<>
	inline string Datum::ToString<string>(size_t index)
	{
		if (index >= _size)
		{
			throw runtime_error("Attempting to access index outside of _size in ToString()");
		}

		if (_type != DatumType::String)
		{
			throw runtime_error("Incorrect template specialization for Datum::ToString()");
		}

		return _data.s[index];
	}

	template<>
	inline string Datum::ToString<vec4>(size_t index)
	{
		if (index >= _size)
		{
			throw runtime_error("Attempting to access index outside of _size in ToString()");
		}

		if (_type != DatumType::Vector)
		{
			throw runtime_error("Incorrect template specialization for Datum::ToString()");
		}

		return glm::to_string(_data.v[index]);
	}
#pragma endregion

#pragma region SetStorage

	inline bool Datum::SetStorage(float* arr, size_t count)
	{
		return SetStorage(DatumType::Float, arr, count);
	}

	inline bool Datum::SetStorage(int* arr, size_t count)
	{
		return SetStorage(DatumType::Integer, arr, count);
	}

	inline bool Datum::SetStorage(mat4x4* arr, size_t count)
	{
		return SetStorage(DatumType::Matrix, arr, count);
	}

	inline bool Datum::SetStorage(RTTI** arr, size_t count)
	{
		return SetStorage(DatumType::Pointer, arr, count);
	}

	inline bool Datum::SetStorage(string* arr, size_t count)
	{
		return SetStorage(DatumType::String, arr, count);
	}

	inline bool Datum::SetStorage(vec4* arr, size_t count)
	{
		return SetStorage(DatumType::Vector, arr, count);
	}

#pragma endregion

#pragma region Resize Function Table
	inline void Datum::CreateFloat(size_t index)
	{
		new (_data.f + index)float(0.0f);
	}

	inline void Datum::CreateInteger(size_t index)
	{
		new (_data.i + index)int(0);
	}

	inline void Datum::CreateMatrix(size_t index)
	{
		new (_data.m + index)mat4x4(0);
	}

	inline void Datum::CreatePointer(size_t index)
	{
		new (_data.p + index)RTTI* (nullptr);
	}

	inline void Datum::CreateString(size_t index)
	{
		new (_data.s + index)string();
	}

	inline void Datum::CreateVector(size_t index)
	{
		new (_data.v + index)vec4(0);
	}

	inline void Datum::CreateTable(size_t index)
	{
		new (_data.t + index)Scope* (nullptr);
	}
#pragma endregion

#pragma region Copy Function Table
	inline void Datum::CreateFloatCopy(const DatumValues& data, size_t size)
	{
		for (size_t i = 0; i < size; ++i)
		{
			new (_data.f + i)float(data.f[i]);
		}
	}

	inline void Datum::CreateIntegerCopy(const DatumValues& data, size_t size)
	{
		for (size_t i = 0; i < size; ++i)
		{
			new (_data.i + i)int(data.i[i]);
		}
	}

	inline void Datum::CreateMatrixCopy(const DatumValues& data, size_t size)
	{
		for (size_t i = 0; i < size; ++i)
		{
			new (_data.m + i)mat4x4(data.m[i]);
		}
	}

	inline void Datum::CreatePointerCopy(const DatumValues& data, size_t size)
	{
		for (size_t i = 0; i < size; ++i)
		{
			new (_data.p + i)RTTI* (data.p[i]);
		}
	}

	inline void Datum::CreateStringCopy(const DatumValues& data, size_t size)
	{
		for (size_t i = 0; i < size; ++i)
		{
			new (_data.s + i)string(data.s[i]);
		}
	}

	inline void Datum::CreateVectorCopy(const DatumValues& data, size_t size)
	{
		for (size_t i = 0; i < size; ++i)
		{
			new (_data.v + i)vec4(data.v[i]);
		}
	}

	inline void Datum::CreateTableCopy(const DatumValues& data, size_t size)
	{
		for (size_t i = 0; i < size; ++i)
		{
			new (_data.t + i)Scope* (data.t[i]);
		}
	}
#pragma endregion

#pragma region Memmove Function Table
	inline void Datum::MoveFloatData(size_t index)
	{
		memmove(&(_data.f[index]), &(_data.f[index + 1]), (sizeof(float) * ((_size)-((index)+1))));
	}

	inline void Datum::MoveIntData(size_t index)
	{
		memmove(&(_data.i[index]), &(_data.i[index + 1]), (sizeof(int) * ((_size)-((index)+1))));
	}

	inline void Datum::MoveMatrixData(size_t index)
	{
		memmove(&(_data.m[index]), &(_data.m[index + 1]), (sizeof(mat4x4) * ((_size)-((index)+1))));
	}

	inline void Datum::MovePointerData(size_t index)
	{
		memmove(&(_data.p[index]), &(_data.p[index + 1]), (sizeof(RTTI*) * ((_size)-((index)+1))));
	}

	inline void Datum::MoveStringData(size_t index)
	{
		string* temp = &(_data.s[index]);
		std::copy(&(_data.s[index + 1]), &(_data.s[_size]), temp);
	}

	inline void Datum::MoveVectorData(size_t index)
	{
		memmove(&(_data.v[index]), &(_data.v[index + 1]), (sizeof(vec4) * ((_size)-((index)+1))));
	}

	inline void Datum::MoveTableData(size_t index)
	{
		memmove(&(_data.t[index]), &(_data.t[index + 1]), (sizeof(Scope*) * ((_size)-((index)+1))));
	}

#pragma endregion

#pragma region Comparison Function Table
	inline bool Datum::CompareFloat(const Datum& other, size_t index) const
	{
		return (other._data.f[index] == _data.f[index]);
	}

	inline bool Datum::CompareInt(const Datum& other, size_t index) const
	{
		return (other._data.i[index] == _data.i[index]);
	}

	inline bool Datum::CompareMatrix(const Datum& other, size_t index) const
	{
		return (other._data.m[index] == _data.m[index]);
	}

	inline bool Datum::ComparePointer(const Datum& other, size_t index) const
	{
		return (other._data.p[index]->Equals(_data.p[index]));
	}

	inline bool Datum::CompareString(const Datum& other, size_t index) const
	{
		return (other._data.s[index] == _data.s[index]);
	}

	inline bool Datum::CompareVector(const Datum& other, size_t index) const
	{
		return (other._data.v[index] == _data.v[index]);
	}

	inline bool Datum::CompareTable(const Datum& other, size_t index) const
	{
		return other._data.p[index]->Equals(_data.p[index]);
	}

#pragma endregion

#pragma region String Parse Function Table

	inline bool Datum::ParseFloat(string s, size_t index)
	{
		float val;
		sscanf_s(s.c_str(), "%f", &val);

		return Set(val, index);
	}

	inline bool Datum::ParseInt(string s, size_t index)
	{
		int val;
		sscanf_s(s.c_str(), "%d", &val);

		return Set(val, index);
	}

	inline bool Datum::ParseMatrix(string s, size_t index)
	{
		mat4x4 matrix;

		sscanf_s(s.c_str(), "mat4x4((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))", 
			&matrix[0][0], &matrix[1][0], &matrix[2][0], &matrix[3][0],
			&matrix[0][1], &matrix[1][1], &matrix[2][1], &matrix[3][1],
			&matrix[0][2], &matrix[1][2], &matrix[2][2], &matrix[3][2],
			&matrix[0][3], &matrix[1][3], &matrix[2][3], &matrix[3][3]);

		return Set(matrix, index);
	}

	inline bool Datum::ParseVector(string s, size_t index)
	{
		vec4 vector;

		sscanf_s(s.c_str(), "vec4(%f, %f, %f, %f)", &vector.x, &vector.y, &vector.z, &vector.w);

		return Set(vector, index);
	}

	inline void Datum::SetFromJsonValue(Json::Value object, size_t index)
	{
		SetJsonValueFunction func = _setJsonFunctions[static_cast<int>(_type)];
		assert(func != nullptr);
		(this->*func)(object, index);
	}

	inline void Datum::SetJsonFloat(Json::Value object, size_t index)
	{
		Set(object.asFloat(), index);
	}

	inline void Datum::SetJsonInteger(Json::Value object, size_t index)
	{
		Set(object.asInt(), index);
	}

	inline void Datum::SetJsonMatrix(Json::Value object, size_t index)
	{
		SetFromString(object.asString(), index);
	}

	inline void Datum::SetJsonString(Json::Value object, size_t index)
	{
		Set(object.asString(), index);
	}

	inline void Datum::SetJsonVector(Json::Value object, size_t index)
	{
		SetFromString(object.asString(), index);
	}

	inline void Datum::PushBackFromJsonValue(Json::Value object)
	{
		PushBackJsonValueFunction func = _pushbackFunctions[static_cast<int>(_type)];
		assert(func != nullptr);
		(this->*func)(object);
	}

	inline void Datum::ParsePushBackFloat(Json::Value object)
	{
		PushBack(object.asFloat());
	}

	inline void Datum::ParsePushBackInteger(Json::Value object)
	{
		PushBack(object.asInt());
	}

	inline void Datum::ParsePushBackMatrix(Json::Value object)
	{
		mat4x4 matrix;

		sscanf_s(object.asCString(), "mat4x4((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))",
			&matrix[0][0], &matrix[1][0], &matrix[2][0], &matrix[3][0],
			&matrix[0][1], &matrix[1][1], &matrix[2][1], &matrix[3][1],
			&matrix[0][2], &matrix[1][2], &matrix[2][2], &matrix[3][2],
			&matrix[0][3], &matrix[1][3], &matrix[2][3], &matrix[3][3]);

		PushBack(matrix);
	}

	inline void Datum::ParsePushBackString(Json::Value object)
	{
		PushBack(object.asString());
	}

	inline void Datum::ParsePushBackVector(Json::Value object)
	{
		vec4 vector;

		sscanf_s(object.asCString(), "vec4(%f, %f, %f, %f)", &vector.x, &vector.y, &vector.z, &vector.w);

		PushBack(vector);
	}

	inline bool Datum::ParsePushBackMatrixString(string s)
	{
		mat4x4 matrix;

		sscanf_s(s.c_str(), "mat4x4((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))",
			&matrix[0][0], &matrix[1][0], &matrix[2][0], &matrix[3][0],
			&matrix[0][1], &matrix[1][1], &matrix[2][1], &matrix[3][1],
			&matrix[0][2], &matrix[1][2], &matrix[2][2], &matrix[3][2],
			&matrix[0][3], &matrix[1][3], &matrix[2][3], &matrix[3][3]);

		return PushBack(matrix);
	}

	inline bool Datum::ParsePushBackVectorString(string s)
	{
		vec4 vector;

		sscanf_s(s.c_str(), "vec4(%f, %f, %f, %f)", &vector.x, &vector.y, &vector.z, &vector.w);

		return PushBack(vector);
	}

#pragma endregion

	inline bool Datum::operator!=(const Datum& other) const
	{
		return !(operator==(other));
	}

	inline void Datum::SetType(DatumType type)
	{
		if (_type != DatumType::Unknown && type != _type)
		{
			throw std::runtime_error("Cant change types once assigned.");
		}

		_type = type;
	}
}
#include "pch.h"
#include "Foo.h"
#include <exception>

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Foo)
	RTTI_DEFINITIONS(Bar)

#pragma region Rule_Of_Six
	//	Constructor
	Foo::Foo(int value) :
		_data(new int(value))
	{
	}

	//	Copy Constructor
	Foo::Foo(const Foo& other) :
		_data(new int(*other._data))
	{
	}

	//	Move Constructor
	Foo::Foo(Foo&& other) noexcept :
		_data(other._data)
	{
		other._data = nullptr;
	}

	//	Assignment Operator overload
	Foo& Foo::operator=(const Foo& other)
	{
		//	Sameness test
		if (this != &other)
		{
			*_data = *other._data;
		}

		return *this;
	}

	//	Assignment operator overload for move semantics
	Foo& Foo::operator=(Foo&& other) noexcept
	{
		if (this != &other)
		{
			delete _data;
			_data = other._data;
			other._data = nullptr;
		}

		return *this;
	}

	//	Destructor
	Foo::~Foo()
	{
		delete _data;
	}
#pragma endregion

#pragma region Equality_Operators
	//	Equality Operator overload
	bool Foo::operator==(const Foo& other) const
	{
		if (_data == nullptr || other._data == nullptr)
		{
			throw std::runtime_error("_data should not be a nullptr. Did you std::move() an instance?");
		}

		return *_data == *other._data;
	}

	//	Not-Equal Operator overload
	bool Foo::operator!=(const Foo& other) const
	{
		return !(operator==(other));
	}
#pragma endregion

	//	Get Data as int
	int Foo::Data() const
	{
		if (_data == nullptr)
		{
			throw std::exception("_data should not be a nullptr. Did you std::move() this instance?");
		}

		return *_data;
	}

	bool Foo::Equals(const RTTI* rhs) const
	{
		const Foo* other = rhs->As<Foo>();
		return other != nullptr ? *this == *other : false;
	}

	std::string Foo::ToString() const
	{
		if (_data == nullptr)
		{
			throw std::exception("_data should not be a nullptr. Foo::ToString().");
		}

		return std::to_string(*_data);
	}
}
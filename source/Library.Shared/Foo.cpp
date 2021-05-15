#include "pch.h"
#include "Foo.h"
#include <exception>

namespace FieaGameEngine
{
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

	//	Equality Operator overload
	bool Foo::operator==(const Foo& other) const
	{
		if (_data == nullptr || other._data == nullptr)
		{
			throw std::exception("_data should not be a nullptr. Did you std::move() an instance?");
		}

		return *_data == *other._data;
	}

	//	Not-Equal Operator overload
	bool Foo::operator!=(const Foo& other) const
	{
		return !(operator==(other));
	}

	//	Get Data as int
	const int Foo::Data() const
	{
		if (_data == nullptr)
		{
			throw std::exception("_data should not be a nullptr. Did you std::move() this instance?");
		}

		return *_data;
	}
}
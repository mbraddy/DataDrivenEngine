#pragma once

namespace FieaGameEngine
{
	class Foo
	{
	public:
		//	Constructor
		explicit Foo(int value = 0);
		//	Copy Constructor
		Foo(const Foo& other);
		//	Move Constructor
		Foo(Foo&& other) noexcept;
		//	Foo assignment operator ( A = B )
		Foo& operator=(const Foo& other);
		//	Move Assignment operator
		Foo& operator=(Foo&& other) noexcept;
		//	Destructor
		~Foo();
		
		//	Overloading == and != for Foo comparisons
		bool operator==(const Foo& other) const;
		bool operator!=(const Foo& other) const;

		const int Data() const;

	private:
		int* _data;
	};
}
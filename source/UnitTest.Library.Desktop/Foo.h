#pragma once
#include "RTTI.h"
#include <string>

namespace FieaGameEngine
{
	/// <summary>
	/// Foo Test class. Only used for unit testing.
	/// </summary>
	class Foo : public FieaGameEngine::RTTI
	{
		RTTI_DECLARATIONS(Foo, RTTI);

	public:
		/// <summary>
		/// Create a new instance of Foo.
		/// </summary>
		/// <param name="value">Provide an initial value for the Data() member.</param>
		explicit Foo(int value = 0);
		
		/// <summary>
		/// Copy constructor.
		/// </summary>
		/// <param name="other">Foo to be copied.</param>
		Foo(const Foo& other);

		/// <summary>
		/// Move Constructor
		/// </summary>
		/// <param name="other">Foo to be moved.</param>
		Foo(Foo&& other) noexcept;

		/// <summary>
		/// Copy Assignment Operator
		/// </summary>
		/// <param name="other">Foo to be copied.</param>
		/// <returns>Reference to the newly created Foo.</returns>
		Foo& operator=(const Foo& other);

		/// <summary>
		/// Move Assignment Operator
		/// </summary>
		/// <param name="other">Foo to be moved.</param>
		/// <returns>Reference to the newly created Foo.</returns>
		Foo& operator=(Foo&& other) noexcept;

		/// <summary>
		/// Destructor
		/// </summary>
		~Foo();
		
		/// <summary>
		/// Equality Operator.
		/// </summary>
		/// <param name="other">Foo to compare to.</param>
		/// <returns>True if the _data members dereferenced values are equivalant, false otherwise.</returns>
		bool operator==(const Foo& other) const;

		/// <summary>
		/// Not Equals operator.
		/// </summary>
		/// <param name="other">Foo to compare to.</param>
		/// <returns>False if the _data members dereferenced values are not equivalent. True otherwise.</returns>
		bool operator!=(const Foo& other) const;

		/// <summary>
		/// Getter for _data member.
		/// </summary>
		/// <returns>A mutable reference to the data member.</returns>
		/// <exception cref="std::runtime_error">Thrown if _data is nullptr. (Only possible if instance has been moved.)</exception>
		int Data() const;

		/// <summary>
		/// Equals override implementation for RTTI parent. Calls As<Foo>() on rhs then returns true if == elsewise false.
		/// </summary>
		/// <param name="rhs">The RTTI pointer to be compared against this.</param>
		/// <returns>True if both are Foo objects that have equal values - otherwise false.</returns>
		bool Equals(const RTTI* rhs) const override;

		/// <summary>
		/// Calls std::to_string on Foo's data member if not nullptr.
		/// </summary>
		/// <returns>_data member of Foo as a string.</returns>
		/// <exception cref="std::runtime_error">Calling ToString() on a Foo with a _data member pointing to nullptr will throw an exception.</exception>
		std::string ToString() const override;

	private:
		/// <summary>
		/// Pointer to the int within Foo.
		/// </summary>
		int* _data;
	};

	class Bar : public FieaGameEngine::RTTI
	{
		RTTI_DECLARATIONS(Bar, RTTI);

	};
}
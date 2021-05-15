#pragma once
#include "RTTI.h"
#include "Attributed.h"
#include <string>

namespace FieaGameEngine
{
	/// <summary>
	/// Foo Test class. Only used for unit testing.
	/// </summary>
	class AttributedFoo : public Attributed
	{
		RTTI_DECLARATIONS(AttributedFoo, Attributed);

	public:
		/// <summary>
		/// Create a new instance of Foo.
		/// </summary>
		/// <param name="value">Provide an initial value for the Data() member.</param>
		AttributedFoo();

		/// <summary>
		/// Clone - Used to create a copy of Attributed Foo
		/// </summary>
		/// <returns>Address of the newly instantiated copy.</returns>
		gsl::owner<AttributedFoo*> Clone() const override;

		/// <summary>
		/// Signatures - Static vector of signatures containing the signatures for the prescribed attributes for this attributed class. Required for registration in the type manager.
		/// </summary>
		/// <returns>Vector of the class's prescribed attributes' signatures.</returns>
		static const Vector<Signature> Signatures();

	private:
		int Integer = 10;
		float Float = 10.0f;
		vec4 Vector = { 10.0f, 10.0f, 10.0f, 10.0f };
		mat4 Matrix = { 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f };
		std::string String = "10";
		Scope scope;
		RTTI* Rtti;
		int IntegerArray[2] = { 10, 20 };
		float FloatArray[2] = { 10.0f, 20.0f };
		vec4 VectorArray[2] = { { 10.0f, 10.0f, 10.0f, 10.0f }, { 20.0f, 20.0f, 20.0f, 20.0f } };
		mat4 MatrixArray[2] = { { 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f },
			{ 20.0f, 20.0f, 20.0f, 20.0f, 20.0f, 20.0f, 20.0f, 20.0f, 20.0f, 20.0f, 20.0f, 20.0f, 20.0f, 20.0f, 20.0f, 20.0f } };
		std::string StringArray[2] = { "10", "20" };
		Scope scopeArray[2];
		RTTI* RttiArray[2];
	};
}
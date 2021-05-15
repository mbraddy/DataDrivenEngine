#pragma once
#include "pch.h"
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/string_cast.hpp>
#include "RTTI.h"
#include "json/json.h"
#include "HashMap.h"

using namespace glm;
using namespace std;

namespace FieaGameEngine
{

	class Scope;
	class Attributed;

	/// <summary>
	/// Datum Class - Allows instantiation of objects at runtime.
	/// </summary>
	class Datum final
	{
	public:

		friend Scope;
		friend Attributed;

		/// <summary>
		/// Supported Datum Types
		/// </summary>
		enum class DatumType
		{
			Float,
			Integer,
			Matrix,
			Pointer,
			String,
			Vector,
			Table,
			Unknown
		};

		const inline static HashMap<std::string, DatumType> _setTypeJsonTableParseMap = {
			{"float", DatumType::Float},
			{"integer", DatumType::Integer},
			{"matrix", DatumType::Matrix},
			{"string", DatumType::String},
			{"table", DatumType::Table},
			{"vector", DatumType::Vector}
		};

#pragma region Datum Rule of 6

		/// <summary>
		/// Default constructor that allows setting type at object creation.
		/// </summary>
		/// <param name="type">Allows you to bypass calling SetType - The type of the datum.</param>
		explicit Datum(DatumType type = DatumType::Unknown);

		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="other">Datum to be copied.</param>
		Datum(const Datum& other);
		
		/// <summary>
		/// Move constructor
		/// </summary>
		/// <param name="other">Datum to be moved.</param>
		/// <returns>Reference to the newly created Datum</returns>
		Datum(Datum&& other) noexcept;

		/// <summary>
		/// Copy assignment operator
		/// </summary>
		/// <param name="other">Datum to be copied.</param>
		/// <returns>The copied Datum</returns>
		Datum& operator=(const Datum& other);

		/// <summary>
		/// Move assignment operator
		/// </summary>
		/// <param name="other">Datum to be moved.</param>
		/// <returns>Reference to the newly created datum</returns>
		Datum& operator=(Datum&& other) noexcept;

		/// <summary>
		/// Destructor for Datum (doesn't do anything if doesnt own data)
		/// </summary>
		~Datum();

#pragma endregion

#pragma region Datum Copy Assignment Operator Overloads (scalars)

		/// <summary>
		/// Datum Copy assignment operator for float scalars.
		/// </summary>
		/// <param name="other">The scalar to be copied.</param>
		/// <returns>Datum set to the scalar's type with it's value in _data[0]</returns>
		Datum& operator=(const float& other);

		/// <summary>
		/// Datum Copy assignment operator for int scalars.
		/// </summary>
		/// <param name="other">The scalar to be copied.</param>
		/// <returns>Datum set to the scalar's type with it's value in _data[0]</returns>
		Datum& operator=(const int& other);

		/// <summary>
		/// Datum Copy assignment operator for mat4x4 scalars.
		/// </summary>
		/// <param name="other">The scalar to be copied.</param>
		/// <returns>Datum set to the scalar's type with it's value in _data[0]</returns>
		Datum& operator=(const mat4x4& other);

		/// <summary>
		/// Datum Copy assignment operator for RTTI Pointers.
		/// </summary>
		/// <param name="other">The scalar to be copied.</param>
		/// <returns>Datum set to the scalar's type with it's value in _data[0]</returns>
		Datum& operator=(RTTI* const& other);

		/// <summary>
		/// Datum Copy assignment operator for std::string literals.
		/// </summary>
		/// <param name="other">The scalar to be copied.</param>
		/// <returns>Datum set to the scalar's type with it's value in _data[0]</returns>
		Datum& operator=(const string& other);

		/// <summary>
		/// Datum Copy assignment operator for vec4 scalars.
		/// </summary>
		/// <param name="other">The scalar to be copied.</param>
		/// <returns>Datum set to the scalar's type with it's value in _data[0]</returns>
		Datum& operator=(const vec4& other);

#pragma endregion

#pragma region Datum String Methods

		/// <summary>
		/// Returns a string for each DatumType
		/// </summary>
		/// <param name="index">Optional Index to print out</param>
		/// <typeparam name="T">Data Types (Inherently supported are DatumTypes)</typeparam>
		/// <returns>String of the data value.</returns>
		template <typename T>
		string ToString(size_t index = 0);

		/// <summary>
		/// Parses a string and sets a value at the provided index to the data member for it.
		/// </summary>
		/// <param name="s">String to be parsed.</param>
		/// <param name="index">Index to store the parsed value.</param>
		/// <returns>True if a value was successfully parsed from the string and inserted.</returns>
		bool SetFromString(string s, size_t index = 0);

		/// <summary>
		/// Parses a string and pushes the value to the back of the datum.
		/// </summary>
		/// <param name="s">String to be parsed.</param>
		/// <returns>True if a value was successfully parsed and pushed back.</returns>
		bool PushBackFromString(string s);
#pragma endregion

#pragma region Datum Equality Operator Overloads

		/// <summary>
		/// Equality operator for Datum to Datum
		/// </summary>
		/// <param name="other">Datum to compare against</param>
		/// <returns>True if the size, type, and elements are the same. Else false.</returns>
		bool operator==(const Datum& other) const;

		/// <summary>
		/// Not Equals operator for datum to datum
		/// </summary>
		/// <param name="other">Datum to compare against</param>
		/// <returns>False if the size, type, or elements are different. Else true.</returns>
		bool operator!=(const Datum& other) const;

		/// <summary>
		/// Equality operator for Datum to float Scalars
		/// </summary>
		/// <param name="other">Scalar to compare against.</param>
		/// <returns>True if the type is correct and _data[0] is equal to the scalar. Else false</returns>

		bool operator==(const float& other) const;

		/// <summary>
		/// Equality operator for Datum to int Scalars
		/// </summary>
		/// <param name="other">Scalar to compare against.</param>
		/// <returns>True if the type is correct and _data[0] is equal to the scalar. Else false</returns>
		bool operator==(const int& other) const;

		/// <summary>
		/// Equality operator for Datum to mat4x4 Scalars
		/// </summary>
		/// <param name="other">Scalar to compare against.</param>
		/// <returns>True if the type is correct and _data[0] is equal to the scalar. Else false</returns>
		bool operator==(const mat4x4& other) const;

		/// <summary>
		/// Equality operator for Datum to string literals
		/// </summary>
		/// <param name="other">Scalar to compare against.</param>
		/// <returns>True if the type is correct and _data[0] is equal to the scalar. Else false</returns>
		bool operator==(const string& other) const;

		/// <summary>
		/// Equality operator for Datum to RTTI pointers
		/// </summary>
		/// <param name="other">Scalar to compare against.</param>
		/// <returns>True if the type is correct and _data[0] is equal to the scalar. Else false</returns>
		bool operator==(RTTI* const & other) const;

		/// <summary>
		/// Equality operator for Datum to vector Scalars
		/// </summary>
		/// <param name="other">Scalar to compare against.</param>
		/// <returns>True if the type is correct and _data[0] is equal to the scalar. Else false</returns>
		bool operator==(const vec4& other) const;

		/// <summary>
		/// Not Equals operator for Datum to float scalars
		/// </summary>
		/// <param name="other">Scalar to be compared.</param>
		/// <returns>True if either the typing is different or the scalar is different from _data[0]</returns>
		
		bool operator!=(const float& other) const;

		/// <summary>
		/// Not Equals operator for Datum to int scalars
		/// </summary>
		/// <param name="other">Scalar to be compared.</param>
		/// <returns>True if either the typing is different or the scalar is different from _data[0]</returns>
		bool operator!=(const int& other) const;

		/// <summary>
		/// Not Equals operator for Datum to mat4x4 scalars
		/// </summary>
		/// <param name="other">Scalar to be compared.</param>
		/// <returns>True if either the typing is different or the scalar is different from _data[0]</returns>
		bool operator!=(const mat4x4& other) const;

		/// <summary>
		/// Not Equals operator for Datum to string literals
		/// </summary>
		/// <param name="other">Scalar to be compared.</param>
		/// <returns>True if either the typing is different or the scalar is different from _data[0]</returns>
		bool operator!=(const string& other) const;

		/// <summary>
		/// Not Equals operator for Datum to RTTI pointers
		/// </summary>
		/// <param name="other">Scalar to be compared.</param>
		/// <returns>True if either the typing is different or the scalar is different from _data[0]</returns>
		bool operator!=(RTTI* const& other) const;

		/// <summary>
		/// Not Equals operator for Datum to vector scalars
		/// </summary>
		/// <param name="other">Scalar to be compared.</param>
		/// <returns>True if either the typing is different or the scalar is different from _data[0]</returns>
		bool operator!=(const vec4& other) const;

#pragma endregion
		 
#pragma region Datum Member Getters

		/// <summary>
		/// Getter for _size
		/// </summary>
		/// <returns>Number of Elements in the Datum's value array.</returns>
		size_t Size() const;

		/// <summary>
		/// Getter for _capacity
		/// </summary>
		/// <returns>Number of Elements allocated to memory in the Datum's value array.</returns>
		size_t Capacity() const;

		/// <summary>
		/// Getter for _type
		/// </summary>
		/// <returns>Datum::DatumType::* enum that matches the type of data in value array.</returns>
		DatumType Type() const;

		/// <summary>
		/// OwnsData - Getter for _OwnsData
		/// </summary>
		/// <returns>True if the datum owns the data, else false.</returns>
		bool OwnsData() const;

#pragma endregion

#pragma region Datum Data Setters and Getters

		/// <summary>
		/// Sets an L-value to a desired index in a datum's value array.
		/// </summary>
		/// <param name="value">Value you wish to set in the array</param>
		/// <param name="index">Index for where you want to set it to (must be allocated)</param>
		/// <returns>True if the value was set successfully.</returns>
		/// <exception cref="std::runtime_error">If Index >= _size you will get a runtime error for accessing beyond capacity</exception>
		/// <exception cref="std::runtime_error">If the value type doesn't match the datum type you will get a runtime error.</exception>
		/// <exception cref="std::runtime_error">If you try to set on data the datum doesn't own you will get a runtime error.</exception>
		bool Set(const float& value, size_t index = 0);

		/// <summary>
		/// Sets an L-value to a desired index in a datum's value array.
		/// </summary>
		/// <param name="value">Value you wish to set in the array</param>
		/// <param name="index">Index for where you want to set it to (must be allocated)</param>
		/// <returns>True if the value was set successfully.</returns>
		/// <exception cref="std::runtime_error">If Index >= _size you will get a runtime error for accessing beyond capacity</exception>
		/// <exception cref="std::runtime_error">If the value type doesn't match the datum type you will get a runtime error.</exception>
		/// <exception cref="std::runtime_error">If you try to set on data the datum doesn't own you will get a runtime error.</exception>
		bool Set(const int& value, size_t index = 0);

		/// <summary>
		/// Sets an L-value to a desired index in a datum's value array.
		/// </summary>
		/// <param name="value">Value you wish to set in the array</param>
		/// <param name="index">Index for where you want to set it to (must be allocated)</param>
		/// <returns>True if the value was set successfully.</returns>
		/// <exception cref="std::runtime_error">If Index >= _size you will get a runtime error for accessing beyond capacity</exception>
		/// <exception cref="std::runtime_error">If the value type doesn't match the datum type you will get a runtime error.</exception>
		/// <exception cref="std::runtime_error">If you try to set on data the datum doesn't own you will get a runtime error.</exception>
		bool Set(const mat4x4& value, size_t index = 0);

		/// <summary>
		/// Sets an L-value to a desired index in a datum's value array.
		/// </summary>
		/// <param name="value">Value you wish to set in the array</param>
		/// <param name="index">Index for where you want to set it to (must be allocated)</param>
		/// <returns>True if the value was set successfully.</returns>
		/// <exception cref="std::runtime_error">If Index >= _size you will get a runtime error for accessing beyond capacity</exception>
		/// <exception cref="std::runtime_error">If the value type doesn't match the datum type you will get a runtime error.</exception>
		/// <exception cref="std::runtime_error">If you try to set on data the datum doesn't own you will get a runtime error.</exception>
		bool Set(RTTI* const & value, size_t index = 0);

		/// <summary>
		/// Sets an L-value to a desired index in a datum's value array.
		/// </summary>
		/// <param name="value">Value you wish to set in the array</param>
		/// <param name="index">Index for where you want to set it to (must be allocated)</param>
		/// <returns>True if the value was set successfully.</returns>
		/// <exception cref="std::runtime_error">If Index >= _size you will get a runtime error for accessing beyond capacity</exception>
		/// <exception cref="std::runtime_error">If the value type doesn't match the datum type you will get a runtime error.</exception>
		/// <exception cref="std::runtime_error">If you try to set on data the datum doesn't own you will get a runtime error.</exception>
		bool Set(const string& value, size_t index = 0);

		/// <summary>
		/// Sets an L-value to a desired index in a datum's value array.
		/// </summary>
		/// <param name="value">Value you wish to set in the array</param>
		/// <param name="index">Index for where you want to set it to (must be allocated)</param>
		/// <returns>True if the value was set successfully.</returns>
		/// <exception cref="std::runtime_error">If Index >= _size you will get a runtime error for accessing beyond capacity</exception>
		/// <exception cref="std::runtime_error">If the value type doesn't match the datum type you will get a runtime error.</exception>
		/// <exception cref="std::runtime_error">If you try to set on data the datum doesn't own you will get a runtime error.</exception>
		bool Set(const vec4& value, size_t index = 0);

		/// <summary>
		/// Sets an R-value to a desired index in a datum's value array.
		/// </summary>
		/// <param name="value">Value you wish to set in the array</param>
		/// <param name="index">Index for where you want to set it to (must be allocated)</param>
		/// <returns>True if the value was set successfully.</returns>
		/// <exception cref="std::runtime_error">If Index >= _size you will get a runtime error for accessing beyond capacity</exception>
		/// <exception cref="std::runtime_error">If the value type doesn't match the datum type you will get a runtime error.</exception>
		/// <exception cref="std::runtime_error">If you try to set on data the datum doesn't own you will get a runtime error.</exception>
		bool Set(string&& value, size_t index = 0);

		/// <summary>
		/// Sets an L-value to a desired index in a datum's value array.
		/// </summary>
		/// <param name="value">Value you wish to set in the array</param>
		/// <param name="index">Index for where you want to set it to (must be allocated)</param>
		/// <returns>True if the value was set successfully.</returns>
		/// <exception cref="std::runtime_error">If Index >= _size you will get a runtime error for accessing beyond capacity</exception>
		/// <exception cref="std::runtime_error">If the value type doesn't match the datum type you will get a runtime error.</exception>
		/// <exception cref="std::runtime_error">If you try to set on data the datum doesn't own you will get a runtime error.</exception>
		bool Set(const Scope& value, size_t index = 0);

		/// <summary>
		/// Gets a mutable value reference from a Datum _value array at a desired index.
		/// </summary>
		/// <typeparam name="T">Data Types (Inherently supported are DatumTypes)</typeparam>
		/// <param name="index">Index for where you want to retrieve data from.</param>
		/// <returns>Mutable Reference to value array location.</returns>
		/// <exception cref="std::runtime_error">If Index >= _size you will get a runtime error for accessing out of bounds</exception>
		/// <exception cref="std::runtime_error">If you call the wrong <valueType>Get you will get a runtime error</exception>
		template <typename T>
		T& Get(size_t index = 0);

		/// <summary>
		/// Gets a nonmutable value reference from a Datum _value array at a desired index.
		/// </summary>
		/// <typeparam name="T">Data Types (Inherently supported are DatumTypes)</typeparam>
		/// <param name="index">Index for where you want to retrieve data from.</param>
		/// <returns>Nonmutable Reference to value array location.</returns>
		/// <exception cref="std::runtime_error">If Index >= _size you will get a runtime error for accessing out of bounds</exception>
		/// <exception cref="std::runtime_error">If you call the wrong <valueType>Get you will get a runtime error</exception>
		template <typename T>
		const T& Get(size_t index = 0) const;

		/// <summary>
		/// SetFromJsonValue - Takes in a de-serialized json object and the proper index and calls the correct Set() based on the datum type.
		/// </summary>
		void SetFromJsonValue(Json::Value object, size_t index);

#pragma endregion

#pragma region Datum Pushback Methods

		/// <summary>
		/// Inserts L-value to the back of the Datum value array. Will expand capacity if full.
		/// </summary>
		/// <param name="value">Value to be inserted to the array.</param>
		/// <returns>Index of the newly placed object.</returns>
		/// <exception cref="std::runtime_error">Calling when datum doesn't own the array causes a runtime error.</exception>

		size_t PushBack(const float& value);

		/// <summary>
		/// Inserts L-value to the back of the Datum value array. Will expand capacity if full.
		/// </summary>
		/// <param name="value">Value to be inserted to the array.</param>
		/// <returns>Index of the newly placed object.</returns>
		/// <exception cref="std::runtime_error">Calling when datum doesn't own the array causes a runtime error.</exception>
		size_t PushBack(const int& value);

		/// <summary>
		/// Inserts L-value to the back of the Datum value array. Will expand capacity if full.
		/// </summary>
		/// <param name="value">Value to be inserted to the array.</param>
		/// <returns>Index of the newly placed object.</returns>
		/// <exception cref="std::runtime_error">Calling when datum doesn't own the array causes a runtime error.</exception>
		size_t PushBack(const mat4x4& value);

		/// <summary>
		/// Inserts L-value to the back of the Datum value array. Will expand capacity if full.
		/// </summary>
		/// <param name="value">Value to be inserted to the array.</param>
		/// <returns>Index of the newly placed object.</returns>
		/// <exception cref="std::runtime_error">Calling when datum doesn't own the array causes a runtime error.</exception>
		size_t PushBack(RTTI* const& value);

		/// <summary>
		/// Inserts L-value to the back of the Datum value array. Will expand capacity if full.
		/// </summary>
		/// <param name="value">Value to be inserted to the array.</param>
		/// <returns>Index of the newly placed object.</returns>
		/// <exception cref="std::runtime_error">Calling when datum doesn't own the array causes a runtime error.</exception>
		size_t PushBack(const string& value);

		/// <summary>
		/// Inserts L-value to the back of the Datum value array. Will expand capacity if full.
		/// </summary>
		/// <param name="value">Value to be inserted to the array.</param>
		/// <returns>Index of the newly placed object.</returns>
		/// <exception cref="std::runtime_error">Calling when datum doesn't own the array causes a runtime error.</exception>
		size_t PushBack(const vec4& value);

		/// <summary>
		/// Inserts R-value to the back of the Datum value array. Will expand capacity if full.
		/// </summary>
		/// <param name="value">Value to be inserted to the array.</param>
		/// <returns>Index of the newly placed object.</returns>
		/// <exception cref="std::runtime_error">Calling when datum doesn't own the array causes a runtime error.</exception>
		size_t PushBack(string&& value);

		/// <summary>
		/// Inserts L-value to the back of the Datum value array. Will expand capacity if full.
		/// </summary>
		/// <param name="value">Value to be inserted to the array.</param>
		/// <returns>Index of the newly placed object.</returns>
		/// <exception cref="std::runtime_error">Calling when datum doesn't own the array causes a runtime error.</exception>
		size_t PushBack(const Scope& value);

		/// <summary>
		/// PushBackFromJsonValue - Takes in a de-serialized json object and passes it into the correct pushback based on the datum's type.
		/// </summary>
		void PushBackFromJsonValue(Json::Value object);

#pragma endregion

#pragma region Datum Remove Methods

		/// <summary>
		/// Deletes the designated object from the Datum value array.
		/// </summary>
		/// <param name="index">The location in the value array of the thing to be deleted</param>
		/// <returns>False if Index is >= size, otherwise true when it deletes something.</returns>
		/// <exception cref="std::runtime_error">Calling when datum doesn't own the array causes a runtime error.</exception>
		bool RemoveAt(size_t index);

		/// <summary>
		/// Remove the specified value from the array of the Datum. Calls RemoveAt(IndexOf())
		/// </summary>
		/// <param name="value">Value to be removed to the array.</param>
		/// <returns>True if successfully found and removed. False otherwise.</returns>
		/// <exception cref="std::runtime_error">Calling when datum doesn't own the array causes a runtime error.</exception>
		/// <exception cref="std::runtime_error">Attempting to call Remove on incompatible data types causes a runtime error.</exception>
		bool Remove(const float& value);

		/// <summary>
		/// Remove the specified value from the array of the Datum. Calls RemoveAt(IndexOf())
		/// </summary>
		/// <param name="value">Value to be removed to the array.</param>
		/// <returns>True if successfully found and removed. False otherwise.</returns>
		/// <exception cref="std::runtime_error">Calling when datum doesn't own the array causes a runtime error.</exception>
		/// <exception cref="std::runtime_error">Attempting to call Remove on incompatible data types causes a runtime error.</exception>
		bool Remove(const int& value);

		/// <summary>
		/// Remove the specified value from the array of the Datum. Calls RemoveAt(IndexOf())
		/// </summary>
		/// <param name="value">Value to be removed to the array.</param>
		/// <returns>True if successfully found and removed. False otherwise.</returns>
		/// <exception cref="std::runtime_error">Calling when datum doesn't own the array causes a runtime error.</exception>
		/// <exception cref="std::runtime_error">Attempting to call Remove on incompatible data types causes a runtime error.</exception>
		bool Remove(const mat4x4& value);

		/// <summary>
		/// Remove the specified value from the array of the Datum. Calls RemoveAt(IndexOf())
		/// </summary>
		/// <param name="value">Value to be removed to the array.</param>
		/// <returns>True if successfully found and removed. False otherwise.</returns>
		/// <exception cref="std::runtime_error">Calling when datum doesn't own the array causes a runtime error.</exception>
		/// <exception cref="std::runtime_error">Attempting to call Remove on incompatible data types causes a runtime error.</exception>
		bool Remove(RTTI* const& value);

		/// <summary>
		/// Remove the specified value from the array of the Datum. Calls RemoveAt(IndexOf())
		/// </summary>
		/// <param name="value">Value to be removed to the array.</param>
		/// <returns>True if successfully found and removed. False otherwise.</returns>
		/// <exception cref="std::runtime_error">Calling when datum doesn't own the array causes a runtime error.</exception>
		/// <exception cref="std::runtime_error">Attempting to call Remove on incompatible data types causes a runtime error.</exception>
		bool Remove(const string& value);

		/// <summary>
		/// Remove the specified value from the array of the Datum. Calls RemoveAt(IndexOf())
		/// </summary>
		/// <param name="value">Value to be removed to the array.</param>
		/// <returns>True if successfully found and removed. False otherwise.</returns>
		/// <exception cref="std::runtime_error">Calling when datum doesn't own the array causes a runtime error.</exception>
		/// <exception cref="std::runtime_error">Attempting to call Remove on incompatible data types causes a runtime error.</exception>
		bool Remove(const vec4& value);

		/// <summary>
		/// Remove the specified value from the array of the Datum. Calls RemoveAt(IndexOf())
		/// </summary>
		/// <param name="value">Value to be removed to the array.</param>
		/// <returns>True if successfully found and removed. False otherwise.</returns>
		/// <exception cref="std::runtime_error">Calling when datum doesn't own the array causes a runtime error.</exception>
		/// <exception cref="std::runtime_error">Attempting to call Remove on incompatible data types causes a runtime error.</exception>
		bool Remove(const Scope& value);

#pragma endregion

#pragma region Datum SetStorage Methods

		/// <summary>
		/// Sets the data array of the Datum to a pointer of a data type and marks it as unowned data.
		/// </summary>
		/// <param name="arr">Pointer to array or value</param>
		/// <param name="count">Number of elements in the array</param>
		/// <returns>True if the set storage was successful</returns>
		/// <exception cref="std::runtime_error">Calling setstorage of a different type causes a runtime error.</exception>
		/// <exception cref="std::runtime_error">Passing count as 0 or arr as nullptr causes a runtime error.</exception>
		bool SetStorage(float* arr, size_t count);

		/// <summary>
		/// Sets the data array of the Datum to a pointer of a data type and marks it as unowned data.
		/// </summary>
		/// <param name="arr">Pointer to array or value</param>
		/// <param name="count">Number of elements in the array</param>
		/// <returns>True if the set storage was successful</returns>
		/// <exception cref="std::runtime_error">Calling setstorage of a different type causes a runtime error.</exception>
		/// <exception cref="std::runtime_error">Passing count as 0 or arr as nullptr causes a runtime error.</exception>
		bool SetStorage(int* arr, size_t count);

		/// <summary>
		/// Sets the data array of the Datum to a pointer of a data type and marks it as unowned data.
		/// </summary>
		/// <param name="arr">Pointer to array or value</param>
		/// <param name="count">Number of elements in the array</param>
		/// <returns>True if the set storage was successful</returns>
		/// <exception cref="std::runtime_error">Calling setstorage of a different type causes a runtime error.</exception>
		/// <exception cref="std::runtime_error">Passing count as 0 or arr as nullptr causes a runtime error.</exception>
		bool SetStorage(mat4x4* arr, size_t count);

		/// <summary>
		/// Sets the data array of the Datum to a pointer of a data type and marks it as unowned data.
		/// </summary>
		/// <param name="arr">Pointer to array or value</param>
		/// <param name="count">Number of elements in the array</param>
		/// <returns>True if the set storage was successful</returns>
		/// <exception cref="std::runtime_error">Calling setstorage of a different type causes a runtime error.</exception>
		/// <exception cref="std::runtime_error">Passing count as 0 or arr as nullptr causes a runtime error.</exception>
		bool SetStorage(string* arr, size_t count);

		/// <summary>
		/// Sets the data array of the Datum to a pointer of a data type and marks it as unowned data.
		/// </summary>
		/// <param name="arr">Pointer to array or value</param>
		/// <param name="count">Number of elements in the array</param>
		/// <returns>True if the set storage was successful</returns>
		/// <exception cref="std::runtime_error">Calling setstorage of a different type causes a runtime error.</exception>
		/// <exception cref="std::runtime_error">Passing count as 0 or arr as nullptr causes a runtime error.</exception>
		bool SetStorage(RTTI** arr, size_t count);

		/// <summary>
		/// Sets the data array of the Datum to a pointer of a data type and marks it as unowned data.
		/// </summary>
		/// <param name="arr">Pointer to array or value</param>
		/// <param name="count">Number of elements in the array</param>
		/// <returns>True if the set storage was successful</returns>
		/// <exception cref="std::runtime_error">Calling setstorage of a different type causes a runtime error.</exception>
		/// <exception cref="std::runtime_error">Passing count as 0 or arr as nullptr causes a runtime error.</exception>
		bool SetStorage(vec4* arr, size_t count);

#pragma endregion

#pragma region Datum IndexOf Methods

		/// <summary>
		/// Returns an index referencing the desired element in the value array.
		/// </summary>
		/// <param name="value">The value to be found</param>
		/// <returns>Index to the location of the passed in value in the data array or _size if not in it.</returns>
		/// <exception cref="std::runtime_error">Calling IndexOf() using a value with a different type than the datum will cause a runtime error.</exception>
		size_t IndexOf(const float& value) const;

		/// <summary>
		/// Returns an index referencing the desired element in the value array.
		/// </summary>
		/// <param name="value">The value to be found</param>
		/// <returns>Index to the location of the passed in value in the data array or _size if not in it.</returns>
		/// <exception cref="std::runtime_error">Calling IndexOf() using a value with a different type than the datum will cause a runtime error.</exception>
		size_t IndexOf(const int& value) const;

		/// <summary>
		/// Returns an index referencing the desired element in the value array.
		/// </summary>
		/// <param name="value">The value to be found</param>
		/// <returns>Index to the location of the passed in value in the data array or _size if not in it.</returns>
		/// <exception cref="std::runtime_error">Calling IndexOf() using a value with a different type than the datum will cause a runtime error.</exception>
		size_t IndexOf(const mat4x4& value) const;

		/// <summary>
		/// Returns an index referencing the desired element in the value array.
		/// </summary>
		/// <param name="value">The value to be found</param>
		/// <returns>Index to the location of the passed in value in the data array or _size if not in it.</returns>
		/// <exception cref="std::runtime_error">Calling IndexOf() using a value with a different type than the datum will cause a runtime error.</exception>
		size_t IndexOf(RTTI* const& value) const;

		/// <summary>
		/// Returns an index referencing the desired element in the value array.
		/// </summary>
		/// <param name="value">The value to be found</param>
		/// <returns>Index to the location of the passed in value in the data array or _size if not in it.</returns>
		/// <exception cref="std::runtime_error">Calling IndexOf() using a value with a different type than the datum will cause a runtime error.</exception>
		size_t IndexOf(const string& value) const;

		/// <summary>
		/// Returns an index referencing the desired element in the value array.
		/// </summary>
		/// <param name="value">The value to be found</param>
		/// <returns>Index to the location of the passed in value in the data array or _size if not in it.</returns>
		/// <exception cref="std::runtime_error">Calling IndexOf() using a value with a different type than the datum will cause a runtime error.</exception>
		size_t IndexOf(const vec4& value) const;

		/// <summary>
		/// Returns an index referencing the desired element in the value array.
		/// </summary>
		/// <param name="value">The value to be found</param>
		/// <returns>Index to the location of the passed in value in the data array or _size if not in it.</returns>
		/// <exception cref="std::runtime_error">Calling IndexOf() using a value with a different type than the datum will cause a runtime error.</exception>
		size_t IndexOf(Scope* const& value) const;

#pragma endregion

#pragma region Datum - Other Control Methods

		/// <summary>
		/// If Datum owns data, Deconstructs string types, sets size to 0.
		/// </summary>
		void Clear();

		/// <summary>
		/// Deletes the last object from the Datum value array.
		/// </summary>
		/// <returns>False if size == 0, otherwise true when it deletes something.</returns>
		/// <exception cref="std::runtime_error">Calling when datum doesn't own the array causes a runtime error.</exception>
		bool PopBack();

		/// <summary>
		/// Allocates memory of sizeof(datum type) * capacity and sets value array to point to it.
		/// </summary>
		/// <param name="capacity">The number of objects you wish to allocate</param>
		/// <exception cref="std::runtime_error">Calling reserve on an un-typed Datum gets a runtime error</exception>
		/// <exception cref="std::runtime_error">Calling when datum doesn't own the array causes a runtime error.</exception>
		void Reserve(size_t capacity);

		/// <summary>
		/// Resizes the Datum value array to size and sets value array to point to it.
		/// Creates default constructed DatumTypes to fill the _size to _capacity
		/// </summary>
		/// <param name="size">The number of objects you wish to resize to</param>
		/// <exception cref="std::runtime_error">Calling resize on an un-typed Datum gets a runtime error</exception>
		/// <exception cref="std::runtime_error">Calling when datum doesn't own the array causes a runtime error.</exception>
		void Resize(size_t size);

		/// <summary>
		/// Sets the _type of the Datum to the passed in DatumType
		/// </summary>
		/// <param name="type">The DatumType you wish to set Datum to.</param>
		/// <exception cref="std::runtime_error">You can't manually change datum types once it is set.</exception>
		void SetType(DatumType type);

		/// <summary>
		/// Shrinks the Datum's capacity down to the size. Reallocs the memory if necessary.
		/// </summary>
		void ShrinkToFit();

#pragma endregion

#pragma region Datum Accessor Methods

		/// <summary>
		/// Returns a Mutable reference to the last element in the value array
		/// </summary>
		/// <typeparam name="T">Data Types (Inherently supported are DatumTypes)</typeparam>
		/// <returns>Mutable reference to the last element in the value array</returns>
		/// <exception cref="std::runtime_error">If the value array is null, calling Back will cause a runtime error</exception>
		/// <exception cref="std::runtime_error">Calling Back() of an different type than the datum will cause a runtime error.</exception>
		template <typename T>
		T& Back();

		/// <summary>
		/// Returns a Nonmutable reference to the last element in the value array
		/// </summary>
		/// <typeparam name="T">Data Types (Inherently supported are DatumTypes)</typeparam>
		/// <returns>Nonmutable reference to the last element in the value array</returns>
		/// <exception cref="std::runtime_error">If the value array is null, calling Back will cause a runtime error</exception>
		/// <exception cref="std::runtime_error">Calling Back() of an different type than the datum will cause a runtime error.</exception>
		template <typename T>
		const T& Back() const;

		/// <summary>
		/// Returns a Mutable reference to the First element in the value array
		/// </summary>
		/// <typeparam name="T">Data Types (Inherently supported are DatumTypes)</typeparam>
		/// <returns>Mutable reference to the First element in the value array</returns>
		/// <exception cref="std::runtime_error">If the value array is null, calling Front will cause a runtime error</exception>
		/// <exception cref="std::runtime_error">Calling Front() of an different type than the datum will cause a runtime error.</exception>
		template <typename T>
		T& Front();

		/// <summary>
		/// Returns a Nonmutable reference to the First element in the value array
		/// </summary>
		/// <typeparam name="T">Data Types (Inherently supported are DatumTypes)</typeparam>
		/// <returns>Nonmutable reference to the First element in the value array</returns>
		/// <exception cref="std::runtime_error">If the value array is null, calling Front will cause a runtime error</exception>
		/// <exception cref="std::runtime_error">Calling Front() of an different type than the datum will cause a runtime error.</exception>
		template <typename T>
		const T& Front() const;

		/// <summary>
		/// Operator[] for accessing nested scopes
		/// </summary>
		/// <param name="index">Gets the scope at the specified index</param>
		/// <returns>Reference to the scope at the passed in index</returns>
		Scope& operator[](size_t index);

#pragma endregion

	private:	
		/// <summary>
		/// Datum Copy assignment operator for Scope pointers.
		/// </summary>
		/// <param name="other">The scalar to be copied.</param>
		/// <returns>Datum set to the scalar's type with it's value in _data[0]</returns>
		Datum& operator=(const Scope& other);

		/// <summary>
		/// Helper function for SetStorage - Provides type protection and sets the _data pointer to the external storage while setting the datum's ownership to false.
		/// </summary>
		/// <param name="type">The type of the array</param>
		/// <param name="arr">Pointer to the head of the array</param>
		/// <param name="count">Number of elements in the external array</param>
		/// <exception cref="std::runtime_error">Will throw a runtime error if attempting to set an array of the incorrect type to a Datum that's type is already set.</exception>
		/// <exception cref="std::runtime_error">Will throw a runtime error if attempting to pass in a nullptr as arr or 0 as the count.</exception>
		/// <returns>True if set successfully. Can disregard the return value if you wish.</returns>
		bool SetStorage(DatumType type, void* arr, size_t count);

		/// <summary>
		/// Union of pointers to potential Datum Values.
		/// Establishes lens' that we can use for pointer arithmetic.
		/// </summary>
		union DatumValues
		{
			float* f;
			int* i;
			mat4x4* m;
			RTTI** p;
			string* s;
			Scope** t;
			vec4* v;
			void* vp{ nullptr };
		};

		/// <summary>
		/// Type of the Datum. Dictates what can be stored in it's value array.
		/// Default = Unknown.
		/// </summary>
		DatumType _type{ DatumType::Unknown };

		/// <summary>
		/// Number of Elements in the value array.
		/// Default = 0.
		/// </summary>
		size_t _size{ 0 };

		/// <summary>
		/// Number of Elements allocated in memory for the array.
		/// Doesn't allocate any by default.
		/// </summary>
		size_t _capacity{ 0 };

		/// <summary>
		/// Union of the pointers to access the Data array.
		/// </summary>
		DatumValues _data;

		/// <summary>
		/// Indicates whether the data in Datum is owned by datum.
		/// Used to dictate Memory management.
		/// </summary>
		bool _ownsData{ true };

		/// <summary>
		/// An array of potential data sizes. Referenced for allocation.
		/// </summary>
		inline static const size_t _sizeMap[static_cast<int>(DatumType::Unknown)] =
		{
			sizeof(float),		//	DatumTypes::Float,
			sizeof(int),		//	DatumTypes::Integer,
			sizeof(mat4x4),		//	DatumTypes::Matrix,
			sizeof(RTTI*),		//	DatumTypes::Pointer,
			sizeof(string),		//	DatumTypes::String,
			sizeof(vec4),		//	DatumTypes::Vector,
			sizeof(Scope**)		//	DatumTypes::Table
		};

#pragma region Resize Function Table
		/// <summary>
		/// Placement news a default float (0.0f)
		/// </summary>
		/// <param name="index">Index to instantiate the object</param>
		void CreateFloat(size_t index);

		/// <summary>
		/// Placement news a default int (0)
		/// </summary>
		/// <param name="index">Index to instantiate the object</param>
		void CreateInteger(size_t index);

		/// <summary>
		/// Placement news a default mat4 (0.0f)
		/// </summary>
		/// <param name="index">Index to instantiate the object</param>
		void CreateMatrix(size_t index);

		/// <summary>
		/// Placement news a default RTTI* (nullptr)
		/// </summary>
		/// <param name="index">Index to instantiate the object</param>
		void CreatePointer(size_t index);

		/// <summary>
		/// Placement news a default string ()
		/// </summary>
		/// <param name="index">Index to instantiate the object</param>
		void CreateString(size_t index);

		/// <summary>
		/// Placement news a default vec4 (0.0f)
		/// </summary>
		/// <param name="index">Index to instantiate the object</param>
		void CreateVector(size_t index);

		/// <summary>
		/// Placement news a default Scope/Table 
		/// </summary>
		/// <param name="index">Index to instantiate the object</param>
		void CreateTable(size_t index);

		using CreateDefaultFunction = void(Datum::*)(size_t index);

		/// <summary>
		/// Array of function pointers to the different placement news used for allocation. Indexed by DatumTypes.
		/// </summary>
		inline static const CreateDefaultFunction _createFunctions[static_cast<int>(DatumType::Unknown)] =
		{
			&Datum::CreateFloat,
			&Datum::CreateInteger,
			&Datum::CreateMatrix,
			&Datum::CreatePointer,
			&Datum::CreateString,
			&Datum::CreateVector,
			&Datum::CreateTable
		};
#pragma endregion

#pragma region Memmove Function Table

		/// <summary>
		/// Memmoves data shifting out an index - templated for the correct typing.
		/// Called upon when removing specific indeces from the data array.
		/// </summary>
		/// <param name="index">Index that was deleted.</param>
		void MoveFloatData(size_t index);

		/// <summary>
		/// Memmoves data shifting out an index - templated for the correct typing.
		/// Called upon when removing specific indeces from the data array.
		/// </summary>
		/// <param name="index">Index that was deleted.</param>
		void MoveIntData(size_t index);

		/// <summary>
		/// Memmoves data shifting out an index - templated for the correct typing.
		/// Called upon when removing specific indeces from the data array.
		/// </summary>
		/// <param name="index">Index that was deleted.</param>
		void MoveMatrixData(size_t index);

		/// <summary>
		/// Memmoves data shifting out an index - templated for the correct typing.
		/// Called upon when removing specific indeces from the data array.
		/// </summary>
		/// <param name="index">Index that was deleted.</param>
		void MovePointerData(size_t index);

		/// <summary>
		/// std:copy() the data shifting out an index - templated for the correct typing.
		/// Called upon when removing specific indeces from the data array.
		/// </summary>
		/// <param name="index">Index that was destructed.</param>
		void MoveStringData(size_t index);

		/// <summary>
		/// Memmoves data shifting out an index - templated for the correct typing.
		/// Called upon when removing specific indeces from the data array.
		/// </summary>
		/// <param name="index">Index that was deleted.</param>
		void MoveVectorData(size_t index);

		/// <summary>
		/// Memmoves data shifting out an index - templated for the correct typing.
		/// Called upon when removing specific indeces from the data array.
		/// </summary>
		/// <param name="index">Index that was deleted.</param>
		void MoveTableData(size_t index);

		using MemMoveDefaultFunction = void(Datum::*)(size_t index);

		/// <summary>
		/// Array of function pointers that point to Memmoves. Indexed by DatumTypes.
		/// </summary>
		inline static const MemMoveDefaultFunction _memMoveFunctions[static_cast<int>(DatumType::Unknown)] =
		{
			&Datum::MoveFloatData,
			&Datum::MoveIntData,
			&Datum::MoveMatrixData,
			&Datum::MovePointerData,
			&Datum::MoveStringData,
			&Datum::MoveVectorData,
			&Datum::MoveTableData
		};
#pragma endregion

#pragma region Equality Comparison Function Table

		/// <summary>
		/// Looks at Data array through typed lens in order to use the correct == operator or Equals Method (for RTTI*).
		/// </summary>
		/// <param name="other">Other datum to compare against.</param>
		/// <param name="index">Index for the comparison.</param>
		/// <returns>True if the elements at index are the same, otherwise false.</returns>
		bool CompareFloat(const Datum& other, size_t index) const;

		/// <summary>
		/// Looks at Data array through typed lens in order to use the correct == operator or Equals Method (for RTTI*).
		/// </summary>
		/// <param name="other">Other datum to compare against.</param>
		/// <param name="index">Index for the comparison.</param>
		/// <returns>True if the elements at index are the same, otherwise false.</returns>
		bool CompareInt(const Datum& other, size_t index) const;

		/// <summary>
		/// Looks at Data array through typed lens in order to use the correct == operator or Equals Method (for RTTI*).
		/// </summary>
		/// <param name="other">Other datum to compare against.</param>
		/// <param name="index">Index for the comparison.</param>
		/// <returns>True if the elements at index are the same, otherwise false.</returns>
		bool CompareMatrix(const Datum& other, size_t index) const;

		/// <summary>
		/// Looks at Data array through typed lens in order to use the correct == operator or Equals Method (for RTTI*).
		/// </summary>
		/// <param name="other">Other datum to compare against.</param>
		/// <param name="index">Index for the comparison.</param>
		/// <returns>True if the elements at index are the same, otherwise false.</returns>
		bool ComparePointer(const Datum& other, size_t index) const;

		/// <summary>
		/// Looks at Data array through typed lens in order to use the correct == operator or Equals Method (for RTTI*).
		/// </summary>
		/// <param name="other">Other datum to compare against.</param>
		/// <param name="index">Index for the comparison.</param>
		/// <returns>True if the elements at index are the same, otherwise false.</returns>
		bool CompareString(const Datum& other, size_t index) const;

		/// <summary>
		/// Looks at Data array through typed lens in order to use the correct == operator or Equals Method (for RTTI*).
		/// </summary>
		/// <param name="other">Other datum to compare against.</param>
		/// <param name="index">Index for the comparison.</param>
		/// <returns>True if the elements at index are the same, otherwise false.</returns>
		bool CompareVector(const Datum& other, size_t index) const;

		/// <summary>
		/// Looks at Data array through typed lens in order to use the correct == operator or Equals Method (for Scope*).
		/// </summary>
		/// <param name="other">Other datum to compare against.</param>
		/// <param name="index">Index for the comparison.</param>
		/// <returns>True if the elements at index are the same, otherwise false.</returns>
		bool CompareTable(const Datum& other, size_t index) const;

		using ComparisonDefaultFunction = bool(Datum::*)(const Datum& other, size_t index) const;

		/// <summary>
		/// Array of function pointers that point to proper comparisons for the datum types.
		/// Indexed by DatumType.
		/// </summary>
		inline static const ComparisonDefaultFunction _comparisonFunctions[static_cast<int>(DatumType::Unknown)] =
		{
			&Datum::CompareFloat,
			&Datum::CompareInt,
			&Datum::CompareMatrix,
			&Datum::ComparePointer,
			&Datum::CompareString,
			&Datum::CompareVector,
			&Datum::CompareTable
		};
#pragma endregion

#pragma region Copy Function Table

		/// <summary>
		/// Placement news a copy of the passed in data value array at the passed in index into *this's value array at the same location.
		/// </summary>
		/// <param name="index">What index is being copied</param>
		/// <param name="data">The copy source array.</param>
		void CreateFloatCopy(const DatumValues& data, size_t size);

		/// <summary>
		/// Placement news a copy of the passed in data value array at the passed in index into *this's value array at the same location.
		/// </summary>
		/// <param name="index">What index is being copied</param>
		/// <param name="data">The copy source array.</param>
		void CreateIntegerCopy(const DatumValues& data, size_t size);

		/// <summary>
		/// Placement news a copy of the passed in data value array at the passed in index into *this's value array at the same location.
		/// </summary>
		/// <param name="index">What index is being copied</param>
		/// <param name="data">The copy source array.</param>
		void CreateMatrixCopy(const DatumValues& data, size_t size);

		/// <summary>
		/// Placement news a copy of the passed in data value array at the passed in index into *this's value array at the same location.
		/// </summary>
		/// <param name="index">What index is being copied</param>
		/// <param name="data">The copy source array.</param>
		void CreatePointerCopy(const DatumValues& data, size_t size);

		/// <summary>
		/// Placement news a copy of the passed in data value array at the passed in index into *this's value array at the same location.
		/// </summary>
		/// <param name="index">What index is being copied</param>
		/// <param name="data">The copy source array.</param>
		void CreateStringCopy(const DatumValues& data, size_t size);

		/// <summary>
		/// Placement news a copy of the passed in data value array at the passed in index into *this's value array at the same location.
		/// </summary>
		/// <param name="index">What index is being copied</param>
		/// <param name="data">The copy source array.</param>
		void CreateVectorCopy(const DatumValues& data, size_t size);

		/// <summary>
		/// Placement news a copy of the passed in data value array at the passed in index into *this's value array at the same location.
		/// </summary>
		/// <param name="index">What index is being copied</param>
		/// <param name="data">The copy source array.</param>
		void CreateTableCopy(const DatumValues& data, size_t size);

		using CreateCopyFunction = void(Datum::*)(const DatumValues& data, size_t size);

		/// <summary>
		/// Array of function pointers that point to correctly typed placement new calls for copying a datum's value array.
		/// </summary>
		inline static const CreateCopyFunction _copyFunctions[static_cast<int>(DatumType::Unknown)] =
		{
			&Datum::CreateFloatCopy,
			&Datum::CreateIntegerCopy,
			&Datum::CreateMatrixCopy,
			&Datum::CreatePointerCopy,
			&Datum::CreateStringCopy,
			&Datum::CreateVectorCopy,
			&Datum::CreateTableCopy
		};
#pragma endregion

#pragma region Parse String Function Table

		/// <summary>
		/// Parses the given datatype out of the given string string and calls Set(parsedValue, index)
		/// </summary>
		/// <param name="s">String with the value to be parsed</param>
		/// <param name="index">Position in the value array to set the value to</param>
		/// <returns>True if the value is Set</returns>
		bool ParseFloat(string s, size_t index);

		/// <summary>
		/// Parses the given datatype out of the given string string and calls Set(parsedValue, index)
		/// </summary>
		/// <param name="s">String with the value to be parsed</param>
		/// <param name="index">Position in the value array to set the value to</param>
		/// <returns>True if the value is Set</returns>
		bool ParseInt(string s, size_t index);

		/// <summary>
		/// Parses the given datatype out of the given string string and calls Set(parsedValue, index)
		/// </summary>
		/// <param name="s">String with the value to be parsed</param>
		/// <param name="index">Position in the value array to set the value to</param>
		/// <returns>True if the value is Set</returns>
		bool ParseMatrix(string s, size_t index);

		/// <summary>
		/// Parses the given datatype out of the given string string and calls Set(parsedValue, index)
		/// </summary>
		/// <param name="s">String with the value to be parsed</param>
		/// <param name="index">Position in the value array to set the value to</param>
		/// <returns>True if the value is Set</returns>
		bool ParseVector(string s, size_t index);

		using CreateParseFunc = bool(Datum::*)(string s, size_t index);

		/// <summary>
		/// Array of function pointers that point to correctly typed sscanf calls to parse incoming strings for Data values.
		/// </summary>
		inline static const CreateParseFunc _parseFunctions[static_cast<int>(DatumType::Unknown)] =
		{
			&Datum::ParseFloat,
			&Datum::ParseInt,
			&Datum::ParseMatrix,
			nullptr,
			nullptr,
			&Datum::ParseVector,
			nullptr
		};

#pragma endregion
		
		void ParsePushBackFloat(Json::Value object);
		void ParsePushBackInteger(Json::Value object);
		void ParsePushBackMatrix(Json::Value object);
		void ParsePushBackString(Json::Value object);
		void ParsePushBackVector(Json::Value object);

		using PushBackJsonValueFunction = void(Datum::*)(Json::Value object);

		/// <summary>
		/// Array of function pointers to the different placement news used for allocation. Indexed by DatumTypes.
		/// </summary>
		inline static const PushBackJsonValueFunction _pushbackFunctions[static_cast<int>(DatumType::Unknown)] =
		{
			&Datum::ParsePushBackFloat,
			&Datum::ParsePushBackInteger,
			&Datum::ParsePushBackMatrix,
			nullptr,
			&Datum::ParsePushBackString,
			&Datum::ParsePushBackVector,
			nullptr
		};


		void SetJsonFloat(Json::Value object, size_t index);
		void SetJsonInteger(Json::Value object, size_t index);
		void SetJsonMatrix(Json::Value object, size_t index);
		void SetJsonString(Json::Value object, size_t index);
		void SetJsonVector(Json::Value object, size_t index);

		using SetJsonValueFunction = void(Datum::*)(Json::Value object, size_t index);

		inline static const SetJsonValueFunction _setJsonFunctions[static_cast<int>(DatumType::Unknown)] =
		{
			&Datum::SetJsonFloat,
			&Datum::SetJsonInteger,
			&Datum::SetJsonMatrix,
			nullptr,
			&Datum::SetJsonString,
			&Datum::SetJsonVector,
			nullptr
		};


		bool ParsePushBackMatrixString(string s);
		bool ParsePushBackVectorString(string s);

		using CreateParsePushBackFunc = bool(Datum::*)(string s);

		/// <summary>
		/// Array of function pointers that point to correctly typed sscanf calls to parse incoming strings for Data values.
		/// </summary>
		inline static const CreateParsePushBackFunc _parsePushBackFunctions[static_cast<int>(DatumType::Unknown)] =
		{
			nullptr,
			nullptr,
			&Datum::ParsePushBackMatrixString,
			nullptr,
			nullptr,
			&Datum::ParsePushBackVectorString,
			nullptr
		};
	};
}

#include "Datum.inl"
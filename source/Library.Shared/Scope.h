#pragma once
#include "Datum.h"
#include "HashMap.h"
#include "Vector.h"
#include "IFactory.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Scope Class - Used to create tables of String, Datum pairs. Supports nesting Scopes. Inherits from RTTI's public interface.
	/// </summary>
	class Scope : public RTTI
	{

		/// <summary>
		/// Macro for RTTI inheritance.
		/// </summary>
		RTTI_DECLARATIONS(Scope, RTTI)

	public:
		/// <summary>
		/// PairType - Typedef for pair<const string, Datum>, used for HashMap as Tkey, TData
		/// </summary>
		using PairType = pair<const string, Datum>;

		/// <summary>
		/// OrderListType - Typedef for Vector<PairType*>, used for the definition of the order list vector that maintains the order of insertion
		/// for the Hashmap entries.
		/// </summary>
		using OrderListType = Vector<PairType*>;

		/// <summary>
		/// TableIteratorType - Typedef for HashMap<string, Datum>::Iterator, the return type for the iterators returned when inserting in to the hashmap, dereferenced
		/// to provide the PairType address that gets inserted into the order vector (to maintain order of entry).
		/// </summary>
		using TableIteratorType = HashMap<const string, Datum>::Iterator;

#pragma region Scope Rule of 6

		/// <summary>
		/// Constructor - Default Constructor for Scope, takes in an optional capacity that allocates the orderList size.
		/// </summary>
		/// <param name="capacity">Optional size_t that allows you to pre-allocate for entries in the order list.</param>
		explicit Scope(size_t capacity = 0);

		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="other">Scope L value reference to be copied.</param>
		Scope(const Scope& other);

		/// <summary>
		/// Move Constructor
		/// </summary>
		/// <param name="other">Scope R value reference to be moved.</param>
		Scope(Scope&& other) noexcept;

		/// <summary>
		/// Move assignment operator
		/// </summary>
		/// <param name="other">Scope R value reference to be moved.</param>
		/// <returns>Reference to the newly moved Scope</returns>
		Scope& operator=(Scope&& other) noexcept;

		/// <summary>
		/// Copy Assignment Operator
		/// </summary>
		/// <param name="other">Reference to the scope to be copied.</param>
		/// <returns>Reference to the newly copied scope.</returns>
		Scope& operator=(const Scope& other);

		/// <summary>
		/// Destructor
		/// </summary>
		virtual ~Scope();

#pragma endregion

		/// <summary>
		/// Append - Returns a reference to a pre-existing datum or newly created datum within the scope paired with the keystring.
		/// </summary>
		/// <param name="keyString">The String paired with the Datum you are trying to access/instantiate</param>
		/// <returns>Datum Reference paired with the keystring.</returns>
		Datum& Append(const string& keyString);

		/// <summary>
		/// AppendScope - Special case of Append where it is a Scope you are appending. Creates a nested scope and attaches to the Datum associated with
		/// the keystring provided. If there was no existing datum, it creates one of type table and pushes the scope to it.
		/// </summary>
		/// <param name="keyString">The string paired with the Datum you are trying to access/instantiate</param>
		/// <returns>Reference to the newly instantiated nested scope.</returns>
		Scope& AppendScope(const string& keyString);

		/// <summary>
		/// Find - Takes a constant string and returns the address of a Datum associated with the given string in this scope if it exists,
		/// otherwise returns nullptr.
		/// </summary>
		/// <param name="keyString">The string the datum you are looking for is associated with.</param>
		/// <returns>The address of the datum associated with the string if it exists, otherwise nullptr.</returns>
		Datum* Find(const string& keyString);

		/// <summary>
		/// Find - Takes a constant string and returns the address of a Datum associated with the given string in this scope if it exists,
		/// otherwise returns nullptr. Const version
		/// </summary>
		/// <param name="keyString">The string the datum you are looking for is associated with.</param>
		/// <returns>The address of the datum associated with the string if it exists, otherwise nullptr.</returns>
		const Datum* Find(const string& keyString) const;

		/// <summary>
		/// Search - Takes a constant string and a double pointer to a scope. Returns the address of the most closely associated
		/// nested Datum associated with the given string in either it or it's ancestors (up the parent hierarchy).
		/// </summary>
		/// <param name="keyString">The string associated with the datum you are looking for.</param>
		/// <param name="scope">If you know which scope the datum is in, you can pass it in as to not need to scale the hierarchy.</param>
		/// <returns>Address of the Datum most closely associated with the string passed in - nullptr if it doesnt exist.</returns>
		Datum* Search(const string& keyString, Scope*& scope);

		/// <summary>
		/// Search - Takes a constant string and a double pointer to a scope. Returns the address of the most closely associated
		/// nested Datum associated with the given string in either it or it's ancestors (up the parent hierarchy). Const version
		/// </summary>
		/// <param name="keyString">The string associated with the datum you are looking for.</param>
		/// <param name="scope">If you know which scope the datum is in, you can pass it in as to not need to scale the hierarchy.</param>
		/// <returns>Address of the Datum most closely associated with the string passed in - nullptr if it doesnt exist.</returns>
		const Datum* Search(const string& keyString, const Scope*& scope) const;

		/// <summary>
		/// Search - Takes a constant string and a double pointer to a scope. Returns the address of the most closely associated
		/// nested Datum associated with the given string in either it or it's ancestors (up the parent hierarchy).
		/// </summary>
		/// <param name="keyString">The string associated with the datum you are looking for.</param>
		/// <param name="scope">If you know which scope the datum is in, you can pass it in as to not need to scale the hierarchy.</param>
		/// <returns>Address of the Datum most closely associated with the string passed in - nullptr if it doesnt exist.</returns>
		Datum* Search(const string& keyString);

		/// <summary>
		/// Search - Takes a constant string and a double pointer to a scope. Returns the address of the most closely associated
		/// nested Datum associated with the given string in either it or it's ancestors (up the parent hierarchy). Const version
		/// </summary>
		/// <param name="keyString">The string associated with the datum you are looking for.</param>
		/// <param name="scope">If you know which scope the datum is in, you can pass it in as to not need to scale the hierarchy.</param>
		/// <returns>Address of the Datum most closely associated with the string passed in - nullptr if it doesnt exist.</returns>
		const Datum* Search(const string& keyString) const;

		/// <summary>
		/// FindContainedScope - Takes the constant address of a scope and returns the datum pointer and index at which the scope was found.
		/// </summary>
		/// <param name="sp">Pointer to the scope you are looking for.</param>
		/// <param name="index">Outparam - Will be set to the index position within the datum where scope was found.</param>
		/// <returns>Address of the Datum that contains the scope sp, nullptr if not found.</returns>
		Datum* FindContainedScope(const Scope* sp, size_t& index);

		/// <summary>
		/// FindContainedScope - Takes the constant address of a scope and returns the datum pointer and index at which the scope was found. Const version
		/// </summary>
		/// <param name="sp">Pointer to the scope you are looking for.</param>
		/// <param name="index">Outparam - Will be set to the index position within the datum where scope was found.</param>
		/// <returns>Address of the Datum that contains the scope sp, nullptr if not found.</returns>
		const Datum* FindContainedScope(const Scope* sp, size_t& index) const;

		/// <summary>
		/// Adopt - Takes a reference to a scope and string for it to be associated with then inserts it into a datum within *this scope
		/// and sets it's _parent to this.
		/// </summary>
		/// <param name="child">The scope to be adopted.</param>
		/// <param name="name">The string to pair the scope with in the Hashmap.</param>
		/// <exception cref="std::runtime_error">Attempting to Adopt a scope and pair it with a name that is already associated with a Datum - but the datum
		/// is not of DatumType::Table - will throw a runtime error.</exception>
		void Adopt(Scope& child, const string& name);
		
		/// <summary>
		/// GetParent - Returns the address to the parent of the scope.
		/// </summary>
		/// <returns>Address to the Parent of the scope, or nullptr if this is a root scope.</returns>
		Scope* GetParent() const;

		/// <summary>
		/// Operator[] - Operator[] where the argument is a Keystring that you pass in. This just calls Append(). Looks nicer though.
		/// </summary>
		/// <param name="keyString">String to be associated with the datum you are accessing.</param>
		/// <returns>Reference to a datum associated with the string passed in.</returns>
		Datum& operator[](const string& keyString);

		/// <summary>
		/// Operator[] - Operator[] where the argument is a size_t index that you pass in. This dereferences the _orderList vector at index
		/// and returns the reference to the datum in that pair.
		/// </summary>
		/// <param name="index">Which entry within the Scope you wish to access the datum of.</param>
		/// <returns>Reference to the datum member at the passed index.</returns>
		/// <exception cref="runtime_error">Attempting to Access an index >= _size will go out of bounds and throw a runtime error.</exception>
		Datum& operator[](size_t index);

		/// <summary>
		/// Operator== - Compares Two scopes for equality (not just sameness). Recursively walks through nested children.
		/// Compares keyStrings and datums for every pair within the Scope.
		/// </summary>
		/// <param name="other">The scope you are comparing against.</param>
		/// <returns>true if all the values are equivalent, else false.</returns>
		bool operator==(const Scope& other) const;

		/// <summary>
		/// Operator!= - Compares two scopes for not equality. Just calls !operator==().
		/// </summary>
		/// <param name="other">Scope you are comparing against.</param>
		/// <returns>true if at least one value or keystring is different within the two scopes, else false.</returns>
		bool operator!=(const Scope& other) const;

		/// <summary>
		/// Size - Returns the _size member - The number of pairs inserted into the Scope.
		/// </summary>
		/// <returns>The number of pairs that are currently in the scope.</returns>
		size_t Size() const;

		/// <summary>
		/// Clear - Walks the Scope and clears all datums within (and children nested scopes). Also calls Clear on the Hashmap _table.
		/// </summary>
		void Clear();

#pragma region RTTI Overrides

		/// <summary>
		/// Equals method override - Inherited from RTTI. Uses rhs As(Scope), dereferences, and invokes the == operator.
		/// </summary>
		/// <param name="rhs">RTTI pointer to be compared against</param>
		/// <returns>True if the Scopes pointed at by the RTTI pointer and this pointer are equivalant.</returns>
		bool Equals(const RTTI* rhs) const override;

#pragma endregion

	private:

		/// <summary>
		/// Orphan - Ascends to the Scope's parent in the hierarchy, locates it's reference to this, then sets that reference to nullptr to remove any parent dependency.
		/// </summary>
		void Orphan();

		/// <summary>
		/// Pointer to the parent Scope of this scope if this is nested. nullptr if this is a root scope.
		/// </summary>
		Scope* _parent = nullptr;

	protected:

		/// <summary>
		/// Clone - Virtual method for Attributed - The Scope version Allocates a copy of this scope. For Child it's expected to be updated to allocate a copy of "this" child.
		/// </summary>
		/// <returns>Pointer to the copied Scope</returns>
		virtual gsl::owner<Scope*> Clone() const;

		/// <summary>
		/// HashMap of string, Datum pairs that is used as the basis of the table for scope.
		/// </summary>
		HashMap<string, Datum> _table;

		/// <summary>
		/// Vector of PairType addresses that is used to maintain insertion order and provide quick access to the locations of string, Datum pairs.
		/// </summary>
		OrderListType _orderList;
	};

	ConcreteFactory(Scope, Scope)
}

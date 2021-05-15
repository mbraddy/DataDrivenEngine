#pragma once
#include "Scope.h"
#include "TypeManager.h"

namespace FieaGameEngine
{

	/// <summary>
	/// Attributed class - Abstract class meant to provide a means for children to convert their members into string, datum pairs.
	/// </summary>
	class Attributed : public Scope
	{
		/// <summary>
		/// Macro for RTTI declarations.
		/// </summary>
		RTTI_DECLARATIONS(Attributed, Scope)

	public:
#pragma region Attributed::Rule of 6

		/// <summary>
		/// Default Constructor - deleted to prevent instantiation.
		/// </summary>
		Attributed() = delete;

		/// <summary>
		/// Copy Constructor - Invokes the Copy constructor of the parent class Scope then registers a this pointer and updates any external storage.
		/// </summary>
		/// <param name="other">Other Attributed derived class to be copied.</param>
		Attributed(const Attributed& other);

		/// <summary>
		/// Move Constructor - Invokes the Move constructor of the parent class Scope then registers a this pointer and updates any external storage.
		/// </summary>
		/// <param name="other">Other Attributed derived class to be Moved.</param>
		Attributed(Attributed&& other) noexcept;

		/// <summary>
		/// Copy Assignment operator - Checks for sameness then Invokes the Copy constructor of the parent class Scope then registers a this pointer and updates any external storage.
		/// </summary>
		/// <param name="other">Other Attributed derived class to be copied.</param>
		Attributed& operator=(const Attributed& other);

		/// <summary>
		/// Move Assignment operator - Checks for sameness then Invokes the Move constructor of the parent class Scope then registers a this pointer and updates any external storage.
		/// </summary>
		/// <param name="other">Other Attributed derived class to be Moved.</param>
		Attributed& operator=(Attributed&& other) noexcept;

		/// <summary>
		/// Default Destructor - Declared virtual so that it appears in the v-table of derived classes.
		/// </summary>
		virtual ~Attributed() = default;

	protected:

		/// <summary>
		/// Protected Constructor - Expected to be called by any derived class in their constructor, creates a this pointer in the Attributed instance and then populates the
		/// scope with string, datum pairs of prescribed members.
		/// </summary>
		/// <param name="typeID">The typeID instance of the child class that is invoking the attributed constructor</param>
		explicit Attributed(RTTI::IdType typeID);

#pragma endregion

	public:

		/// <summary>
		/// IsAttribute - returns a boolean value indicating if the passed in string is the name of an attribute known to this Attributed class.
		/// </summary>
		/// <param name="name">The name of the attribute you are looking for.</param>
		/// <returns>True if an attribute exists with a matching name, otherwise false.</returns>
		bool IsAttribute(const string& name) const;

		/// <summary>
		/// IsAuxiliaryAttribute - returns a boolean value indicating if the passed in string is the name of an auxiliary attribute known to this Attributed class.
		/// </summary>
		/// <param name="name">The name of the attribute you are looking for.</param>
		/// <returns>True if an attribute exists with a matching name - that is *not* a prescribed attribute, otherwise false.</returns>
		bool IsAuxiliaryAttribute(const string& name) const;

		/// <summary>
		/// IsPrescribedAttribute - returns a boolean value indicating if the passed in string is the name of a prescribed attribute known to this Attributed class.
		/// </summary>
		/// <param name="name">The name of the attribute you are looking for.</param>
		/// <returns>True if an attribute exists with a matching name that is inside of the class's signature array, otherwise false.</returns>
		bool IsPrescribedAttribute(const string& name) const;

		/// <summary>
		/// AppendAuxiliaryAttribute - Appends a datum to this attributed scope that did not exist as a prescribed attribute.
		/// </summary>
		/// <param name="name">The name of the attribute you are attaching to the attributed scope.</param>
		/// <returns>Reference to the un-typed datum that was appended to this attributed scope.</returns>
		Datum& AppendAuxiliaryAttribute(const string& name);

		/// <summary>
		/// Attributes - Returns a vector of signatures of all the attributes that have been entered into the attributed scope.
		/// Note that the offset field of the signatures is used to reference their order of entry for this particular vector - not their actual byte value offset.
		/// </summary>
		/// <returns>A Vector containing a signature for each attribute in the attributed scope.</returns>
		Vector<Signature> Attributes() const;

		/// <summary>
		/// AuxiliaryAttributes - Returns a vector of signatures of all the Auxiliary attributes that have been entered into the attributed scope. (That is - the non-prescribed ones).
		/// Note that the offset field of the signatures is used to reference their order of entry for this particular vector - not their actual byte value offset.
		/// </summary>
		/// <returns>A Vector containing a signature for each auxiliary attribute in the attributed scope.</returns>
		Vector<Signature> AuxiliaryAttributes() const;

		/// <summary>
		/// PrescribedAttributes - Returns a const reference to the vector of signatures of all prescribed attributes that have been entered into the attributed scope.
		/// </summary>
		/// <returns>A reference to the Vector containing a signature for each prescribed attribute in the attributed scope.</returns>
		const Vector<Signature>& PrescribedAttributes() const;

		/// <summary>
		/// Clone Method - Purely virtual to require overload by any class that derives Attributed. This Method is required to be overloaded by Scope
		///  to invoke the copy constructor. It clones *this and returns a pointer to the clone. There is a commented out example implementation in Attributed.cpp.
		/// </summary>
		/// <returns>Pointer to the cloned Attributed derived child.</returns>
		virtual gsl::owner<Attributed*> Clone() const = 0;
		

	private:
		/// <summary>
		/// Populate - Binds all prescribed attributed to datum/scopes and attaches them to the Attributed Scope. Utilized the typeID in order to retrieve the
		/// signature vector from the TypeManager and iterates through it creating the datums and scopes.
		/// </summary>
		/// <param name="typeID">TypeID of the class that is being bound to the attributed scope.</param>
		void Populate(RTTI::IdType typeID);

		/// <summary>
		/// UpdateExternalStorage - Loops through the attributes of the attributed scope and updates the pointers to external data to point to the correct addresses.
		/// </summary>
		/// <param name="typeID">TypeID of the class that owns the signature array for updating.</param>
		void UpdateExternalStorage(RTTI::IdType typeID);
	};
}


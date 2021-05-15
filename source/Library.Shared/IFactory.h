#pragma once
#include "HashMap.h"

namespace FieaGameEngine
{
	/// <summary>
	/// IFactory Class - Interface for the ConcreteFactory macro. Also provides an interface as a manager for Factory Classes.
	/// </summary>
	template <typename T>
	class IFactory
	{
#pragma region Factory Interface

	public:
		/// <summary>
		/// ClassName - Pure Virtual, returns the class name of a child class as a string. Implemented by the ConcreteFactory macro.
		/// </summary>
		/// <returns>String containing the class name of the calling child.</returns>
		virtual const std::string& ClassName() const = 0;
	private:
		/// <summary>
		/// Create - Pure Virtual, Creates an instance of a product in a factory class. Returns a pointer to the newly instantiated object.
		/// </summary>
		/// <returns>Address of the newly instantiated product. The calling scope must also delete the memory when done.</returns>
		virtual gsl::owner<T*> Create() const = 0;

#pragma endregion

#pragma region Factory Manager

	public:
		/// <summary>
		/// Create - Searches the Factory Manager for a ProductFactory that produces classes of a matching std::string name. Creates an instance of that product.
		/// </summary>
		/// <param name="className">String containing the name of the class whose factory you are trying to access.</param>
		/// <returns>Address of the newly instantiated product. The calling scope must also delete the memory when done.</returns>
		static gsl::owner<T*> Create(const std::string& className);

		/// <summary>
		/// Find - Finds and returns the address of a factory that produces products of the className passed into this method.
		/// </summary>
		/// <param name="className">String containing the name of the class whose factory you are trying to retrieve.</param>
		/// <returns>Address to the ProductFactory of the corresponding class that was passed into this method.</returns>
		static const IFactory* Find(const std::string& className);

		/// <summary>
		/// Size - Returns the number of ProductFactories that have been registered into the manager at current time.
		/// </summary>
		/// <returns>size_t corresponding to the number of registered factories at the time of invocation.</returns>
		static size_t Size();

		/// <summary>
		/// Clear - Clears all factories in the factory manager.
		/// </summary>
		static void Clear();

		/// <summary>
		/// IsEmpty - Tells you if there are any factories registered in the manager.
		/// </summary>
		/// <returns>True if the factory manager is empty, else false.</returns>
		static bool IsEmpty();

		/// <summary>
		/// Resize - Resizes the factory manager's hashmap to however many buckets you pass in. Preserves any already existing entries but does rehash them.
		/// </summary>
		static void Resize(const size_t& size);

	protected:
		/// <summary>
		/// Add - Adds the passed in factory to the Factory manager.
		/// </summary>
		/// <param name="factory">The factory to be added to the manager.</param>
		/// <exception cref="std::runtime_error">Attempting to add a factory that is the same time as a factory that already exists within the factory manager will throw
		/// a runtime error.</exception>
		static void Add(const IFactory& factory);

		/// <summary>
		/// Remove - Removes the passed in factory from the factory manager.
		/// </summary>
		/// <param name="factory">Factory to be removed from the manager.</param>
		static void Remove(const IFactory& factory);

	private:
		/// <summary>
		/// _factoryTable - A table of string : factory pairs that stores all currently registered factories.
		/// </summary>
		inline static HashMap<const std::string, const IFactory*> _factoryTable;

#pragma endregion

	};

#pragma region ConcreteFactory Macro

/// <summary>
/// ConcreteFactory - Macro that creates a ProductFactory class for integration with Json Parsing. Registers the factory in the IFactory manager
/// upon construction of a ProductFactory and similarly removes it upon destruction.
/// </summary>
#define ConcreteFactory(Product, AbstractProduct)										\
	class Product##Factory final : FieaGameEngine::IFactory<AbstractProduct>			\
	{																					\
	public:																				\
		Product##Factory()																\
		{																				\
			Add(*this);																	\
		}																				\
																						\
		~Product##Factory()																\
		{																				\
			Remove(*this);																\
		}																				\
																						\
		const std::string& ClassName() const override									\
		{																				\
			return _className;															\
		}																				\
																						\
	private:																			\
		gsl::owner<AbstractProduct*> Create() const override							\
		{																				\
			return new Product();														\
		}																				\
																						\
		const std::string _className = #Product;										\
	};

#pragma endregion

}

#include "IFactory.inl"
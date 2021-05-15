#include "pch.h"
#include "IFactory.h"

namespace FieaGameEngine
{
	template<typename T>
	inline void IFactory<T>::Add(const IFactory<T>& factory)
	{
		auto it = _factoryTable.Insert(std::make_pair(factory.ClassName(), &factory));

		//	If wasInserted == false
		if (it.second == false)
		{
			throw std::runtime_error("Attempting to register a class into the factory table that is already present.");
		}
	}

	template<typename T>
	inline gsl::owner<T*> IFactory<T>::Create(const std::string& className)
	{
		const IFactory* const factory = Find(className);
		return (factory != nullptr ? factory->Create() : nullptr);
	}

	template<typename T>
	inline typename const IFactory<T>* IFactory<T>::Find(const std::string& className)
	{
		auto it = _factoryTable.Find(className);
		return (it != _factoryTable.end() ? it->second : nullptr);
	}

	template<typename T>
	inline void IFactory<T>::Remove(const IFactory<T>& factory)
	{
		_factoryTable.Remove(factory.ClassName());
	}

	template<typename T>
	inline size_t IFactory<T>::Size()
	{
		return _factoryTable.Size();
	}

	template<typename T>
	inline void IFactory<T>::Clear()
	{
		_factoryTable.Clear();
	}

	template<typename T>
	inline bool IFactory<T>::IsEmpty()
	{
		return _factoryTable.IsEmpty();
	}

	template<typename T>
	inline void IFactory<T>::Resize(const size_t& size)
	{
		_factoryTable.Resize(size);
	}

}
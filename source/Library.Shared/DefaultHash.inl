#include "DefaultHash.h"
#include <string>
#include <cstdint>

namespace FieaGameEngine
{
	const size_t HashPrime = 13;

	inline size_t AdditiveHash(const uint8_t* data, size_t length)
	{
		size_t hash = 0;

		for (size_t i = 0; i < length; ++i)
		{
			hash += HashPrime * data[i];
		}

		return hash;
	}

	template<typename TKey>
	inline size_t DefaultHash<TKey>::operator()(const TKey& key) const
	{
		const uint8_t* data = reinterpret_cast<const uint8_t*>(&key);		
		return AdditiveHash(data, sizeof(TKey));
	}

	template<>
	struct DefaultHash<char*>
	{
		inline size_t operator()(const char* key) const
		{
			const uint8_t* data = reinterpret_cast<const uint8_t*>(key);
			return AdditiveHash(data, strlen(key));
		}
	};

	template<>
	struct DefaultHash<const char*>
	{
		inline size_t operator()(const char* key) const
		{
			const uint8_t* data = reinterpret_cast<const uint8_t*>(key);
			return AdditiveHash(data, strlen(key));
		}
	};

	template<>
	struct DefaultHash<char* const>
	{
		inline size_t operator()(const char* key) const
		{
			const uint8_t* data = reinterpret_cast<const uint8_t*>(key);
			return AdditiveHash(data, strlen(key));
		}
	};

	template<>
	struct DefaultHash<const char* const>
	{
		inline size_t operator()(const char* key) const
		{
			const uint8_t* data = reinterpret_cast<const uint8_t*>(key);
			return AdditiveHash(data, strlen(key));
		}
	};

	template<>
	struct DefaultHash<std::string>
	{
		inline size_t operator()(const std::string& key) const
		{
			const uint8_t* data = reinterpret_cast<const uint8_t*>(key.c_str());
			return AdditiveHash(data, key.length());
		}
	};

	template<>
	struct DefaultHash<const std::string>
	{
		inline size_t operator()(const std::string& key) const
		{
			const uint8_t* data = reinterpret_cast<const uint8_t*>(key.c_str());
			return AdditiveHash(data, key.length());
		}
	};
}
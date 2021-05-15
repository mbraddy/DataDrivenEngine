#include "pch.h"
#include "DefaultIncrement.h"

namespace FieaGameEngine
{
	size_t DefaultIncrement::operator()(size_t /*size*/, size_t capacity) const
	{
		return (size_t)((capacity + 1) * 2);
	}
}
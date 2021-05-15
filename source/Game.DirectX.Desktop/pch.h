#pragma once

#include <iostream>
#include <cstdint>

#ifndef UNREFERENCED_LOCAL
#define UNREFERENCED_LOCAL(P) (P)
#endif

_inline std::size_t operator "" _z(unsigned long long int x)
{
	return static_cast<size_t>(x);
} 
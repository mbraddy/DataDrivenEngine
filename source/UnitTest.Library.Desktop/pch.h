#pragma once
//	Standard Library
#include <crtdbg.h>
#include <exception>
#include <stdexcept>
#include <cassert>

//	Microsoft Provided Library
#include <CppUnitTest.h>

#ifndef UNREFERENCED_LOCAL
#define UNREFERENCED_LOCAL(P) (P)
#endif

//inline std::size_t operator "" _z(unsigned long long int x)
//{
//	return static_cast<size_t>(x);
//}
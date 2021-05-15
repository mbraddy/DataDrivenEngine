#pragma once

#include <iostream>
#include <cstdint>
#include <exception>
#include <stdexcept>
#include <initializer_list>
#include <cassert>
#include <string>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <cstddef>
#include <gsl/gsl>
#include <fstream>
#include <istream>
#include <chrono>
#include <memory>

#ifndef UNREFERENCED_LOCAL
#define UNREFERENCED_LOCAL(P) (P)
#endif

_inline std::size_t operator "" _z(unsigned long long int x)
{
	return static_cast<size_t>(x);
}
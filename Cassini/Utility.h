#pragma once
#include "Windows.h"
#include <chrono>
#include <random>
#include <sstream>

namespace utility
{
	std::string GenerateUniqueID();
	// Function to convert std::string to LPWSTR
	LPWSTR ConvertToLPWSTR(const std::string& str);
};


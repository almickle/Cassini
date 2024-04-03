#include "Utility.h"

using namespace std;
using namespace std::chrono;

std::string utility::GenerateUniqueID() {
	// Get the current time as a string
	auto now = system_clock::now();
	auto now_ms = time_point_cast<milliseconds>(now);
	auto epoch = now_ms.time_since_epoch();
	auto value = duration_cast<milliseconds>(epoch);
	stringstream ss;
	ss << value.count();

	// Generate a random number and append it to the string
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(0, 9999);
	ss << dis(gen);

	return ss.str();
}

LPWSTR utility::ConvertToLPWSTR(const std::string& str) {
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], static_cast<int>(str.size()), nullptr, 0);
	wchar_t* result = new wchar_t[size_needed + 1];
	MultiByteToWideChar(CP_UTF8, 0, &str[0], static_cast<int>(str.size()), result, size_needed);
	result[size_needed] = L'\0'; // Null-terminate the string
	return result;
}
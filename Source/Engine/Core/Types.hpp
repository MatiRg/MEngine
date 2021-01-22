#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

using StringVec = std::vector<std::string>;

template<class T>
using StringMap = std::unordered_map<std::string, T>;
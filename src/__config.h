#pragma once

#include <cstring>

#include <string>
#include <vector>

#include <unordered_map>

#include "fmt/format.h"

#include "cereal/access.hpp"
#include "cereal/archives/portable_binary.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/unordered_map.hpp"

namespace xscoresheet {
using std::string, std::vector;
using std::unordered_map;
using namespace std::string_literals;

}
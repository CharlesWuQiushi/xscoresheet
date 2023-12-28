#pragma once

#include "__config.h"
#include "spdlog/spdlog.h"

namespace xscoresheet {

class error : public std::runtime_error {
public:
	error (const string &loc, const string &msg, const string &info = "")
	  : std::runtime_error (fmt::format ("[{}] {} {}", loc, msg, info == "" ? "" : "（" + info + "）")) {}
};

}
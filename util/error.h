#pragma once

#include "__config.h"
#include <exception>
#include "spdlog/spdlog.h"

namespace xscoresheet {

class error : public std::runtime_error {
public:
	error (const string &loc, const string &msg, const string &info = "")
		: std::runtime_error (fmt::format ("[{}] {} {}", loc, msg, info == "" ? "" : "（" + info + "）")) {
		// TODO: logging.
	}
	error (const error &e) = default;
	error (error &&e) = default;
};

}
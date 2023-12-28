#pragma once

#include "__config.h"
#include <exception>
#include "spdlog/spdlog.h"

namespace xscoresheet {

class error : public std::runtime_error {
public:
	explicit error (const string &loc, const string &msg, const string &info = "")
		: std::runtime_error (fmt::format ("[{}] {} {}", loc, msg, info == "" ? "" : "（" + info + "）")) {
		// TODO: logging.
	}
	error (const error&) = default;
	error (error&&) = default;
	error& operator = (const error &) = default;
	error& operator = (error &&) = default;
	~error () noexcept override = default;
};

}
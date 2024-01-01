#pragma once

#include "__config.h"
#include "err.h"
#include "formatting/variables.h"

#include <regex>

namespace xscoresheet::formatting {

class pattern {
public:
	class parse_error : public error {
	public:
		parse_error (const std::string &msg, const std::string &info = "")
			: error ("格式串解析器", "解析格式串时发生错误：" + msg, info) {}
	};
	class match_error : public error {
	public:
		match_error (const std::string &msg, const std::string &info = "")
			: error ("格式串解析器", "将格式串和输入匹配时发生错误：" + msg, info) {}
	};

	using mask_t = variable_mask;
	pattern () = default;
	pattern (const std::string &p) : re (), mask (0), pat (p), t (""), perm ("") {
		const std::string re_int = "([+-]?[0-9]+)",
			re_float = "([+-]?(?:[0-9]+(?:[.][0-9]*)?)|(?:[.][0-9]+))";
		constexpr char esc[] = ".^$*+?()[{\\|-]";
		pat = std::regex_replace (pat, std::regex ("^ +"), "");
		pat = std::regex_replace (pat, std::regex (" +$"), "");
		for (auto it = pat.c_str (); *it; ++it)
			if (*it == '$' && isalpha (it[1])) {
				const char c = *++it;
				if (mask (c))
					throw parse_error (fmt::format ("变量 ${} 出现了多次。", c), pat);
				mask (c) = 1;
				if (isupper (c)) t += re_int;
				else t += re_float;
				perm += c;
			} else {
				if (std::find (std::begin (esc), std::end (esc), *it) != std::end (esc))
					t += "\\";
				t += *it;
			}
		re = std::regex (t);
	}
	pattern (const pattern&) = default;
	pattern (pattern&&) = default;
	pattern& operator = (const pattern&) = default;
	pattern& operator = (pattern&&) = default;

	variables operator () (std::string text) const {
		std::smatch res;
		text = std::regex_replace (text, std::regex ("^\\s+"), "");
		text = std::regex_replace (text, std::regex ("\\s+$"), "");
		if (!std::regex_match (text, res, re))
			throw match_error ("不能匹配。", fmt::format ("格式串 “{}”；文本串 “{}”", pat, text));
		variables vars;
		auto it = std::next (std::begin (res));
		const auto ed = std::end (res);
		for (const char c : perm)
			vars (c) = stod (it++->str ());
		return vars;
	}
	mask_t var_mask () const { return mask; }
	std::string string () const { return pat; }

private:
	std::regex re;
	mask_t mask;
	std::string pat, t, perm;

	friend class cereal::access;
	template <class archive>
	void save (archive &a) const {
		a (mask, pat, t, perm);
	}
	template <class archive>
	void load (archive &a) {
		a (mask, pat, t, perm);
		re.assign (t);
	}
};

}
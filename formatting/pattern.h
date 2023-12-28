#include "__config.h"
#include "err.h"
#include "variables.h"

#include <regex>

namespace xscoresheet::formatting {

class pattern {
public:
	class parse_error : error {
	public:
		parse_error (const string &msg, const string &info = "")
			: error ("格式串解析器", "解析格式串时发生错误：" + msg, info) {}
	};
	class match_error : error {
	public:
		match_error (const string &msg, const string &info = "")
			: error ("格式串解析器", "将格式串和输入匹配时发生错误：" + msg, info) {}
	};

	using mask_t = variable_mask;
	pattern () = default;
	pattern (const string &s) : re (), mask (0), pat (s), perm ("") {
		string t = "";
		const string re_int = "([+-]?[0-9]+)",
			re_float = "([+-]?(?:[0-9]+(?:[.][0-9]*)?)|(?:[.][0-9]+))";
		constexpr char esc[] = ".^$*+?()[{\\|-]";
		for (auto it = s.c_str (); *it; ++it)
			if (*it == '$' && isalpha (it[1])) {
				const char c = *++it;
				if (mask (c))
					throw parse_error ("变量 $"s + c + " 出现了多次。", s);
				mask.set (c);
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

	variables operator () (const string &t) const {
		std::smatch res;
		if (!std::regex_match (t, res, re))
			throw match_error ("不能匹配。", fmt::format ("格式串：{}；文本串：{}", pat, t));
		variables vars;
		auto it = std::next (std::begin (res));
		const auto ed = std::end (res);
		for (const char c : perm)
			vars (c) = stod (it++->str ());
		return vars;
	}
	mask_t var_mask () const { return mask; }

private:
	std::regex re;
	mask_t mask;
	string pat, perm;
};

}
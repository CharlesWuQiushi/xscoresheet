#pragma once

#include "__config.h"
#include "pattern.h"
#include "formula.h"

namespace xscoresheet::formatting {

struct converter {
	pattern p;
	formula f;
	class convert_error : public error {
	public:
		convert_error (const string &msg, const string &info = "")
			: error ("串值转换器", "转换时出错：" + msg, info) {}
	};

	double operator () (const string &text) const {
		const auto s1 = p.var_mask (), s2 = f.var_mask ();
		if ((s1 & s2) != s2)
			throw convert_error ("格式串提供的变量集合不是公式使用的变量的超集。",
				fmt::format ("格式串 “{}”；公式：{}", p.get_pattern (), f.get_formula ()));
		return f (p (text));
	}
};

}
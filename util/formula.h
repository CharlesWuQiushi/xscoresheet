#pragma once

#include "__config.h"
#include "metl/metl.h"

namespace xscoresheet {

class formula {
	static decltype (metl::makeCompiler <double> ()) compiler;
	static const regex re;
	static map <string, pair <int, double>> pool;
	string s;
	metl::exprType <double> expr;

	void erase_occurences () {

	}

public:
	formula () = default;
	formula (const string &s) : s (s) {
		auto it = std::sregex_iterator (s.begin (), s.end (), re);
		const std::sregex_iterator ed;
		while (it != ed) {
			const string var = it->str ();
			auto &[occ, val] = pool[var];
			if (!occ++) compiler.setVariable <double> (var, &val);
		}
		try {

		} catch (const std::exception &err) {

		}
	}

	~formula () {

	}
};
auto formula::compiler = metl::makeCompiler <double, int> ();
const regex formula::re ("\\$\\<[^\\<\\>]*\\>");
map <string, pair <int, double>> formula::pool;

}
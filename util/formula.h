#pragma once

#include "__config.h"
#include "error.h"

namespace xscoresheet {

class formula {
public:
	struct variables : array <double, 52> {
		variables () { fill (NAN); };
		static size_t encode (char c) {
			return c >= 'a' ? c - 'a' : c - 'A' + 26;
		}
		static char decode (size_t k) {
			return k < 26 ? 'a' + k : 'A' + k - 26;
		}
		double& operator () (char c) {
			return (*this)[encode (c)];
		}
		const double& operator () (char c) const {
			return (*this)[encode (c)];
		}
	};
	enum operation {
		ADD,
		SUB,
		MUL,
		DIV,
		POW,
		MAX,
		MIN,
		PUSHI,
		PUSHM,
		MOV,
		NEG,
		NOP
	};
	static constexpr int precedence (operation op) {
		switch (op) {
			case NEG: return 4;
			case POW: return 3;
			case MUL: case DIV: return 2;
			case ADD: case SUB: return 1;
			default: return 0;
		}
	}
	static constexpr int operand_count (operation op) {
		switch (op) {
			case NEG: case PUSHI: case PUSHM: return 1;
			case ADD: case SUB: case MUL: case DIV: case POW: case MOV: return 2;
			default: return 3;
		}
	}
	static constexpr operation encode (char c, bool single = false) {
		switch (c) {
			case '-': return single ? NEG : SUB;
			case '+': return ADD;
			case '*': return MUL;
			case '/': return DIV;
			case '^': return POW;
			default: return NOP;
		}
	}
	class parse_error : public error {
	public:
		parse_error (const string &msg, const string &info = "")
			: error ("公式解析器", msg, info) {}
	};

private:
	using code_t = uint64_t;
	using sequence = vector <code_t>;
	static constexpr size_t STACK_SIZE = 200;

	static double stack[STACK_SIZE];
	static variables vars;
	sequence seq;
	uint64_t var_mask;

	class reader {
		std::string::const_iterator it, ed;
		struct operand {
			bool type;
			char var_name;
			double immediate;
		} num[STACK_SIZE];
		operation op[STACK_SIZE];
		size_t num_c, op_c;
		sequence *s;

		void read_int () {
			
		}
		void read_args () {

		}
		void read_var () {
			char c;
			if (it == ed || !isalpha (c = *it++)
				|| (it != ed && (isalpha (*it) || isdigit (*it))))
				throw parse_error ("`$` 后没有紧跟单个英文字母。");
			
		}
		void read () {
			bool last_is_op = 1;
			while (it != ed) {
				char c = *it++;
				if (c == '$') {
					read_var ();
				} else if (c == '-' && last_is_op) {
					
				} else if () {

				}
			}
		}
	public:
		void read (const string &f, sequence &seq) {
			it = std::begin (f);
			ed = std::end (f);
			num_c = op_c = 0;
			s = &seq; read ();
		}
	};

public:
	formula (const string &f)
		: seq {}, var_mask (0) {
		static reader r;
		r.read (f, seq);
	}

	double operator () (const variables& args) const {
		double *pt = stack;
		auto it = std::begin (seq);
		const auto to_pointer = std::bit_cast <double*, code_t>;
		const auto to_immediate = std::bit_cast <double, code_t>;
		while (it != std::end (seq)) {
			switch ((operation) *it) {
				case ADD: --pt; *pt += pt[1]; break;
				case SUB: --pt; *pt -= pt[1]; break;
				case MUL: --pt; *pt *= pt[1]; break;
				case DIV: --pt; *pt /= pt[1]; break;
				case POW: --pt; *pt = std::pow (*pt, pt[1]); break;
				case MAX: --pt; *pt = std::MAX (*pt, pt[1]); break;
				case MIN: --pt; *pt = std::MIN (*pt, pt[1]); break;
				case PUSHI: *++pt = to_immediate (*++it); break;
				case PUSHM: *++pt = *to_pointer (*++it); break;
				case MOV: *to_pointer (*++it) = *pt; break;
				case NEG: *pt = -*pt; break;
			}
			++it;
		}
		return *pt;
	}
	double calculate (const variables& args) const {
		for (uint64_t s = var_mask; s; s &= s - 1) {
			int k = __builtin_ctzll (s);
			if (std::isnan (args[k])) {
				// TODO: throw;
				throw "one or more parameters unset.";
			}
		}
		return (*this) (args);
	}
};
double formula::stack[];
formula::variables formula::vars;

}
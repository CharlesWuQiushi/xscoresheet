#pragma once

#include "__config.h"
#include "error.h"

namespace xscoresheet {

class formula {
public:
	struct variables : array <double, 52> {
		variables () { fill (NAN); };
		variables (const variables&) = default;
		variables (const variables&&) = default;
		variables& operator = (const variables&) = default;
		variables& operator = (const variables&&) = default;
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

	enum class operation {
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
		using enum operation;
		switch (op) {
			case NEG: return 4;
			case POW: return 3;
			case MUL: case DIV: return 2;
			case ADD: case SUB: return 1;
			default: return 0;
		}
	}
	static constexpr int operand_count (operation op) {
		using enum operation;
		switch (op) {
			case NOP: return 0;
			case NEG: case PUSHI: case PUSHM: return 1;
			case ADD: case SUB: case MUL: case DIV: case POW: case MOV: return 2;
			default: return 3;
		}
	}
	static constexpr operation encode_op (char c, bool single = false) {
		using enum operation;
		if (single) return c == '-' ? NEG : NOP;
		switch (c) {
			case '-': SUB;
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
	using mask_t = uint64_t;
	using sequence = vector <code_t>;
	static constexpr size_t STACK_SIZE = 256;

	static double stack[STACK_SIZE];
	static variables vars;
	sequence seq;
	mask_t mask;

public:
	formula (const string &s) : seq {}, mask (0) {
		static operation op_s[STACK_SIZE];
		size_t num_c = 0, op_c = 0;
		auto it = s.c_str ();

		std::function <void (size_t, size_t, size_t)> read;
		read = [&] (size_t op_c0, size_t num_c0, size_t layer) {
			bool last_is_op = 1;

			auto read_var = [&] () {
				char c = *++it;
				if (!isalpha (c) || (*++it && (isalpha (*it) || isdigit (*it))))
					throw parse_error ("输入了 `$`，但其后没有紧跟单个英文字母。", s);
				mask |= (mask_t) 1 << variables::encode (c);
				seq.push_back ((code_t) operation::PUSHM);
				seq.push_back ((code_t) &vars (c));
				++num_c;
			};
			auto read_num = [&] () {
				if (!last_is_op) throw parse_error ("连续输入了多个数字。", s);
				int len; double a;
				sscanf (it, "%lf%n", &a, &len);
				it += len;
				seq.push_back ((code_t) operation::PUSHI);
				seq.push_back (std::bit_cast <code_t, double> (a));
				last_is_op = 0;
			};
			auto read_op = [&] () {
				operation op = encode_op (*it++, last_is_op), _op;
				const auto p = precedence (op);
				if (op == operation::NOP)
					throw parse_error ("输入了多余的运算符。", s);
				while (op_c > op_c0 && precedence (_op = op_s[op_c - 1]) > p) {
					seq.push_back ((code_t) _op);
					num_c -= operand_count (_op) - 1;
					--op_c;
				}
				if (op_c > op_c0 && precedence (_op) == p) {
					if (_op == operation::NEG) --op_c;
					else {
						seq.push_back ((code_t) op_s[--op_c]), --num_c;
						op_s[op_c++] = op;
					}
				}
				last_is_op = 1;
			};
			auto read_func = [&] () {
				string fun_name = "";
				while (isalpha (*it) || isdigit (*it))
					fun_name += *it++;
				if (fun_name == "max") {
					
				} else if (fun_name == "min") {

				} else if (fun_name == "average") {

				} else {
					throw parse_error ("输入了无效的函数名。");
				}
			};

			while (*it) {
				if (*it == '$') {
					read_var ();
				} else if (precedence (encode_op (*it))) {
					read_op ();
				} else if (isalpha (*it)) {
					read_func ();
				} else if (isdigit (*it) || *it == '.') {
					read_num ();
				} else if (isspace (*it)) {
					++it; continue;
				} else if (*it == ',') {
					if (layer == 0)
						throw parse_error ("在最外层表达式输入了 `,`。", s);
					return ++it, read (op_c, num_c, layer);
				} else if (*it == '(') {
					const auto _num_c = num_c;
					read (op_c, num_c, layer + 1);
					if (num_c != _num_c + 1)
						throw parse_error ("输入了一对内部不是恰好一条表达式的括号。", s);
				} else if (*it == ')') {
					if (layer == 0)
						throw parse_error ("输入了多余的右括号。", s);
					break;
				} else {
					throw parse_error ("输入了非法字符。", s);
				}
			}
		};
	}

	double operator () (const variables& args) const {
		using enum operation;
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
				case MAX: --pt; *pt = std::max (*pt, pt[1]); break;
				case MIN: --pt; *pt = std::min (*pt, pt[1]); break;
				case PUSHI: *++pt = to_immediate (*++it); break;
				case PUSHM: *++pt = *to_pointer (*++it); break;
				case MOV: *to_pointer (*++it) = *pt; break;
				case NEG: *pt = -*pt; break;
			}
			++it;
		}
		return *pt;
	}
	mask_t var_mask () const { return mask; }
};
double formula::stack[];
formula::variables formula::vars;

}
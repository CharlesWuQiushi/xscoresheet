#pragma once

#include "__config.h"
#include "err.h"
#include "formatting/variables.h"

#include <functional>
#include <cmath>

namespace xscoresheet::formatting {

class formula {
	using mask_t = variable_mask;
	using code_t = uint64_t;
	using sequence = std::vector <code_t>;
	static constexpr size_t stack_size = 256;

	enum class instruction : code_t {
		nop = 0,
		add = 1,
		sub = 2,
		mul = 3,
		div = 4,
		pow = 5,
		max = 6,
		min = 7,
		pushi = 8,
		pushv = 9,
		ceil = 10,
		floor = 11,
		round = 12,
		abs = 13,
		neg = 14
	};
	enum class math_operator : code_t {
		plus = (size_t) instruction::add,
		minus = (size_t) instruction::sub,
		multiplies = (size_t) instruction::mul,
		divides = (size_t) instruction::div,
		power = (size_t) instruction::pow,
		negate = (size_t) instruction::neg,
		null = (size_t) instruction::nop
	};
	enum class associativity {
		left = 0,
		right = 1
	};
	enum class arity {
		nullary = 0,
		unary = 1,
		binary = 2
	};
	static size_t get_precedence (math_operator op) {
		using enum math_operator;
		switch (op) {
			case negate: return 4;
			case power: return 3;
			case multiplies: case divides: return 2;
			case plus: case minus: return 1;
			default: return 0;
		}
	}
	static arity get_arity (math_operator op) {
		using enum math_operator;
		using enum arity;
		switch (op) {
			case null: return nullary;
			case negate: return unary;
			default: return binary;
		}
	}
	static int32_t get_delta (instruction op) {
		using enum instruction;
		switch (op) {
			case pushi: case pushv: return 1;
			case nop: case neg: case abs:
			case ceil: case floor: case round: return 0;
			default: return -1;
		}
	}
	static associativity get_associativity (math_operator op) {
		using enum math_operator;
		using enum associativity;
		switch (op) {
			case negate: case power: return right;
			default: return left;
		}
	}
	static math_operator encode_operator (char c, bool single = 0) {
		using enum math_operator;
		if (single) return c == '-' ? negate : null;
		switch (c) {
			case '-': return minus;
			case '+': return plus;
			case '*': return multiplies;
			case '/': return divides;
			case '^': return power;
			default: return null;
		}
	}

	static double stack[stack_size];
	static variables vars;
	sequence seq;
	std::string f_str;
	mask_t mask;

	friend class cereal::access;
	template <class archive>
	void serialize (archive &a) {
		a (seq, f_str, mask);
	}

public:
	class parse_error : public error {
	public:
		parse_error (const std::string &msg, const std::string &info = "")
			: error ("公式解析器", "解析公式时发生错误：" + msg, info) {}
	};

	formula () = default;
	formula (const std::string &s) : mask (0), seq {}, f_str (s) {
		using enum instruction;
		using enum math_operator;
		static math_operator op_s[stack_size];
		size_t num_c = 0, op_c = 0;
		auto it = s.c_str ();

		std::function <void (size_t, size_t, size_t, bool)> read;
		read = [&] (size_t op_c0, size_t num_c0, size_t layer, bool multi_arg) {
			bool last_is_op = 1;
			auto push_instr = [&] (instruction instr) {
				seq.push_back ((code_t) instr);
				num_c += get_delta (instr);
			};
			auto push_imm = [&] (double a) {
				push_instr (pushi);
				seq.push_back (std::bit_cast <code_t, double> (a));
				last_is_op = 0;
			};
			auto push_var = [&] (code_t k) {
				push_instr (pushv);
				seq.push_back (k);
				last_is_op = 0;
			};
			auto pop_op = [&] () {
				push_instr ((instruction) op_s[--op_c]);
			};
			auto has_op = [&] () { return op_c > op_c0; };
			auto append_op = [&] (math_operator op) {
				op_s[op_c++] = op;
				last_is_op = 1;
			};
			auto top_op = [&] () { return op_s[op_c - 1]; };

			auto read_var = [&] () {
				if (!last_is_op) throw parse_error ("连续输入了多个数字或变量。", s);
				char c = *++it;
				if (!isalpha (c) || (*++it && (isalpha (*it) || isdigit (*it))))
					throw parse_error ("输入了 `$`，但其后没有紧跟单个英文字母。", s);
				mask (c) = 1;
				push_var (variables::encode (c));
			};
			auto read_num = [&] () {
				if (!last_is_op) throw parse_error ("连续输入了多个数字或变量。", s);
				size_t len; double a;
				if (!sscanf (it, "%lf%zn", &a, &len))
					throw parse_error ("输入了小数点，但不是有效的浮点数。", s);
				it += len;
				push_imm (a);
			};
			auto read_op = [&] () {
				math_operator op = encode_operator (*it++, last_is_op);
				const auto p = get_precedence (op);
				if (op == null)
					throw parse_error ("输入了多余的运算符。", s);
				while (has_op () && get_precedence (top_op ()) > p)
					pop_op ();
				if (has_op () && get_precedence (top_op ()) == p
					&& get_associativity (op) == associativity::left)
					pop_op ();
				append_op (op);
			};
			auto read_func = [&] () {
				std::string name = "";
				while (isalpha (*it) || isdigit (*it))
					name += *it++;
				while (isspace (*it)) ++it;
				if (*it != '(')
					throw parse_error ("未在函数名后紧跟输入由小括号括起的参数列表。", s);
				++it;
				const auto num_c1 = num_c;
				read (op_c, num_c, layer + 1, 1);
				const auto argc = num_c - num_c1;
				if (argc != 1 && (name == "ceil" || name == "floor"
					|| name == "round" || name == "abs"))
					throw parse_error (name + " 是单变量函数。", s);
				if (name == "ceil") {
					push_instr (ceil);
				} else if (name == "floor") {
					push_instr (floor);
				} else if (name == "abs") {
					push_instr (abs);
				} else if (name == "round") {
					push_instr (round);
				} else if (name == "max") {
					for (size_t i = 1; i < argc; ++i)
						push_instr (max);
				} else if (name == "min") {
					for (size_t i = 1; i < argc; ++i)
						push_instr (min);
				} else if (name == "average") {
					for (size_t i = 1; i < argc; ++i)
						push_instr (add);
					push_imm (argc);
					append_op (divides);
				} else throw parse_error ("输入了无效的函数名。", s);
			};

			while (*it) {
				if (*it == '$') {
					read_var ();
				} else if (encode_operator (*it) != null) {
					read_op ();
				} else if (isalpha (*it)) {
					read_func ();
				} else if (isdigit (*it) || *it == '.') {
					read_num ();
				} else if (isspace (*it)) {
					++it; continue;
				} else if (*it == ',') {
					if (!multi_arg)
						throw parse_error ("只有函数调用的括号内部可存在多个用 `,` 分隔的表达式。", s);
					while (has_op ()) pop_op ();
					return ++it, read (op_c, num_c, layer, multi_arg);
				} else if (*it == '(') {
					++it; read (op_c, num_c, layer + 1, 0);
					last_is_op = 0;
				} else if (*it == ')') {
					if (layer == 0)
						throw parse_error ("输入了未匹配的右括号。", s);
					break;
				} else {
					throw parse_error ("输入了非法字符。", s);
				}
			}
			if (*it == 0) {
				if (layer > 0)
					throw parse_error ("输入了未匹配的左括号。", s);
			} else ++it;
			while (has_op ()) pop_op ();
			if (num_c == num_c0)
				throw parse_error ("子表达式需含有至少一个参数。", s);
		};
		read (0, 0, 0, 0);
	}
	formula (const formula&) = default;
	formula (formula&&) = default;
	formula& operator = (const formula&) = default;
	formula& operator = (formula&&) = default;

	double operator () (const variables& args) const {
		using enum instruction;
		vars = args;
		double *pt = stack;
		*pt = 0;
		auto it = std::begin (seq);
		while (it != std::end (seq)) {
			switch ((instruction) *it) {
				case add: --pt; *pt += pt[1]; break;
				case sub: --pt; *pt -= pt[1]; break;
				case mul: --pt; *pt *= pt[1]; break;
				case div: --pt; *pt /= pt[1]; break;
				case pow: --pt; *pt = std::pow (*pt, pt[1]); break;
				case max: --pt; *pt = std::max (*pt, pt[1]); break;
				case min: --pt; *pt = std::min (*pt, pt[1]); break;
				case pushi: *++pt = std::bit_cast <double, code_t> (*++it); break;
				case pushv: *++pt = vars[*++it]; break;
				case neg: *pt = -*pt; break;
				case abs: *pt = std::abs (*pt); break;
				case ceil: *pt = std::ceil (*pt); break;
				case floor: *pt = std::floor (*pt); break;
				case round: *pt = std::round (*pt); break;
				case nop: break;
			}
			++it;
		}
		return *pt;
	}
	mask_t var_mask () const { return mask; }
	std::string string () const { return f_str; }
};
double formula::stack[];
variables formula::vars;

}
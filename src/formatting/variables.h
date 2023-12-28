#pragma once

#include "__config.h"

#include <array>
#include <bitset>

#include "cereal/types/array.hpp"
#include "cereal/types/bitset.hpp"

namespace xscoresheet::formatting {

struct variables : std::array <double, 52> {
	variables () { fill (NAN); };
	variables (const variables&) = default;
	variables (variables&&) = default;
	variables& operator = (const variables&) = default;
	variables& operator = (variables&&) = default;

	static constexpr size_t encode (char c) {
		return c >= 'a' && c <= 'z' ? c - 'a' : c >= 'A' && c <= 'Z' ? c - 'A' + 26 : 52;
	}
	static constexpr char decode (size_t k) {
		return k < 26 ? 'a' + k : k < 52 ? 'A' + k - 26 : 0;
	}
	double& operator () (char c) {
		return (*this)[encode (c)];
	}
	const double& operator () (char c) const {
		return (*this)[encode (c)];
	}
};

struct variable_mask : std::bitset <52> {
	variable_mask (uint64_t s = 0) : std::bitset <52> (s) {}
	class iterator {
		const variable_mask *p; size_t k;
	public:
		iterator () = default;
		iterator (const variable_mask *p, size_t k) : p (p), k (k) {}
		iterator& operator ++ () {
			k = p->_Find_next (k);
			return *this;
		}
		bool operator == (const iterator &b) {
			return p == b.p && k == b.k;
		}
		bool operator != (const iterator &b) {
			return !(*this == b);
		}
		char operator * () const {
			return variables::decode (k);
		}
	};
	iterator begin () const {
		return iterator (this, _Find_first ());
	}
	iterator end () const {
		return iterator (this, 52);
	};
	auto operator () (char c) {
		return (*this)[variables::encode (c)];
	}
	auto operator () (char c) const {
		return (*this)[variables::encode (c)];
	}
};

}
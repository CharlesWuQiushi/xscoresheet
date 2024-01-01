#pragma once

#include "__config.h"
#include "rubric.h"

#include <unordered_map>

#include "cereal/types/unordered_map.hpp"

namespace xscoresheet {

struct exam {
	std::string name;
	rubric guide;
	std::unordered_map <std::string, std::string> fixed_properties;

	struct item {
		enum class item_type : size_t {
			sport,
			compound,
			manual
		} type;
		std::string name;
		double sons_total, adjusted_total;
		std::vector <item> sons;
		enum class scoring_type : size_t {
			sum,
			sum_k_max,
			sum_k_min
		} scoring;
		size_t scoring_k;

	private:
		template <class archive>
		void serialize (archive &a) {
			a (type, name, sons_total, adjusted_total);
			if (type == item_type::compound) {
				a (sons, scoring);
				if (scoring != scoring_type::sum)
					a (scoring_k);
			}
		}
	};
	item root;

private:
	template <class archive>
	void serialize (archive &a) {
		a (name, guide, fixed_properties, root);
	}
};

}
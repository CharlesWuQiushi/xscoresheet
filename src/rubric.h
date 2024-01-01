#pragma once

#include "__config.h"
#include "formatting/converter.h"

#include <unordered_map>
#include <chrono>

#include "cereal/types/unordered_map.hpp"
#include "cereal/types/chrono.hpp"

namespace xscoresheet {

class rubric {
	using time_point = std::chrono::time_point <std::chrono::system_clock>;

public:
	struct sport {
		std::string name, note, unit;
		double full_score;
		enum struct comparator {
			less_equal,
			greater_equal,
			equal
		} comp;
	private:
		friend class cereal::access;
		template <class archive>
		void serialize (archive &a) {
			a (name, note, unit, full_score, comp);
		}
	};

	struct table {
		std::string sport_name;
		std::unordered_map <std::string, std::string> properties;
		struct entry {
			std::string data;
			double score;
		private:
			friend class cereal::access;
			template <class archive>
			void serialize (archive &a) {
				a (data, score);
			}
		};
		std::vector <entry> entries;
		formatting::converter conv;
	private:
		friend class cereal::access;
		template <class archive>
		void serialize (archive &a) {
			a (sport_name, properties, entries, conv);
		}
	};

	std::string name, note;
	time_point created, modified;
	std::vector <sport> sports;
	std::vector <table> tables;

private:
	friend class cereal::access;
	template <class archive>
	void serialize (archive &a) {
		a (name, note, created, modified, sports, tables);
	}
};

}
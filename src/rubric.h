#pragma once

#include "__config.h"
#include "formatting/converter.h"

#include <chrono>

#include "cereal/types/chrono.hpp"

namespace xscoresheet {

class rubric {
	using time_point = std::chrono::time_point <std::chrono::system_clock>;

public:
	struct sport {
		string name, note, unit;
		double full_score;
		enum struct comparator {
			less_equal,
			greater_equal
		} comp;
	private:
		friend class cereal::access;
		template <class archive>
		void serialize (archive &a) {
			a (name, note, unit, full_score, comp);
		}
	};

	struct table {
		string sport_name;
		unordered_map <string, string> properties;
		struct entry {
			string data;
			double score;
		private:
			friend class cereal::access;
			template <class archive>
			void serialize (archive &a) {
				a (data, score);
			}
		};
		vector <entry> entries;
		formatting::converter conv;
	private:
		friend class cereal::access;
		template <class archive>
		void serialize (archive &a) {
			a (sport_name, properties, entries, conv);
		}
	};

	string name, note;
	time_point created, modified;
	vector <sport> sports;
	vector <table> tables;

private:
	friend class cereal::access;
	template <class archive>
	void serialize (archive &a) {
		a (name, note, created, modified, sports, tables);
	}
};

}
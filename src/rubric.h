#pragma once

#include "__config.h"
#include "formatting/converter.h"

#include <chrono>

namespace xscoresheet {

struct rubric {
	struct sport {
		string name, note, unit;
		double full_score;
		enum struct comparator {
			less_equal,
			greater_equal
		};
	};
	struct table {
		string sport_name;
		unordered_map <string, string> properties;
		struct entry {
			string data;
			double score;
		};
		vector <entry> entries;
		formatting::converter conv;
	};

	string name, note;
	time_t created, modified;
	vector <sport> sports;
	vector <table> tables;
};

}
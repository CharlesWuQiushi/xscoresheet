#include "__config.h"

namespace xscoresheet {
struct rubric {
	struct sport {
		string name, note;
		double full_score;
		enum {
			LESS_EQUAL,
			GREATER_EQUAL,
			MINIMUM,
			MAXIMUM
		} comparator;
	};
	struct guide {
		string sport_name;
		unordered_map <string, string> properties;
		struct d_s_pair {
			string data;
			double score;
		};
		vector <d_s_pair> table;
	};

	string name, note;
	time_t created, modified;
	vector <sport> sports;
	vector <guide> guides;

	rubric () {
		
	}
	rubric& operator = (const rubric &s) = default;
};
}
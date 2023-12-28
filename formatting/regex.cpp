#include <bits/stdc++.h>
using namespace std;

int main () {
	string pat, t;
	cin >> pat;
	regex re (pat);
	while (cin >> t) {
		smatch res;
		if (!regex_match (t, res, re)) {
			cerr << "Not matched!" << endl;
			continue;
		}
		int i = 0;
		for (const auto &sub : res) {
			cerr << i++ << ": " << sub.str () << endl;
		}
	}

	return 0;
}
#include <bits/stdc++.h>
using namespace std;

int main () {
	regex re ("\\$\\<[^\\<\\>\\$]*\\>");
	string s;
	getline (cin, s);
	for (const char c : s)
		fprintf (stderr, "%2x ", int (c));
	cerr << endl;
	for (auto it = sregex_iterator (begin (s), end (s), re); it != sregex_iterator (); ++it) {
		auto t = it->str ();
		cerr << t << " at " << it->position () << endl;
	}

	return 0;
}
#include "formula.h"
#include <bits/stdc++.h>
using namespace xscoresheet;
using namespace std;

int main () {
	string s;
	getline (cin, s);
	formula f;
	try {
		f = formula (s);
	} catch (const formula::parse_error &e) {
		cerr << e.what () << endl;
		return 1;
	}
	while (true) {
		formula::variables args;
		for (size_t msk = f.var_mask (); msk; msk &= msk - 1) {
			const int k = __builtin_ctzll (msk);
			cerr << "$" << formula::variables::decode (k) << ": ";
			cin >> args[k];
		}
		cerr << f (args) << endl;
	}

	return 0;
}
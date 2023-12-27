#include "formula.h"
#include <bits/stdc++.h>
using namespace xscoresheet;
using namespace std;

int main () {
	formula f ("");
	double y, z;
	cin >> y >> z;
	f.sequence = {
		formula::pushi,
		bit_cast <uint64_t, double> (20.6232),
		formula::pushm,
		bit_cast <uint64_t, double*> (&y),
		formula::pushm,
		bit_cast <uint64_t, double*> (&z),
		formula::div,
		formula::sub
	};

	auto e = formula::parse_error ("wer", "wegvin");
	freopen ("temp.log", "w", stdout);
	cout << e.what () << endl;
	e = formula::parse_error ("wer");
	cout << e.what () << endl;

	return 0;
}
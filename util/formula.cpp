#include "formula.h"
#include <bits/stdc++.h>
using namespace xscoresheet;
using namespace std;

int main () {
	formula f ("");
	double y, z;
	cin >> y >> z;
	f.seq = {
		formula::PUSHI,
		bit_cast <uint64_t, double> (20.6232),
		formula::PUSHM,
		bit_cast <uint64_t, double*> (&y),
		formula::PUSHM,
		bit_cast <uint64_t, double*> (&z),
		formula::DIV,
		formula::SUB
	};

	auto e = formula::parse_error ("wer", "wegvin");
	freopen ("temp.log", "w", stdout);
	cout << e.what () << endl;
	e = formula::parse_error ("wer");
	cout << e.what () << endl;

	return 0;
}
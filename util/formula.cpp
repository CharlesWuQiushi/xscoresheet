#include <bits/stdc++.h>
#include "metl/metl.h"
using namespace std;



int main () {
	auto compiler = metl::makeCompiler <double, int> ();
	compiler.setCast <int> ([] (const auto &x) { return double (x); });
	compiler.setOperatorPrecedence ("+", 4);
	compiler.setOperator <double, double> ("+", plus <double> ());
	compiler.setOperatorPrecedence ("-", 4);
	compiler.setOperator <double, double> ("-", minus <double> ());
	compiler.setOperatorPrecedence ("*", 3);
	compiler.setOperator <double, double> ("*", multiplies <double> ());
	compiler.setOperatorPrecedence ("/", 3);
	compiler.setOperator <double, double> ("/", divides <double> ());
	compiler.setOperatorPrecedence ("^", 2);
	compiler.setOperator <double, double, double(*) (double, double)> ("^", pow);
	compiler.setUnaryOperatorPrecedence ("-", 1);
	compiler.setUnaryOperator <double> ("-", negate <double> ());
	compiler.setFunction <double, double> ("max", [] (double x, double y) { return max (x, y); });
	compiler.setFunction <double, double> ("min", [] (double x, double y) { return min (x, y); });
	compiler.setFunction <double> ("abs", (double(*) (double)) abs);

	auto e = compiler.build <double> ("124512*1242");
	auto f = e;
	auto g = f;
	cout << g () << endl;
	f = compiler.build <double> ("15123/1235");
	cout << f () << " " << g () << endl;

	return 0;
}
#include "rubric.h"
#include <bits/stdc++.h>
using namespace xscoresheet;

int main () {
	std::ifstream fin ("test.xsrubric");
	cereal::PortableBinaryInputArchive ar (fin);

	rubric g;
	ar (g);
	std::cout << g.tables[1].conv ("3'20");

	return 0;
}
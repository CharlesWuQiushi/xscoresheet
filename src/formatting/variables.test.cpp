#include "variables.h"
#include <iostream>
using namespace xscoresheet;

int main () {
	formatting::variables vars;
	formatting::variable_mask s;
	char c; double x;
	while (true) {
		std::cin >> c >> x;
		vars (c) = x;
		s (c) = 1;
		std::cin >> c;
		std::cout << vars (c) << " " << s (c) << std::endl;
	}


	return 0;
}
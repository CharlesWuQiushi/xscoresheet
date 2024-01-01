#include "formatting/converter.h"
#include <bits/stdc++.h>

int main () {
	std::string f, p, t;
	while (true) {
		try {
			std::getline (std::cin, p);
			std::getline (std::cin, f);
			xscoresheet::formatting::converter g (p, f);
			while (std::getline (std::cin, t)) {
				std::cout << std::fixed << std::setprecision (8) << g (t) << std::endl;
			}
		} catch (const std::runtime_error &e) {
			std::cerr << e.what () << std::endl;
		}
	}

	return 0;
}
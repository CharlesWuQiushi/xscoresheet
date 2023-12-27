#include <cstdio>

int main () {
	char s[200] = "";
	for (int i = 0, c = getchar (); c != '\n'; s[i++] = c, c = getchar ());
	double a; int dt;
	int x = sscanf (s, "%lf%n", &a, &dt);
	printf ("%d %lf\n", dt, a);

	return 0;
}
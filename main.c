#include <stdio.h>

#include "assemble.h"


int main(int argc, char** argv) {
	char test[] = "label: draw V1, V2, 1";
	Symbol o = parse_line(test);
	printf("O %s\n",o.label);
	printf("args %d %d %d\n", o.args[0], o.args[1], o.args[2]);

	return 0;
}

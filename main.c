#include <stdio.h>
#include <stdlib.h>

#include "assemble.h"

uint16_t* assemble_file(char* filename, int *num_syms) {
	FILE *fp;
	char *line;
	size_t len;
	ssize_t read;
	struct SymbolTable symbols;
	Symbol *tmp;

	uint16_t *out;

	line = NULL;
	len = 0;
	read = 0;
	symbols.alloc = 32;
	symbols.tbl = malloc(sizeof(*symbols.tbl) * symbols.alloc);
	symbols.len = 0;


	fp = fopen(filename, "r");
	if (fp == NULL) exit(-1);
	while ((read = getline(&line, &len, fp)) != -1) {
		if (line[0] == ';' || line[0] == '\n') continue;
		symbols.tbl[symbols.len] = parse_line(line);
		if (symbols.tbl[symbols.len].opc.text[0] == '\13') {
			printf("Unrecognized Instruction on line %d\n", symbols.len);
			continue;
		}
		++symbols.len;
		if (symbols.len == symbols.alloc) {
			symbols.alloc *= 2;
			tmp = realloc(symbols.tbl, symbols.alloc * sizeof(*symbols.tbl));
			if (tmp) {
				symbols.tbl = tmp;
			} else {
				printf("realloc fail\n");
				exit(-1);
			}
		}	
	}
	fclose(fp);
	free(line);

	if (num_syms)
		*num_syms = symbols.len;

	out = assemble(symbols);
	free(symbols.tbl);
	return out;
}


int main(int argc, char** argv) {
	int len, i;
	uint16_t *data;

	if (argc > 1) {
		data = assemble_file(argv[1], &len);
		for (i=0; i<len; ++i) {
			printf("%04x ",data[i]);
			if (i % 16 == 15) {
				printf("\n");
			}
		}
		printf("\n");
	}
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "assemble.h"

int htoi_c(char ch) {
	if (ch >= '0' && ch <= '9') {
		return ch - '0';
	} else if (ch >= 'a' && ch <= 'f') {
		return ch - 'a' + 10;
	} else if (ch >= 'A' && ch <= 'F') {
		return ch - 'A' + 10;
	} else {
		return 0;
	}
}

int isgap(char c) {
	return c == ' ' || c == '\t';
}
int min(int a, int b) {
	if (a < b)
		return a;
	return b;
}

int match_register(char *start, int *index) {
	while (isgap(start[*index])) {
		++ *index;
	}
	if (start[*index] != 'V' && start[*index] != 'v') {
		exit(-1);
	}
	*index += 2;
	return htoi_c(start[*index - 1]);
}

int skip_gap(char *line, int index) {
	while(isgap(line[index])) {
		++index;
	}
	return index;
}

substr next(char *line, int idx) {
	substr ret;
	idx = skip_gap(line, idx);
	ret.start = line + idx;
	while (isalnum(line[idx]) || line[idx] == '_') {
		++idx;
	}
	ret.len = line + idx - ret.start;
	return ret;
}

int match_comma(char *line, int idx) {
	substr t;
	idx = skip_gap(line, idx);
	if (line[idx] == ',') {
		return idx + 1;
	}
	printf("total failure\n");
	exit(-1);
	return -1;
}

int match_label(Symbol *sym, char *line, int idx) {
	int s_idx;
	s_idx = skip_gap(line, idx);	
	idx = s_idx;
	while (!isgap(line[idx]) && line[idx] != '\0') {
		++idx;
	}
	--idx;
	if (line[idx] == ':') {
		memcpy(sym->label, line+s_idx, min(15, idx-s_idx));
		sym->label[min(15, idx-s_idx)+0] = '\0';
		++idx;
	} else {
		idx = s_idx;
	}
	return idx;
}

int hash(char *text, int len) {
	int ret = 0;
	int i;
	for (i=0; i<len; ++i) {
		ret = text[i] + (ret  << 6) + (ret << 16) - ret;
	}
	return ret;
}


int parse_opcode(Symbol *sym, char *line, int s_idx) {
	int i, curarg;
	int arg_i;
	char *endptr;
	substr token, argtok;

	arg_i = 0;
	token = next(line, s_idx);
	s_idx += token.len;

	while (arg_i < MAX_ARGS && sym->opc.args[arg_i] != NA) {
		token = next(line, s_idx);
		if (sym->opc.args[arg_i] == REG) {
			if (token.start[0] == 'V' || token.start[0] == 'v') {
				if (token.len == 2) {
					sym->args[arg_i] = htoi_c(token.start[1]);
				} else {
					return 0;
				}
			} else {
				return 0;
			}
		} else {
			if (token.start[0] == 'V' || token.start[0] == 'v') {
				return 0;
			} else {
				sym->args[arg_i] = strtol(token.start, &endptr, 0);
				if (token.start == endptr) {
					argtok = next(token.start, 0);
					sym->args[arg_i] = hash(argtok.start, argtok.len);
				}
			}
		}
			
		++arg_i;
		if (arg_i < MAX_ARGS && sym->opc.args[arg_i] != NA) { 
			s_idx += token.len + 1;
			s_idx = match_comma(line, s_idx);
			if (s_idx == -1) {
				printf("horrible result!\n");
				return 0;
			}
		}
	}
	return 1;
}

Symbol parse_line(char *line) {
	Symbol sym;
	int idx, s_idx;
	int op_idx, arg_idx;
	substr token;
	idx = 0;
	idx = match_label(&sym, line, idx);
	
	s_idx = idx = skip_gap(line, idx);
	token = next(line, idx);
	idx = s_idx;
	for (op_idx=0; op_idx<num_ops; ++op_idx) {
		if (strncmp(oplist[op_idx].text,token.start,token.len) == 0) {
			sym.opc = oplist[op_idx];
			arg_idx = 0;
			if (parse_opcode(&sym, line, idx)) {
				return sym;
			}
		}
	}
	sym.opc = oplist[0];
	return sym;
}

uint16_t assemble_symbol(Symbol sym, struct SymbolTable tbl) {
	int i, arg_i, li, hash_i, value;
	uint16_t ret;
	ret = 0;
	arg_i = 0;
	if (sym.opc.text[0] == '\13') {
		return 0;
	}
	for (i=0; i<4; ++i) {
		if (sym.opc.code[i] == '\255') {
			ret += sym.args[arg_i];
			if (i != 3)
				ret <<= 4;
			++arg_i;
		} else if (sym.opc.code[i] == 0) {
			break;
		} else if (sym.opc.code[i] <= '\3') {
			value = 0;
			if (sym.args[arg_i] > (1 << 12)) {
				value = 255;
				for (hash_i=0; hash_i<tbl.len; ++hash_i) {
					if (hash(tbl.tbl[hash_i].label, strlen(tbl.tbl[hash_i].label)) == sym.args[arg_i]) {
						value = hash_i * 2;
						break;
					}
				}
			} else {
				value = sym.args[arg_i];
			}

			for (li=sym.opc.code[i]-1; li>0; --li) {
				ret += (value >> (4 * li)) & 0xF;
				ret <<= 4;
			}
			ret += value & 0xF;
		
		} else {
			ret += htoi_c(sym.opc.code[i]);
			if (i != 3)
				ret <<= 4;
		}
	}
	return ret;
}

uint16_t* assemble(struct SymbolTable tbl) {
	uint16_t *out;
	int i, len;
	out = malloc(sizeof(*out) * tbl.len);
	for (i=0; i<tbl.len; ++i) {
		out[i] = assemble_symbol(tbl.tbl[i], tbl);
	}
	return out;
}


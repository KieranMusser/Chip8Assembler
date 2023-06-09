#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "assemble.h"

void pss(substr s) {
	printf("SUBSTR %.*s\n", s.len, s.start);
}

int htoi_c(char ch) {
	if (ch >= '0' && ch <= '9') {
		return ch - '0';
	} else if (ch >= 'a' && ch <= 'f') {
		return ch - 'a';
	} else if (ch >= 'A' && ch <= 'F') {
		return ch - 'A';
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
		sym->label[min(15, idx-s_idx)+1] = '\0';
	} else {
		idx = s_idx;
	}
	++idx;
	return idx;
}




int parse_opcode(Symbol *sym, char *line, int s_idx) {
	int i, curarg;
	int arg_i;
	substr token;

	arg_i = 0;
	printf("idk = %s\n", sym->opc.text);
	token = next(line, s_idx);
	s_idx += token.len;

	while (arg_i < MAX_ARGS && sym->opc.args[arg_i] != NA) {
		token = next(line, s_idx);
		pss(token);
		if (sym->opc.args[arg_i] == REG) {
			if (token.start[0] == 'V' || token.start[0] == 'v') {
				if (token.len == 2) {
					sym->args[arg_i] = htoi_c(token.start[1]);
				} else {
					printf("bad reg\n");
					return 0;
				}
			} else {
				printf("expected reg\n");
				return 0;
			}
		} else {
			if (token.start[0] == 'V' || token.start[0] == 'v') {
				printf("got reg not good\n");
				return 0;
			} else {
				sym->args[arg_i] = strtol(token.start, NULL, 0);
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

	curarg = 0;
	i = 0;
	s_idx += token.len;
	token = next(line, s_idx);
	for (i=0; i<MAX_ARGS; ++i) {
		switch (sym->opc.args[i]) {
		case NA:
			goto done;
		case REG:
			break;
		default:
			break;
		}
	}
done: return 0;
	/*while (i < 5) {
		printf("idx %d\n",s_idx);
		token = next(line, s_idx);
		printf("substr [%d] [%d] %.*s\n", line[s_idx], token.len, token.len, token.start);
		s_idx += token.len;
		++curarg;
		printf("idx %d %c\n",s_idx,line[s_idx]);
		while (line[s_idx] != ',' && line[s_idx] != '\0' && line[s_idx] != '\n') {
			++s_idx;
		}
		printf("done [%c] %d\n",line[s_idx], line[s_idx]);
		if (line[s_idx] != ',') {
			return;
		}
		if (curarg == sym->opc.args) {
			printf("Too many args\n");
			return;
		}
		++s_idx;
		++i;
	}*/
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
	pss(token);
	idx = s_idx;
	for (op_idx=0; op_idx<num_ops; ++op_idx) {
		if (strncmp(oplist[op_idx].text,token.start,token.len) == 0) {
			sym.opc = oplist[op_idx];
			arg_idx = 0;
			if (parse_opcode(&sym, line, idx)) {
				return sym;
			} else {
				printf("fail\n");
			}
		}
	}
	printf("overall failure\n");
	return sym;

	while (!isgap(line[idx]) && line[idx] != '\0' && line[idx] != ',') {
		++idx;
	}
	printf("cur %d [%c]\n",idx,line[idx]);
	printf("label %s\n",sym.label);
	printf("instr %.*s\n",idx-s_idx,line+s_idx);


	return sym;
}








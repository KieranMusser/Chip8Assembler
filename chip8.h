#include <stdint.h>

static int MAX_ARGS = 3;
enum ARGTYPE {
	NA = -1,
	REG
};

typedef struct {
	char text[5];
	char code[5];
	enum ARGTYPE args[3];
} instruction;


/* 1 to 1 translations */
static instruction oplist[] = {
	{"clr","00E0",{NA}},
	{"ret","00EE",{NA}},
	{"jmp","1\3",{3,NA}},
	{"draw","D\255\255\1",{REG,REG,1}},
};
static int num_ops = sizeof(oplist) / sizeof(*oplist);




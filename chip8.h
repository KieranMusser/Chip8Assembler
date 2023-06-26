#include <stdint.h>

static int MAX_ARGS = 3;
enum ARGTYPE {
	NA = -1,
	REG
};

typedef struct {
	char text[9];
	unsigned char code[5];
	enum ARGTYPE args[3];
} instruction;

static instruction oplist[] = {
	{"\13", "", {NA}},
	{"call","2\3",{3,NA}},
	{"clr","00E0",{NA}},
	{"ret","00EE",{NA}},
	{"jmp","1\3",{3,NA}},
	{"jmp0","B\3",{3,NA}},
	{"draw","D\255\255\1",{REG,REG,1}},
	{"sprite","F\25529", {REG,NA}},
	{"add", "7\255\2",{REG,2,NA}},
	{"add", "8\255\2554",{REG,REG,NA}},
	{"sub", "8\255\2555", {REG,REG,NA}},
	{"sbn", "8\255\2557", {REG,REG,NA}},
	{"ld","6\255\2",{REG,2,NA}},
	{"ld","8\255\2550",{REG,REG,NA}},
	{"or", "8\255\2551", {REG,REG,NA}},
	{"and", "8\255\2552", {REG,REG,NA}},
	{"xor", "8\255\2553", {REG,REG,NA}},
	{"ifne", "3\255\2", {REG,2,NA}},
	{"ifne", "5\255\2550", {REG,REG,NA}},
	{"ife", "4\255\2", {REG,2,NA}},
	{"ife", "9\255\2550", {REG,REG,NA}},
	{"rand", "C\255\2", {REG,2,NA}},
	{"save", "F\25555", {REG,NA}},
	{"load", "F\25565", {REG,NA}},
	{"sr", "8\255\2556", {REG,REG,NA}},
	{"sl", "8\255\255E", {REG,REG,NA}},
	{"getdelay", "F\25507", {REG,NA}},
	{"key", "F\2550A", {REG,NA}},
	{"ifk", "E\255A1", {REG,NA}},
	{"ifnk", "E\2559E", {REG,NA}},
	{"setdelay", "F\25515", {REG,NA}},
	{"buzz", "F\25518", {REG,NA}},
	{"addi", "F\2551E", {REG,NA}},
	{"seti", "A\3", {3,NA}},
	{"bcd", "F\25533", {REG,NA}},
};
static int num_ops = sizeof(oplist) / sizeof(*oplist);


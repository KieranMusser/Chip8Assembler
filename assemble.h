#include <stdint.h>
#include "chip8.h"


typedef struct {
    char *start;
    int len;
} substr;


typedef struct {
    char label[16];
    instruction opc;
    int args[3];
} Symbol;

struct SymbolTable {
    Symbol *tbl;
    int len;
    int alloc;
};

Symbol parse_line(char *line);
uint16_t* assemble(struct SymbolTable tbl);

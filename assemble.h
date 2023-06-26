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
int hash(char *line, int len); 
uint16_t assemble_symbol(Symbol sym, struct SymbolTable tbl);
uint16_t* assemble(struct SymbolTable tbl);

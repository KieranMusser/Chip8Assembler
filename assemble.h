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

/*
clr
ret
jp NNN
jpo NNN
call NNN

cmp F, IDK
ld VX, IDK
add VX, IDK

simple intructions
sub VX,VY
OR VX,VY
AND VX,VY
xor VX,VY
sr VY,VX
sl VY,VX

rnd VX, NN
drw VX,VY,N
cpk 
bcd

stor
fill

*/

/*
draw v1, v2, 5
{
    draw
    1
    1
    5
}


*/
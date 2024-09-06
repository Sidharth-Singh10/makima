#ifndef DEVILVAL_H
#define DEVILVAL_H

#include <stdio.h>
#include <stdlib.h>

typedef struct devilval {
    int type;
    union {
        long l;
        double dec;
    } data;
    char* err;
    char* sym;
    int count;
    struct devilval** cell;  // Correctly reference the struct using the typedef
} devilval;

enum {
    DEVILVAL_NUM,
    DEVILVAL_DEC,
    DEVILVAL_ERR,
    DEVILVAL_SYM,
    DEVILVAL_SEXPR
};

enum {
    DERR_DIV_ZERO,
    DERR_BAD_OP,
    DERR_BAD_NUM
};

// Function declarations
void devilval_del(devilval* v);
devilval* devilval_num(long x);
devilval* devilval_dec(double x);
devilval* devilval_err(char* m);
devilval* devilval_sym(char* s);
devilval* devilval_sexpr(void);
void devilval_print(devilval* v);
void devilval_println(devilval* v);
void devilval_expr_print(devilval* v, char open, char close);

#endif // DEVILVAL_H

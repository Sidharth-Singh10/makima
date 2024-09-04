#ifndef DEVILVAL_H
#define DEVILVAL_H

#include <stdio.h>

typedef struct
{
    int type;
    union
    {
        long l;
        double dec;
        int err;
    } data;
} devilval;

enum
{
    DEVILVAL_NUM,
    DEVILVAL_DEC,
    DEVILVAL_ERR
};

enum
{
    DERR_DIV_ZERO,
    DERR_BAD_OP,
    DERR_BAD_NUM
};

devilval devilval_num(long x);

devilval devilval_dec(double x);

devilval devilval_err(int x);

void devilval_print(devilval v);

void devilval_println(devilval v);

#endif // DEVILVAL_H

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

// enums under the hood are assinged to integer constant.

// Enumeration of Possible DevilVals Types
enum
{
    DEVILVAL_NUM,
    DEVILVAL_DEC,
    DEVILVAL_ERR
};

//  Create Enumeration of Possible Error Types
enum
{
    DERR_DIV_ZERO,
    DERR_BAD_OP,
    DERR_BAD_NUM
};

/* Create a new number type devilval */
devilval devilval_num(long x)
{
    devilval v;
    v.type = DEVILVAL_NUM;
    v.data.l = x;
    return v;
}

devilval devilval_dec(double x) {
    devilval v;
    v.type = DEVILVAL_DEC;
    v.data.dec = x;
    return v;
}

/* Create a new error type devilval */
devilval devilval_err(int x)
{
    devilval v;
    v.type = DEVILVAL_ERR;
    v.data.err = x;
    return v;
}

void devilval_print(devilval v)
{
    switch (v.type)
    {

    case DEVILVAL_NUM:
        printf("%li", v.data.l);
        break;
    
    case DEVILVAL_DEC:
        printf("%f", v.data.dec);
        break;

    case DEVILVAL_ERR:

        if (v.data.err == DERR_DIV_ZERO)
            printf("Error: Only Gojo can See the INFINITY");

        if (v.data.err == DERR_BAD_OP)
            printf("Error: Pochita doesn't recognize  Operator!");
        
        if (v.data.err == DERR_BAD_NUM)
            printf("Error: Pochita sees a Bad Number!");

        break;
    }
}

void devilval_println(devilval v)
{
    devilval_print(v);
    putchar('\n');
}
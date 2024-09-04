#include <string.h>
#include <stdio.h>
#include "mpc.h"
#include "errors.h"

devilval eval_op(devilval x, char *op, devilval y)
{

    /* If either value is an error return it */
    if (x.type == DEVILVAL_ERR)
    {
        return x;
    }
    if (y.type == DEVILVAL_ERR)
    {
        return y;
    }

    /* Otherwise do maths on the number values */
    if (strcmp(op, "+") == 0)
    {
        printf("I am here \n");

        if(y.type == DEVILVAL_NUM)
        {
            printf("debugging");
        }       
        return x.type == DEVILVAL_DEC || y.type == DEVILVAL_DEC ? devilval_dec((x.type == DEVILVAL_DEC ? x.data.dec : x.data.l) + (y.type == DEVILVAL_DEC ? y.data.dec : y.data.l)) : devilval_num(x.data.l + y.data.l);
    }

    if (strcmp(op, "-") == 0)
        return x.type == DEVILVAL_DEC || y.type == DEVILVAL_DEC ? devilval_dec((x.type == DEVILVAL_DEC ? x.data.dec : x.data.l) - (y.type == DEVILVAL_DEC ? y.data.dec : y.data.l)) : devilval_num(x.data.l - y.data.l);

    if (strcmp(op, "*") == 0)
        return x.type == DEVILVAL_DEC || y.type == DEVILVAL_DEC ? devilval_dec((x.type == DEVILVAL_DEC ? x.data.dec : x.data.l) * (y.type == DEVILVAL_DEC ? y.data.dec : y.data.l)) : devilval_num(x.data.l * y.data.l);

    if (strcmp(op, "%") == 0)
    {
        if (x.type == DEVILVAL_DEC || y.type == DEVILVAL_DEC)
            return devilval_err(DERR_BAD_OP);

        return devilval_num(x.data.l % y.data.l);
    }

    if (strcmp(op, "/") == 0)
    {
        if (y.data.l == 0)
            return devilval_err(DERR_DIV_ZERO);

        return x.type == DEVILVAL_DEC || y.type == DEVILVAL_DEC ? devilval_dec((x.type == DEVILVAL_DEC ? x.data.dec : x.data.l) / (y.type == DEVILVAL_DEC ? y.data.dec : y.data.l)) : devilval_num(x.data.l / y.data.l);
    }
    if (strcmp(op, "^") == 0)
    {

        if (x.type == DEVILVAL_DEC || y.type == DEVILVAL_DEC)
        {
            double result = 1;
            for (int i = 0; i < (int)y.data.dec; i++)
            {
                result *= x.data.dec;
            }
            return devilval_dec(result);
        }
        else
        {
            long result = 1;
            for (int i = 0; i < y.data.l; i++)
            {
                result *= x.data.l;
            }
            return devilval_num(result);
        }
    }

    return devilval_err(DERR_BAD_OP);
}

devilval eval(mpc_ast_t *t)
{
    /* If tagged as number return it directly. */
   if (strstr(t->tag, "number"))
{
    /* Check if the number is a decimal by looking for a decimal point */
    if (strchr(t->contents, '.') != NULL)
    {
        /* It's a decimal, convert to double */
        errno = 0;
        double d = strtod(t->contents, NULL);
        return errno != ERANGE ? devilval_dec(d) : devilval_err(DERR_BAD_NUM);
    }
    else
    {
        /* It's an integer, convert to long */
        errno = 0;
        long x = strtol(t->contents, NULL, 10);
        return errno != ERANGE ? devilval_num(x) : devilval_err(DERR_BAD_NUM);
    }
}

    /* The operator is always second child. */
    char *op = t->children[1]->contents;

    /* We store the third child in `x` */
    devilval x = eval(t->children[2]);

    /* Iterate the remaining children and combining. */
    int i = 3;

    if (strcmp(op, "min") == 0)
    {
        devilval result = x;
        while (strstr(t->children[i]->tag, "expr"))
        {
            devilval next = eval(t->children[i]);
            if (next.data.l < result.data.l)
                result = next;
            i++;
        }
        return result;
    }

    if (strcmp(op, "max") == 0)
    {
        devilval result = x;
        while (strstr(t->children[i]->tag, "expr"))
        {
            devilval next = eval(t->children[i]);
            if (next.data.l > result.data.l)
                result = next;
            i++;
        }
        return result;
    }

    while (strstr(t->children[i]->tag, "expr"))
    {
        x = eval_op(x, op, eval(t->children[i]));
        i++;
    }

    return x;
}

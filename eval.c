#include <string.h>
#include <stdio.h>
#include "mpc.h"
#include "errors.h"
#include "eval.h"

devilval* devilval_pop(devilval* v, int i) {
  /* Find the item at "i" */
  devilval* x = v->cell[i];

  /* Shift memory after the item at "i" over the top */
  memmove(&v->cell[i], &v->cell[i+1],
    sizeof(devilval*) * (v->count-i-1));

  /* Decrease the count of items in the list */
  v->count--;

  /* Reallocate the memory used */
  v->cell = realloc(v->cell, sizeof(devilval*) * v->count);
  return x;
}

devilval* devilval_take(devilval* v, int i) {
  devilval* x = devilval_pop(v, i);
  devilval_del(v);
  return x;
}

devilval* devilval_eval_sexpr(devilval* v) {

  /* Evaluate Children */
  for (int i = 0; i < v->count; i++) {
    v->cell[i] = devilval_eval(v->cell[i]);
  }

  /* Error Checking */
  for (int i = 0; i < v->count; i++) {
    if (v->cell[i]->type == DEVILVAL_ERR) { return devilval_take(v, i); }
  }

  /* Empty Expression */
  if (v->count == 0) { return v; }

  /* Single Expression */
  if (v->count == 1) { return devilval_take(v, 0); }

  /* Ensure First Element is Symbol */
  devilval* f = devilval_pop(v, 0);
  if (f->type != DEVILVAL_SYM) {
    devilval_del(f); devilval_del(v);
    return devilval_err("Pochita Wants S-expression to  Start With Symbol!!!!!");
  }

  /* Call builtin with operator */
  devilval* result = builtin_op(v, f->sym);
  devilval_del(f);
  return result;
}

devilval* devilval_eval(devilval* v) {
  /* Evaluate Sexpressions */
  if (v->type == DEVILVAL_SEXPR) { return devilval_eval_sexpr(v); }
  /* All other devilval types remain the same */
  return v;
}

devilval* builtin_op(devilval* a, char* op) {

  /* Ensure all arguments are numbers */
  for (int i = 0; i < a->count; i++) {
    if (a->cell[i]->type != DEVILVAL_NUM) {
      devilval_del(a);
      return devilval_err("Pochita Sees a  Non-Number!");
    }
  }

  /* Pop the first element */
  devilval* x = devilval_pop(a, 0);

  /* If no arguments and sub then perform unary negation */
  if ((strcmp(op, "-") == 0) && a->count == 0) {
    x->data.l = -x->data.l;
  }

  /* While there are still elements remaining */
  while (a->count > 0) {

    /* Pop the next element */
    devilval* y = devilval_pop(a, 0);

    if (strcmp(op, "+") == 0) { x->data.l += y->data.l; }
    if (strcmp(op, "-") == 0) { x->data.l -= y->data.l; }
    if (strcmp(op, "*") == 0) { x->data.l *= y->data.l; }
    if (strcmp(op, "/") == 0) {
      if (y->data.l == 0) {
        devilval_del(x); devilval_del(y);
        x = devilval_err("Only Gojo Can See the Infinity"); break;
      }
      x->data.l /= y->data.l;
    }

    devilval_del(y);
  }

  devilval_del(a); 
  return x;
}





// devidevilval eval_op(devidevilval x, char *op, devidevilval y)
// {

//     /* If either value is an error return it */
//     if (x.type == DEVIdevilval_ERR)
//     {
//         return x;
//     }
//     if (y.type == DEVIdevilval_ERR)
//     {
//         return y;
//     }

//     /* Otherwise do maths on the number values */
//     if (strcmp(op, "+") == 0)
//     {
//         printf("I am here \n");

//         if(y.type == DEVIdevilval_NUM)
//         {
//             printf("debugging");
//         }       
//         return x.type == DEVIdevilval_DEC || y.type == DEVIdevilval_DEC ? devidevilval_dec((x.type == DEVIdevilval_DEC ? x.data.dec : x.data.l) + (y.type == DEVIdevilval_DEC ? y.data.dec : y.data.l)) : devidevilval_num(x.data.l + y.data.l);
//     }

//     if (strcmp(op, "-") == 0)
//         return x.type == DEVIdevilval_DEC || y.type == DEVIdevilval_DEC ? devidevilval_dec((x.type == DEVIdevilval_DEC ? x.data.dec : x.data.l) - (y.type == DEVIdevilval_DEC ? y.data.dec : y.data.l)) : devidevilval_num(x.data.l - y.data.l);

//     if (strcmp(op, "*") == 0)
//         return x.type == DEVIdevilval_DEC || y.type == DEVIdevilval_DEC ? devidevilval_dec((x.type == DEVIdevilval_DEC ? x.data.dec : x.data.l) * (y.type == DEVIdevilval_DEC ? y.data.dec : y.data.l)) : devidevilval_num(x.data.l * y.data.l);

//     if (strcmp(op, "%") == 0)
//     {
//         if (x.type == DEVIdevilval_DEC || y.type == DEVIdevilval_DEC)
//             return devidevilval_err(DERR_BAD_OP);

//         return devidevilval_num(x.data.l % y.data.l);
//     }

//     if (strcmp(op, "/") == 0)
//     {
//         if (y.data.l == 0)
//             return devidevilval_err(DERR_DIV_ZERO);

//         return x.type == DEVIdevilval_DEC || y.type == DEVIdevilval_DEC ? devidevilval_dec((x.type == DEVIdevilval_DEC ? x.data.dec : x.data.l) / (y.type == DEVIdevilval_DEC ? y.data.dec : y.data.l)) : devidevilval_num(x.data.l / y.data.l);
//     }
//     if (strcmp(op, "^") == 0)
//     {

//         if (x.type == DEVIdevilval_DEC || y.type == DEVIdevilval_DEC)
//         {
//             double result = 1;
//             for (int i = 0; i < (int)y.data.dec; i++)
//             {
//                 result *= x.data.dec;
//             }
//             return devidevilval_dec(result);
//         }
//         else
//         {
//             long result = 1;
//             for (int i = 0; i < y.data.l; i++)
//             {
//                 result *= x.data.l;
//             }
//             return devidevilval_num(result);
//         }
//     }

//     return devidevilval_err(DERR_BAD_OP);
// }

// devidevilval eval(mpc_ast_t *t)
// {
//     /* If tagged as number return it directly. */
//    if (strstr(t->tag, "number"))
// {
//     /* Check if the number is a decimal by looking for a decimal point */
//     if (strchr(t->contents, '.') != NULL)
//     {
//         /* It's a decimal, convert to double */
//         errno = 0;
//         double d = strtod(t->contents, NULL);
//         return errno != ERANGE ? devidevilval_dec(d) : devidevilval_err(DERR_BAD_NUM);
//     }
//     else
//     {
//         /* It's an integer, convert to long */
//         errno = 0;
//         long x = strtol(t->contents, NULL, 10);
//         return errno != ERANGE ? devidevilval_num(x) : devidevilval_err(DERR_BAD_NUM);
//     }
// }

//     /* The operator is always second child. */
//     char *op = t->children[1]->contents;

//     /* We store the third child in `x` */
//     devidevilval x = eval(t->children[2]);

//     /* Iterate the remaining children and combining. */
//     int i = 3;

//     if (strcmp(op, "min") == 0)
//     {
//         devidevilval result = x;
//         while (strstr(t->children[i]->tag, "expr"))
//         {
//             devidevilval next = eval(t->children[i]);
//             if (next.data.l < result.data.l)
//                 result = next;
//             i++;
//         }
//         return result;
//     }

//     if (strcmp(op, "max") == 0)
//     {
//         devidevilval result = x;
//         while (strstr(t->children[i]->tag, "expr"))
//         {
//             devidevilval next = eval(t->children[i]);
//             if (next.data.l > result.data.l)
//                 result = next;
//             i++;
//         }
//         return result;
//     }

//     while (strstr(t->children[i]->tag, "expr"))
//     {
//         x = eval_op(x, op, eval(t->children[i]));
//         i++;
//     }

//     return x;
// }

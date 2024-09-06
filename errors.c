#include "errors.h"
#include <string.h>

devilval* devilval_num(long x) {
    devilval* v = malloc(sizeof(devilval));
    v->type = DEVILVAL_NUM;
    v->data.l = x;
    return v;
}

devilval* devilval_dec(double x) {
    devilval* v = malloc(sizeof(devilval));
    v->type = DEVILVAL_DEC;
    v->data.dec = x;
    return v;
}

devilval* devilval_err(char* m) {
    devilval* v = malloc(sizeof(devilval));
    v->type = DEVILVAL_ERR;
    v->err = malloc(strlen(m) + 1);
    strcpy(v->err, m);
    return v;
}

devilval* devilval_sym(char* s) {
    devilval* v = malloc(sizeof(devilval));
    v->type = DEVILVAL_SYM;
    v->sym = malloc(strlen(s) + 1);
    strcpy(v->sym, s);
    return v;
}

devilval* devilval_sexpr(void) {
    devilval* v = malloc(sizeof(devilval));
    v->type = DEVILVAL_SEXPR;
    v->count = 0;
    v->cell = NULL;
    return v;
}

void devilval_del(devilval* v) {
    switch (v->type) {
        case DEVILVAL_NUM: break;
        case DEVILVAL_DEC: break;
        case DEVILVAL_ERR: free(v->err); break;
        case DEVILVAL_SYM: free(v->sym); break;
        case DEVILVAL_SEXPR:
            for (int i = 0; i < v->count; i++) {
                devilval_del(v->cell[i]);
            }
            free(v->cell);
        break;
    }
    free(v);
}

void devilval_print(devilval* v) {
    switch (v->type) {
        case DEVILVAL_NUM: printf("%li", v->data.l); break;
        case DEVILVAL_DEC: printf("%f", v->data.dec); break;
        case DEVILVAL_ERR: printf("Error: %s", v->err); break;
        case DEVILVAL_SYM: printf("%s", v->sym); break;
        case DEVILVAL_SEXPR: devilval_expr_print(v, '(', ')'); break;
    }
}

void devilval_println(devilval* v) {
    devilval_print(v);
    putchar('\n');
}

void devilval_expr_print(devilval* v, char open, char close) {
    putchar(open);
    for (int i = 0; i < v->count; i++) {
        devilval_print(v->cell[i]);
        if (i != (v->count - 1)) {
            putchar(' ');
        }
    }
    putchar(close);
}

// eval.h
#ifndef EVAL_H  // if not defined
#define EVAL_H

#include "mpc.h"
#include "errors.h"

devilval* devilval_pop(devilval* v, int i);
devilval* devilval_take(devilval* v, int i);
devilval* devilval_eval(devilval* v);
devilval* builtin_op(devilval* a, char* op);
devilval* devilval_eval_sexpr(devilval* v);

#endif

// eval.h
#ifndef EVAL_H  // if not defined
#define EVAL_H

#include "mpc.h"
#include "errors.h"

devilval eval_op(long x, char *op, long y);
devilval eval(mpc_ast_t *t);

#endif

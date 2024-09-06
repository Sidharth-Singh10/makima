#include "mpc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "eval.h"
#include "errors.h"

/* If we are compiling on Windows compile these functions */
#ifdef _WIN32

static char buffer[2048];

/* Fake readline function */
char *readline(char *prompt)
{
  fputs(prompt, stdout);
  fgets(buffer, 2048, stdin);
  size_t len = strlen(buffer);
  char *cpy = malloc(len + 1);
  strcpy(cpy, buffer);
  if (len > 0 && cpy[len - 1] == '\n')
  {
    cpy[len - 1] = '\0'; // Remove newline character if present
  }
  return cpy;
}

/* Fake add_history function */
void add_history(char *unused) {}

#else
#include <editline/readline.h>
#include <histedit.h>
#endif

mpc_parser_t *Number;
mpc_parser_t *Symbol;
mpc_parser_t *Sexpr;
mpc_parser_t *Expr;
mpc_parser_t *maki;

devilval *devilval_read_num(mpc_ast_t *t)
{
  errno = 0;
  long x = strtol(t->contents, NULL, 10);
  return errno != ERANGE ? devilval_num(x) : devilval_err("invalid number");
}

devilval *devilval_add(devilval *v, devilval *x)
{
  v->count++;
  v->cell = realloc(v->cell, sizeof(devilval *) * v->count);
  v->cell[v->count - 1] = x;
  return v;
}

devilval *devilval_read(mpc_ast_t *t)
{

  /* If Symbol or Number return conversion to that type */
  if (strstr(t->tag, "number"))
  {
    return devilval_read_num(t);
  }
  if (strstr(t->tag, "symbol"))
  {
    return devilval_sym(t->contents);
  }

  /* If root (>) or sexpr then create empty list */
  devilval *x = NULL;
  if (strcmp(t->tag, ">") == 0)
  {
    x = devilval_sexpr();
  }
  if (strstr(t->tag, "sexpr"))
  {
    x = devilval_sexpr();
  }

  /* Fill this list with any valid expression contained within */
  for (int i = 0; i < t->children_num; i++)
  {
    if (strcmp(t->children[i]->contents, "(") == 0)
    {
      continue;
    }
    if (strcmp(t->children[i]->contents, ")") == 0)
    {
      continue;
    }
    if (strcmp(t->children[i]->tag, "regex") == 0)
    {
      continue;
    }
    x = devilval_add(x, devilval_read(t->children[i]));
  }

  return x;
}

int main(int argc, char *argv[])
{
  // Create parsers
  Number = mpc_new("number");
  Symbol = mpc_new("symbol");
  Sexpr = mpc_new("sexpr");
  Expr = mpc_new("expr");
  maki = mpc_new("maki");

  // Define them with the following Language
  mpca_lang(MPCA_LANG_DEFAULT,
            "                                                     \
    number   : /-?[0-9]+(\\.[0-9]+)?/ ;                             \
    symbol : '+' | '-' | '*' | '/' | '^' | \"max\" | \"min\";  \
    sexpr  : '(' <expr>* ')' ; \
    expr   : <number> | <symbol> | <sexpr> ; \
    maki    : /^/ <expr>* /$/ ;             \
  ",
            Number, Symbol, Sexpr, Expr, maki);

  puts("makima version 0.0.0.2");
  puts("Press Ctrl + C to exit ");

  while (1)
  {
    char *input = readline("makima> ");
    add_history(input);

    mpc_result_t r;
    if (mpc_parse("<stdin>", input, maki, &r))
    {
      /* On Success Print the AST */
      mpc_ast_print(r.output);

      devilval *x = devilval_read(r.output);
      devilval_println(x);
      devilval_del(x);
    }
    else
    {
      /* Otherwise Print the Error */
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }

    free(input);
  }

  /* Cleanup */
  mpc_cleanup(5, Number, Symbol, Sexpr, Expr, maki);
  return 0;
}

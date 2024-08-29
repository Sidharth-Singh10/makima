#include <editline/readline.h>
#include <histedit.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  puts("makima version 0.0.0.1");
  puts("Press Ctrl + C to exit ");

  while (1) {

    char *input = readline("makima> ");

    add_history(input);

    printf("Your mom %s\n", input);

    free(input);
  }

  return 0;
}

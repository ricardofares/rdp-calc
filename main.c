#include <stdio.h>
#include <stdlib.h>

#include "./lexer/lexer.h"
#include "./parser/parser.h"

extern struct lexer lexer;
extern struct token *curr_token;

int main(int argc, char **argv) {
    --argc, ++argv;

    FILE *input;

    /* It checks if the user is running the */
    /* application without specyfing an input stream */
    if (0 == argc)
        input = stdin;
    else if (!(input = fopen(argv[0], "r"))) {
        printf("RDP-CALC: Input stream %s could not be opened.\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    init_lexer(input);
    parse();

    return 0;
}

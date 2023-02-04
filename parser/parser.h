#ifndef PARSER_H
#define PARSER_H

/**
 * It prints a message to the standard output indicating
 * an error at the parsing phase has occurred.
 * <p>
 * Further, after the message printing the program is
 * exited.
 *
 * @param message the message to be printed out to the
 *                standard output
 */
#define PARSE_ERROR( MESSAGE, ... ) do {                                   \
                                printf("parser: " MESSAGE, ##__VA_ARGS__); \
                                exit(EXIT_FAILURE);                        \
                               } while (0)

/**
 * It prints a message to the standard output indicating
 * a syntax error has been identified.
 * <p>
 * Further, after the message printing the program is
 * exited.
 *
 * @param message the message to be printed out to
 *                the standard output, indicating which
 *                syntax error has occurred
 */
#define SYNTAX_ERROR( MESSAGE, ... ) PARSE_ERROR("A syntax error has been identified. " MESSAGE, ##__VA_ARGS__)

/* Function Declaration */

/**
 * It performs the syntax analysis in the
 * provided input.
 * <p>
 * More generally, this function does parse
 * the following grammar production rule
 *
 *      <S> ::= <expr> $
 *
 * in which <S> is the start symbol from the
 * grammar.
 */
void parse();

#endif // PARSER_H

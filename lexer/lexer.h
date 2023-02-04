#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>

/**
 * It prints a message to the standard output indicating
 * an error at the lexer phase has occurred.
 * <p>
 * Further, after the message printing the program is
 * exited.
 *
 * @param message the message to be printed out to the
 *                standard output
 */
#define LEXER_ERROR( MESSAGE, ... ) do {                                  \
                                printf("lexer: " MESSAGE, ##__VA_ARGS__); \
                                exit(EXIT_FAILURE);                       \
                               } while (0)

/**
 * Lexer constants definition.
 */
#define LEXER_INPUT_BUFLEN (256)

/**
 * Lexer tokens definition.
 */
#define LEXER_TOKEN_EOF              (0x0)
#define LEXER_TOKEN_PLUS             (0x1)
#define LEXER_TOKEN_MINUS            (0x2)
#define LEXER_TOKEN_MULTIPLY         (0x3)
#define LEXER_TOKEN_DIVIDE           (0x4)
#define LEXER_TOKEN_POW              (0x5)
#define LEXER_TOKEN_NUMBER           (0x6)
#define LEXER_TOKEN_LPAREN           (0x7)
#define LEXER_TOKEN_RPAREN           (0x8)
#define LEXER_TOKEN_LBRACKET         (0x9)
#define LEXER_TOKEN_RBRACKET         (0xA)
#define LEXER_TOKEN_PIPE             (0xB)

#define DEFINE_LEXER() struct lexer lexer;
#define DEFINE_CURRENT_TOKEN() struct token *curr_token;

#define TOKEN_TYPE()  (curr_token->type)
#define TOKEN_VALUE() (curr_token->metadata.value)
#define NEXT_TOKEN()  (curr_token = next_token())

/* Structure Definitions */

struct lexer {
    /**
     * It represents the buffer to store the characters
     * that will be analyzed by the lexer.
     */
    char     *buf;

    /**
     * It represents the buffer length.
     */
    unsigned  buflen;

    /**
     * It represents the position of the current character
     * to be analyzed by the lexer.
     * <p>
     * It is recommended not to change this variable directly,
     * only by the functions provided for the lexical analysis.
     */
    unsigned pos;

    /**
     * It represents a cache for the position of the current
     * character to be analyzed by the lexer.
     * <p>
     * It is recomended not to change this variable directly,
     * only by the functions provided for the lexical analysis.
     */
    unsigned mark;
};

struct token {
    /**
     * It stores the token type.
     */
    unsigned type;

    /**
     * It stores the token metadata.
     */
    union {
        double value;
    } metadata;
};

/**
 * It retrieves the next token from the buffer.
 *
 * @return the next token from the buffer
 */
struct token *
next_token();

/**
 * It initializes the lexer loading from the specified
 * stream the characters that will be lexical analyzed.
 *
 * @param stream the stream to be analyzed by the lexer
 */
void
init_lexer(FILE *stream);

#endif // LEXER_H

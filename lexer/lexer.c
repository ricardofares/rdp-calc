#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "./lexer.h"

/* Global Variables */

/**
 * It stores the lexer structure that is used
 * to perform the lexical analysis in the buffer.
 */
DEFINE_LEXER();

/**
 * It stores the current token that has been
 * identified by the lexical analyzer.
 */
DEFINE_CURRENT_TOKEN();

/* Function Declaration */

/**
 * It retrieves the next character to be analyzed in the buffer.
 * <p>
 * Further, note that this function does modify the character position
 * in the buffer.
 *
 * @return the next character to be analyzed in the buffer
 */
static char
next_char();

/**
 * It retrieves the next character to be analyzed from the buffer,
 * without incrementing the character position in the buffer.
 *
 * @return the next character to be analyzed in the buffer without
 *         incrementing the character position
 */
static char
peek_char();

/**
 * It skips `n` characters from the buffer.
 *
 * @param n the amount of characters to be skipped
 *          in the buffer
 */
static void
skip(unsigned n);

/**
 * It marks the current character position in the buffer.
 */
static void
mark();

/**
 * It retracts by one the current character position in the
 * buffer.
 */
static void
retract();

/**
 * It allocates a token structure containing the
 * specified token type.
 * <p>
 * Further, if the token could not be allocated,
 * then the program is exited.
 *
 * @param type the token type
 *
 * @return a token structure containing the
 *         specified token type
 */
static struct token *
make_token(unsigned type);

/**
 * It returns the next number identified from the
 * buffer.
 *
 * @return the next number from the buffer
 */
static struct token *
next_number();

/**
 * It returns a number token containing the specified
 * value.
 *
 * @param value the value
 *
 * @return a number token containing the specified value
 */
static struct token *
make_number(double value);

/* Function Definition */

void
init_lexer(FILE *stream) {
    /* It checks if a stream has been specified */
    if (!stream)
        LEXER_ERROR("A stream must be specified to initialize the lexer.\n");

    lexer.buf    = (char *)calloc(LEXER_INPUT_BUFLEN, sizeof(char));
    lexer.buflen = LEXER_INPUT_BUFLEN;
    lexer.pos    = 0;
    lexer.mark   = 0;

    /* It checks if a buffer could not be allocated for the lexer */
    if (!lexer.buf)
        LEXER_ERROR("A buffer could not be allocated for the lexer.\n");

    /* It reads the contents from the specified stream to the lexer buffer */
    fread(lexer.buf, sizeof(char), LEXER_INPUT_BUFLEN, stream);
}


struct token *
next_token() {
    /* It skips the blank spaces */
    while (peek_char() == ' ') skip(1);

    /* LF files */
    if (peek_char() == '\n') skip(1);

    /* It returns the `EOF` token */
    if (peek_char() == '\0') return make_token(LEXER_TOKEN_EOF);

    /* It peeks the next character to be analyzed */
    const char c = peek_char();

    /* It checks if the peeked character is a digit */
    /* Therefore, it tries to get the next number */
    if (isdigit(c))
        return next_number();

    switch (c) {
        case '+': { skip(1); return make_token(LEXER_TOKEN_PLUS); }
        case '-': { skip(1); return make_token(LEXER_TOKEN_MINUS); }
        case '*': {
            skip(1);

            /* It checks if the input matches the `**` */
            if (peek_char() == '*') { skip(1); return make_token(LEXER_TOKEN_POW); }

            return make_token(LEXER_TOKEN_MULTIPLY);
        }
        case '/': { skip(1); return make_token(LEXER_TOKEN_DIVIDE); }
        case '(': { skip(1); return make_token(LEXER_TOKEN_LPAREN); }
        case ')': { skip(1); return make_token(LEXER_TOKEN_RPAREN); }
        case '[': { skip(1); return make_token(LEXER_TOKEN_LBRACKET); }
        case ']': { skip(1); return make_token(LEXER_TOKEN_RBRACKET); }
        case '|': { skip(1); return make_token(LEXER_TOKEN_PIPE); }
        case 'e': { skip(1); return make_number(M_E); }
        case 'p': {
            skip(1);

            /* It checks if the input matches the `pi` */
            if (peek_char() == 'i') { skip(1); return make_number(M_PI); }

            LEXER_ERROR("Unexpected character (%c). It was to be 'pi'?.\n", peek_char());
        }
        default: LEXER_ERROR("Unexpected character (%c).\n", c);
    }
}

/* Static Function Declaration */

inline static char
next_char() {
    return lexer.buf[lexer.pos++];
}

inline static char
peek_char() {
    return lexer.buf[lexer.pos];
}

inline static void
skip(const unsigned n) {
    lexer.pos += n;
}

inline static void
retract() {
    lexer.pos--;
}

inline static void
mark() {
    lexer.mark = lexer.pos;
}

static struct token *
make_token(unsigned type) {
    struct token *token = (struct token *)malloc(sizeof(struct token));

    /* It checks if a token could not be allocated */
    if (!token)
        LEXER_ERROR("A token structure for type (%u) could not be allocated.\n", type);

    token->type = type;

    return token;
}

static struct token *
next_number() {
    struct token *token;
    char         *number;
    unsigned      numberlen;

    mark();

    /* It is reading the digits */
    while (isdigit(peek_char())) skip(1);

    /* It has been found a `dot` for a floating-point number */
    if (peek_char() == '.') {
        /* It skips the dot */
        skip(1);

        /* It is reading the digits after the dot */
        while (isdigit(peek_char())) skip(1);
    }

    numberlen = lexer.pos - lexer.mark + 1; /* number length */
    number    = (char *)malloc(sizeof(char) * numberlen);
    token     = make_token(LEXER_TOKEN_NUMBER);

    /* It checks if the number buffer could not be allocated */
    if (!number)
        LEXER_ERROR("A temporary buffer for the lexical analysis of a number could not be allocated.\n");

    memcpy(number, lexer.buf + lexer.mark, numberlen);

    /* It terminates the temporary string with \0 */
    number[numberlen - 1] = '\0';

    token->metadata.value = atof(number); // NOLINT(cert-err34-c)

    /* It frees the temporary buffer */
    free(number);

    return token;
}

static struct token *
make_number(double value) {
    struct token *token   = make_token(LEXER_TOKEN_NUMBER);
    token->metadata.value = value;

    return token;
}

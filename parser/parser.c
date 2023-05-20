/*
 * MIT License
 *
 * Copyright (c) 2023 Ricardo Fares
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../lexer/lexer.h"
#include "../util/hashtable.h"
#include "./parser.h"

/* Variables */

extern struct token *curr_token;
struct hashtable    *ht;

/* Function Declaration */

/**
 * It checks if the next token to be analyzed has
 * the same type as the specified type.
 * <p>
 * If the token type does not match, then a syntax
 * error has been identified and the program is exited.
 *
 * @param type the token type to be specified
 */
static void
match(unsigned type);

/**
 * It parses the following production rule (in BNF-notation).
 *
 *      <S> ::= { $id = <expr> ; } <expr>
 *
 */
static double
S();

/**
 * It parses the following production rule (in BNF-notation).
 *
 *      <expr> ::= <term> { ( + | - ) <term> }
 *
 */
static double
expr();

/**
 * It parses the following production rule (in BNF-notation).
 *
 *      <term> ::= <base> { ( * | / ) <base> }
 *
 */
static double
term();

/**
 * It parses the following production rule (in BNF-notation).
 *
 *      <base> ::= <factor> { ** <factor> }
 *
 */
static double
base();

/**
 * It parses the following production rule (in BNF-notation).
 *
 *      <factor> ::= number          |
 *                   + number        |
 *                   - number        |
 *                   [<expr>]        |
 *                   '|' <expr> '|'  |
 *                   ( <expr> )      |
 *                   id              |
 *                   sin( <expr> )   |
 *                   cos( <expr> )   |
 *                   tan( <expr> )   |
 *                   csc( <expr> )   |
 *                   sec( <expr> )   |
 *                   cot( <expr> )   |
 *                   floor( <expr> ) |
 *                   ceil( <expr> )  |
 *                   sqrt( <expr> )  |
 *                   cbrt( <expr> )  |
 *                   log10( <expr> ) |
 *                   log2( <expr> )  |
 *
 */
static double
factor();

/**
 * It calculates the factorial of the specified double
 * floating-point number x, that is, x!.
 *
 * @param x the number to calculate its factorial
 *
 * @return the factorial of x, that is, x!
 */
static double
factorial(double x);

/* Function Definition */

void parse() {
    ht = hashtable_new(10);

    NEXT_TOKEN();

    double value = S();
    match(LEXER_TOKEN_EOF);

    printf("Value: %lf.\n", value);
}

static double
S() {
    while (TOKEN_TYPE() == LEXER_TOKEN_DOLLAR) {
        char                    *id;
        double                   value;
        struct var_descriptor_t *placeholder;
        struct var_descriptor_t  var_desc;

        match(LEXER_TOKEN_DOLLAR);

        id          = TOKEN_ID();
        placeholder = hashtable_find(ht, id);

        // It check if a variable already exists with that id. If so,
        // then it is checked if the variable is constant. Therefore, if
        // the variable is constant a parser error occurr since it is not
        // possible to re-assign a read-only variable.
        if (placeholder && placeholder->flags & IS_CONSTANT)
            PARSE_ERROR("Variable `%s` cannot be re-assigned since it is read-only.\n", id);

        match(LEXER_TOKEN_ID);

        // It checks if the equals token `=` is preceded by the 
        // colon token `:`. Such that, we are analyzing a syntax
        // of `:=`. Therefore, this will indicate that a constant
        // is being defined instead of a variable.
        var_desc.flags = 0;
        if (TOKEN_TYPE() == LEXER_TOKEN_COLON) {
            match(LEXER_TOKEN_COLON);
            var_desc.flags = IS_CONSTANT;
        }

        match(LEXER_TOKEN_EQUALS);

        value          = expr();
        var_desc.value = value;

        hashtable_insert(ht, id, &var_desc);

        match(LEXER_TOKEN_SEMICOLON);
    }

    return expr();
}

static double
expr() {
    double value = term();

    while ((TOKEN_TYPE() == LEXER_TOKEN_PLUS) || (TOKEN_TYPE() == LEXER_TOKEN_MINUS)) {
        switch (TOKEN_TYPE()) {
            case LEXER_TOKEN_PLUS:
                match(LEXER_TOKEN_PLUS);
                value += term();
                break;
            case LEXER_TOKEN_MINUS:
                match(LEXER_TOKEN_MINUS);
                value -= term();
                break;
            default:
                SYNTAX_ERROR("Token Caught: %d, Token Expected: %d or %d.\n", TOKEN_TYPE(),
                             LEXER_TOKEN_PLUS, LEXER_TOKEN_MINUS);
        }
    }

    return value;
}

static double
term() {
    double value = base();

    while ((TOKEN_TYPE() == LEXER_TOKEN_MULTIPLY) || (TOKEN_TYPE() == LEXER_TOKEN_DIVIDE)) {
        switch (TOKEN_TYPE()) {
            case LEXER_TOKEN_MULTIPLY:
                match(LEXER_TOKEN_MULTIPLY);
                value *= base();
                break;
            case LEXER_TOKEN_DIVIDE:
                match(LEXER_TOKEN_DIVIDE);
                value /= base();
                break;
            default:
                SYNTAX_ERROR("Token Caught: %d, Token Expected: %d or %d.\n",
                             TOKEN_TYPE(), LEXER_TOKEN_MULTIPLY, LEXER_TOKEN_DIVIDE);
        }
    }

    return value;
}

static double
base() {
    double value = factor();

    while (TOKEN_TYPE() == LEXER_TOKEN_POW) {
        switch (TOKEN_TYPE()) {
            case LEXER_TOKEN_POW:
                match(LEXER_TOKEN_POW);
                value = pow(value, factor());
                break;
            default:
                SYNTAX_ERROR("Token Caught: %d, Token Expected: %d.\n", TOKEN_TYPE(), LEXER_TOKEN_POW);
        }
    }

    return value;
}

static double
factor() {
    if (TOKEN_TYPE() == LEXER_TOKEN_NUMBER) {
        const double value = TOKEN_VALUE();
        match(LEXER_TOKEN_NUMBER);
        return value;
    } else if (TOKEN_TYPE() == LEXER_TOKEN_LPAREN) {
        match(LEXER_TOKEN_LPAREN);
        const double value = expr();
        match(LEXER_TOKEN_RPAREN);
        return value;
    } else if (TOKEN_TYPE() == LEXER_TOKEN_LBRACKET) {
        match(LEXER_TOKEN_LBRACKET);
        const double value = factorial(expr());
        match(LEXER_TOKEN_RBRACKET);
        return value;
    } else if (TOKEN_TYPE() == LEXER_TOKEN_PLUS) {
        match(LEXER_TOKEN_PLUS);
        const double value = TOKEN_VALUE();
        match(LEXER_TOKEN_NUMBER);
        return value;
    } else if (TOKEN_TYPE() == LEXER_TOKEN_MINUS) {
        match(LEXER_TOKEN_MINUS);
        const double value = -TOKEN_VALUE();
        match(LEXER_TOKEN_NUMBER);
        return value;
    } else if (TOKEN_TYPE() == LEXER_TOKEN_PIPE) {
        match(LEXER_TOKEN_PIPE);
        const double value = fabs(expr());
        match(LEXER_TOKEN_PIPE);
        return value;
    } else if (TOKEN_TYPE() == LEXER_TOKEN_ID) {
        const char              *id = TOKEN_ID();
        struct var_descriptor_t *placeholder = NULL;

        match(LEXER_TOKEN_ID);

        placeholder = hashtable_find(ht, id);

        /* It checks if there is no mapping to a value */
        /* for the specified identifier */
        if (!placeholder)
            PARSE_ERROR("Use of undeclared variable %s.\n", id);

        return placeholder->value;
    } else if (TOKEN_TYPE() == LEXER_TOKEN_FUNCTION_SIN) {  /* sin( <expr> ) */
        match(LEXER_TOKEN_FUNCTION_SIN);
        match(LEXER_TOKEN_LPAREN);
        const double value = expr();
        match(LEXER_TOKEN_RPAREN);
        return sin(value);
    } else if (TOKEN_TYPE() == LEXER_TOKEN_FUNCTION_COS) {  /* cos( <expr> ) */
        match(LEXER_TOKEN_FUNCTION_COS);
        match(LEXER_TOKEN_LPAREN);
        const double value = expr();
        match(LEXER_TOKEN_RPAREN);
        return cos(value);
    } else if (TOKEN_TYPE() == LEXER_TOKEN_FUNCTION_TAN) {  /* tan( <expr> ) */
        match(LEXER_TOKEN_FUNCTION_TAN);
        match(LEXER_TOKEN_LPAREN);
        const double value = expr();
        match(LEXER_TOKEN_RPAREN);
        return tan(value);
    } else if (TOKEN_TYPE() == LEXER_TOKEN_FUNCTION_CSC) {  /* csc( <expr> ) */
        match(LEXER_TOKEN_FUNCTION_CSC);
        match(LEXER_TOKEN_LPAREN);
        const double value = expr();
        match(LEXER_TOKEN_RPAREN);
        return 1.0 / sin(value);
    } else if (TOKEN_TYPE() == LEXER_TOKEN_FUNCTION_SEC) {  /* sec( <expr> ) */
        match(LEXER_TOKEN_FUNCTION_SEC);
        match(LEXER_TOKEN_LPAREN);
        const double value = expr();
        match(LEXER_TOKEN_RPAREN);
        return 1.0 / cos(value);
    } else if (TOKEN_TYPE() == LEXER_TOKEN_FUNCTION_COT) {  /* cot( <expr> ) */
        match(LEXER_TOKEN_FUNCTION_COT);
        match(LEXER_TOKEN_LPAREN);
        const double value = expr();
        match(LEXER_TOKEN_RPAREN);
        return 1.0 / tan(value);
    } else if (TOKEN_TYPE() == LEXER_TOKEN_FUNCTION_FLOOR) {  /* floor( <expr> ) */
        match(LEXER_TOKEN_FUNCTION_FLOOR);
        match(LEXER_TOKEN_LPAREN);
        const double value = expr();
        match(LEXER_TOKEN_RPAREN);
        return floor(value);
    } else if (TOKEN_TYPE() == LEXER_TOKEN_FUNCTION_CEIL) {  /* ceil( <expr> ) */
        match(LEXER_TOKEN_FUNCTION_CEIL);
        match(LEXER_TOKEN_LPAREN);
        const double value = expr();
        match(LEXER_TOKEN_RPAREN);
        return ceil(value);
    } else if (TOKEN_TYPE() == LEXER_TOKEN_FUNCTION_SQRT) {  /* sqrt( <expr> ) */
        match(LEXER_TOKEN_FUNCTION_SQRT);
        match(LEXER_TOKEN_LPAREN);
        const double value = expr();
        match(LEXER_TOKEN_RPAREN);
        return sqrt(value);
    } else if (TOKEN_TYPE() == LEXER_TOKEN_FUNCTION_CBRT) {  /* cbrt( <expr> ) */
        match(LEXER_TOKEN_FUNCTION_CBRT);
        match(LEXER_TOKEN_LPAREN);
        const double value = expr();
        match(LEXER_TOKEN_RPAREN);
        return cbrt(value);
    } else if (TOKEN_TYPE() == LEXER_TOKEN_FUNCTION_LOG10) {  /* log10( <expr> ) */
        match(LEXER_TOKEN_FUNCTION_LOG10);
        match(LEXER_TOKEN_LPAREN);
        const double value = expr();
        match(LEXER_TOKEN_RPAREN);
        return log10(value);
    } else if (TOKEN_TYPE() == LEXER_TOKEN_FUNCTION_LOG2) {  /* log2( <expr> ) */
        match(LEXER_TOKEN_FUNCTION_LOG2);
        match(LEXER_TOKEN_LPAREN);
        const double value = expr();
        match(LEXER_TOKEN_RPAREN);
        return log2(value);
    }

    printf("factor: Should not reach here!");
    exit(EXIT_FAILURE);
}

static void
match(const unsigned type) {
    /* It checks if the current token that has been identified by the */
    /* lexical analysis matches its type with the specified type */
    /* If so, then the next token in the buffer is got */
    if (curr_token->type == type) {
        NEXT_TOKEN();
    } else {
        SYNTAX_ERROR("Token Caught: %d, Expected: %d.\n", TOKEN_TYPE(), type);
    }
}

static double
factorial(const double x) {
    double i = 0.0;
    double f = 1.0;
    while ((++i) <= x) f *= i;
    return f;
}

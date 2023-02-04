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

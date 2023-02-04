cmake_minimum_required(VERSION 3.24)
project(calc C)

set(CMAKE_C_STANDARD 17)

add_executable(calc main.c lexer/lexer.h lexer/lexer.c parser/parser.h parser/parser.c util/hashtable.h util/hashtable.c)
target_link_libraries(calc m)

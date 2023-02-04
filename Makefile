LEXER	:=	lexer/lexer.c
PARSER	:=	parser/parser.c
UTIL	:=	util/hashtable.c
OUTPUT	:=	rdp_calc
FLAGS	:=	-lm

build:
		gcc main.c $(LEXER) $(PARSER) $(UTIL) -o $(OUTPUT) $(FLAGS)
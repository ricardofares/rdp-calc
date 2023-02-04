LEXER	:=	lexer/lexer.c
PARSER	:=	parser/parser.c
OUTPUT	:=	rdp_calc
FLAGS	:=	-lm

build:
		gcc main.c $(LEXER) $(PARSER) -o $(OUTPUT) $(FLAGS)
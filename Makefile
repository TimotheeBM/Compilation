CC = gcc

LIBS = -lm 
CCFLAGS = -Wall -ggdb

OBJ = analyseur_lexical.o util.o premiers.o suivants.o tabsymboles.o syntabs.o affiche_arbre_abstrait.o parcours.o

all: test_yylex analyseur_syntaxique

test_yylex: test_yylex.c $(OBJ)
	$(CC) $(CCFLAGS) -o test_yylex test_yylex.c $(OBJ)

analyseur_lexical.o: analyseur_lexical.c
	$(CC) $(CCFLAGS) -c  analyseur_lexical.c

analyseur_syntaxique: analyseur_syntaxique.c $(OBJ)
	$(CC) $(CCFLAGS) -o  analyseur_syntaxique analyseur_syntaxique.c $(OBJ)

.PHONY : clean

clean:
	- rm -f $(OBJ)
	- rm -f test_yylex
	- rm -f analyseur_syntaxique

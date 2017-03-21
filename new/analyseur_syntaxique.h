#ifndef __ANALYSEUR_SYNTAXIQUE__
#define __ANALYSEUR_SYNTAXIQUE__

#include "premiers.h"
#include "suivants.h"
#include "util.h"

void erreur_syntaxe(void);
void programme(void);
void optDecVariables(void);
void listeDecVariables(void);
void listeDecVariablesBis(void);
void declarationVariable(void);
void optTailleTableau(void);
void listeDecFonctions(void);
void declarationFonction(void);
void listeParam(void);
void optListeDecVariables(void);
void instruction(void);
void instructionAffect(void);
void instructionBloc(void);
void listeInstructions(void);
void instructionSi(void);
void optSinon(void);
void instructionTantque(void);
void instructionAppel(void);
void instructionRetour(void);
void instructionEcriture(void);
void instructionVide(void);
void expression(void);
void expressionBis(void);
void conjonction(void);
void conjonctionBis(void);
void comparaison(void);
void comparaisonBis(void);
void expArith(void);
void expArithBis(void);
void terme(void);
void termeBis(void);
void negation(void);
void facteur(void);
void var(void);
void optIndice(void);
void appelFct(void);
void listeExpressions(void);
void listeExpressionsBis(void);

#endif

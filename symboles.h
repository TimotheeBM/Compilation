/* symboles non terminaux */

#define EPSILON 0

#define NB_NON_TERMINAUX 41

#define _listeDecVariables_ 1				//LDV
#define _listeDecFonctions_ 2				//LDF
#define _declarationVariable_ 3				//DV
#define _declarationFonction_ 4				//DF
#define _listeParam_ 5						//LP
#define _listeInstructions_ 6				//LI
#define _instruction_ 8						//I
#define _instructionAffect_ 9  				//IAFF
#define _instructionBloc_ 10				//IB
#define _instructionSi_ 11					//ISI
#define _instructionTantque_ 12 			//ITQ
#define _instructionAppel_ 13				//IAPP
#define _instructionRetour_ 14				//IRET
#define _instructionEcriture_ 15			//IECR
#define _instructionVide_ 16				//IVIDE
#define _var_ 17							//VAR
#define _expression_ 18						//EXP
#define _appelFct_ 19						//APPF
#define _conjonction_ 20					//CONJ
#define _negation_ 21						//NEG
#define _comparaison_ 22					//COMP
#define _expArith_ 23						//E
#define _terme_ 24							//T
#define _facteur_ 25						//F
#define _argumentsEffectifs_ 26 								//  !!!!!!!!!!!!!!!!!!
#define _listeExpressions_ 27				//LEXP
#define _listeExpressionsBis_ 7				//LEXPB
#define _programme_ 28						//PG
#define _conjonctionBis_ 29					//CONJB
#define _optTailleTableau_ 30				//OTT
#define _expArithBis_ 31					//EB
#define _optSinon_ 32						//OSINON
#define _comparaisonBis_ 33					//COMPB
#define _optDecVariables_ 34				//ODV
#define _optIndice_ 35						//OIND
#define _listeDecVariablesBis_ 36			//LDVB
#define _instructionPour_ 37									// !!!!!!!!!!!!!!!!!!!
#define _termeBis_ 38						//TB
#define _expressionBis_ 39					//EXPB
#define _instructionFaire_ 40									// !!!!!!!!!!!!!!!!!!!
#define _optListeDecVariables_ 41			//OLDV



/* symboles terminaux */
#define NB_TERMINAUX 31

#define POINT_VIRGULE 1
#define PLUS 2
#define MOINS 3
#define FOIS 4
#define DIVISE 5
#define PARENTHESE_OUVRANTE 6
#define PARENTHESE_FERMANTE 7
#define CROCHET_OUVRANT 8
#define CROCHET_FERMANT 9
#define ACCOLADE_OUVRANTE 10
#define ACCOLADE_FERMANTE 11
#define EGAL 12
#define INFERIEUR 13
#define ET 14
#define OU 15
#define NON 16
#define SI 17
#define ALORS 18
#define SINON 19
#define TANTQUE 20
#define FAIRE 21
#define ENTIER 22
#define RETOUR 23
#define LIRE 24
#define ECRIRE 25
#define ID_VAR 26
#define ID_FCT 27
#define NOMBRE 28
#define FIN 29
#define VIRGULE 30



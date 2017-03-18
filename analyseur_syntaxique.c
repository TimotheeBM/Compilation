#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analyseur_lexical.h"
#include "symboles.h"
#include "premiers.h"
#include "suivants.h"
#include "analyseur_syntaxique.h"

char yytext[100];
char nom[100];
char valeur[100];
int analyse_lex = 0;
int analyse_synt = 0;

FILE *yyin;
int uniteCourante;


void erreur_syntaxe(void)
{
	printf("Expression incorrecte !\n");
	exit(-1);
}

void programme(void)
{
	affiche_balise_ouvrante(__FUNCTION__,1);
	if(est_premier(_optDecVariables_, uniteCourante) || est_premier(_listeDecFonctions_, uniteCourante ) ||
	est_suivant(_programme_,uniteCourante))
	{
		optDecVariables();
		listeDecFonctions();
		affiche_balise_fermante(__FUNCTION__,1);
		return;
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

void optDecVariables(void)
{
	affiche_balise_ouvrante(__FUNCTION__,1);
	if(est_premier(_listeDecVariables_, uniteCourante ))
	{
		listeDecVariables();
		if(uniteCourante==POINT_VIRGULE)
		{
			nom_token(uniteCourante,nom,valeur);
			affiche_element(nom,valeur,1);
			uniteCourante=yylex();
			affiche_balise_fermante(__FUNCTION__,1);
			return;
		}
	}
	else if(est_suivant(_optDecVariables_,uniteCourante)) 
	{
		affiche_balise_fermante(__FUNCTION__,1);
		return;
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

void listeDecVariables(void)
{
	affiche_balise_ouvrante(__FUNCTION__,1);
	if(est_premier(_declarationVariable_, uniteCourante ))
	{
		declarationVariable();
		listeDecVariablesBis();
		affiche_balise_fermante(__FUNCTION__,1);
		return;
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

void listeDecVariablesBis(void)
{
	affiche_balise_ouvrante(__FUNCTION__,1);
	if(uniteCourante==VIRGULE){
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if(est_premier(_declarationVariable_,uniteCourante))
		{
			declarationVariable();
			listeDecVariablesBis();
			affiche_balise_fermante(__FUNCTION__,1);
			return;
		}
	}
	else if(est_suivant(_listeDecVariablesBis_,uniteCourante))
	{
		affiche_balise_fermante(__FUNCTION__,1);
		return;
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

void declarationVariable(void)
{
	affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==ENTIER)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if(uniteCourante==ID_VAR)
		{
			nom_token(uniteCourante,nom,valeur);
			affiche_element(nom,valeur,1);
			uniteCourante=yylex();
			if(est_premier(_optTailleTableau_,uniteCourante) || est_suivant(_declarationVariable_,uniteCourante))
			{
				optTailleTableau();	
				affiche_balise_fermante(__FUNCTION__,1);
				return ;
			}
		}
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

void optTailleTableau(void)
{
	affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==CROCHET_OUVRANT)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if(uniteCourante==NOMBRE)
		{
			nom_token(uniteCourante,nom,valeur);
			affiche_element(nom,valeur,1);
			uniteCourante=yylex();
			if(uniteCourante==CROCHET_FERMANT)
			{
				nom_token(uniteCourante,nom,valeur);
				affiche_element(nom,valeur,1);
				uniteCourante=yylex();
				affiche_balise_fermante(__FUNCTION__,1);
				return;
			}
		}
	}
	else if (est_suivant(_optTailleTableau_,uniteCourante))
		{
			affiche_balise_fermante(__FUNCTION__,1);
			return;
		}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

void listeDecFonctions(void)
{
	affiche_balise_ouvrante(__FUNCTION__,1);
	if(est_premier(_declarationFonction_, uniteCourante ))
	{
		declarationFonction();
		listeDecFonctions();
		affiche_balise_fermante(__FUNCTION__,1);
		return;
	}
	else if (est_suivant(_listeDecFonctions_,uniteCourante))
	{
		affiche_balise_fermante(__FUNCTION__,1);
		return;
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}
 
void declarationFonction(void)
{
	affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==ID_FCT)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if (est_premier(_listeParam_, uniteCourante ))
		{
			listeParam();
			optDecVariables();
			instructionBloc();
			affiche_balise_fermante(__FUNCTION__,1);
			return;
		}
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

void listeParam(void)
{
	affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==PARENTHESE_OUVRANTE)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if(est_premier(_optListeDecVariables_,uniteCourante) || est_suivant(_optListeDecVariables_,uniteCourante))
		{
			optListeDecVariables();
			if (uniteCourante==PARENTHESE_FERMANTE)
			{
				nom_token(uniteCourante,nom,valeur);
				affiche_element(nom,valeur,1);
				uniteCourante=yylex();
				affiche_balise_fermante(__FUNCTION__,1);
				return;
			}
		}
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

void optListeDecVariables(void)
{
	affiche_balise_ouvrante(__FUNCTION__,1);
	if(est_premier(_listeDecVariables_,uniteCourante))
	{
		listeDecVariables();
		affiche_balise_fermante(__FUNCTION__,1);
		return;
	}
	else if (est_suivant(_optListeDecVariables_,uniteCourante))
	{
		affiche_balise_fermante(__FUNCTION__,1);
		return;
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

void instruction(void)
{
	affiche_balise_ouvrante(__FUNCTION__,1);
	if(est_premier(_instructionAffect_,uniteCourante))
	{
		instructionAffect();
		affiche_balise_fermante(__FUNCTION__,1);
		return;
	}
	else if(est_premier(_instructionBloc_,uniteCourante))
	{
		instructionBloc();
		affiche_balise_fermante(__FUNCTION__,1);
		return;
	}
	
	else if(est_premier(_instructionSi_,uniteCourante))
	{
		instructionSi();
		affiche_balise_fermante(__FUNCTION__,1);
		return;
	}
	else if(est_premier(_instructionTantque_,uniteCourante))
	{
		instructionTantque();
		affiche_balise_fermante(__FUNCTION__,1);
		return;
	}
	else if(est_premier(_instructionAppel_,uniteCourante))
	{
		instructionAppel();
		affiche_balise_fermante(__FUNCTION__,1);
		return;
	}
	else if(est_premier(_instructionRetour_,uniteCourante))
	{
		instructionRetour();
		affiche_balise_fermante(__FUNCTION__,1);
		return;
	}
	else if(est_premier(_instructionEcriture_,uniteCourante))
	{
		instructionEcriture();
		affiche_balise_fermante(__FUNCTION__,1);
		return;
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

void instructionAffect(void)
{
	affiche_balise_ouvrante(__FUNCTION__,1);
	if (est_premier(_var_,uniteCourante))
	{
		var();
		if (uniteCourante==EGAL)
		{
			nom_token(uniteCourante,nom,valeur);
			affiche_element(nom,valeur,1);
			uniteCourante=yylex();
			if (est_premier(_expression_,uniteCourante))
			{
				expression();
				if (uniteCourante==POINT_VIRGULE)
				{
					nom_token(uniteCourante,nom,valeur);
					affiche_element(nom,valeur,1);
					uniteCourante=yylex();
					affiche_balise_fermante(__FUNCTION__,1);
					return;
				}
			}
		}
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

void instructionBloc(void)
{
	affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==ACCOLADE_OUVRANTE)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if (est_premier(_listeInstructions_,uniteCourante) || est_suivant(_instructionBloc_,uniteCourante))
		{
			listeInstructions();
			if (uniteCourante==ACCOLADE_FERMANTE)
			{
				nom_token(uniteCourante,nom,valeur);
				affiche_element(nom,valeur,1);
				uniteCourante=yylex();
				affiche_balise_fermante(__FUNCTION__,1);
				return;
			}
		}
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

void listeInstructions(void)
{
	affiche_balise_ouvrante(__FUNCTION__,1);
	if (est_premier(_instruction_,uniteCourante))
	{
		instruction();
		listeInstructions();
		affiche_balise_fermante(__FUNCTION__,1);
		return;
	}
	else if (est_suivant(_listeInstructions_,uniteCourante))
	{
		affiche_balise_fermante(__FUNCTION__,1);
		return;
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

void instructionSi(void)
{
	affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==SI)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if (est_premier(_expression_,uniteCourante)) 
		{
			expression();
			if (uniteCourante==ALORS)
			{
				nom_token(uniteCourante,nom,valeur);
				affiche_element(nom,valeur,1);
				uniteCourante=yylex();
				if (est_premier(_instructionBloc_,uniteCourante)) 
				{
					instructionBloc();
					optSinon();
					affiche_balise_fermante(__FUNCTION__,1);
					return;
				}
			}
		}
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);	
}

void optSinon(void)
{
	affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==SINON)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if(est_premier(_instructionBloc_,uniteCourante))
		{
			instructionBloc();
			affiche_balise_fermante(__FUNCTION__,1);
			return;
		}
	}
	else if(est_suivant(_optSinon_,uniteCourante))
	{
		affiche_balise_fermante(__FUNCTION__,1);
		return;
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

void instructionTantque(void)
{
	affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==TANTQUE)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if(est_premier(_expression_,uniteCourante))
		{
			expression();
			if (uniteCourante==FAIRE)
			{
				nom_token(uniteCourante,nom,valeur);
				affiche_element(nom,valeur,1);
				uniteCourante=yylex();
				if(est_premier(_instructionBloc_,uniteCourante))
				{
					instructionBloc();
					affiche_balise_fermante(__FUNCTION__,1);
					return;
				}
			}
		}
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}		

void instructionAppel(void)
{
	affiche_balise_ouvrante(__FUNCTION__,1);
	if(est_premier(_appelFct_,uniteCourante))
	{
		appelFct();
		if (uniteCourante==POINT_VIRGULE)
		{
			nom_token(uniteCourante,nom,valeur);
			affiche_element(nom,valeur,1);
			uniteCourante=yylex();
			affiche_balise_fermante(__FUNCTION__,1);
			return;
		}
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

void instructionRetour(void)
{
	affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==RETOUR)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if(est_premier(_expression_,uniteCourante))
		{
			expression();
			if (uniteCourante==POINT_VIRGULE)
			{
				nom_token(uniteCourante,nom,valeur);
				affiche_element(nom,valeur,1);
				uniteCourante=yylex();
				affiche_balise_fermante(__FUNCTION__,1);
				return;
			}
		}
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

void instructionEcriture(void)
{
	affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==ECRIRE)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if (uniteCourante==PARENTHESE_OUVRANTE)
		{
			nom_token(uniteCourante,nom,valeur);
			affiche_element(nom,valeur,1);
			uniteCourante=yylex();
			if(est_premier(_expression_,uniteCourante))
			{
				expression();
				if (uniteCourante==PARENTHESE_FERMANTE)
				{
					nom_token(uniteCourante,nom,valeur);
					affiche_element(nom,valeur,1);
					uniteCourante=yylex();
					if (uniteCourante==POINT_VIRGULE)
					{
						nom_token(uniteCourante,nom,valeur);
						affiche_element(nom,valeur,1);
						uniteCourante=yylex();
						affiche_balise_fermante(__FUNCTION__,1);
						return;
					}
				}
			}
		}
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

void instructionVide(void)
{
	affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==POINT_VIRGULE)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		affiche_balise_fermante(__FUNCTION__,1);
		return;
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

void expression(void)
{
	affiche_balise_ouvrante(__FUNCTION__,1);
	if (est_premier(_conjonction_,uniteCourante))
	{
		conjonction();
		expressionBis();
		affiche_balise_fermante(__FUNCTION__,1);
		return;
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

void expressionBis(void)
{
	affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==OU)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if (est_premier(_conjonction_,uniteCourante))
		{
			conjonction();
			expressionBis();
			affiche_balise_fermante(__FUNCTION__,1);
			return;
		}
	}
	else if(est_suivant(_expressionBis_,uniteCourante))
		{
			affiche_balise_fermante(__FUNCTION__,1);
			return;
		}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

void conjonction(void)
{
	affiche_balise_ouvrante(__FUNCTION__,1);
	if (est_premier(_comparaison_,uniteCourante))
	{
		comparaison();
		conjonctionBis();
		affiche_balise_fermante(__FUNCTION__,1);
		return;
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

void conjonctionBis(void)
{
	affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==ET)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if (est_premier(_comparaison_,uniteCourante))
		{
			comparaison();
			conjonctionBis();
			affiche_balise_fermante(__FUNCTION__,1);
			return;
		}
	}
	else if(est_suivant(_conjonctionBis_,uniteCourante))
		{
			affiche_balise_fermante(__FUNCTION__,1);
			return;
		}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

void comparaison(void)
{
	affiche_balise_ouvrante(__FUNCTION__,1);
	if(est_premier(_expArith_,uniteCourante))
	{
		expArith();
		comparaisonBis();
		affiche_balise_fermante(__FUNCTION__,1);
		return;
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

void comparaisonBis(void)
{
	affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==EGAL) 
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if (est_premier(_expArith_,uniteCourante))
		{
			expArith();
			comparaisonBis();
			affiche_balise_fermante(__FUNCTION__,1);
			return;
		}
	}
	else if(uniteCourante==INFERIEUR)
		{
			nom_token(uniteCourante,nom,valeur);
			affiche_element(nom,valeur,1);
			uniteCourante=yylex();
			if (est_premier(_expArith_,uniteCourante))
			{
				expArith();
				comparaisonBis();
				affiche_balise_fermante(__FUNCTION__,1);
				return;
			}
		}
		else if(est_suivant(_comparaisonBis_,uniteCourante))
			{
				affiche_balise_fermante(__FUNCTION__,1);
				return;
			}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

void expArith(void)
{
	affiche_balise_ouvrante(__FUNCTION__,1);
	if (est_premier(_terme_,uniteCourante))
	{
		terme();
		expArithBis();
		affiche_balise_fermante(__FUNCTION__,1);
		return;
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

void expArithBis(void)
{
	affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==PLUS)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if (est_premier(_terme_,uniteCourante))
		{
			terme();
			expArithBis();
			affiche_balise_fermante(__FUNCTION__,1);
			return;
		}
	}
	else if(uniteCourante==MOINS) 
		{
			nom_token(uniteCourante,nom,valeur);
			affiche_element(nom,valeur,1);
			uniteCourante=yylex();
			if (est_premier(_terme_,uniteCourante))
			{
				terme();
				expArithBis();
				affiche_balise_fermante(__FUNCTION__,1);
				return;
			}
		}
		else if(est_suivant(_expArithBis_,uniteCourante))
		{
			affiche_balise_fermante(__FUNCTION__,1);
			return;
		}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

void terme(void)
{
	affiche_balise_ouvrante(__FUNCTION__,1);
	if(est_premier(_negation_,uniteCourante))
	{
		negation();
		termeBis();
		affiche_balise_fermante(__FUNCTION__,1);
		return;
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

void termeBis()
{
	affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==FOIS)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if (est_premier(_negation_,uniteCourante))
		{
			negation();
			termeBis();
			affiche_balise_fermante(__FUNCTION__,1);
			return;
		}
	}
	else if(uniteCourante==DIVISE)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if (est_premier(_negation_,uniteCourante))
		{
			negation();
			termeBis();
			affiche_balise_fermante(__FUNCTION__,1);
			return;
		}
	}
	else if(est_suivant(_termeBis_,uniteCourante))
	{
		affiche_balise_fermante(__FUNCTION__,1);
		return;
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

void negation(void)
{
	affiche_balise_ouvrante(__FUNCTION__,1);
	if(uniteCourante==NON)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if (est_premier(_negation_,uniteCourante))
		{
			negation();
			affiche_balise_fermante(__FUNCTION__,1);
			return;
		}
	}
	else if(est_premier(_facteur_,uniteCourante))
	{
		facteur();
		affiche_balise_fermante(__FUNCTION__,1);
		return;
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

void facteur(void)
{
	affiche_balise_ouvrante(__FUNCTION__,1);
	if(uniteCourante==PARENTHESE_OUVRANTE)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if(est_premier(_expression_,uniteCourante))
		{
			expression();
			if(uniteCourante==PARENTHESE_FERMANTE)
			{
				nom_token(uniteCourante,nom,valeur);
				affiche_element(nom,valeur,1);
				uniteCourante=yylex();
				affiche_balise_fermante(__FUNCTION__,1);
				return;
			}
		}
	}
	else if(uniteCourante==NOMBRE)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		affiche_balise_fermante(__FUNCTION__,1);
		return;
	}
	else if(est_premier(_appelFct_,uniteCourante))
	{
		appelFct();
		affiche_balise_fermante(__FUNCTION__,1);
		return;
	}
	else if(est_premier(_var_,uniteCourante))
	{
		var();
		affiche_balise_fermante(__FUNCTION__,1);
		return;
	}
	else if(uniteCourante==LIRE) 
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if(uniteCourante==PARENTHESE_OUVRANTE) 
		{
			nom_token(uniteCourante,nom,valeur);
			affiche_element(nom,valeur,1);
			uniteCourante=yylex();
			if(uniteCourante==PARENTHESE_FERMANTE) 
			{
				nom_token(uniteCourante,nom,valeur);
				affiche_element(nom,valeur,1);
				uniteCourante=yylex();
				affiche_balise_fermante(__FUNCTION__,1);
				return;
			}
		}
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

void var(void)
{
	affiche_balise_ouvrante(__FUNCTION__,1);
	if(uniteCourante==ID_VAR)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if(est_premier(_optIndice_,uniteCourante) || est_suivant(_var_,uniteCourante))
		{
			optIndice();
			affiche_balise_fermante(__FUNCTION__,1);
			return;
		}
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

void optIndice(void)
{
	affiche_balise_ouvrante(__FUNCTION__,1);
	if(uniteCourante==CROCHET_OUVRANT)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if(est_premier(_expression_,uniteCourante))
		{
			expression();
			if(uniteCourante==CROCHET_FERMANT)
			{
				nom_token(uniteCourante,nom,valeur);
				affiche_element(nom,valeur,1);
				uniteCourante=yylex();
				affiche_balise_fermante(__FUNCTION__,1);
				return;
			}
		}
	}
		
	if(est_suivant(_optIndice_,uniteCourante))
	{
		affiche_balise_fermante(__FUNCTION__,1);
		return;
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

void appelFct(void)
{
	affiche_balise_ouvrante(__FUNCTION__,1);
	if(uniteCourante==ID_FCT)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if(uniteCourante==PARENTHESE_OUVRANTE)
		{
			nom_token(uniteCourante,nom,valeur);
			affiche_element(nom,valeur,1);
			uniteCourante=yylex();
			if(est_premier(_listeExpressions_,uniteCourante) || est_suivant(_listeExpressions_,uniteCourante))
			{
				listeExpressions();
				if(uniteCourante==PARENTHESE_FERMANTE)
				{
					nom_token(uniteCourante,nom,valeur);
					affiche_element(nom,valeur,1);
					uniteCourante=yylex();
					affiche_balise_fermante(__FUNCTION__,1);
					return;
				}
			}
			
		}
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

void listeExpressions(void)
{
	affiche_balise_ouvrante(__FUNCTION__,1);
	if(est_premier(_expression_,uniteCourante))
	{
		expression();
		listeExpressionsBis();
		affiche_balise_fermante(__FUNCTION__,1);
		return;
	}
	else if(est_suivant(_listeExpressions_,uniteCourante))
	{
		affiche_balise_fermante(__FUNCTION__,1);
		return;
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

void listeExpressionsBis(void)
{
	affiche_balise_ouvrante(__FUNCTION__,1);
	if(uniteCourante==VIRGULE)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if(est_premier(_expression_,uniteCourante))
		{
			expression();
			listeExpressionsBis();
			affiche_balise_fermante(__FUNCTION__,1);
			return;
		}
	}
	else if(est_suivant(_listeExpressionsBis_,uniteCourante))
		{
			affiche_balise_fermante(__FUNCTION__,1);
			return;
		}
	else erreur_syntaxe();	
	affiche_balise_fermante(__FUNCTION__,1);
}

int main(int argc, char **argv)
{
	int i;
	for(i=1;i<argc;i++)
	{
		if(strcmp(argv[i],"-l")==0)
		{
			analyse_lex = 1;
			continue;
		}
		if(strcmp(argv[i],"-s")==0)
		{
			analyse_synt=1;
			continue;
		}
		yyin = fopen(argv[i], "r");
		if(yyin == NULL){
		fprintf(stderr, "impossible d'ouvrir le fichier %s\n", argv[1]);
		exit(1);
  }
	}
	if(analyse_lex==1)
	{
		test_yylex_internal(yyin);
	}
	
	if(analyse_synt==1)
	{
		initialise_premiers();
		initialise_suivants();
		uniteCourante = yylex();
		programme();
	}
 
  
  //~ if(uniteCourante == FIN)
  //~ {
	  //~ printf("Expression correcte !\n");
	  //~ return 0;
  //~ }
  //~ else
  //~ {
	  //~ printf("Expression incorrecte !\n");
	  //~ exit(-1);
  //~ }
	return 0;
}

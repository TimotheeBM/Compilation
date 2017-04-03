#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analyseur_lexical.h"
#include "symboles.h"
#include "premiers.h"
#include "suivants.h"
#include "syntabs.h"
#include "affiche_arbre_abstrait.h"
#include "analyseur_syntaxique.h"
#include "tabsymboles.h"
#include "parcours.h"


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

n_prog *programme(void)
{
	n_prog *$$ = NULL;
	n_l_dec *$1 = NULL;
	n_l_dec *$2 = NULL;
	
	//~ affiche_balise_ouvrante(__FUNCTION__,1);
	
	if(est_premier(_optDecVariables_, uniteCourante) || est_premier(_listeDecFonctions_, uniteCourante ) ||
	est_suivant(_programme_,uniteCourante))
	{
		$1 = optDecVariables();
		$2 = listeDecFonctions();
		$$ = cree_n_prog($1, $2);
		return $$;
		//~ affiche_balise_fermante(__FUNCTION__,1);
	}
	else {
		printf("Expression incorrecte !\n");
		exit (-1);
	}
}

n_l_dec *optDecVariables(void)
{
	n_l_dec *$$ = NULL;
	
	//~ affiche_balise_ouvrante(__FUNCTION__,1);
	if(est_premier(_listeDecVariables_, uniteCourante ))
	{
		$$ = listeDecVariables();
		if(uniteCourante==POINT_VIRGULE)
		{
			nom_token(uniteCourante,nom,valeur);
			//~ affiche_element(nom,valeur,1);
			uniteCourante=yylex();
			//~ affiche_balise_fermante(__FUNCTION__,1);
			return $$;
		}
		else {
		printf("Expression incorrecte !\n");
		exit (-1);
		}
	}
	else if(est_suivant(_optDecVariables_,uniteCourante)) 
		{
			//~ affiche_balise_fermante(__FUNCTION__,1);
			return $$;
		}
	else {
		printf("Expression incorrecte !\n");
		exit (-1);
	}
	//~ affiche_balise_fermante(__FUNCTION__,1);
}

n_l_dec *listeDecVariables(void)
{
	n_l_dec *$$ = NULL;
	n_dec *$1 = NULL;
	n_l_dec *$2 = NULL;
	
	//~ affiche_balise_ouvrante(__FUNCTION__,1);
	if(est_premier(_declarationVariable_, uniteCourante ))
	{
		$1 = declarationVariable();
		$2 = listeDecVariablesBis();
		$$ = cree_n_l_dec($1, $2);
		//~ affiche_balise_fermante(__FUNCTION__,1);
		return $$;
	}
	else {
		printf("Expression incorrecte !\n");
		exit (-1);
	}
	//~ affiche_balise_fermante(__FUNCTION__,1);
}

n_l_dec *listeDecVariablesBis(void){
	n_l_dec *$$ = NULL;
	n_dec *$1 = NULL;
	n_l_dec *$2 = NULL;
	
	//~ affiche_balise_ouvrante(__FUNCTION__,1);
	if(uniteCourante==VIRGULE){
		nom_token(uniteCourante,nom,valeur);
		//~ affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if(est_premier(_declarationVariable_,uniteCourante))
		{
			$1 = declarationVariable();
			$2 = listeDecVariablesBis();
			$$ = cree_n_l_dec($1, $2);
			//~ affiche_balise_fermante(__FUNCTION__,1);
			return $$;
		}
		else {
		printf("Expression incorrecte !\n");
		exit (-1);
		}
	}
	else if(est_suivant(_listeDecVariablesBis_,uniteCourante))
	{
		//~ affiche_balise_fermante(__FUNCTION__,1);
		return $$;
	}
	else {
		printf("Expression incorrecte !\n");
		exit (-1);
	}
	//~ affiche_balise_fermante(__FUNCTION__,1);
}

n_dec *declarationVariable(void)
{
	n_dec *$$ = NULL;
	int $1 = -1;
	char *cpy_valeur = malloc(sizeof(valeur));

	//~ affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==ENTIER)
	{
		nom_token(uniteCourante,nom,valeur);
		//~ affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if(uniteCourante==ID_VAR)
		{
			nom_token(uniteCourante,nom,valeur);
			strcpy(cpy_valeur,valeur);
			//~ affiche_element(nom,valeur,1);
			uniteCourante=yylex();
			if(est_premier(_optTailleTableau_,uniteCourante) || est_suivant(_declarationVariable_,uniteCourante))
			{
				$1 = optTailleTableau();
				if($1 != -1)$$ = cree_n_dec_tab(cpy_valeur,$1);
				else $$ = cree_n_dec_var(cpy_valeur);
				//~ affiche_balise_fermante(__FUNCTION__,1);
				return $$;
			}
			else {
				printf("Expression incorrecte !\n");
				exit (-1);
			}
		}
		else {
			printf("Expression incorrecte !\n");
			exit (-1);
		}
	}
	else {
		printf("Expression incorrecte !\n");
		exit (-1);
	}
	//~ affiche_balise_fermante(__FUNCTION__,1);
}

int optTailleTableau(void)
{	
	int $$ = -1;
	//~ affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==CROCHET_OUVRANT)
	{
		nom_token(uniteCourante,nom,valeur);
		//~ affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if(uniteCourante==NOMBRE)
		{
			nom_token(uniteCourante,nom,valeur);
			//~ affiche_element(nom,valeur,1);
			$$ = atoi(valeur);
			uniteCourante=yylex();
			if(uniteCourante==CROCHET_FERMANT)
			{
				nom_token(uniteCourante,nom,valeur);
				//~ affiche_element(nom,valeur,1);
				uniteCourante=yylex();
				//~ affiche_balise_fermante(__FUNCTION__,1);
				return $$;
			}
			else {
				printf("Expression incorrecte !\n");
				exit (-1);
			}
		}
		else {
			printf("Expression incorrecte !\n");
			exit (-1);
		}
	}
	else if (est_suivant(_optTailleTableau_,uniteCourante))
		{
			//~ affiche_balise_fermante(__FUNCTION__,1);
			return $$;
		}
	else {
		printf("Expression incorrecte !\n");
		exit (-1);
	}
	//~ affiche_balise_fermante(__FUNCTION__,1);
}

n_l_dec *listeDecFonctions(void)
{
	n_l_dec *$$ = NULL;
	n_dec *$1 = NULL;
	n_l_dec *$2 = NULL;
	
	//~ affiche_balise_ouvrante(__FUNCTION__,1);
	if(est_premier(_declarationFonction_, uniteCourante ))
	{
		$1 = declarationFonction();
		$2 = listeDecFonctions();
		$$ = cree_n_l_dec($1, $2);
		//~ affiche_balise_fermante(__FUNCTION__,1);
		return $$;
	}
	else if (est_suivant(_listeDecFonctions_,uniteCourante))
	{
		//~ affiche_balise_fermante(__FUNCTION__,1);
		return $$;
	}
	else {
		printf("Expression incorrecte !\n");
		exit (-1);
	}
	//~ affiche_balise_fermante(__FUNCTION__,1);
}
 
n_dec *declarationFonction(void)
{
	n_dec *$$ = NULL;
	n_l_dec *$1 = NULL;
	n_l_dec *$2 = NULL;
	n_instr *$3 = NULL;
	char *cpy_valeur = malloc(sizeof(valeur));
	
	//~ affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==ID_FCT)
	{
		nom_token(uniteCourante,nom,valeur);
		strcpy(cpy_valeur,valeur);
		//~ affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if (est_premier(_listeParam_, uniteCourante ))
		{
			$1 = listeParam();
			$2 = optDecVariables();
			$3 = instructionBloc();
			$$ = cree_n_dec_fonc(cpy_valeur, $1, $2, $3);
			//~ affiche_balise_fermante(__FUNCTION__,1);
			return $$;
		}
		else {
			printf("Expression incorrecte !\n");
			exit (-1);
		}
	}
	else {
		printf("Expression incorrecte !\n");
		exit (-1);
	}
	//~ affiche_balise_fermante(__FUNCTION__,1);
}

n_l_dec *listeParam(void)
{
	n_l_dec *$1 = NULL;
	 
	//~ affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==PARENTHESE_OUVRANTE)
	{
		nom_token(uniteCourante,nom,valeur);
		//~ affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if(est_premier(_optListeDecVariables_,uniteCourante) || est_suivant(_optListeDecVariables_,uniteCourante))
		{
			$1 = optListeDecVariables();
			if (uniteCourante==PARENTHESE_FERMANTE)
			{
				nom_token(uniteCourante,nom,valeur);
				//~ affiche_element(nom,valeur,1);
				uniteCourante=yylex();
				//~ affiche_balise_fermante(__FUNCTION__,1);
				return $1;
			}
			else {
				printf("Expression incorrecte !\n");
				exit (-1);
			}
		}
		else {
			printf("Expression incorrecte !\n");
			exit (-1);
		}
	}
	else {
		printf("Expression incorrecte !\n");
		exit (-1);
	}
	//~ affiche_balise_fermante(__FUNCTION__,1);
}

n_l_dec *optListeDecVariables(void)
{
	n_l_dec *$1 = NULL;
	
	//~ affiche_balise_ouvrante(__FUNCTION__,1);
	if(est_premier(_listeDecVariables_,uniteCourante))
	{
		$1 =listeDecVariables();
		//~ affiche_balise_fermante(__FUNCTION__,1);
		return $1;
	}
	else if (est_suivant(_optListeDecVariables_,uniteCourante))
	{
		//~ affiche_balise_fermante(__FUNCTION__,1);
		return $1;
	}
	else {
		printf("Expression incorrecte !\n");
		exit (-1);
	}
	//~ affiche_balise_fermante(__FUNCTION__,1);
}

n_instr *instruction(void)
{
	n_instr *$1 = NULL;
	
	//~ affiche_balise_ouvrante(__FUNCTION__,1);
	if(est_premier(_instructionAffect_,uniteCourante))
	{
		$1 = instructionAffect();
		//~ affiche_balise_fermante(__FUNCTION__,1);
		return $1;
	}
	else if(est_premier(_instructionBloc_,uniteCourante))
	{
		$1 = instructionBloc();
		//~ affiche_balise_fermante(__FUNCTION__,1);
		return $1;
	}
	
	else if(est_premier(_instructionSi_,uniteCourante))
	{
		$1 = instructionSi();
		//~ affiche_balise_fermante(__FUNCTION__,1);
		return $1;
	}
	else if(est_premier(_instructionTantque_,uniteCourante))
	{
		$1 = instructionTantque();
		//~ affiche_balise_fermante(__FUNCTION__,1);
		return $1;
	}
	else if(est_premier(_instructionAppel_,uniteCourante))
	{
		$1 = instructionAppel();
		//~ affiche_balise_fermante(__FUNCTION__,1);
		return $1;
	}
	else if(est_premier(_instructionRetour_,uniteCourante))
	{
		$1 = instructionRetour();
		//~ affiche_balise_fermante(__FUNCTION__,1);
		return $1;
	}
	else if(est_premier(_instructionEcriture_,uniteCourante))
	{
		$1 = instructionEcriture();
		//~ affiche_balise_fermante(__FUNCTION__,1);
		return $1;
	}
	else if(est_premier(_instructionFaire_,uniteCourante))
	{
		$1 = instructionFaire();
		//~ affiche_balise_fermante(__FUNCTION__,1);
		return $1;
	}
	else {
		printf("Expression incorrecte !\n");
		exit (-1);
	}
	//~ affiche_balise_fermante(__FUNCTION__,1);
}

n_instr *instructionAffect(void)
{
	n_instr *$$ = NULL;
	n_var *$1 = NULL;
	n_exp *$2 = NULL;
	
	//~ affiche_balise_ouvrante(__FUNCTION__,1);
	if (est_premier(_var_,uniteCourante))
	{
		$1 = var();
		if (uniteCourante==EGAL)
		{
			nom_token(uniteCourante,nom,valeur);
			//~ affiche_element(nom,valeur,1);
			uniteCourante=yylex();
			if (est_premier(_expression_,uniteCourante))
			{
				$2 = expression();
				if (uniteCourante==POINT_VIRGULE)
				{
					nom_token(uniteCourante,nom,valeur);
					//~ affiche_element(nom,valeur,1);
					uniteCourante=yylex();
					$$ = cree_n_instr_affect($1, $2);
					//~ affiche_balise_fermante(__FUNCTION__,1);
					return $$;
				}
				else {
					printf("Expression incorrecte !\n");
					exit (-1);
				}
			}
			else {
				printf("Expression incorrecte !\n");
				exit (-1);
			}
		}
		else {
			printf("Expression incorrecte !\n");
			exit (-1);
		}
	}
	else {
		printf("Expression incorrecte !\n");
		exit (-1);
	}
	//~ affiche_balise_fermante(__FUNCTION__,1);
}

n_instr *instructionBloc(void)
{
	n_instr *$$ = NULL;
	n_l_instr *$1 = NULL;
	
	//~ affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==ACCOLADE_OUVRANTE)
	{
		nom_token(uniteCourante,nom,valeur);
		//~ affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if (est_premier(_listeInstructions_,uniteCourante) || est_suivant(_instructionBloc_,uniteCourante))
		{
			$1 = listeInstructions();
			if (uniteCourante==ACCOLADE_FERMANTE)
			{
				nom_token(uniteCourante,nom,valeur);
				//~ affiche_element(nom,valeur,1);
				uniteCourante=yylex();
				$$ = cree_n_instr_bloc($1);
				//~ affiche_balise_fermante(__FUNCTION__,1);
				return $$;
			}
			else {
				printf("Expression incorrecte !\n");
				exit (-1);
			}
		}
		else {
			printf("Expression incorrecte !\n");
			exit (-1);
		}
	}
	else {
		printf("Expression incorrecte !\n");
		exit (-1);
	}
	//~ affiche_balise_fermante(__FUNCTION__,1);
}

n_l_instr *listeInstructions(void)
{
	n_l_instr *$$ = NULL;
	n_instr *$1 = NULL;
	n_l_instr *$2 = NULL;
	
	//~ affiche_balise_ouvrante(__FUNCTION__,1);
	if (est_premier(_instruction_,uniteCourante))
	{
		$1 = instruction();
		$2 = listeInstructions();
		$$ = cree_n_l_instr($1, $2);
		//~ affiche_balise_fermante(__FUNCTION__,1);
		return $$;
	}
	else if (est_suivant(_listeInstructions_,uniteCourante))
	{
		//~ affiche_balise_fermante(__FUNCTION__,1);
		return $$;
	}
	else {
		printf("Expression incorrecte !\n");
		exit (-1);
	}
	//~ affiche_balise_fermante(__FUNCTION__,1);
}

n_instr *instructionSi(void)
{
	n_instr *$$ = NULL;
	n_exp *$1 = NULL;
	n_instr *$2 = NULL;
	n_instr *$3 = NULL;
	
	//~ affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==SI)
	{
		nom_token(uniteCourante,nom,valeur);
		//~ affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if (est_premier(_expression_,uniteCourante)) 
		{
			$1 = expression();
			if (uniteCourante==ALORS)
			{
				nom_token(uniteCourante,nom,valeur);
				//~ affiche_element(nom,valeur,1);
				uniteCourante=yylex();
				if (est_premier(_instructionBloc_,uniteCourante)) 
				{
					$2 = instructionBloc();
					$3 = optSinon();
					$$ = cree_n_instr_si($1, $2, $3);
					//~ affiche_balise_fermante(__FUNCTION__,1);
					return $$;
				}
				else {
					printf("Expression incorrecte !\n");
					exit (-1);
				}
			}
			else {
				printf("Expression incorrecte !\n");
				exit (-1);
			}
		}
		else {
			printf("Expression incorrecte !\n");
			exit (-1);
		}
		
	}
	else {
		printf("Expression incorrecte !\n");
		exit (-1);
	}
	//~ affiche_balise_fermante(__FUNCTION__,1);	
}

n_instr *optSinon(void)
{
	n_instr *$1 = NULL;
	
	//~ affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==SINON)
	{
		nom_token(uniteCourante,nom,valeur);
		//~ affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if(est_premier(_instructionBloc_,uniteCourante))
		{
			$1 = instructionBloc();
			//~ affiche_balise_fermante(__FUNCTION__,1);
			return $1;
		}
		else {
			printf("Expression incorrecte !\n");
			exit (-1);
		}
	}
	else if(est_suivant(_optSinon_,uniteCourante))
	{
		//~ affiche_balise_fermante(__FUNCTION__,1);
		return $1;
	}
	else {
		printf("Expression incorrecte !\n");
		exit (-1);
	}
	//~ affiche_balise_fermante(__FUNCTION__,1);
}

n_instr *instructionTantque(void)
{
	n_instr *$$ = NULL;
	n_exp *$1 = NULL;
	n_instr *$2 = NULL;
	
	//~ affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==TANTQUE)
	{
		nom_token(uniteCourante,nom,valeur);
		//~ affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if(est_premier(_expression_,uniteCourante))
		{
			$1 = expression();
			if (uniteCourante==FAIRE)
			{
				nom_token(uniteCourante,nom,valeur);
				//~ affiche_element(nom,valeur,1);
				uniteCourante=yylex();
				if(est_premier(_instructionBloc_,uniteCourante))
				{
					$2 = instructionBloc();
					$$ = cree_n_instr_tantque($1, $2);
					//~ affiche_balise_fermante(__FUNCTION__,1);
					return $$;
				}
				else {
					printf("Expression incorrecte !\n");
					exit (-1);
				}
			}
			else {
				printf("Expression incorrecte !\n");
				exit (-1);
			}
		}
		else {
			printf("Expression incorrecte !\n");
			exit (-1);
		}
	}
	else {
		printf("Expression incorrecte !\n");
		exit (-1);
	}
	//~ affiche_balise_fermante(__FUNCTION__,1);
}		

n_instr *instructionAppel(void)
{
	n_instr *$$ = NULL;
	n_appel *$1 = NULL;
	
	//~ affiche_balise_ouvrante(__FUNCTION__,1);
	if(est_premier(_appelFct_,uniteCourante))
	{
		$1 = appelFct();
		if (uniteCourante==POINT_VIRGULE)
		{
			nom_token(uniteCourante,nom,valeur);
			//~ affiche_element(nom,valeur,1);
			uniteCourante=yylex();
			$$ = cree_n_instr_appel($1);
			//~ affiche_balise_fermante(__FUNCTION__,1);
			return $$;
		}
		else {
			printf("Expression incorrecte !\n");
			exit (-1);
		}
	}
	else {
		printf("Expression incorrecte !\n");
		exit (-1);
	}
	//~ affiche_balise_fermante(__FUNCTION__,1);
}

n_instr *instructionRetour(void)
{
	n_instr *$$ = NULL;
	n_exp *$1 = NULL;
	
	//~ affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==RETOUR)
	{
		nom_token(uniteCourante,nom,valeur);
		//~ affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if(est_premier(_expression_,uniteCourante))
		{
			$1 = expression();
			if (uniteCourante==POINT_VIRGULE)
			{
				nom_token(uniteCourante,nom,valeur);
				//~ affiche_element(nom,valeur,1);
				uniteCourante=yylex();
				$$ = cree_n_instr_retour($1);
				//~ affiche_balise_fermante(__FUNCTION__,1);
				return $$;
			}
			else {
				printf("Expression incorrecte !\n");
				exit (-1);
			}
		}
		else {
			printf("Expression incorrecte !\n");
			exit (-1);
		}
	}
	else {
		printf("Expression incorrecte !\n");
		exit (-1);
	}
	//~ affiche_balise_fermante(__FUNCTION__,1);
}

n_instr *instructionEcriture(void)
{
	n_instr *$$ = NULL;
	n_exp *$1 = NULL;
	
	//~ affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==ECRIRE)
	{
		nom_token(uniteCourante,nom,valeur);
		//~ affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if (uniteCourante==PARENTHESE_OUVRANTE)
		{
			nom_token(uniteCourante,nom,valeur);
			//~ affiche_element(nom,valeur,1);
			uniteCourante=yylex();
			if(est_premier(_expression_,uniteCourante))
			{
				$1 = expression();
				if (uniteCourante==PARENTHESE_FERMANTE)
				{
					nom_token(uniteCourante,nom,valeur);
					//~ affiche_element(nom,valeur,1);
					uniteCourante=yylex();
					if (uniteCourante==POINT_VIRGULE)
					{
						nom_token(uniteCourante,nom,valeur);
						//~ affiche_element(nom,valeur,1);
						uniteCourante=yylex();
						$$ = cree_n_instr_ecrire($1);
						//~ affiche_balise_fermante(__FUNCTION__,1);
						return $$;
					}
					else {
						printf("Expression incorrecte !\n");
						exit (-1);
					}
				}
				else {
					printf("Expression incorrecte !\n");
					exit (-1);
				}
			}
			else {
				printf("Expression incorrecte !\n");
				exit (-1);
			}
		}
		else {
			printf("Expression incorrecte !\n");
			exit (-1);
		}
	}
	else {
		printf("Expression incorrecte !\n");
		exit (-1);
	}
	//~ affiche_balise_fermante(__FUNCTION__,1);
}

n_instr *instructionVide(void)
{
	n_instr *$$ = NULL;
	
	//~ affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==POINT_VIRGULE)
	{
		nom_token(uniteCourante,nom,valeur);
		//~ affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		//~ affiche_balise_fermante(__FUNCTION__,1);
		return $$;
	}
	else {
		printf("Expression incorrecte !\n");
		exit (-1);
	}
	//~ affiche_balise_fermante(__FUNCTION__,1);
}

n_exp *expression(void)
{
	n_exp *$1 = NULL;
	n_exp *$2 = NULL;
	//~ affiche_balise_ouvrante(__FUNCTION__,1);
	if (est_premier(_conjonction_,uniteCourante))
	{
		$1 = conjonction();
		$2 = expressionBis($1);
		//~ affiche_balise_fermante(__FUNCTION__,1);
		return $2;
	}
	else {
		printf("Expression incorrecte !\n");
		exit (-1);
	}
	//~ affiche_balise_fermante(__FUNCTION__,1);
}

n_exp *expressionBis(n_exp *h)
{
	n_exp *$$ = NULL;
	n_exp *$1 = NULL;
	n_exp *$2 = NULL;
	
	//~ affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==OU)
	{
		nom_token(uniteCourante,nom,valeur);
		//~ affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if (est_premier(_conjonction_,uniteCourante))
		{
			$1 = conjonction();
			$2 = expressionBis($1);
			$$ = cree_n_exp_op(ou,h,$2);
			//~ affiche_balise_fermante(__FUNCTION__,1);
			return $$;
		}
		else {
			printf("Expression incorrecte !\n");
			exit (-1);
		}
	}
	else if(est_suivant(_expressionBis_,uniteCourante))
		{
			//~ affiche_balise_fermante(__FUNCTION__,1);
			return h;
		}
	else {
		printf("Expression incorrecte !\n");
		exit (-1);
	}
	//~ affiche_balise_fermante(__FUNCTION__,1);
}

n_exp *conjonction(void)
{
	n_exp *$1 = NULL;
	n_exp *$2 = NULL;
	
	//~ affiche_balise_ouvrante(__FUNCTION__,1);
	if (est_premier(_comparaison_,uniteCourante))
	{
		$1 = comparaison();
		$2 = conjonctionBis($1);
		//~ affiche_balise_fermante(__FUNCTION__,1);
		return $2;
	}
	else {
		printf("Expression incorrecte !\n");
		exit (-1);
	}
	//~ affiche_balise_fermante(__FUNCTION__,1);
}

n_exp *conjonctionBis(n_exp *h)
{
	n_exp *$$ = NULL;
	n_exp *$1 = NULL;
	n_exp *$2 = NULL;
	
	//~ affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==ET)
	{
		nom_token(uniteCourante,nom,valeur);
		//~ affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if (est_premier(_comparaison_,uniteCourante))
		{
			$1 = comparaison();
			$2 = conjonctionBis($1);
			$$ = cree_n_exp_op(et,h,$2);
			//~ affiche_balise_fermante(__FUNCTION__,1);
			return $$;
		}
		else {
			printf("Expression incorrecte !\n");
			exit (-1);
		}
	}
	else if(est_suivant(_conjonctionBis_,uniteCourante))
		{
			//~ affiche_balise_fermante(__FUNCTION__,1);
			return h;
		}
	else {
		printf("Expression incorrecte !\n");
		exit (-1);
	}
	//~ affiche_balise_fermante(__FUNCTION__,1);
}

n_exp *comparaison(void)
{
	n_exp *$1 = NULL;
	n_exp *$2 = NULL;
	//~ affiche_balise_ouvrante(__FUNCTION__,1);
	if(est_premier(_expArith_,uniteCourante))
	{
		$1 = expArith();
		$2 = comparaisonBis($1);
		//~ affiche_balise_fermante(__FUNCTION__,1);
		return $2;
	}
	else {
		printf("Expression incorrecte !\n");
		exit (-1);
	}
	//~ affiche_balise_fermante(__FUNCTION__,1);
}

n_exp *comparaisonBis(n_exp *h)
{
	n_exp *$$ = NULL;
	n_exp *$1 = NULL;
	n_exp *$2 = NULL;
	//~ affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==EGAL) 
	{
		nom_token(uniteCourante,nom,valeur);
		//~ affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if (est_premier(_expArith_,uniteCourante))
		{
			$1 = expArith();
			$2 = comparaisonBis($1);
			$$ = cree_n_exp_op(egal,h,$2);
			//~ affiche_balise_fermante(__FUNCTION__,1);
			return $$;
		}
		else {
			printf("Expression incorrecte !\n");
			exit (-1);
		}
	}
	else if(uniteCourante==INFERIEUR)
		{
			nom_token(uniteCourante,nom,valeur);
			//~ affiche_element(nom,valeur,1);
			uniteCourante=yylex();
			if (est_premier(_expArith_,uniteCourante))
			{
				$1 = expArith();
				$2 = comparaisonBis($1);
				$$ = cree_n_exp_op(inf,h,$2);
				//~ affiche_balise_fermante(__FUNCTION__,1);
				return $$;
			}
			else {
				printf("Expression incorrecte !\n");
				exit (-1);
			}
		}
		else if(est_suivant(_comparaisonBis_,uniteCourante))
			{
				//~ affiche_balise_fermante(__FUNCTION__,1);
				return h;
			}
	else {
		printf("Expression incorrecte !\n");
		exit (-1);
	}
	//~ affiche_balise_fermante(__FUNCTION__,1);
}

n_exp *expArith(void)
{
	n_exp *$1 = NULL;
	n_exp *$2 = NULL;
	//~ affiche_balise_ouvrante(__FUNCTION__,1);
	if (est_premier(_terme_,uniteCourante))
	{
		$1 = terme();
		$2 = expArithBis($1);
		//~ affiche_balise_fermante(__FUNCTION__,1);
		return $2;
	}
	else {
		printf("Expression incorrecte !\n");
		exit (-1);
	}
	//~ affiche_balise_fermante(__FUNCTION__,1);
}

n_exp *expArithBis(n_exp *h)
{
	n_exp *$$ = NULL;
	n_exp *$1 = NULL;
	n_exp *$2 = NULL;
	//~ affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==PLUS)
	{
		nom_token(uniteCourante,nom,valeur);
		//~ affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if (est_premier(_terme_,uniteCourante))
		{
			$1 = terme();
			$2 = expArithBis($1);
			$$ = cree_n_exp_op(plus,h,$2);
			//~ affiche_balise_fermante(__FUNCTION__,1);
			return $$;
		}
		else {
			printf("Expression incorrecte !\n");
			exit (-1);
		}
	}
	else if(uniteCourante==MOINS) 
		{
			
			nom_token(uniteCourante,nom,valeur);
			//~ affiche_element(nom,valeur,1);
			uniteCourante=yylex();
			if (est_premier(_terme_,uniteCourante))
			{
				$1 = terme();
				$2 = expArithBis($1);
				$$ = cree_n_exp_op(moins,h,$2);
				//~ affiche_balise_fermante(__FUNCTION__,1);
				return $$;
			}
			else {
				printf("Expression incorrecte !\n");
				exit (-1);
			}
		}
		else if(est_suivant(_expArithBis_,uniteCourante))
		{
			//~ affiche_balise_fermante(__FUNCTION__,1);
			return h;
		}
	else {
		printf("Expression incorrecte !\n");
		exit (-1);
	}
	//~ affiche_balise_fermante(__FUNCTION__,1);
}

n_exp *terme(void)
{
	n_exp *$1 = NULL;
	n_exp *$2 = NULL;
	//~ affiche_balise_ouvrante(__FUNCTION__,1);
	if(est_premier(_negation_,uniteCourante))
	{
		$1 = negation();
		$2 = termeBis($1);
		//~ affiche_balise_fermante(__FUNCTION__,1);
		return $2;
	}
	else {
		printf("Expression incorrecte !\n");
		exit (-1);
	}
	//~ affiche_balise_fermante(__FUNCTION__,1);
}

n_exp *termeBis(n_exp *h)
{
	n_exp *$$ = NULL;
	n_exp *$1 = NULL;
	n_exp *$2 = NULL;
	//~ affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==FOIS)
	{
		nom_token(uniteCourante,nom,valeur);
		//~ affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if (est_premier(_negation_,uniteCourante))
		{
			$1 = negation();
			$2 = termeBis($1);
			$$ = cree_n_exp_op(fois,h,$2);
			//~ affiche_balise_fermante(__FUNCTION__,1);
			return $$;
		}
		else {
			printf("Expression incorrecte !\n");
			exit (-1);
		}
	}
	else if(uniteCourante==DIVISE)
	{
		nom_token(uniteCourante,nom,valeur);
		//~ affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if (est_premier(_negation_,uniteCourante))
		{
			$1 = negation();
			$2 = termeBis($1);
			$$ = cree_n_exp_op(divise,h,$2);
			//~ affiche_balise_fermante(__FUNCTION__,1);
			return $$;
		}
		else {
			printf("Expression incorrecte !\n");
			exit (-1);
		}
	}
	else if(est_suivant(_termeBis_,uniteCourante))
	{
		//~ affiche_balise_fermante(__FUNCTION__,1);
		return h;
	}
	else {
		printf("Expression incorrecte !\n");
		exit (-1);
	}
	//~ affiche_balise_fermante(__FUNCTION__,1);
}

n_exp *negation(void)
{
	n_exp *$1 = NULL;
	n_exp *$$ = NULL;
	
	//~ affiche_balise_ouvrante(__FUNCTION__,1);
	if(uniteCourante==NON)
	{
		nom_token(uniteCourante,nom,valeur);
		//~ affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if (est_premier(_negation_,uniteCourante))
		{
			$1 = negation();
			$$ = cree_n_exp_op(non, $1, NULL);
			//~ affiche_balise_fermante(__FUNCTION__,1);
			return $$;
		}
		else {
			printf("Expression incorrecte !\n");
			exit (-1);
		}
	}
	else if(est_premier(_facteur_,uniteCourante))
	{
		$1 = facteur();
		//~ affiche_balise_fermante(__FUNCTION__,1);
		return $1;
	}
	else {
		printf("Expression incorrecte !\n");
		exit (-1);
	}
	//~ affiche_balise_fermante(__FUNCTION__,1);
}

n_exp *facteur(void)
{
	n_exp *$$ = NULL;
	n_appel *$1 = NULL;
	n_var *$2 = NULL;
	
	//~ affiche_balise_ouvrante(__FUNCTION__,1);
	if(uniteCourante==PARENTHESE_OUVRANTE)
	{
		nom_token(uniteCourante,nom,valeur);
		//~ affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if(est_premier(_expression_,uniteCourante))
		{
			$$ = expression();
			if(uniteCourante==PARENTHESE_FERMANTE)
			{
				nom_token(uniteCourante,nom,valeur);
				//~ affiche_element(nom,valeur,1);
				uniteCourante=yylex();
				//~ affiche_balise_fermante(__FUNCTION__,1);
				return $$;
			}
			else {
				printf("Expression incorrecte !\n");
				exit (-1);
			}
		}
		else {
			printf("Expression incorrecte !\n");
			exit (-1);
		}
	}
	else if(uniteCourante==NOMBRE)
	{
		nom_token(uniteCourante,nom,valeur);
		//~ affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		//~ affiche_balise_fermante(__FUNCTION__,1);
		$$ = cree_n_exp_entier(atoi(valeur));
		return $$;
	}
	else if(est_premier(_appelFct_,uniteCourante))
	{
		$1 = appelFct();
		$$ = cree_n_exp_appel($1);
		//~ affiche_balise_fermante(__FUNCTION__,1);
		return $$;
	}
	else if(est_premier(_var_,uniteCourante))
	{
		$2 = var();
		$$ = cree_n_exp_var($2);
		//~ affiche_balise_fermante(__FUNCTION__,1);
		return $$;
	}
	else if(uniteCourante==LIRE) 
	{
		nom_token(uniteCourante,nom,valeur);
		//~ affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if(uniteCourante==PARENTHESE_OUVRANTE) 
		{
			nom_token(uniteCourante,nom,valeur);
			//~ affiche_element(nom,valeur,1);
			uniteCourante=yylex();
			if(uniteCourante==PARENTHESE_FERMANTE) 
			{
				nom_token(uniteCourante,nom,valeur);
				//~ affiche_element(nom,valeur,1);
				uniteCourante=yylex();
				$$ = cree_n_exp_lire();
				//~ affiche_balise_fermante(__FUNCTION__,1);
				return $$;
			}
			else {
				printf("Expression incorrecte !\n");
				exit (-1);
			}
		}
		else {
			printf("Expression incorrecte !\n");
			exit (-1);
		}
	}
	else {
		printf("Expression incorrecte !\n");
		exit (-1);
	}
	//~ affiche_balise_fermante(__FUNCTION__,1);
}

n_var *var(void)
{
	n_var *$$ = NULL;
	n_exp *$1 = NULL;
	char *cpy_valeur = malloc(sizeof(valeur));
	
	//~ affiche_balise_ouvrante(__FUNCTION__,1);
	if(uniteCourante==ID_VAR)
	{
		nom_token(uniteCourante,nom,valeur);
		strcpy(cpy_valeur,valeur);
		//~ affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if(est_premier(_optIndice_,uniteCourante) || est_suivant(_var_,uniteCourante))
		{
			$1 = optIndice();
			if($1 == NULL) $$ = cree_n_var_simple(cpy_valeur);
			else $$ = cree_n_var_indicee(cpy_valeur, $1);
			//~ affiche_balise_fermante(__FUNCTION__,1);
			return $$;
		}
		else {
			printf("Expression incorrecte !\n");
			exit (-1);
		}
	}
	else {
		printf("Expression incorrecte !\n");
		exit (-1);
	}
	//~ affiche_balise_fermante(__FUNCTION__,1);
}

n_exp *optIndice(void)
{
	n_exp *$$ = NULL;
	//~ affiche_balise_ouvrante(__FUNCTION__,1);
	if(uniteCourante==CROCHET_OUVRANT)
	{
		nom_token(uniteCourante,nom,valeur);
		//~ affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if(est_premier(_expression_,uniteCourante))
		{
			$$ = expression();
			if(uniteCourante==CROCHET_FERMANT)
			{
				nom_token(uniteCourante,nom,valeur);
				//~ affiche_element(nom,valeur,1);
				uniteCourante=yylex();
				//~ affiche_balise_fermante(__FUNCTION__,1);
				return $$;
			}
			else {
				printf("Expression incorrecte !\n");
				exit (-1);
			}
		}
		else {
			printf("Expression incorrecte !\n");
			exit (-1);
		}
	}
		
	if(est_suivant(_optIndice_,uniteCourante))
	{
		//~ affiche_balise_fermante(__FUNCTION__,1);
		return $$;
	}
	else {
		printf("Expression incorrecte !\n");
		exit (-1);
	}
	//~ affiche_balise_fermante(__FUNCTION__,1);
}

n_appel *appelFct(void)
{
	n_appel *$$ = NULL;
	n_l_exp *$1 = NULL;
	char *cpy_valeur = malloc(sizeof(valeur));
	
	//~ affiche_balise_ouvrante(__FUNCTION__,1);
	if(uniteCourante==ID_FCT)
	{
		nom_token(uniteCourante,nom,valeur);
		strcpy(cpy_valeur,valeur);
		//~ affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if(uniteCourante==PARENTHESE_OUVRANTE)
		{
			nom_token(uniteCourante,nom,valeur);
			//~ affiche_element(nom,valeur,1);
			uniteCourante=yylex();
			if(est_premier(_listeExpressions_,uniteCourante) || est_suivant(_listeExpressions_,uniteCourante))
			{
				$1 = listeExpressions();
				if(uniteCourante==PARENTHESE_FERMANTE)
				{
					nom_token(uniteCourante,nom,valeur);
					//~ affiche_element(nom,valeur,1);
					$$ = cree_n_appel(cpy_valeur, $1);
					uniteCourante=yylex();
					//~ affiche_balise_fermante(__FUNCTION__,1);
					return $$;
				}
				else {
					printf("Expression incorrecte !\n");
					exit (-1);
				}
			}
			else {
				printf("Expression incorrecte !\n");
				exit (-1);
			}	
		}
		else {
			printf("Expression incorrecte !\n");
			exit (-1);
		}
	}
	else {
		printf("Expression incorrecte !\n");
		exit (-1);
	}
	//~ affiche_balise_fermante(__FUNCTION__,1);
}

n_l_exp *listeExpressions(void)
{
	n_l_exp *$$ = NULL;
	n_exp *$1 = NULL;
	n_l_exp *$2 = NULL;
	
	//~ affiche_balise_ouvrante(__FUNCTION__,1);
	if(est_premier(_expression_,uniteCourante))
	{
		$1 = expression();
		$2 = listeExpressionsBis();
		$$ = cree_n_l_exp($1, $2);
		//~ affiche_balise_fermante(__FUNCTION__,1);
		return $$ ;
	}
	else if(est_suivant(_listeExpressions_,uniteCourante))
	{
		//~ affiche_balise_fermante(__FUNCTION__,1);
		return $$;
	}
	else {
		printf("Expression incorrecte !\n");
		exit (-1);
	}
	//~ affiche_balise_fermante(__FUNCTION__,1);
}

n_l_exp *listeExpressionsBis(void)
{
	n_l_exp *$$ = NULL;
	n_exp *$1 = NULL;
	n_l_exp *$2 = NULL;
	
	//~ affiche_balise_ouvrante(__FUNCTION__,1);
	if(uniteCourante==VIRGULE)
	{
		nom_token(uniteCourante,nom,valeur);
		//~ affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if(est_premier(_expression_,uniteCourante))
		{
			$1 = expression();
			$2 = listeExpressionsBis();
			$$ = cree_n_l_exp($1, $2);
			//~ affiche_balise_fermante(__FUNCTION__,1);
			return $$;
		}
		else {
			printf("Expression incorrecte !\n");
			exit (-1);
		}
	}
	else if(est_suivant(_listeExpressionsBis_,uniteCourante))
		{
			//~ affiche_balise_fermante(__FUNCTION__,1);
			return $$;
		}
	else {
		printf("Expression incorrecte !\n");
		exit (-1);
	}
	//~ affiche_balise_fermante(__FUNCTION__,1);
}

n_instr *instructionFaire(void)
{
	n_instr *$$ = NULL;
	n_instr *$1 = NULL;
	n_exp *$2 = NULL;
	
	//~ affiche_balise_ouvrante(__FUNCTION__,1);
	if(uniteCourante==FAIRE)
	{
		nom_token(uniteCourante,nom,valeur);
		//~ affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if(est_premier(_instructionBloc_,uniteCourante))
		{
			$1 = instructionBloc();
			if(uniteCourante==TANTQUE)
			{
				nom_token(uniteCourante,nom,valeur);
				//~ affiche_element(nom,valeur,1);
				uniteCourante=yylex();
				if(est_premier(_expression_,uniteCourante))
				{
					$2 = expression();
					if(uniteCourante==POINT_VIRGULE)
					{
						nom_token(uniteCourante,nom,valeur);
						//~ affiche_element(nom,valeur,1);
						uniteCourante=yylex();
						$$ = cree_n_instr_faire($1, $2);
						//~ affiche_balise_fermante(__FUNCTION__,1);
						return $$;
					}
					else {
						printf("Expression incorrecte !\n");
						exit (-1);
					}
				}
				else {
					printf("Expression incorrecte !\n");
					exit (-1);
				}
			}
			else {
				printf("Expression incorrecte !\n");
				exit (-1);
			}
		}
		else {
			printf("Expression incorrecte !\n");
			exit (-1);
		}
	}
	else {
		printf("Expression incorrecte !\n");
		exit (-1);
	}
	//~ affiche_balise_fermante(__FUNCTION__,1);
}

int main(int argc, char **argv)
{
	int i;
	n_prog *$$;
	initialise_premiers();
	initialise_suivants();
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
		if(yyin == NULL)
		{
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
		
		uniteCourante = yylex();
		$$=programme();
		affiche_n_prog($$);		//Arbre abstrait
		parcours_n_prog($$);
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

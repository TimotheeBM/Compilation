#include "parcours.h"

int portee=P_VARIABLE_GLOBALE;
int adresseLocaleCourante=0;
int adresseArgumentCourant=0;
/*-------------------------------------------------------------------------*/

void parcours_n_prog(n_prog *n)
{
	char *fct = "prog";
	parcours_l_dec(n->variables);
	parcours_l_dec(n->fonctions);
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/

void parcours_l_instr(n_l_instr *n)
{
  char *fct = "l_instr";
  if(n){
  //~ parcours_balise_ouvrante(fct, trace_abs);
  parcours_instr(n->tete);
  parcours_l_instr(n->queue);
  //~ parcours_balise_fermante(fct, trace_abs);
  }
}

/*-------------------------------------------------------------------------*/

void parcours_instr(n_instr *n)
{
  if(n){
    if(n->type == blocInst) parcours_l_instr(n->u.liste);
    else if(n->type == affecteInst) parcours_instr_affect(n);
    else if(n->type == siInst) parcours_instr_si(n);
    else if(n->type == tantqueInst) parcours_instr_tantque(n);
    else if(n->type == faireInst) parcours_instr_faire(n);
    //~ else if(n->type == pourInst) parcours_instr_pour(n);    
    else if(n->type == appelInst) parcours_instr_appel(n);
    else if(n->type == retourInst) parcours_instr_retour(n);
    else if(n->type == ecrireInst) parcours_instr_ecrire(n);
  }
}

/*-------------------------------------------------------------------------*/

void parcours_instr_si(n_instr *n)
{  
  char *fct = "instr_si";
  //~ parcours_balise_ouvrante(fct, trace_abs);

  parcours_exp(n->u.si_.test);
  parcours_instr(n->u.si_.alors);
  if(n->u.si_.sinon){
    parcours_instr(n->u.si_.sinon);
  }
  //~ parcours_balise_fermante(fct, trace_abs);
}

/*-------------------------------------------------------------------------*/

void parcours_instr_tantque(n_instr *n)
{
  char *fct = "instr_tantque";
  //~ parcours_balise_ouvrante(fct, trace_abs);

  parcours_exp(n->u.tantque_.test);
  parcours_instr(n->u.tantque_.faire);
  //~ parcours_balise_fermante(fct, trace_abs);
}

/*-------------------------------------------------------------------------*/

void parcours_instr_faire(n_instr *n)          /* MODIFIE POUR EVAL */
{                                             /* MODIFIE POUR EVAL */
  char *fct = "instr_faire";                  /* MODIFIE POUR EVAL */
  //~ parcours_balise_ouvrante(fct, trace_abs);    /* MODIFIE POUR EVAL */
  parcours_instr(n->u.faire_.faire);           /* MODIFIE POUR EVAL */
  parcours_exp(n->u.faire_.test);              /* MODIFIE POUR EVAL */
  //~ parcours_balise_fermante(fct, trace_abs);    /* MODIFIE POUR EVAL */
}                                             /* MODIFIE POUR EVAL */

/*-------------------------------------------------------------------------*/

void parcours_instr_pour(n_instr *n)                /* MODIFIE POUR EVAL */
{                                                  /* MODIFIE POUR EVAL */
  //~ char *fct = "instr_pour";                        /* MODIFIE POUR EVAL */
  //~ parcours_instr(n->u.pour_.init);                  /* MODIFIE POUR EVAL */
  //~ parcours_exp(n->u.pour_.test);                    /* MODIFIE POUR EVAL */
  //~ parcours_instr(n->u.pour_.faire);                 /* MODIFIE POUR EVAL */
  //~ parcours_instr(n->u.pour_.incr);                  /* MODIFIE POUR EVAL */
}                                                  /* MODIFIE POUR EVAL */

/*-------------------------------------------------------------------------*/

void parcours_instr_affect(n_instr *n)
{
	char *fct = "instr_affect";
	
	if(rechercheExecutable(n->u.affecte_.var->nom)!=-1){
		parcours_var(n->u.affecte_.var);
		parcours_exp(n->u.affecte_.exp);
	}
	
}

/*-------------------------------------------------------------------------*/

void parcours_instr_appel(n_instr *n)
{
  char *fct = "instr_appel";
  //~ parcours_balise_ouvrante(fct, trace_abs);


  parcours_appel(n->u.appel);
  //~ parcours_balise_fermante(fct, trace_abs);
}
/*-------------------------------------------------------------------------*/

void parcours_appel(n_appel *n)
{
	char *fct = "appel";
	if(rechercheExecutable(n->fonction)!=-1){
		parcours_l_exp(n->args);
	}
	else
	{
		printf("La fonction %s n'est pas déclarée\n",n->fonction);
	}
}

/*-------------------------------------------------------------------------*/

void parcours_instr_retour(n_instr *n)
{
  char *fct = "instr_retour";
  //~ parcours_balise_ouvrante(fct, trace_abs);
  parcours_exp(n->u.retour_.expression);
  //~ parcours_balise_fermante(fct, trace_abs);

}

/*-------------------------------------------------------------------------*/

void parcours_instr_ecrire(n_instr *n)
{
  char *fct = "instr_ecrire";
  //~ parcours_balise_ouvrante(fct, trace_abs);
  parcours_exp(n->u.ecrire_.expression);
  //~ parcours_balise_fermante(fct, trace_abs);
}

/*-------------------------------------------------------------------------*/

void parcours_l_exp(n_l_exp *n)
{
  char *fct = "l_exp";
  //~ parcours_balise_ouvrante(fct, trace_abs);

  if(n){
    parcours_exp(n->tete);
    parcours_l_exp(n->queue);
  }
  //~ parcours_balise_fermante(fct, trace_abs);
}

/*-------------------------------------------------------------------------*/

void parcours_exp(n_exp *n)
{
	if(n->type == varExp) parcours_varExp(n);
	else if(n->type == opExp) parcours_opExp(n);
	else if(n->type == intExp) parcours_intExp(n);
	else if(n->type == appelExp) parcours_appelExp(n);
	else if(n->type == lireExp) parcours_lireExp(n);
}

/*-------------------------------------------------------------------------*/

void parcours_varExp(n_exp *n)
{
	char *fct = "varExp";
	if (rechercheExecutable(n->u.var->nom)==-1){
		parcours_var(n->u.var);
	}
}

/*-------------------------------------------------------------------------*/
void parcours_opExp(n_exp *n)
{
  char *fct = "opExp";
 
  if( n->u.opExp_.op1 != NULL ) {
    parcours_exp(n->u.opExp_.op1);
  }
  if( n->u.opExp_.op2 != NULL ) {
    parcours_exp(n->u.opExp_.op2);
  }
}

/*-------------------------------------------------------------------------*/

void parcours_intExp(n_exp *n)
{
	char texte[ 50 ]; // Max. 50 chiffres
	sprintf(texte, "%d", n->u.entier);
}

/*-------------------------------------------------------------------------*/
void parcours_lireExp(n_exp *n)
{
	char *fct = "lireExp";
}

/*-------------------------------------------------------------------------*/

void parcours_appelExp(n_exp *n)
{
	char *fct = "appelExp";
	//~ parcours_balise_ouvrante(fct, trace_abs);
	parcours_appel(n->u.appel);
	//~ parcours_balise_fermante(fct, trace_abs);
}

/*-------------------------------------------------------------------------*/

void parcours_l_dec(n_l_dec *n)
{
	char *fct = "l_dec";
	if( n ){
		parcours_dec(n->tete);
		parcours_l_dec(n->queue);
	}
}

int nb_param(n_l_dec *n)
{
	int x=0;
	if(n!=NULL){
		x=1+nb_param(n->queue);
	}
	return x;
}

/*-------------------------------------------------------------------------*/

void parcours_dec(n_dec *n)
{	
	if(n){
		if(n->type == foncDec) {
			parcours_foncDec(n);
		}
		else if(n->type == varDec) {
			parcours_varDec(n);
		}
		else if(n->type == tabDec) { 
			parcours_tabDec(n);
		}
	}
	
}

/*-------------------------------------------------------------------------*/

void parcours_foncDec(n_dec *n)
{
	ajouteIdentificateur(n->nom,portee,T_FONCTION,0,nb_param(n->u.foncDec_.param));
	entreeFonction();
	char *fct = "foncDec";
	
	portee=P_ARGUMENT;
	parcours_l_dec(n->u.foncDec_.param);		//arguments
	
	portee=P_VARIABLE_LOCALE;
	adresseLocaleCourante=0;
	parcours_l_dec(n->u.foncDec_.variables);
	parcours_instr(n->u.foncDec_.corps);
	afficheTabsymboles();						//Affchage de la table de symboles
	sortieFonction();
	
}

/*-------------------------------------------------------------------------*/

void parcours_varDec(n_dec *n)
{
	if(rechercheDeclarative(n->nom)==-1){
		ajouteIdentificateur(n->nom, portee, T_ENTIER, adresseLocaleCourante, 1);
		adresseLocaleCourante+=4;
	}
}

/*-------------------------------------------------------------------------*/

void parcours_tabDec(n_dec *n)
{
	char texte[100]; // Max. 100 chars nom tab + taille
	sprintf(texte, "%s[%d]", n->nom, n->u.tabDec_.taille);
	ajouteIdentificateur(n->nom,portee,T_TABLEAU_ENTIER,adresseLocaleCourante, n->u.tabDec_.taille);
}

/*-------------------------------------------------------------------------*/

void parcours_var(n_var *n)
{
  if(n->type == simple) {
    parcours_var_simple(n);
  }
  else if(n->type == indicee) {
    parcours_var_indicee(n);
  }
}

/*-------------------------------------------------------------------------*/
void parcours_var_simple(n_var *n)
{
  //~ parcours_element("var_simple", n->nom, trace_abs);
}

/*-------------------------------------------------------------------------*/
void parcours_var_indicee(n_var *n)
{
  char *fct = "var_indicee";
  //~ parcours_element("var_base_tableau", n->nom, trace_abs);
  parcours_exp( n->u.indicee_.indice );

}
/*-------------------------------------------------------------------------*/

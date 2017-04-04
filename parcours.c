#include "parcours.h"
#include "syntabs.h"

int portee=P_VARIABLE_GLOBALE;
int adresseLocaleCourante=0;
int adresseArgumentCourant=0;
int ind_fonc=0;
/*-------------------------------------------------------------------------*/

void parcours_n_prog(n_prog *n)
{
	//~ FINI
	char *fct = "prog";
	printf("\n%%include \"io.asm\"\nsection .bss\n	sinput: resb 255\n");
	parcours_l_dec(n->variables);
	printf("section .text\nglobal _start\n_start:\n	call main\n	mov eax, 1\n	int 0x80\n");
	parcours_l_dec(n->fonctions);
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/

void parcours_l_instr(n_l_instr *n)
{
  char *fct = "l_instr";
  if(n){
  parcours_instr(n->tete);
  parcours_l_instr(n->queue);
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
  //~ FINI
  char *fct = "instr_si";
  char debut[30], suite[30], sinon[30];
  sprintf(debut,"debut_%p",debut);
  sprintf(suite,"suite_%p",suite);
  sprintf(sinon,"sinon_%p",sinon);
  
  parcours_exp(n->u.si_.test);
  printf("	pop eax\n	cmp eax, 0\n	je %s\n",sinon);
  parcours_instr(n->u.si_.alors);
  if(n->u.si_.sinon){
	printf("	jmp %s\n",suite);
	printf("%s:\n",sinon);
    parcours_instr(n->u.si_.sinon);
  }
  printf("%s:\n",suite);
}

/*-------------------------------------------------------------------------*/

void parcours_instr_tantque(n_instr *n)
{
  //~ FINI
  char *fct = "instr_tantque";
  char debut[30], suite[30];
  sprintf(debut,"debut_%p",debut);
  sprintf(suite,"suite_%p",suite);
  
  printf("%s:\n",debut);
  parcours_exp(n->u.tantque_.test);
  printf("	pop eax\n	cmp eax, 0\n	je %s\n",suite);
  parcours_instr(n->u.tantque_.faire);
  printf("	j %s\n%s:\n",debut,suite);
}

/*-------------------------------------------------------------------------*/

void parcours_instr_faire(n_instr *n)
{
  char *fct = "instr_faire";
  parcours_instr(n->u.faire_.faire);
  parcours_exp(n->u.faire_.test);
}
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
	//~ FINI
	char *fct = "instr_affect";
	int k = rechercheExecutable(n->u.affecte_.var->nom);
	
	if(k !=-1){
		parcours_var(n->u.affecte_.var);
		parcours_exp(n->u.affecte_.exp);
		printf("	pop ebx\n");
		if(tabsymboles.tab[k].portee == P_VARIABLE_GLOBALE)
		{
			if(tabsymboles.tab[k].type == T_TABLEAU_ENTIER)
			{
				printf("	pop eax\n	imul eax, 4\n	mov[%s + eax], ebx\n",n->u.affecte_.var->nom);
			}
			else printf("	mov [%s], ebx\n",n->u.affecte_.var->nom);
		}
		else if (tabsymboles.tab[k].portee == P_VARIABLE_LOCALE) printf("	mov [ebp %d], ebx\n",-4-tabsymboles.tab[k].adresse);
	}
	else
	{
		printf("Erreur : variable %s non déclaré\n", n->u.affecte_.var->nom);
		exit(-1);
	}
	
}

/*-------------------------------------------------------------------------*/

void parcours_instr_appel(n_instr *n)
{
  char *fct = "instr_appel";
  parcours_appel(n->u.appel);
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
  parcours_exp(n->u.retour_.expression);
  printf("	pop	eax\n	mov	[ebp + %d], eax\n",8+4*tabsymboles.tab[ind_fonc].complement);
			
}

/*-------------------------------------------------------------------------*/

void parcours_instr_ecrire(n_instr *n)
{
  char *fct = "instr_ecrire";
  parcours_exp(n->u.ecrire_.expression);
  printf("	pop eax\n	call iprintLF\n");
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
	int k = rechercheExecutable(n->u.var->nom);
	if (k ==-1){
		parcours_var(n->u.var);
	}
	if(tabsymboles.tab[k].portee == P_ARGUMENT) printf("	mov ebx, [ebp + %d]\n	push ebx\n",4+4*tabsymboles.tab[ind_fonc].complement-tabsymboles.tab[k].adresse);
	else if (tabsymboles.tab[k].portee == P_VARIABLE_GLOBALE) printf("	mov ebx, %s\n	push ebx\n",n->u.var->nom);
	else if (tabsymboles.tab[k].portee == P_VARIABLE_LOCALE) printf("	\n");
	
}

/*-------------------------------------------------------------------------*/
void parcours_opExp(n_exp *n)
{
  char *fct = "opExp";
  char debut[30], suite[30], sinon[30];
  sprintf(debut,"debut_%p",debut);
  sprintf(suite,"suite_%p",suite);
  sprintf(sinon,"sinon_%p",sinon);
  
  if( n->u.opExp_.op1 != NULL ) {
    parcours_exp(n->u.opExp_.op1);
  }
  
  if( n->u.opExp_.op2 != NULL ) {
    parcours_exp(n->u.opExp_.op2);
  }  
  
  if(n->u.opExp_.op == plus) printf("	pop ebx\n	pop eax\n	add eax, ebx\n	push eax\n");
  else if(n->u.opExp_.op == moins) printf("	pop ebx\n	pop eax\n	sub eax, ebx\n	push eax\n");
  else if(n->u.opExp_.op == fois) printf("	pop ebx\n	pop eax\n	mul ebx\n	push eax\n");
  else if(n->u.opExp_.op == divise) printf("	pop ebx\n	pop eax\n	div ebx\n	push eax\n");
  else if(n->u.opExp_.op == modulo) printf("	pop ebx\n	pop eax\n	div ebx\n	push edx\n");
  
  else if(n->u.opExp_.op == egal)
  {
	  printf("	pop ebx\n	pop eax\n	cmp eax, ebx\n	je %s\n	push 0\n	jmp %s\n%s:\n	push 1\n%s:\n",sinon,suite,sinon,suite);
  }
  else if(n->u.opExp_.op == diff)
  {
	  printf("	pop ebx\n	pop eax\n	cmp eax, ebx\n	jne %s\n	push 0\n	jmp %s\n%s:\n	push 1\n%s:\n",sinon,suite,sinon,suite);
  }
  else if(n->u.opExp_.op == inf)
  {
	  printf("	pop ebx\n	pop eax\n	cmp eax, ebx\n	jl %s\n	push 0\n	jmp %s\n%s:\n	push 1\n%s:\n",sinon,suite,sinon,suite);
  }
  else if(n->u.opExp_.op == sup)
  {
	  printf("	pop ebx\n	pop eax\n	cmp eax, ebx\n	jg %s\n	push 0\n	jmp %s\n%s:\n	push 1\n%s:\n",sinon,suite,sinon,suite);
  }
  else if(n->u.opExp_.op == infeg)
  {
	  printf("	pop ebx\n	pop eax\n	cmp eax, ebx\n	jle %s\n	push 0\n	jmp %s\n%s:\n	push 1\n%s:\n",sinon,suite,sinon,suite);
  }
  else if(n->u.opExp_.op == supeg)
  {
	  printf("	pop ebx\n	pop eax\n	cmp eax, ebx\n	jge %s\n	push 0\n	jmp %s\n%s:\n	push 1\n%s:\n",sinon,suite,sinon,suite);
  }
  else if(n->u.opExp_.op == ou)
  {
	  printf("%s:\n	pop ebx\n	pop eax\n	add eax, ebx\n	cmp eax, 0\n	je %s\n	push 1\n	jmp %s\n%s:\n	push 0\n%s:\n",debut,debut,suite,debut,suite);
  }
  else if(n->u.opExp_.op == et)
  {
	  printf("%s:\n	pop ebx\n	pop eax\n	imul eax, ebx\n	cmp eax, 0\n	je %s\n	push 1\n	jmp %s\n%s:\n	push 0\n%s:\n",debut,debut,suite,debut,suite);
  }
  else if(n->u.opExp_.op == non)
  {
	  printf("%s\n	pop eax\n	cmp eax, 0\n	je %s\n	push 0\n	jmp %s\n%s:\n	push 1\n%s:",debut,debut,suite,debut,suite);
  }
  else if(n->u.opExp_.op == negatif)
  {
	  printf("negatif");
  }
}

/*-------------------------------------------------------------------------*/

void parcours_intExp(n_exp *n)
{
	char texte[ 50 ]; // Max. 50 chiffres
	sprintf(texte, "%d", n->u.entier);
	printf("	push %d\n",n->u.entier);
}

/*-------------------------------------------------------------------------*/
void parcours_lireExp(n_exp *n)
{
	char *fct = "lireExp";
	printf("	mov eax, sinput\n	call readline\n	mov eax, sinput\n	call atoi\n	push eax\n");
}

/*-------------------------------------------------------------------------*/

void parcours_appelExp(n_exp *n)
{
	char *fct = "appelExp";
	parcours_appel(n->u.appel);
}

/*-------------------------------------------------------------------------*/

void parcours_l_dec(n_l_dec *n)
{
	char *fct = "l_dec";
	if(n){
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
	char *fct = "foncDec";
	char etiquette[30];
	
	ajouteIdentificateur(n->nom,portee,T_FONCTION,0,nb_param(n->u.foncDec_.param));
	sprintf(etiquette,"%s",n->nom);
	
	entreeFonction();
	ind_fonc=rechercheExecutable(n->nom);
	
	portee=P_ARGUMENT;
	parcours_l_dec(n->u.foncDec_.param);		//arguments
	
	portee=P_VARIABLE_LOCALE;
	adresseLocaleCourante=0;
	
	printf("%s:\n	push ebp\n	mov ebp, esp\n",etiquette);
	
	parcours_l_dec(n->u.foncDec_.variables);
	parcours_instr(n->u.foncDec_.corps);
	afficheTabsymboles();						//Affchage de la table de symboles
	sortieFonction();
	printf("	pop ebp\n	ret\n");
}

/*-------------------------------------------------------------------------*/

void parcours_varDec(n_dec *n)
{
	int k = rechercheDeclarative(n->nom);
	if(k==-1){
		ajouteIdentificateur(n->nom, portee, T_ENTIER, adresseLocaleCourante, 1);
		adresseLocaleCourante+=4;
		if(portee == P_VARIABLE_GLOBALE)
		{
			printf("	%s: rest 1\n",n->nom);
		}
		else if(portee == P_VARIABLE_LOCALE)
		{
			printf("	sub esp, 4\n");
		}
	}
}

/*-------------------------------------------------------------------------*/

void parcours_tabDec(n_dec *n)
{
	char texte[100]; // Max. 100 chars nom tab + taille
	int k = rechercheDeclarative(n->nom);
	sprintf(texte, "%s[%d]", n->nom, n->u.tabDec_.taille);
	ajouteIdentificateur(n->nom,portee,T_TABLEAU_ENTIER,adresseLocaleCourante, n->u.tabDec_.taille);
	if(tabsymboles.tab[k].portee == P_VARIABLE_GLOBALE)
	{
		printf("	%s resw %d\n",n->nom,n->u.tabDec_.taille);
	}
	if(tabsymboles.tab[k].portee == P_VARIABLE_LOCALE)
	{
		
	}
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

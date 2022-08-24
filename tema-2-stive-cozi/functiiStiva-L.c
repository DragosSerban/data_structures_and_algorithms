/*-- functiiStiva-L.c -- elementele stivei sunt memorate intr-o lista --*/
#include "TStiva-L.h"

typedef void (*TF)(void*);

// creeaza stiva vida cu elemente de dimensiune d
void *InitS(size_t d)
{ 
	ASt a = (ASt)malloc(sizeof (TStiva));
  	if (!a) 
		return NULL;

  	a->dime = d;
  	a->b = NULL;
  	a->vf = NULL;

  	return (void *)a;
}

// pune element in varful stivei
int Push(void *a, void *ae)
{ 
	ACelSt aux = (ACelSt)malloc(sizeof(TCelSt));
	if(!aux)
		return 0;
	aux->info = malloc(((TStiva*)a)->dime);
	if(!aux->info)
		return 0;
  	memcpy (aux->info, ae, ((TStiva*)a)->dime); // copiaza elementul in vf
  	aux->urm = ((TStiva*)a)->vf; // deplaseaza varful stivei
  	if(((TStiva*)a)->vf)
  		((TStiva*)a)->vf->ant = aux;
  	((TStiva*)a)->vf = aux;
  	if(!(((TStiva*)a)->b))
  		((TStiva*)a)->b = aux;
  	return 1;
}

// extrage elementul din varful stivei la adresa ae
int Pop(void *a, void *ae)
{ 
	if(VIDA(a)) 
		return 0;
	memcpy(ae, ((TStiva*)a)->vf->info, ((TStiva*)a)->dime);
	ACelSt aux = ((TStiva*)a)->vf;
	((TStiva*)a)->vf = aux->urm;
	free(aux->info);
	free(aux);
  	return 1;
}

// copiaza elementul din varful stivei la adresa ae
int Top(void *a, void *ae)
{ 
	if(VIDA(a)) 
		return 0;
  	memcpy (ae, VF(a)->info, DIME(a));
  	return 1;
}

// test stiva vida
int VidaS(void *a)
{ 
	if(VIDA(a))
		return 1;
	return 0;
}

// dimensiunea elementelor din stiva
size_t DimES(void *a)
{ 
	return DIME(a); 
}

// elibereaza intregul spatiu ocupat de stiva
void DistrS(void **aa)
{
	// TODO
  	*aa = NULL;    
}

// suprapune inversul sursei destinatie
int Rastoarna(void *ad, void *as)
{
  	if(DIMDIF(as,ad)) 
		return -1;
  	if(VIDA(as)) 
		return 0;
	ACelSt ld = ((TStiva*)ad)->vf;
	ACelSt ls = ((TStiva*)as)->vf;
 	while(ls)
  	{ 
		ACelSt aux = ls;
		ls = ls->urm;
		if(ld)
			ld->ant = aux;
		else
			((TStiva*)ad)->b = aux;
		aux->urm = ld;
		ld = aux;
		if(!ls)
			ld->ant = NULL;
  	}
  	((TStiva*)as)->vf = NULL;
  	((TStiva*)as)->b = NULL;
  	((TStiva*)ad)->vf = ld;
  	return 1;
}

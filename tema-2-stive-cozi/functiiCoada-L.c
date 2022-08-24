/*-- functiiCoada-L.c -- elementele cozii sunt memorate intr-o lista --*/
#include "TCoada-L.h"

// creeaza coada vida cu elemente de dimensiune d
void* InitQ(size_t d)
{ 
	AQL a = (AQL)malloc(sizeof(TCoadaV));
  	if (!a) 
		return NULL;

 	a->dime = d; 
  	a->ic = NULL;
  	a->sc = NULL;

  	return (void *)a;
}

// adauga element la sfarsitul cozii
int IntrQ(void *a, void *ae)
{ 
	ACel aux = (ACel)malloc(sizeof(TCel));
	if(!aux)
		return 0;
	aux->info = malloc(((TCoadaV*)a)->dime);
	if(!aux->info)
		return 0;
	memcpy(aux->info, ae, ((TCoadaV*)a)->dime);
	aux->urm = NULL;
	if(VidaQ(a)) {
		((AQL)a)->ic = ((AQL)a)->sc = aux;
	} else {
		((AQL)a)->sc->urm = aux;
		((AQL)a)->sc = aux;
	}
	return 1;
}

// extrage primul element din coada la adresa ae
int ExtrQ (void *a, void *ae)
{ 
	if(VIDA(a)) 
		return 0;
	ACel aux = ((TCoadaV*)a)->ic;
	((TCoadaV*)a)->ic = aux->urm;
	memcpy(ae, aux->info, DIME(a));
	free(aux->info);
	free(aux);
	if(((TCoadaV*)a)->ic == NULL)
		((TCoadaV*)a)->sc = NULL;
  	return 1;
}

// copiaza primul element din coada la adresa ae
int PrimQ(void *a, void *ae)
{ 
	if(VIDA(a)) 
		return 0;
  	memcpy(ae, IC(a)->info, DIME(a));
	return 1;
}

// test coada vida
int VidaQ(void *a)
{ 
	return VIDA(a); 
}

// dimensiunea elementelor din coada
size_t DimEQ(void *a)
{
	return DIME(a); 
}

// transforma coada in coada vida
void ResetQ(void *a)
{
	// AQL p = ((AQL)a)->ic;
	((AQL)a)->ic = NULL;
	((AQL)a)->sc = NULL;
	// TODO distruge lista
}  

// elibereaza intregul spatiu ocupat de coada
void DistrQ(void **aa)
{
	// TODO
  	*aa = NULL;
}

// afisare elemente din coada
void AfisareQ(void *a, TF1 afi_el, FILE *g)
{
	fprintf(g, "[");
	if(VidaQ(a)) {
		fprintf(g, "]\n");
		return;
	}
	void *caux = InitQ(DIME(a));
	if(!caux)
		return;
	void *x = malloc(DIME(a));
	if(!x) {
		free(caux);
		return;
	}
	while(ExtrQ(a, x)) {
		afi_el(x, g);
		if(!VidaQ(a))
			fprintf(g, ",\n");
		IntrQ(caux, x);
	}
	ConcatQ(a, caux);
	DistrQ(caux);
	fprintf(g, "]\n");
}

// concatenare
int ConcatQ(void *ad, void *as)
{
	if(DIMDIF(as,ad))
		return -1;
  	if(VIDA(as))
		return 0;
	if(VIDA(ad)) {
		((TCoadaV*)ad)->ic = ((TCoadaV*)as)->ic;
		((TCoadaV*)ad)->sc = ((TCoadaV*)as)->sc;
		((TCoadaV*)as)->ic = ((TCoadaV*)as)->sc = NULL;
		return 1;
	}
  	((TCoadaV*)ad)->sc->urm = ((TCoadaV*)as)->ic;
  	((TCoadaV*)ad)->sc = ((TCoadaV*)as)->sc;
    	((TCoadaV*)as)->ic = ((TCoadaV*)as)->sc = NULL;
  	return 1;
}

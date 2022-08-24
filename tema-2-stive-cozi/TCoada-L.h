/*-- TCoada-L.h -- elementele cozii sunt memorate intr-o lista --*/
#include "TCoada.h"

#ifndef _COADA_LISTA_
#define _COADA_LISTA_

typedef struct cel
{
	struct cel *urm;
	void *info;
} TCel, *ACel;

typedef struct
{ 
	size_t dime; // dimensiunea unui element din coada
	ACel ic, sc;
} TCoadaV, *AQL;

/*- macrodefinitii - acces campuri -*/
#define IC(a) 	(((AQL)(a))->ic)
#define SC(a) 	(((AQL)(a))->sc)
#define DIME(a) (((AQL)(a))->dime)

/*- teste si prelucrari -*/
/* verifica daca doua cozi au elemente de dimensiuni diferite 			*/
#define DIMDIF(s,d) 	(DIME(s) != DIME(d))  
/* verifica daca o coada este vida 										*/
#define VIDA(a) 		(IC(a) == NULL && SC(a) == NULL)

#endif

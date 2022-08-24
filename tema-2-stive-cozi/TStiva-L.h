/*-- TStiva-L.h -- elementele stivei sunt memorate intr-o lista --*/
#include "TStiva.h"

#ifndef _STIVA_LISTA_
#define _STIVA_LISTA_

typedef struct celst
{
	struct celst *ant;
	struct celst *urm;
	void *info;
} TCelSt, *ACelSt;

typedef struct stiva
{ 
	size_t dime; // dimensiunea unui element al stivei
	ACelSt b;
	ACelSt vf;
} TStiva, *ASt;

/*- macrodefinitii - acces campuri */
#define DIME(a) (((ASt)(a))->dime)
#define BS(a) 	(((ASt)(a))->b)
#define VF(a) 	(((ASt)(a))->vf)

/*- teste -*/
/* verifica daca doua stive au elemente de dimensiuni diferite */
#define DIMDIF(s,d)	(DIME(s) != DIME(d))
/* verifica daca o stiva este vida */
#define VIDA(a)  	(VF(a) == NULL)

#endif

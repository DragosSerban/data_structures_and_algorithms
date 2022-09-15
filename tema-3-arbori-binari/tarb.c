/* Serban Dragos-Andrei - 312 CB */

#include "tarb.h"

// Functie - construire frunza pt arbore de fisiere
TFile* constrFrunzaFile (void *x)
{
	// name este un pointer catre zona din memorie unde se afla
	// numele fisierului
	char *name = (char*)x;

	// alocam spatiu pt frunza
	TFile *aux = (TFile*)malloc (sizeof(TFile));
	if (!aux)
		return NULL;

	// alocam memorie si copiem numele in structura frunza alocata
	aux->name = malloc (strlen(name)+1);
	if (!aux->name) {
		free(aux);
		return NULL;
	}
	strcpy (aux->name, name);

	aux->left = aux->right = NULL;

	return aux;
}

// Functie - construire frunza pt arbore de directoare
TDirectory* constrFrunzaDirectory (void *x)
{
	// name este un pointer catre zona din memorie unde se afla
	// numele directorului
	char *name = (char*)x;

	// alocam spatiu pt frunza
	TDirectory *aux = (TDirectory*)malloc (sizeof(TDirectory));
	if (!aux)
		return NULL;

	// alocam memorie si copiem numele in structura frunza alocata
	aux->name = malloc (strlen(name)+1);
	if (!aux->name) {
		free(aux);
		return NULL;
	}
	strcpy (aux->name, name);

	// pointerii files si directories pointeaza catre NULL momentan
	aux->files = NULL;
	aux->directories = NULL;
	aux->left = aux->right = NULL;

	return aux;
}

// Functie - compara 2 siruri de caractere
int fcmp (void *x, void *y)
{
	return strcmp ((char*)x, (char*)y);
}

// Functie - insereaza un nou fisier in arborele de fisiere
int insertFile (TFileTree *fTree, TDirTree dTree, TDirectory *parent, void *x)
{
	TFileTree aux, p, n;
	TDirTree m = dTree;

	// verificam daca exista vreun director cu denumirea pe care vrem sa
	// o dam fisierului
	while (m) {
		if (!fcmp (x, m->name)) {
			// daca exista, nu cream fisierul
			printf("Directory %s already exists!\n", (char*)x);
			return 0;
		}
		if (fcmp (x, m->name) < 0)
			m = m->left;
		else
			m = m->right;
	}

	// in caz ca arborele de fisiere e vid, fisierul actual va fi radacina
	if (*fTree == NULL) {	
		*fTree = constrFrunzaFile(x);
		if(!*fTree)
			return 0;
		return 1;
	}

	// verificam daca exista vreun fisier cu denumirea pe care vrem sa
	// o dam fisierului actual
	n = *fTree;
	while (n) {
		p = n;
		if (!fcmp (x, n->name)) {
			// daca exista, nu cream fisierul
			printf("File %s already exists!\n", (char*)x);
			return 0;
		}
		if (fcmp (x, n->name) < 0)
			n = n->left;
		else
			n = n->right;
	}

	// cream frunza cu informatia data ca parametru
	aux = constrFrunzaFile(x);
	if (!aux)
		return 0;
	aux->parent = parent;

	// inseram frunza in arborele binar de cautare la locul potrivit
	if (fcmp (x, p->name) < 0)
		p->left = aux;
	else
		p->right = aux;
	return 1;
}

// Functie - insereaza un nou director in arborele de directoare
int insertDir (TDirTree *dTree, TFileTree fTree, TDirectory *parent, void* x)
{
	TDirTree aux, p, n;
	TFileTree m = fTree;

	// verificam daca exista vreun fisier cu denumirea pe care vrem sa
	// o dam directorului
	while (m) {
		if (!fcmp (x, m->name)) {
			// daca exista, nu cream directorul
			printf("File %s already exists!\n", (char*)x);
			return 0;
		}
		if (fcmp (x, m->name) < 0)
			m = m->left;
		else
			m = m->right;
	}

	// in caz ca arborele de directoare e vid, directorul actual va fi radacina
	if (*dTree == NULL) {	
		*dTree = constrFrunzaDirectory(x);
		(*dTree)->parent = parent;
		if (!*dTree)
			return 0;
		return 1;
	}

	// verificam daca exista vreun director cu denumirea pe care vrem sa
	// o dam directorului actual
	n = *dTree;
	while (n) {
		p = n;
		if (!fcmp (x, n->name)) {
			// daca exista, nu cream directorul
			printf("Directory %s already exists!\n", (char*)x);
			return 0;
		}
		if (fcmp (x, n->name) < 0)
			n = n->left;
		else
			n = n->right;
	}

	// cream frunza cu informatia data ca parametru
	aux = constrFrunzaDirectory(x);
	if (!aux)
		return 0;
	aux->parent = parent;

	// inseram frunza in arborele binar de cautare la locul potrivit
	if (fcmp (x, p->name) < 0)
		p->left = aux;
	else
		p->right = aux;
	return 1;
}

// Functie - distruge arbore de fisiere
void destroyFiles (TFileTree *r)
{
	if (!*r)
		return;
	destroyFiles (&(*r)->left);
	destroyFiles (&(*r)->right);
	// elibereaza spatiul ocupat de nume si distruge nodul radacina
	free ((*r)->name);
	free (*r);
	*r = NULL;
}

// Functie - distruge arbore de directoare
void destroyDirectory (TDirTree *a)
{
	if (!(*a))
		return;
	destroyDirectory (&(*a)->left);
	destroyDirectory (&(*a)->right);
	// distruge recursiv toate directoarele din directorul curent
	if ((*a)->directories)
		destroyDirectory (&(*a)->directories);
	// distruge toate fisierele din directorul curent
	destroyFiles (&(*a)->files);
	// elibereaza spatiul ocupat de nume si distruge nodul radacina
	free ((*a)->name);
	free (*a);
}


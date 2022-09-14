/* Serban Dragos-Andrei - 312 CB */

#include "tarb.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 256

// Functie - initializare director root
TDirectory *initializeRoot ()
{
	// alocam spatiu pentru director si numele directorului
	TDirectory *root = malloc (sizeof(TDirectory));
	if (!root)
		return NULL;
	root->name = malloc (strlen("root")+1);
	if (!root->name) {
		free(root);
		return NULL;
	}

	// initializam campurile structurii
	strcpy (root->name, "root");
	root->parent = NULL;
	root->directories = NULL;
	root->files = NULL;
	root->left = NULL;
	root->right = NULL;

	return root;
}

// Functie - creare fisier si introducere in arborele de fisiere
int touch (TDirectory *currentDirectory, char *p)
{
	p = strtok(NULL, " \n");
	insertFile (&currentDirectory->files,
	currentDirectory->directories, currentDirectory, p);
	return 1;
}

// Functie - creare director si introducere in arborele de directoare
int mkdir (TDirectory *currentDirectory, char *p)
{
	p = strtok(NULL, " \n");
	insertDir (&currentDirectory->directories,
	currentDirectory->files, currentDirectory, p);
	return 1;
}

// Functie - afisare directoare din arborele de directoare
void lsDir (TDirectory* a)
{
	if (!a)
		return;
	// parcurgere SRD
	lsDir (a->left);
	printf ("%s ", a->name);
	lsDir (a->right);
}

// Functie - afisare fisiere din arborele de fisiere
void lsFile (TFile* a)
{
	if (!a)
		return;
	// parcurgere SRD
	lsFile (a->left);
	printf ("%s ", a->name);
	lsFile (a->right);
}

// Functie - afisare elemente din directorul curent
void ls (TDirectory *currentDirectory)
{
	// afisare directoare
	lsDir (currentDirectory->directories);
	// afisare fisiere
	lsFile (currentDirectory->files);
	printf ("\n");
}

// Functie - eliminare fisier din arborele de fisiere
TFileTree rm (TFileTree a, char *name)
{
	// arbore vid => fisierul nu exista
	if (a == NULL) {
		printf ("File %s doesn't exist!\n", name);
		return a;
	}

	// coboram in arborele binar de cautare nivel cu nivel pana gasim locul
	// unde ar trebui sa se afle fisierul cautat
	if (strcmp(a->name, name) > 0)
		a->left = rm (a->left, name);
	else if (strcmp(a->name, name) < 0)
		a->right = rm (a->right, name);
	else {
		// fisierul este gasit
		if (a->left == NULL && a->right == NULL) {
			// cazul - nod frunza
			// eliminam nodul frunza
			free (a->name);
			free (a);
			return NULL;
		} else if (a->left == NULL) {
			// cazul - nod cu un singur fiu in dreapta
			// eliberam memoria si mutam nodul din dreapta
			TFileTree aux = a->right;
			free (a->name);
			free (a);
			return aux;
		} else if (a->right == NULL) {
			// cazul - nod cu un singur fiu in stanga
			// eliberam memoria si mutam nodul din stanga
			TFileTree aux = a->left;
			free (a->name);
			free (a);
			return aux;
		} else {
			// cazul - nod cu 2 fii
			// aflam minimul de pe ramura din dreapta a parintelui
			// si elementul aflat inaintea sa
			TFileTree min = a->right;
			while (min->left != NULL)
				min = min->left;
			// mutam fisierul minim de mai sus in locul fisierului
			// vechi - copiem numele lui
			strcpy (a->name, min->name);

			a->right = rm (a->right, min->name);
		}
	}
	return a;
}

// Functie - eliminare director din arborele de directoare
TDirTree rmdir(TDirTree a, char *name)
{
	// arbore vid => directorul nu exista
	if (a == NULL) {
		printf ("Directory %s doesn't exist!\n", name);
		return a;
	}

	// coboram in arborele binar de cautare nivel cu nivel pana gasim locul
	// unde ar trebui sa se afle directorul cautat
	if (strcmp(a->name, name) > 0)
		a->left = rmdir (a->left, name);
	else if (strcmp(a->name, name) < 0)
		a->right = rmdir (a->right, name);
	else {
		// directorul este gasit
		if (a->left == NULL && a->right == NULL) {
			// cazul - nod frunza
			// eliminam nodul frunza + fisiere, directoare interne
			destroyFiles (&a->files);
			free (a->name);
			destroyDirectory (&a->directories);
			free (a);
			a = NULL;
			return NULL;
		} else if(a->left == NULL) {
			// cazul - nod cu un singur fiu in dreapta
			// eliberam memoria (+ fisiere, directoare interne)
			// si mutam nodul din dreapta
			TDirTree aux = a->right;
			destroyFiles (&a->files);
			free (a->name);
			destroyDirectory (&a->directories);
			free (a);
			a = NULL;
			return aux;
		} else if (a->right == NULL) {
			// cazul - nod cu un singur fiu in stanga
			// eliberam memoria (+ fisiere, directoare interne)
			// si mutam nodul din stanga
			TDirTree aux = a->left;
			destroyFiles (&a->files);
			free (a->name);
			destroyDirectory (&a->directories);
			free (a);
			a = NULL;
			return aux;
		} else {
			// cazul - nod cu 2 fii
			// aflam minimul de pe ramura din dreapta a parintelui
			// si elementul aflat inaintea sa
			TDirTree min = a->right;
			while (min->left != NULL)
				min = min->left;
			// mutam directorul minim in locul directorului actual
			// vechi - copiem numele lui
			strcpy (a->name, min->name);
			// eliberam spatiul ocupat de vechile fisiere si
			// directoare
			destroyFiles (&a->files);
			destroyDirectory (&a->directories);

			// actualizam pointerii
			a->files = min->files;
			a->directories = min->directories;

			a->right = rmdir(a->right, min->name);
		}
	}
	return a;
}

// Functie - schimbare director
int cd (TDirectory **currentDirectory, char *p)
{
	p = strtok (NULL, " \n");
	char *name = p;
	if (!strcmp(name, "..")) {
		// ne intoarcem inapoi cu un director, daca directorul actual
		// difera de root
		if ((*currentDirectory)->parent)
		*currentDirectory = (*currentDirectory)->parent;
		return 1;
	}

	TDirectory *dir = (*currentDirectory)->directories;
	while (dir != NULL)
	{
		// parcurgem arborele binar de cautare pana gasim directorul
		// cautat
		if (strcmp(dir->name, name) > 0)
			dir = dir->left;
		else if (strcmp(dir->name, name) < 0)
			dir = dir->right;
		else if (!strcmp(dir->name, name)) {
			*currentDirectory = dir;
			return 1;
		}
	}
	// daca nu gasim directorul, afisam mesajul de mai jos
	printf ("Directory not found!\n");
	return 1;
}

// Functie - print working directory
int pwd (TDirectory *currentDirectory)
{
	char print[MAX_LENGTH] = "";
	while (currentDirectory)
	{
		// parcurgem pointer cu pointer de la directorul curent
		// la directorul radacina fiecare parinte
		char *aux = malloc (MAX_LENGTH*sizeof(char));
		if (!aux)
			return 0;
		// folosim un string auxiliar pentru a pune apoi in ordine
		// corecta elementele in stringul principal
		strcpy (aux, "/");
		strcat (aux, currentDirectory->name);
		strcat (aux, print);
		strcpy (print, aux);
		free (aux);
		currentDirectory = currentDirectory->parent;
	}
	// afisam calea catre working directory
	printf ("%s\n", print);
	return 1;
}

// Functie auxiliara - cautam fisierul in directorul curent
void findFileCurrentDirectory (TDirectory *currentDirectory, char *name,
int *found)
{
	TFile *file = currentDirectory->files;
	while (file != NULL)
	{
		// parcurgem arborele binar de cautare si daca gasim fisierul
		// printam directorul curent si *found devine egal cu 1
		if (strcmp (file->name, name) > 0)
			file = file->left;
		else if (strcmp (file->name, name) < 0)
			file = file->right;
		else if (!strcmp (file->name, name)) {
			printf ("File %s found!\n", name);
			pwd (currentDirectory);
			*found = 1;
			return;
		}
	}
}

// Functie auxiliara - cautam fisierul in directoarele din directorul curent
int findFile (TDirectory *currentDirectory, char *name, int *found)
{
	if (!currentDirectory)
		return 0;
	// parcurgem toate directoarele recursiv si apelam functia precedenta
	// pentru fiecare in parte
	findFile (currentDirectory->left, name, found);
	findFile (currentDirectory->right, name, found);
	findFileCurrentDirectory (currentDirectory, name, found);
	if (currentDirectory->directories)
		findFile (currentDirectory->directories, name, found);
	return 1;
}

// Functie auxiliara - cautam directorul in directorul curent
void findDirectoryCurrentDirectory (TDirectory *currentDirectory, char *name,
int *found)
{
	TDirectory *dir = currentDirectory->directories;
	while (dir != NULL)
	{
		// parcurgem arborele binar de cautare si daca gasim directorul
		// printam directorul curent si *found devine egal cu 1
		if (strcmp (dir->name, name) > 0)
			dir = dir->left;
		else if (strcmp (dir->name, name) < 0)
			dir = dir->right;
		else if (!strcmp (dir->name, name)) {
			printf ("Directory %s found!\n", name);
			pwd(dir);
			*found = 1;
			return;
		}
	}
}

// Functie auxiliara - cautam directorul in directoarele din directorul curent
int findDirectory (TDirectory *currentDirectory, char *name, int *found)
{
	if (!currentDirectory)
		return 0;
	// parcurgem toate directoarele recursiv si apelam functia precedenta
	// pentru fiecare in parte
	findDirectory (currentDirectory->left, name, found);
	findDirectory (currentDirectory->right, name, found);
	findDirectoryCurrentDirectory (currentDirectory, name, found);
	if (currentDirectory->directories)
		findDirectory (currentDirectory->directories, name, found);
	return 1;
}

// Functie - find
int find (TDirectory *currentDirectory, char *p)
{
	p = strtok(NULL, " \n");
	int found = 0;
	if (!strcmp(p, "-f")) {
		// cazul find -f
		p = strtok (NULL, " \n");
		// cautam fisierul in directorul curent
		findFileCurrentDirectory (currentDirectory, p, &found);
		// cautam fisierul in directoarele din directorul curent
		findFile (currentDirectory->directories, p, &found);
		if (found == 0)
			printf ("File %s not found!\n", p);
	} else if (!strcmp(p, "-d")) {
		// cazul find -d
		p = strtok (NULL, " \n");
		// cautam directorul in directorul curent
		findDirectoryCurrentDirectory (currentDirectory, p, &found);
		// cautam directorul in directoarele din directorul curent
		findDirectory (currentDirectory->directories, p, &found);
		if (found == 0)
			printf ("Directory %s not found!\n", p);
	}
	return 1;
}

// Functia main
int main ()
{
	// initializam root
	TDirectory *root = initializeRoot ();
	if (!root)
		return 0;
	// directorul curent este root
	TDirectory *currentDirectory = root;

	char instruction[MAX_LENGTH];
	while (fgets(instruction, MAX_LENGTH, stdin)) {
		// citim instructiunea
		if (!strcmp(instruction, "\n"))
			continue;
		char *p = strtok (instruction, " \n");
		if (!strcmp (p, "touch")) {
			// cazul touch
			touch (currentDirectory, p);
		} else if (!strcmp(p, "mkdir")) {
			// cazul mkdir
			mkdir (currentDirectory, p);
		} else if (!strcmp(p, "ls")) {
			// cazul ls
			ls (currentDirectory);
		} else if (!strcmp(p, "rm")) {
			// cazul rm
			p = strtok (NULL, " \n");
			currentDirectory->files =
			rm (currentDirectory->files, p);
		} else if (!strcmp(p, "rmdir")) {
			// cazul rmdir
			p = strtok (NULL, " \n");
			currentDirectory->directories =
			rmdir (currentDirectory->directories, p);
		} else if (!strcmp(p, "cd")) {
			// cazul cd
			cd (&currentDirectory, p);
		} else if (!strcmp(p, "pwd")) {
			// cazul pwd
			pwd (currentDirectory);
		} else if (!strcmp(p, "find")) {
			// cazul find
			find (root, p);
		} else if(!strcmp (p, "quit")) {
			// cazul quit
			// distrugem directoarele si fisierele
			destroyDirectory (&root->directories);
			destroyFiles (&root->files);
			// distrugem informatia din root si nodul root
			free (root->name);
			free (root);
			root = NULL;
			return 0;
		}
	}
	return 0;
}


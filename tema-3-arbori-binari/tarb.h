/* Serban Dragos-Andrei - 312 CB */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#ifndef ARBORE_BINAR
#define ARBORE_BINAR

// structura pt director
typedef struct directory {
	char *name;
	struct directory *parent;
	struct file *files;
	struct directory *directories;
	struct directory *left;
	struct directory *right;
} TDirectory, *TDirTree;

// structura pt fisier
typedef struct file {
	char *name;
	struct directory *parent;
	struct file *left;
	struct file *right;
} TFile, *TFileTree;

// functii elementare utilizate
TFile* constrFrunzaFile (void *x);
TDirectory* constrFrunzaDirectory (void *x);
int fcmp_file (void *x, void *y);
int fcmp_dir (void *x, void *y);
void destroyFiles (TFileTree *a);
void destroyDirectory (TDirTree *a);
int insertFile (TFileTree *fTree, TDirTree dTree, TDirectory *parent, void *x);
int insertDir (TDirTree *dTree, TFileTree fTree, TDirectory *parent, void *x);

#endif

#ifndef _random_
#define _random_
#define random(n) (rand() % (n))
#define randomize() (srand((unsigned int)time(NULL)))
#endif


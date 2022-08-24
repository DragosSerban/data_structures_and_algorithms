/* Serban Dragos-Andrei - 312 CB */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET 26 // nr litere in alfabetul englez
#define MAX 256 // nr max de litere pt citirea instructiunii

// Structuri utilizate

typedef struct celula {
  void *info;
  struct celula *urm;
} TCelulaG, *TLG;

typedef int (*THF)(void *);

typedef struct {
  int M;
  THF fh;
  TLG *v;
} TH;

typedef struct {
  int dimensiune;
  TLG cuvinte;
} TDim;

typedef struct {
  char *cuvant;
  int nrAparitii;
} TCuv;

// Functii utilizate

TLG alocareCelula(void *x);

int fhSiruri(void *x);

TH *initializare(int M, int (*fh)(void *));

int inserare(TH *a, void *x);

int fcmpCrescator(void *x, void *y);

int fcmpCuvinte(void *x, void *y);

int fcmpAparitii(void *x, void *y);

void cuvinteEgale(void *x, TLG *L, TLG ant);

int ordonare(TLG *L, void *x, int (*fcmp)(void*, void*));

void afisareDimensiune(void *dim);

void afisareCuvant(TLG aux);

void afisare(TH *a, void (*afis)(void *));

void afisare2Parametrii(TH *a, int index, int nrLitere);

void afisare1Parametru(TH *a, int nr);

void distrugere(TH **a, void (*fdis)(void *));

void distrugereElement(void *x);

// Codul functiilor

// Functia main

int main(int argc, char **argv)
{
  if (!argv[1])
    return 0;
  FILE *f = fopen(argv[1], "r");
  if (!f)
    return 0;
  TH *a = initializare(ALPHABET, fhSiruri);
  if (!a)
    return 0;
  char s[MAX];
  char separatorInstructiuni[] = " .,\n";
  char separatorCuvinte[] = " .,0123456789\n";
  while (!feof(f) && fgets(s, MAX, f)) {
    char *p = strtok(s, separatorInstructiuni);
    if (!p) {
      continue;
    } else if (!strcmp(p, "insert")) {
      p = strtok(NULL, separatorCuvinte);
      while (p) {
        if (strlen(p) >= 3) {
          char *ptr = malloc((strlen(p)+1)*sizeof(char));
          if (!ptr)
            return 0;
          strcpy(ptr, p);
          inserare(a, ptr);
        }
        p = strtok(NULL, separatorCuvinte);
      }
    } else if (!strcmp(p, "print")) {
      char *parametru1 = strtok(NULL, separatorInstructiuni);
        if (parametru1) {
          char *parametru2 = strtok(NULL, separatorInstructiuni);
          if (parametru2)
            afisare2Parametrii(a, a->fh(parametru1), atoi(parametru2));
          else
            afisare1Parametru(a, atoi(parametru1));
        } else if (!parametru1) {
          afisare(a, afisareDimensiune);
        }
    }
  }
  distrugere(&a, distrugereElement);
  fflush(f);
  fclose(f);
  return 0;
}

// Functie alocare celula din lista

TLG alocareCelula(void *x)
{
  TLG newCell = (TLG)malloc(sizeof(TCelulaG));
  if (!newCell)
    return NULL;
  newCell->info = x;
  newCell->urm = NULL;
  return newCell;
}

// Functie hash (pentru ordine alfabetica)

int fhSiruri(void *x)
{
  int index = -1;
  char *temp = (char *)x;
  if (*temp >= 'a' && *temp <= 'z')
    index = *(char *)x - 'a';
  else if (*temp >= 'A' && *temp <= 'Z')
    index = *(char *)x - 'A';
  return index;
}

// Functie initializare tabela hash

TH *initializare(int M, int (*fh)(void *))
{
  TH *a = (TH *)malloc(sizeof(TH));
  if (!a)
    return NULL;
  a->M = M;
  a->fh = fh;
  a->v = (TLG *)calloc(M, sizeof(TLG));
  if (!a->v) {
    free(a);
    return NULL;
  }
  return a;
}

// Functie de comparare lungimi pt 2 siruri
// returneaza 1 daca primul e mai mare, 0 altfel

int fcmpCrescator(void *x, void *y)
{
  if (((TDim *)x)->dimensiune > ((TDim *)y)->dimensiune)
    return 1;
  return 0;
}

// Functie de inserare cuvant in interiorul listei
// din lista corespunzatoare

int inserare(TH *a, void *x)
{
  int index = a->fh(x);
  TLG aux = a->v[index];
  TCuv *c = malloc(sizeof(TCuv));
  if (!c)
    return 0;
  c->cuvant = (char *)x;
  c->nrAparitii = 1;
  for (; aux != NULL; aux = aux->urm)
    if (((TDim *)aux->info)->dimensiune == strlen((char *)x)) {
      TDim *d = (TDim *)aux->info;
      int cuvantNou = ordonare(&(d->cuvinte), c, fcmpCuvinte);
      if (cuvantNou == -1) {
        free(c->cuvant);
        free(c);
      } else if (cuvantNou == 0) {
        return 0;
      }
      return 1;
    }
  TLG newCell = alocareCelula(c);
  if (!newCell)
    return 0;
  TDim *d = malloc(sizeof(TDim));
  if (!d)
    return 0;
  d->dimensiune = strlen((char *)x);
  d->cuvinte = newCell;
  newCell = alocareCelula(d);
  if (!newCell)
    return 0;
  TLG L = a->v[index], ant = NULL;
  for (; L != NULL; ant = L, L = L->urm)
    if (fcmpCrescator(L->info, d))
      break;
  newCell->urm = L;
  if (ant == NULL)
    a->v[index] = newCell;
  else
    ant->urm = newCell;
  return 1;
}

// Fuctie de comparare alfabetica a cuvintelor

int fcmpCuvinte(void *x, void *y)
{
  return strcmp(((TCuv *)x)->cuvant, ((TCuv *)y)->cuvant);
}

// Functie pt cazul in care cuvintele sunt egale
// folosita pentru incrementarea variabilei nrAparitii
// si pt ordonarea descrescatoare in fct de nr de aparitii

void cuvinteEgale(void *x, TLG *listaCuvinte, TLG anteriorInainte)
{
  TLG aux = (TLG)x;
  TCuv *c = ((TCuv *)aux->info);
  c->nrAparitii++;
  TLG i = *listaCuvinte, anteriorDupa = NULL;
  for (; i != NULL; anteriorDupa = i, i = i->urm)
    if (((TCuv *)i->info)->nrAparitii < c->nrAparitii)
      break;
  if (anteriorDupa) {
    anteriorDupa->urm = aux;
    if (anteriorInainte)
      anteriorInainte->urm = aux->urm;
    aux->urm = i;
  } else {
    *listaCuvinte = aux;
    if (anteriorInainte)
      anteriorInainte->urm = aux->urm;
    aux->urm = i;
  }
}

// Functie pt a compara nrAparitii a doua cuvinte

int fcmpAparitii (void *x, void *y)
{
  if (((TCuv *)x)->nrAparitii == ((TCuv *)y)->nrAparitii)
    return 1;
  return 0;
}

// Functie pt ordonare a celulelor intr-o lista
// intoarce -1 si updateaza nrAparitii daca gaseste cuvantul
// intoarce 1 si il introduce daca nu il gaseste

int ordonare(TLG *L, void *x, int (*fcmp)(void *, void *))
{
  TLG aux = *L, ant = NULL;
  for (; aux != NULL; ant = aux, aux = aux->urm)
    if (!fcmp(aux->info, x)) {
      cuvinteEgale(aux, L, ant);
      return -1;
    } else if (fcmp(aux->info, x) > 0) {
      if (fcmpAparitii(x, aux->info))
        break;
    }
  TLG newCell = alocareCelula(x);
  if (!newCell)
    return 0;
  newCell->urm = aux;
  if (ant == NULL)
    (*L) = newCell;
  else
    ant->urm = newCell;
  return 1;
}

// Urmatoarele 3 functii sunt folosite
// pt afisarea fara niciun parametru la print

void afisareDimensiune(void *dim)
{
  TDim *d = (TDim *)dim;
  printf("(%d:", d->dimensiune);
  afisareCuvant(d->cuvinte);
}

void afisareCuvant(TLG aux)
{
  for (; aux != NULL; aux = aux->urm) {
    TCuv *temp = (TCuv *)aux->info;
    printf("%s/%d", temp->cuvant, temp->nrAparitii);
    if (aux->urm)
      printf(", ");
  }
  printf(")");
}

void afisare(TH *a, void (*afis)(void *))
{
  int i;
  for (i = 0; i < a->M; i++) {
    TLG p = a->v[i];
    if (p) {
      printf("pos %d: ", i);
      for (; p != NULL; p = p->urm)
      afis(p->info);
      printf("\n");
    }
  }
}

// Afisare pt 2 parametrii primiti la print

void afisare2Parametrii(TH *a, int index, int nrLitere)
{
  TLG L = a->v[index];
  for (; L != NULL; L = L->urm)
    if (((TDim *)L->info)->dimensiune == nrLitere) {
      printf("(%d:", nrLitere);
      afisareCuvant(((TDim *)L->info)->cuvinte);
      printf("\n");
    }
}

// Afisare pt un parametru primit la print

void afisare1Parametru(TH *a, int nr)
{
  int i;
  for (i = 0; i < a->M; i++) {
    TLG p = a->v[i];
    int letterIsPrinted = 0;
    if (p) {
      for (; p != NULL; p = p->urm) {
        int dimensionIsPrinted = 0;
        TDim *d = (TDim *)(p->info);
        TLG aux = d->cuvinte;
        for (; aux != NULL; aux = aux->urm) {
          if (((TCuv *)aux->info)->nrAparitii <= nr) {
            if (letterIsPrinted == 0)
              printf("pos%d: ", i);
            letterIsPrinted = 1;
            if (!dimensionIsPrinted)
              printf("(%d: ", d->dimensiune);
            else
              printf(", ");
            TCuv *temp = (TCuv*)aux->info;
            printf("%s/%d", temp->cuvant, temp->nrAparitii);
            dimensionIsPrinted = 1;
          }
        }
        if (dimensionIsPrinted)
          printf(")");
      }
      if (letterIsPrinted)
        printf("\n");
    }
  }
}

// Doua functii pt distrugere tabela hash / liste

void distrugere(TH **a, void (*fdis)(void *))
{
  TLG *p, x, aux;
  for (p = (*a)->v; p < (*a)->v+(*a)->M; p++) {
    for (x = *p; x != NULL; ) {
      aux = x;
      x = x->urm;
      fdis(aux->info);
      free(aux);
    }
  }
  free((*a)->v);
  free(*a);
  *a = NULL;
}

void distrugereElement(void *x)
{
  TDim *d = (TDim *)x;
  TLG L = d->cuvinte;
  for (; L != NULL;) {
    TCuv *c = (TCuv *)L->info;
    free(c->cuvant);
    free(c);
    TLG aux = L;
    L = L->urm;
    free(aux);
  }
  free(d);
}

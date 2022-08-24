/* Serban Dragos-Andrei - 312CB */

- in functia main realizam initializarea lui root, folosind functia
initializaRoot, directorul curent este root momentan
- citim instructiunile, acestea fiind urmatoarele:
- touch: creeaza fisierul cu numele dat ca parametru si il adauga in arborele
de fisiere, utilizand functia insertFile, aflata in tarb.c si descrisa mai jos:
Verificam daca exista vreun director cu numele pe care dorim sa il oferim
fisierului; daca exista, nu cream fisierul
Daca arborele de fisiere este gol, atunci fisierul actual va fi radacina
Verificam daca exista vreun fisier cu numele pe care dorim sa il oferim
fisierului; daca exista, nu cream fisierul
Daca ajungem pana aici, atunci cream o frunza cu informatiile date ca parametru
(name + parent), utilizand functia constrFrunzaFile
Parcurgem arborele binar de cautare si introducem nodul la locul potrivit
- mkdir: creeaza directorul cu numele dat ca parametru si il adauga in arborele
de directoare, utilizand functia insertDir, aflata in tarb.c si descrisa mai jos:
Verificam daca exista vreun fisier cu numele pe care dorim sa il oferim
directorului; daca exista, nu cream directorul
Daca arborele de directoare este gol, atunci directorul actual va fi radacina
Verificam daca exista vreun director cu numele pe care dorim sa il oferim
directorului; daca exista, nu cream directorul
Daca ajungem pana aici, atunci cream o frunza cu informatiile date ca parametru
(name + parent), utilizand functia constrFrunzaDirectory
Parcurgem arborele binar de cautare si introducem nodul la locul potrivit
- ls: afiseaza directoarele si fisierele din directorul curent; aceasta afiseaza
mai intai directoarele, folosind functia auxiliara lsDir, cu parcurgere SRD (in
ordine alfabetica), iar apoi fisierele, folosind functia auxiliara lsFile, cu
acelasi tip de parcurgere
- rm: eliminam fisierul din arborele de fisiere; mai intai verificam daca
arborele este vid; daca e vid, atunci fisierul nu exista
Coboram in arborele binar de cautare nivel cu nivel folosind comparatia intre
nume si ajungem in locul unde ar trebui sa se afle fisierul
Daca gasim fisierul atunci avem mai multe cazuri: cazul nod frunza (eliminam
nodul frunza); cazul nod cu un singur fiu in dreapta (eliberam memoria si mutam
nodul din dreapta); cazul nod cu un singur fiu in stanga (eliberam memoria si
mutam nodul din stanga); cazul nod cu doi fii (aflam minimul de pe ramura din
dreapta a parintelui si elementul aflat inaintea sa; mutam fisierul minim de mai
sus in locul fisierului pe care il stergem - copiem numele lui; eliberam memoria
ocupata de acel nod minim)
- rmdir: eliminam directorul din arborele de directoare; procedam la fel ca la
cazul anterior, doar ca de data asta eliberam spatiul ocupat de fisierele si
directoarele aflate in directorul curent
- cd: schimbam directorul actual: cd .. (ne intoarcem inapoi cu un director, daca
exista, folosind pointerul parent); cd altceva (parcurgem arborele binar de
cautare de directoare pana gasim directorul cautat
- pwd: print working directory: parcurgem de la directorul actual la nodul
radacina fiecare parinte, utilizand pointerul parent; folosind un string
auxiliar pentru a avea calea afisata in ordine corecta la final
- find:
- Cazul find -f: cautam fisierul mai intai in directorul curent; parcurgem
arborele binar de cautare si daca gasim fisierul printam directorul curent,
iar *found devine egal cu 1, pentru a semnifica ca am gasit fisierul; cautam
fisierul in directoarele din directorul curent; parcurgem toate directoarele
recursiv si apelam functia precedenta pentru fiecare in parte
- Cazul find -d: cautam directorul mai intai in directorul curent; parcurgem
arborele binar de cautare si daca gasim directorul printam directorul curent,
iar *found devine egal cu 1, pentru a semnifica ca am gasit directorul; cautam
directorul in directoarele din directorul curent; parcurgem toate directoarele
recursiv si apelam functia precedenta pentru fiecare in parte
- quit: dezaloca toata memoria ocupata de directoarele si fisierele din root,
folosind doua functii: una recursiva de distrugere directoare si alta de
distrugere arbore de fisiere; la final eliberam si memoria ocupata de nodul
root

Local am obtinut 135 de puncte pe taskuri + 22 de puncte pe bonus

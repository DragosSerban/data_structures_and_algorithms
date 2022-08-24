/* Serban Dragos-Andrei - 312 CB */

- Vom folosi structuri pentru celula generica, tabela hash, TDim si TCuv
- TDim contine dimensiune cuvant si lista de cuvinte
- TCuv contine nrAparitii si un pointer la cuvantul propriu-zis
- Functii utilizate:
- alocareCelula: functie de alocare dinamica celula generica
- fhSiruri: functie hash, care verifica prima litera a cuvantului pt a ordona alfabetic in tabela hash
- initializare: functie de initializare tabela hash, returneaza pointer catre tabela hash
- fcmpCrescator: functie de comparare lungimi pt 2 siruri; returneaza 1 daca primul e mai mare, 0 altfel
- inserare: functie de introducere cuvant la locul potrivit; verifica daca exista in tabela hash o lista corespunzatoare dimensiunii cuvantului, folosindu-ne si de functia hash (pt eficienta).
alocam un element nou de tip cuvant pt cuvantul actual si apoi verificam daca exista sau nu un element de tip dimensiune care coincide cu dimensiunea cuvantului actual
daca exista atunci apelam functia ordonare, care returneaza -1 daca cuvantul se regaseste deja in lista din lista, 0 daca alocarea nu reuseste si 1 daca cuvantul nu se regaseste; si introduce ordonat / actualizeaza cuvantul
daca returneaza -1, atunci se dezaloca spatiul alocat pt cuvant in functia inserare
daca nu exista dimensiunea cuvantului cautat alocam o celula noua cu informatia cuvantului, iar apoi una noua pentru dimensiunea cuvantului in care se va aflaprima celula
folosim functia fcmpCrescator pentru a verifica unda anume in prima lista trebuie inserata celula de dimensiune
- fcmpCuvinte: functia de comparare alfabetica a doua cuvinte
- cuvinteEgale: functia primeste ca parametru un void* la o celula de tip cuvant, incrementeaza nrAparitii, iar apoi verifica unde exact trebuie pozitionata celula pt a pastra ordinea descrescatoare in fct de nrAparitii
aici se folosesc si alti doi parametrii, listaCuvinte si anteriorInainte, care prezinta celula care era inaintea celului modificate in momentul actual, inainte de modificare
apoi urmeaza modificarea legaturilor lui anteriorInainte, iar anteriorDupa este celula care se va afla acum (dupa modificare) inaintea celului modificate
- fcmpAparitii: functie pt compararea nrAparitii a doua cuvinte; returneaza 1 daca sunt egale, 0 altfel
- ordonare: functie pentru ordonarea alfabetica a cuvintelor in lista
daca actualul cuvant se regaseste deja in lista, atunci este apelata functia cuvinteEgale, care incrementeaza nrAparitii si ordoneaza descrescator in fct de nrAparitii
altfel, daca nu se regaseste cuvantul, atunci la pozitia corespunzatoare dpdv alfabetic si al nrAparitii va fi inserat noul cuvant
- afisarea se face prin urmatoarele functii:
- afisare: care primeste ca parametrii tabela hash si un pointer la o functie si face afisarea elementelor din tabela hash
- afisareDimensiune: a carui scop este sa afiseze valoarea din campul de dimensiune al primei liste; apoi apeleaza afisareCuvant
- afisareCuvant: are rolul de a afisa nrAparitii si cuvantul propriu-zis din lista de cuvinte
- afisare2Parametrii: functie de afisare in caz ca se primesc 2 parametrii la print
cauta in tabela hash litera corespunzatoare si in lista de dimensiuni dimensiunea corespunzatoare, iar apoi apeleaza afisareCuvant
- afisare1Parametru: functie de afisare in caz ca se primeste un parametru la print
functia trece prin toate listele din tabela hash, urmand sa treaca prin listele de cuvinte
daca exista elemente cu nrAparitii mai mic sau egal decat nr dat, atunci acesta va fi printat
variabilele letterIsPrinted si dimensionIsPrinted sunt initializate cu zero, dar daca se gaseste un cuvant vu proprietatea de mai sus, ele devin 1 pentru a printa cuvantul
- distrugere: functie generica de distrugere a tabelei hash; elibereaza memoria tabelei hash si a listelor de dimensiune
- distrugereElement: elibereaza memoria ocupata de listele pentru cuvinte si de informatia din lista de dimensiune
- main: functia principala
primeste ca argument numele fisierului, il deschide
initializeaza tabela hash, citeste instructiunile, le imaparte in cuvinte cu strtok si apoi se foloseste de acestea pentru a apela functiile de inserare / printare
la sfarsit se dezaloca toata memoria alocata dinamic

- local am primit 105 puncte pe tema

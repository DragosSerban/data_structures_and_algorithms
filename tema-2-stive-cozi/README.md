/* Serban Dragos-Andrei - 312 CB */

- structura Thread contine un singur camp ID
- structura Task contine campurile ID, thread, prioritate, timpDEExecutie si
timpRamas, in care se pastreaza cat timp mai trebuie rulat taskul pana trece in
coada finishedQ

Functii de afisare:
- afiWaiting (afiseaza coada waiting)
- afiFinished (afiseaza coada finished)
- afiRunning (afiseaza coada running)

Functie de introducere task in coada waiting (IntrWaitingQ):
- se extrag elemente din coada waiting si se verifica pe rand mai intai daca
prioritatea elementului extras este mai mica decat cea a taskului nou
- daca da, atunci se introduce elementul nou in coada auxiliara,
urmat de elementul extras anterior (ordonare descrescatoare)
- daca nu, atunci se introduce elementul extras anterior
- daca sunt egale, atunci se verifica dimensiunea timpilor de executie, pentru
a le ordona crescator dupa timpDeExecutie in caz ca au aceeasi prioritate
- daca au acelasi timpDeExecutie, le ordonam crescator dupa ID
- concatenam coada auxiliara la waitingQ

addTasks (functie):
- primim ca parametrii waitingQ, informatiile (sir de caractere),
vectorul de ID-uri disponibile si fisierul de output
- extragem informatiile din sirul dat ca parametru
- verificam care este primul ID disponibil din vectorul de ID-uri (idV)
- il acordam noului task, iar valoarea lui in vector devine 0 (nedisponibil)
- cream structura de tip Task si o adaugam in waitingQ

getTask (functie):
- primeste ca parametrii cele 3 cozi, sirul de caractere cu instructiunea,
fisierul output g
- avem nevoie de o coada auxiliara caux in care vom stoca informatiile extrase
din cozile cu care lucram la un moment dat
- verificam daca taskul se afla in runningQ
- cautam taskul in waitingQ
- verificam daca se afla in finishedQ
- pt aceste 3 verificari vom folosi coada caux pt stocarea provizorie
a informatiilor extrase
- vom da ResetQ de fiecare data cozii caux
- daca nu gasim taskul, afisam un mesaj

getThread (functie):
- primeste ca parametrii stiva Pool si coada runningQ,
informatiile (sir de caractere), fisierul de output
- ne vom folosi din nou de o coada auxiliara pt acelasi motiv ca mai devreme
- extragem elemente din runningQ si verificam daca ruleaza pe Threadul cautat
- daca nu gasim, atunci cautam Threadul in Pool, unde vom avea nevoie de
o stiva auxiliara saux pt a stoca provizoriu elementele extrase
- daca nu gasim nici aici Threadul atunci nu afisam nimic

run (functie):
- primeste ca parametrii Pool, cele 3 cozi, Q si N, sirul de caractere
cu instructiunea, vectorul de ID-uri si fisierul de output g
- time este timpul primit ca parametru in instructiune
- runningTime este contorul, timpul real de rulare, initializat la
inceput cu 0
- vom folosi un loop pt a rula taskurile in functie de cuanta de timp
- variabila runForMiliseconds retine nr de ms pt care ruleaza taskurile
- variabila poate avea valoarea Q, in caz ca mai urmeaza iteratii in for,
sau time - i*Q, daca nu vor mai exista iteratii
- daca nu mai exista taskuri de rulat, atunci functia returneaza runningTime
- daca nu exista taskuri in runningQ, atunci extragem N taskuri din coada
waitingQ, le atribuim un thread si le introducem in runningQ
- extragem task cu task din runningQ si rulam
- daca timpul nu e suficient pt a finaliza taskul, atunci
maxRunnedd = runForMiliseconds si timpul de rulare ramas scade
- daca e suficient, atunci maxRunned devine timpRamas si eliberam threadul,
introducem taskul in finishedQ, ID-ul taskului redevine available
- la final, in locul taskurilor terminate, introducem noi taskuri
in coada runningQ
din waitingQ si le oferim un thread pe care sa ruleze
runningTime += maxRunned; functia returneaza pt cat timp s-a rulat

finish (functie):
- primeste ca parametrii Pool, cele 3 cozi, Q si N
- face exact aceleasi lucruri ca si functia run,
doar ca ruleaza pana nu mai exista taskuri in waitingQ si runningQ,
aceasta fiind singura conditie de oprire

Functia main:
- deschidem fisierele de citire (f) si de afisare (g)
- citim Q si C
- calculam N
- initializam stiva Pool, cozile waitingQ, finishedQ si runningQ
- inseram threadurile in Pool
- initializam vectorul de ID-uri si fiecare dintre ID-uri va avea
valoarea 1 (poate fi folosit) - ID range - 1:MAXSHORT
- in timeRunning se tine minte cat timp au rulat taskurile
- se citesc instructiunile si se executa folosind functiile descrise mai sus
- la final se inchid fisierele

/* Serban Dragos-Andrei - 312 CB */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TCoada.h"
#include "TStiva.h"

#define MAX 256
#define MAXSHORT 32767

// Structura Thread

typedef struct {
	unsigned int ID;
} Thread;

// Structura Task

typedef struct {
	unsigned short ID;
	unsigned int thread;
	char prioritate;
	unsigned int timpDeExecutie;
	unsigned int timpRamas;
} Task;

// Afisare taskuri coada waiting

int AfiWaiting(void *x, FILE *g)
{
	fprintf(g, "(%d: ", (*(Task*)x).ID);
	fprintf(g, "priority = %d, ", (*(Task*)x).prioritate);
	fprintf(g, "remaining_time = %d)", (*(Task*)x).timpRamas);
	return 1;
}

// Afisare taskuri coada finished

int AfiFinished(void *x, FILE *g)
{
	fprintf(g, "(%d: ", (*(Task*)x).ID);
	fprintf(g, "priority = %d, ", (*(Task*)x).prioritate);
	fprintf(g, "executed_time = %d)", (*(Task*)x).timpDeExecutie);
	return 1;
}

// Afisare taskuri coada running

int AfiRunning(void *x, FILE *g)
{
	fprintf(g, "(%d: ", (*(Task*)x).ID);
	fprintf(g, "priority = %d, ", (*(Task*)x).prioritate);
	fprintf(g, "remaining_time = %d, ", (*(Task*)x).timpRamas);
	fprintf(g, "running_thread = %d)", (*(Task*)x).thread);
	return 1;
}

// Introducere task in coada waiting

int IntrWaitingQ(void *waitingQ, Task task)
{
	void *x = malloc(sizeof(Task));
	if(!x)
		return 0;
	void *caux = InitQ(sizeof(Task));
	if(!caux) {
		free(x);
		return 0;
	}

	char introdus = 0;
	while(ExtrQ(waitingQ, x)) {
		Task element = *(Task*)x;
		char elPr = element.prioritate;
		char taskPr = task.prioritate;

		// ordonam descrescator in functie de prioritate
		if(elPr < taskPr && introdus == 0) {
			IntrQ(caux, &task);
			introdus = 1;
		} else if(elPr == taskPr && introdus == 0) {
			unsigned int elTE = element.timpDeExecutie;
			unsigned int taskTE = task.timpDeExecutie;

			// ordonam crescator in functie de timpul de executie
			if(elTE > taskTE && introdus == 0) {
				IntrQ(caux, &task);
				introdus = 1;
			} else if(elTE == taskTE && introdus == 0) {
				if(element.ID > task.ID && introdus == 0) {

					// ordonam crescsator in functie de ID
					IntrQ(caux, &task);
					introdus = 1;
				}
			}
		}
		IntrQ(caux, &element);
	}
	if(!ExtrQ(waitingQ, x) && introdus == 0) {
		IntrQ(caux, &task);
	}
	ConcatQ(waitingQ, caux);
	DistrQ(&caux);
	return 1;
}

// Creare task nou

int addTasks(void *waitingQ, char *p, int *idV, FILE *g)
{
	p = strtok(NULL, " ");
	unsigned int numberOfTasks = atoi(p);
	p = strtok(NULL, " ");
	unsigned int timpDeExecutie = atoi(p);
	p = strtok(NULL, " ");
	char prioritate = atoi(p);
	int i;
	for(i = 0; i < numberOfTasks; i++) {
		Task task;
		int j;
		for(j = 1; j <= MAXSHORT; j++) {
			if(idV[j] == 1) {
				task.ID = j;
				idV[j] = 0;
				break;
			}
		}
		task.thread = -1;
		task.timpDeExecutie = timpDeExecutie;
		task.timpRamas = timpDeExecutie;
		task.prioritate = prioritate;
		IntrWaitingQ(waitingQ, task);
		fprintf(g, "Task created successfully : ID %d.\n", task.ID);
	}
	return 1;
}

// Aflare informatii cu privire la un task

int getTask(void *runningQ, void *waitingQ, void *finishedQ, char *p, FILE *g)
{
	p = strtok(NULL, " ");
	int index = atoi(p);
	int gasit = 0;

	void *x = malloc(sizeof(Task));
	if(!x)
		return 0;
	void *caux = InitQ(sizeof(Task));
	if(!caux) {
		free(x);
		return 0;
	}
	
	// verificam daca taskul se afla in coada running
	while(ExtrQ(runningQ, x)) {
		if(((Task*)x)->ID == index) {
			fprintf(g, "Task %d is running ", ((Task*)x)->ID);
			fprintf(g, "(remaining_time = ");
			fprintf(g, "%d).\n", ((Task*)x)->timpRamas);
			gasit = 1;
		}
		IntrQ(caux, x);
	}
	ConcatQ(runningQ, caux);
	ResetQ(caux);
	
	// verificam daca taskul se afla in coada waiting
	while(ExtrQ(waitingQ, x)) {
		if(((Task*)x)->ID == index) {
			fprintf(g, "Task %d is waiting ", ((Task*)x)->ID);
			fprintf(g, "(remaining_time = ");
			fprintf(g, "%d).\n", ((Task*)x)->timpRamas);
			gasit = 1;
		}
		IntrQ(caux, x);
	}
	ConcatQ(waitingQ, caux);
	ResetQ(caux);
	
	// verificam daca taskul se afla in coada finished
	while(ExtrQ(finishedQ, x)) {
		if(((Task*)x)->ID == index) {
			if(!gasit) {
				fprintf(g, "Task %d ", ((Task*)x)->ID);
				fprintf(g, "is finished ");
				fprintf(g, "(executed_time = ");
				fprintf(g, "%d", ((Task*)x)->timpDeExecutie);
				fprintf(g, ").\n");
				gasit = 1;
			}
		}
		IntrQ(caux, x);
	}
	ConcatQ(finishedQ, caux);
	DistrQ(&caux);
	
	// daca nu am gasit taskul, atunci printam acest fapt
	if(gasit == 0)
		fprintf(g, "Task %d not found.\n", index);
	free(x);
	return 1;
}

// Aflare informatii cu privire la un thread

int getThread(void *Pool, void *runningQ, char *p, FILE *g)
{
	p = strtok(NULL, " ");
	int index = atoi(p);
	int gasit = 0;
	void *x = malloc(sizeof(Task));
	if(!x)
		return 0;

	// cream o coada auxiliara pentru a stoca elementele din runningQ
	void *caux = InitQ(sizeof(Task));
	if(!caux) {
		free(x);
		return 0;
	}

	// verificam daca threadul cu id-ul dat ruleaza un task
	// daca raspunsul este pozitiv, atunci incheiem procesul de cautare
	while(ExtrQ(runningQ, x)) {
		if(((Task*)x)->thread == index) {
			fprintf(g, "Thread %d is running task ", index);
			fprintf(g, "%d ", ((Task*)x)->ID);
			fprintf(g, "(remaining_time = ");
			fprintf(g, "%d).\n", ((Task*)x)->timpRamas);
			gasit = 1;
		}
		IntrQ(caux, x);
	}
	ConcatQ(runningQ, caux);
	free(x);
	DistrQ(&caux);
	if(gasit == 1)
		return 1;

	x = malloc(sizeof(Thread));
	if(!x)
		return 0;

	// cream o stiva auxiliara pentru a stoca elementele din Pool
	void *saux = InitS(sizeof(Thread));
	if(!saux) {
		free(x);
		return 0;
	}

	// verificam daca threadul cu id-ul dat exista,
	// dar nu ruleaza niciun task
	while(Pop(Pool, x)) {
		if(((Thread*)x)->ID == index) {
			fprintf(g, "Thread %d is idle.\n", ((Thread*)x)->ID);
			gasit = 1;
		}
		Push(saux, x);
	}
	Rastoarna(Pool, saux);
	free(x);
	DistrS(&saux);
	return 1;
}


// Rulare taskuri pentru o perioada de timp

int run(void *Pool, void *waitingQ, void *finishedQ, void *runningQ,
int Q, int N, char *p, int *idV, FILE *g)
{
	p = strtok(NULL, " ");
	unsigned int time = atoi(p);
	// runningTime este contorul, timpul real de rulare
	int runningTime = 0;
	fprintf(g, "Running tasks for %d ms...\n", time);

	// rulam in functie de cuanta de timp Q
	int i;
	for(i = 0; i < time/(float)Q; i += 1) {
		int runForMiliseconds; // retine nr de ms pt care ruleaza
		if(Q*(i+1) < time)
			runForMiliseconds = Q;
		else
			runForMiliseconds = time - i*Q;

		void *x = malloc(sizeof(Task));
		if(!x)
			return 0;

		if(VidaQ(runningQ) && VidaQ(waitingQ)) {
			// daca nu mai exista taskuri
			// returneaza runningTime
			return runningTime;
		} else if(VidaQ(runningQ)) {
			int j;
			for(j = 0; j < N; j++) {
				// ruleaza N thread-uri paralel
				if(ExtrQ(waitingQ, x)) {
					void *y = malloc(sizeof(Thread));
					if(!y) {
						free(x);
						return 0;
					}
					Pop(Pool, y);
					// fiecare task extras din coada
					// waiting primeste un thread
					((Task*)x)->thread = ((Thread*)y)->ID;
					IntrQ(runningQ, x);
					free(y);
				} else {
					break;
				}
			}
		}

		void *caux = InitQ(sizeof(Task)); // coada auxiliara
		if(!caux)
			return 0;

		// numberOfTasksStillRunning
		int nbTasksStillRunning = 0;
		int maxRunned = 0;
		while(!VidaQ(runningQ)) {
			ExtrQ(runningQ, x);
			// rulam taskurile din coada runningQ
			if(((Task*)x)->timpRamas > runForMiliseconds) {
				// timpul de rulare ramas scade
				((Task*)x)->timpRamas -= runForMiliseconds;
				IntrQ(caux, x);
				nbTasksStillRunning++;
				maxRunned = runForMiliseconds;
			} else {
				if(((Task*)x)->timpRamas > maxRunned)
					maxRunned = ((Task*)x)->timpRamas;
				// reintroducem threadul in Pool
				// introducem taskul in coada finishedQ
				Push(Pool, &((Task*)x)->thread);
				IntrQ(finishedQ, x);
				// id-ul taskului redevine available
				idV[((Task*)x)->ID] = 1;
			}
		}
		free(x);

		// introducem noi taskuri in coada running
		// dupa terminarea celorlalte
		int j;
		for(j = nbTasksStillRunning; j < N; j++) {
			void *x = malloc(sizeof(Task));
			if(!x)
				return 0;
			if(!VidaQ(waitingQ)) {
				ExtrQ(waitingQ, x);
				void *y = malloc(sizeof(Thread));
				if(!y) {
					free(x);
					return 0;
				}
				if(!VidaS(Pool))
					Pop(Pool, y);
				((Task*)x)->thread = ((Thread*)y)->ID;
				IntrQ(caux, x);
				free(y);
			}
			free(x);
		}
		ConcatQ(runningQ, caux);
		DistrQ(&caux);
		runningTime += maxRunned;
	}

	// returneaza timpul rulat
	return runningTime;

}

// Rulare taskuri pana cand toate ajung in finishedQ

int finish(void *Pool, void *waitingQ, void *finishedQ, void *runningQ,
int Q, int N)
{
	int runningTime = 0;
	while(1) {
		void *x = malloc(sizeof(Task));
		if(!x)
			return 0;

		if(VidaQ(runningQ) && VidaQ(waitingQ)) {
			// daca nu mai exista taskuri
			// returneaza runningTime
			return runningTime;
		} else if(VidaQ(runningQ)) {
			int i;
			for(i = 0; i < N; i++) {
				// ruleaza N thread-uri paralel
				if(ExtrQ(waitingQ, x)) {
					void *y = malloc(sizeof(Thread));
					if(!y) {
						free(x);
						return 0;
					}
					Pop(Pool, y);
					// fiecare task extras din coada
					// waiting primeste un thread
					((Task*)x)->thread = ((Thread*)y)->ID;
					IntrQ(runningQ, x);
					free(y);
				} else {
					break;
				}
			}
		}

		void *caux = InitQ(sizeof(Task));
		if(!caux) {
			free(x);
			return 0;
		}

		int nbTasksStillRunning = 0;
		int maxRunned = 0;
		while(ExtrQ(runningQ, x)) {
			// rulam taskurile din coada runningQ
			if(((Task*)x)->timpRamas > Q) {
				// timpul de rulare ramas scade
				((Task*)x)->timpRamas -= Q;
				IntrQ(caux, x);
				nbTasksStillRunning++;
				maxRunned = Q;
			} else {
				if(((Task*)x)->timpRamas > maxRunned)
					maxRunned = ((Task*)x)->timpRamas;
				// reintroducem threadul in Pool
				// introducem taskul in coada finishedQ
				Push(Pool, &((Task*)x)->thread);
				IntrQ(finishedQ, x);
			}
		}
		free(x);

		// introducem noi taskuri in coada running
		// dupa terminarea celorlalte
		int i;
		for(i = nbTasksStillRunning; i < N; i++) {
			void *x = malloc(sizeof(Thread));
			if(!x)
				return 0;
			if(ExtrQ(waitingQ, x)) {
				void *y = malloc(sizeof(Task));
				if(!y) {
					free(x);
					return 0;
				}
				Pop(Pool, y);
				((Task*)x)->thread = ((Thread*)y)->ID;
				IntrQ(caux, x);
				free(y);
			}
			free(x);
		}
		ConcatQ(runningQ, caux);
		DistrQ(&caux);
		runningTime += maxRunned;
	}

	// returneaza timpul rulat
	return runningTime;
}

// Functia main

int main(int argc, char **argv)
{
	// deschidem fisierele
	if (!argv[1])
   		return 0;
 	FILE *f = fopen(argv[1], "r");
 	if (!f)
   		return 0;
   	if(!argv[2])
   		return 0;
   	FILE *g = fopen(argv[2], "w");
 	if (!g)
   		return 0;

	// citim Q si C
	int Q;
	int C;
	fscanf(f, "%d", &Q);
	fscanf(f, "%d", &C);
	int N = 2*C;

	// initializare stive / cozi
	void *Pool = InitS(sizeof(Thread));
	if(!Pool)
		return 0;
	void *waitingQ = InitQ(sizeof(Task));
	if(!waitingQ) {
		DistrS(&Pool);
		return 0;
	}
	void *finishedQ = InitQ(sizeof(Task));
	if(!finishedQ) {
		DistrQ(&waitingQ);
		DistrS(&Pool);
		return 0;
	}
	void *runningQ = InitQ(sizeof(Task));
	if(!runningQ) {
		DistrQ(&finishedQ);
		DistrQ(&waitingQ);
		DistrS(&Pool);
		return 0;
	}

	// punem elementele de tip Thread in Pool
	int i;
	for(i = N-1; i >= 0; i--) {
		Thread thread;
		thread.ID = i;
		Push(Pool, &thread.ID);
	}

	// initializam vectorul de id-uri valabile
	int idV[MAXSHORT] = {0};
	for(i = 1; i < MAXSHORT; i++)
		idV[i] = 1;

	// timeRunning contine nr de milisecunde in care au rulat taskuri
	unsigned int timeRunning = 0;

	char instruction[256];
	while(fgets(instruction, MAX, f)) {
		// citim instructiuni din fisier
		if(instruction[0] == '\n')
			continue;
		if(instruction[strlen(instruction)-1] == '\n') {
			instruction[strlen(instruction)-1] = '\0';
		}
		if(!strcmp(instruction, "print waiting")) {
			fprintf(g, "====== Waiting queue =======\n");
			AfisareQ(waitingQ, AfiWaiting, g);
			continue;
		} else if(!strcmp(instruction, "print finished")) {
			fprintf(g, "====== Finished queue =======\n");
			AfisareQ(finishedQ, AfiFinished, g);
			continue;
		} else if(!strcmp(instruction, "print running")) {
			fprintf(g, "====== Running in parallel =======\n");
			AfisareQ(runningQ, AfiRunning, g);
			continue;
		}
		char *p = strtok(instruction, " ");
		if(!strcmp(p, "add_tasks")) {
			addTasks(waitingQ, p, idV, g);
		} else if(!strcmp(p, "run")) {
			timeRunning += run(Pool, waitingQ,
			finishedQ, runningQ, Q, N, p, idV, g);

		} else if(!strcmp(p, "get_task")) {
			getTask(runningQ, waitingQ,
			finishedQ, p, g);
		} else if(!strcmp(p, "get_thread")) {
			getThread(Pool, runningQ, p, g);
		} else if(!strcmp(p, "finish")) {
			timeRunning += finish(Pool, waitingQ,
			finishedQ, runningQ, Q, N);

			fprintf(g, "Total time: %d", timeRunning);
		}
	}
	fflush(g);
	fclose(f);
	fclose(g);
 	return 0;
}


/* Home assignment for parallel computing course 2017.
 * MIPT DCAM student Nikita Pettik, 377 group.
 * 
 * Task description: simulate one-dimension brownian motion using any
 * parallel library/language. Support two execution modes:
 * 1. Run program till iteration limit.
 * 2. Run program till time comes up.
 * Probability to move left/right is 0.5
 * It is possible to set different parameters: 
 * crystal length, number of particles, 
 * number of iterations/time of execution, execution mode.
 *
 * To compile enter "make"
 * To run enter ./brownian_motion
 *
 * Tested on Ubuntu 14.04 and MacOS Sierra 10.12.6 with Clang and gcc compilers.
 * Output is displayed in format [number of cell | number of particles inside].
 *
 * Example of usage:
 * help
 * run
 * set particles 100
 * set mode time
 * set time 15
 * set length 20
 * info
 * run
 * set mode iterations
 * set iterations 100
 * run
 * exit
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include "brownian_motion.h"


int main(int argc, char **argv) {
	initSystem();
	greetings();
	
	ProgramParameters params;
	setDefaultParameters(&params);
	char command[64];
	/* Main cycle. */
	for(;;) {

	for (int i = 0; i < 64; i++) command[i] = '\0';
	if (feof(stdin)) flushInput();
	
	if (1 != scanf("%[^\n]%*c", command)) {
		fprintf(stdout, "%s\n", "Scanf failed to read input expression. "\
			"Please, try again.");
		flushInput();
		continue;
	}

	if (0 == strcmp(command, "exit")) {
		finilizeSystem();
		return 0;
	} else if (0 == strcmp(command, "help")) help();
	else if (0 == strcmp(command, "run")) runMasterThread(&params);
	else if (0 == strcmp(command, "info")) info(&params);
	else if (0 == strncmp(command, "set", 3)) setParameters(command, &params); 
	else {
		fprintf(stdout, "Input expression is not a valid command: %s\n", command);
	}}

	finilizeSystem();

	return 0;
}

void
runMasterThread(const ProgramParameters *params) {
	assert(NULL != params);

	Crystal *crystal = createCrystal(params->crystalLength, 
					 params->numbOfParticles, 
					 params->probability);

	pthread_t *threads = createThreads(params->numbOfThreads);
	ThreadArg *args = createThreadArgs(params->numbOfThreads);

	void *threadStatus;
	Particle *currParticle = crystal->cells[0]->headParticle;
	workStop = 0;

	/* Prepare args and create threads. */
	for (size_t i = 0; i < params->numbOfThreads; ++i) {
		args[i].crystal = crystal;
		args[i].particle = currParticle;

		pthread_create(&threads[i], NULL, initThreadFunction, (void*) &args[i]);
		currParticle = currParticle->pNext;
	}
	
	if (params->mode == ITERATION_LIMIT) {
		assert(params->numbOfIterations > 0);
		for (uint32_t iter = 0; iter < params->numbOfIterations; ++iter)
			runIteration(crystal, params->numbOfThreads);
	} else {
		assert(params->executionTime > 0);
		time_t timeToEnd = params->executionTime;
		time_t timeBeforeIter;
		time_t tiemAfterIter;
		while (timeToEnd > 0) {
			time(&timeBeforeIter);
			runIteration(crystal, params->numbOfThreads);
			time(&tiemAfterIter);;
			timeToEnd -= (tiemAfterIter - timeBeforeIter);
		}
	}

	workStop = 1;
	
	for (size_t i = 0; i < params->numbOfThreads; ++i) {
		if (pthread_join(threads[i], &threadStatus)) {
			fprintf(stdout, "Oops, something went wrong with %lu thread\n", i);
		}
	}

	displayCrystal(crystal);
	destroyCrystal(crystal);
	free(threads);
	free(args);
}

static void
runIteration(Crystal *crystal, const uint32_t numbOfThreads) {
	assert(NULL != crystal);
	assert(numbOfThreads > 0);

	pthread_mutex_lock(&currentlyIdleMutex);
	while (currentlyIdle != numbOfThreads)
		pthread_cond_wait(&currentlyIdleCond, &currentlyIdleMutex);
	pthread_mutex_unlock(&currentlyIdleMutex);

	workFinish = 0;
	activeWorkers = numbOfThreads;

	pthread_mutex_lock(&workReadyMutex);
	workReady = 1;
	pthread_cond_broadcast(&workReadyCond);
	pthread_mutex_unlock(&workReadyMutex);

	pthread_mutex_lock(&activeWorkersMutex);
	while (activeWorkers > 0)
		pthread_cond_wait(&activeWorkersCond, &activeWorkersMutex);
	for (size_t i = 0; i < crystal->length; ++i) {
		crystal->cells[i]->headParticle = 
			crystal->cells[i]->newGeneration;
		crystal->cells[i]->newGeneration = NULL;
	}
	pthread_mutex_unlock(&activeWorkersMutex);

	workReady = 0;
	currentlyIdle = 0;

	pthread_mutex_lock(&workFinishMutex);
	workFinish = 1;
	pthread_cond_broadcast(&workFinishCond);
	pthread_mutex_unlock(&workFinishMutex);	
}

void*
initThreadFunction(void *voidArg) {
	assert(NULL != voidArg);

	ThreadArg *arg = (ThreadArg*) voidArg;
	Crystal *crystal = arg->crystal;
	Particle *particle = arg->particle;

	while(!workStop) {
		simulateBrownianMotion(crystal, particle);
	}
	pthread_exit(NULL);
}

static void
simulateBrownianMotion(Crystal *crystal, Particle *particle) {
		pthread_mutex_lock(&currentlyIdleMutex);
		currentlyIdle++;
		pthread_cond_signal(&currentlyIdleCond);
		pthread_mutex_unlock(&currentlyIdleMutex);
		
		pthread_mutex_lock(&workReadyMutex);
		while (!workReady && !workStop)
			pthread_cond_wait(&workReadyCond, &workReadyMutex);
		pthread_mutex_unlock(&workReadyMutex);

		simulateOneParticle(crystal, particle);
		
		pthread_mutex_lock(&activeWorkersMutex);
		activeWorkers--;
		pthread_cond_signal(&activeWorkersCond);
		pthread_mutex_unlock(&activeWorkersMutex);

		pthread_mutex_lock(&workFinishMutex);
		while (!workFinish && !workStop)
			pthread_cond_wait(&workFinishCond, &workFinishMutex);
		pthread_mutex_unlock(&workFinishMutex);
}

static pthread_t*
createThreads(const uint32_t numbOfThreads) {
	return (pthread_t*) calloc(numbOfThreads, sizeof(pthread_t));
}

static ThreadArg*
createThreadArgs(const uint32_t numbOfThreads) {
	return (ThreadArg*) calloc(numbOfThreads, sizeof(ThreadArg));
}

void 
initSystem() {
	srand(time(NULL));
}

void
finilizeSystem() {
	pthread_mutex_destroy(&activeWorkersMutex);
	pthread_mutex_destroy(&currentlyIdleMutex);
	pthread_mutex_destroy(&workReadyMutex);
	pthread_mutex_destroy(&workFinishMutex);

	pthread_cond_destroy(&activeWorkersCond);
	pthread_cond_destroy(&currentlyIdleCond);
	pthread_cond_destroy(&workReadyCond);
	pthread_cond_destroy(&workFinishCond);
}

void 
help() {
	fprintf(stdout, "%s \n", "Author: Nikita Pettik.");
	fprintf(stdout, "%s \n", "Available commands:");
	fprintf(stdout, "%s \n", "1. run");
	fprintf(stdout, "%s \n", "2. exit");
	fprintf(stdout, "%s \n", "3. set [particles | mode | length | time | iterations]");
	fprintf(stdout, "%s \n", "4. info");
	fprintf(stdout, "%s \n", "5. help");
	fprintf(stdout, "\n");
}

static void
greetings() {
	fprintf(stdout, "%s \n", "Welcome to the brownian motion simulation.");
	fprintf(stdout, "%s \n", "Please, enter parameters or use default ones.");
	fprintf(stdout, "%s \n", "To run simulation with default settings "\
		"just input \"run\"");
	fprintf(stdout, "%s \n", "If you want to specify parameters, then "\
		"enter command \"set [particles | mode | length | iterations | time]\"");
	fprintf(stdout, "%s \n", "To get info about crystal params input \"info\"");
	fprintf(stdout, "%s \n", "To get information about available commands "\
		"enter \"help\"");
	fprintf(stdout, "\n"); 
}

static void
setDefaultParameters(ProgramParameters *params) {
	assert(NULL != params);

	params->crystalLength = DEFAULT_CRYSTAL_LENGTH;
	params->numbOfParticles = DEFAULT_NUMB_OF_PARTICLES;
	params->numbOfThreads = params->numbOfParticles;
	params->numbOfIterations = DEFAULT_NUMB_OF_ITERATIONS;
	params->probability = DEFAULT_PROBABILITY;
	params->mode = ITERATION_LIMIT;
	params->executionTime = DEFAULT_EXECUTION_TIME;
}

void 
setParameters(const char* command, ProgramParameters *params) {
	assert(NULL != command);
	assert(NULL != params);

	char *pch = strtok((char*) command, " ,.-");
	while (NULL != pch) {
		if (0 == strcmp(pch, "particles")) {
			pch = strtok(NULL, " ,.=");
			long int newNumbOfParticles = strtol(pch, NULL, 10 /* base */);
			if (0 >= newNumbOfParticles) {
				fprintf(stdout, "%s \n", 
					"Error: can't read number of particles.");
				return;
			}
			params->numbOfParticles = newNumbOfParticles;
			params->numbOfThreads = params->numbOfParticles;
			return;
		}

		if (0 == strcmp(pch, "mode")) {
			pch = strtok(NULL, " ,.=");
			if (0 == strcmp(pch, "iterations"))
				params->mode = ITERATION_LIMIT;
			if (0 == strcmp(pch, "time"))
				params->mode = TIME_LIMIT;
			else {
				fprintf(stdout, "%s \n", 
					"Error: can't read type of mode.");
			}
			return;
		}

		if (0 == strcmp(pch, "length")) {
			pch = strtok(NULL, " ,.=");
			long int newLength = strtol(pch, NULL, 10 /* base */);
			if (0 >= newLength) {
				fprintf(stdout, "%s \n", 
					"Error: can't read length of crystal.");
				return;
			}
			params->crystalLength = newLength;
			return;
		}

		if (0 == strcmp(pch, "iterations")) {
			pch = strtok(NULL, " ,.=");
			long int newNumbOfIterations = strtol(pch, NULL, 10 /* base */);
			if (0 >= newNumbOfIterations) {
				fprintf(stdout, "%s \n", 
					"Error: can't read number of iterations.");
				return;
			}
			params->numbOfIterations = newNumbOfIterations;
			return;
		}

		if (0 == strcmp(pch, "time")) {
			pch = strtok(NULL, " ,.=");
			long int newExecutionTime = strtol(pch, NULL, 10 /* base */);
			if (0 >= newExecutionTime) {
				fprintf(stdout, "%s \n", 
					"Error: can't read execution time.");
				return;
			}
			params->executionTime = newExecutionTime;
			return;
		}

		pch = strtok(NULL, " ,.=");
	}
}

void
info(const ProgramParameters *params) {
	fprintf(stdout, "Crystal state: \n");
	fprintf(stdout, "Length = %d\nNumber of particles = %d\n"\
		"Number of iterations = %d\nProbability = %lg\nTime = %ld\n", 
		params->crystalLength, params->numbOfParticles,
		params->numbOfIterations, params->probability, 
		params->executionTime);
}

static Particle* 
createParticleList(const uint32_t numbOfParticles, Cell *parent) {
	assert(NULL != parent);

	Particle *head  = (Particle*) calloc(1, sizeof(Particle));
	head->pNext = NULL;
	head->pPrev = NULL;
	head->parent = parent;
	head->payload = NULL;
	Particle *currentParticle = head;
	Particle *newParticle;	

	for (size_t i = 0; i < numbOfParticles - 1; ++i) {
		newParticle = (Particle*) calloc(1, sizeof(Particle));
		newParticle->pNext = NULL;
		newParticle->pPrev = currentParticle;
		newParticle->parent = parent;
		newParticle->payload = NULL;
		currentParticle->pNext = newParticle;
		currentParticle = newParticle;
	}

	return head;
}

Crystal*
createCrystal(const uint32_t length, const uint32_t numbOfParticles,
	      const double probability) {
	assert(length > 0);
	assert(numbOfParticles > 0);

	Crystal *crystal = (Crystal*) calloc(1, sizeof(Crystal));
	crystal->length = length;
	crystal->numbOfParticles = numbOfParticles;	
	crystal->probabilityToMoveLeft = probability;	

	crystal->cells = (Cell**) calloc(crystal->length, sizeof(Cell*));
	for (size_t i = 0; i < crystal->length; ++i) {
		crystal->cells[i] = (Cell*) calloc(crystal->length, sizeof(Cell));
	}
	crystal->cells[0]->headParticle = createParticleList(numbOfParticles, 
							     crystal->cells[0]);
	crystal->cells[0]->newGeneration = NULL;

	for (size_t i = 1; i < crystal->length; ++i) {
		crystal->cells[i]->position = i;
		pthread_mutex_init(&crystal->cells[i]->mutex, NULL);
		crystal->cells[i]->headParticle = NULL;
		crystal->cells[i]->newGeneration = NULL;
	}
	
	return crystal;	
}

static SimulationStatus
moveParticle(Particle *particle, Cell *cell) {
	assert(NULL != particle);
	assert(NULL != cell);

	Particle *tail = getTail(cell->newGeneration);

	if (tail == NULL) {
		cell->newGeneration = particle; 	
		particle->pNext = NULL;
		particle->pPrev = NULL;
		particle->parent = cell;
	} else {
		particle->pNext = NULL;
		particle->pPrev = tail;
		particle->parent = cell;
		tail->pNext = particle;
	}

	return SIMULATE_OK;
}

static Particle*
getTail(const Particle *head) {
	if (NULL == head) return (Particle*) head;

	const Particle *tail = head;
	while (NULL != tail->pNext)
		tail = tail->pNext;

	return (Particle*) tail; 
} 

static int
getDirectionToMove(const double referenceProb, const size_t position,
		   const uint32_t border) {
	assert(referenceProb >= 0);

	double prob = getProb();
	if (prob >= referenceProb && position < border)
		return 1;
	if ((prob >= referenceProb && position == border) ||
	    (prob < referenceProb && position == 0))
		return 0;
	return -1;
}	

double
getProb() {
	return ((double)rand()) / RAND_MAX;
}

SimulationStatus
simulateOneParticle(Crystal *crystal, Particle *particle) {
	assert(NULL != crystal);
	assert(NULL != particle);
	
	double referenceProb = crystal->probabilityToMoveLeft;
	uint32_t pos = particle->parent->position;
	uint32_t crystalLength = crystal->length;

	SimulationStatus rc;
	int dir = getDirectionToMove(referenceProb, pos, crystalLength - 1);
	pthread_mutex_lock(&crystal->cells[pos + dir]->mutex);
	rc = moveParticle(particle, crystal->cells[pos + dir]);
	pthread_mutex_unlock(&crystal->cells[pos + dir]->mutex);
	
	return rc;
}

void
displayCrystal(const Crystal *crystal) {
	assert(NULL != crystal);

	fprintf(stdout, "Crystal state: \n");
	fprintf(stdout, "Length = %d\nNumber of particles = %d\n" \
		"probability = %lg\n", crystal->length, crystal->numbOfParticles,
		crystal->probabilityToMoveLeft);
	uint32_t particlesInCell;
	Particle *currParticle;
	for (size_t i = 0; i < crystal->length; ++i) {
		particlesInCell = 0;
		currParticle = crystal->cells[i]->headParticle;
		while (NULL != currParticle) {
			currParticle = currParticle->pNext;
			particlesInCell++;
		}
		fprintf(stdout, " %lu: %d |", i, particlesInCell); 
	}
	fprintf(stdout, "\n--------------------------------------\n");
}


static void 
flushInput() {
	int ch;
	while ((ch = getchar()) != EOF && ch != '\n');
}

static void
destroyParticleList(Particle *particle) {
	Particle *nextParticle;
	while (NULL != particle) {
		nextParticle = particle->pNext;
		free(particle);
		particle = nextParticle;
	}
}

void
destroyCrystal(Crystal *crystal) {
	assert(NULL != crystal);	

	for (size_t i = 0; i < crystal->length; ++i) {
		destroyParticleList(crystal->cells[i]->headParticle);
		pthread_mutex_destroy(&crystal->cells[i]->mutex);
		free(crystal->cells[i]);
	}

	free(crystal->cells);
}

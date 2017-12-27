#ifndef BROWNIAN_MOTION_H
#define BROWNIAN_MOTION_H

#include <pthread.h>

/* By default iteration limit mode is enabled. */
typedef enum ExecutionMode {
	TIME_LIMIT,
	ITERATION_LIMIT
} ExecutionMode;

typedef enum SimulationStatus {
	SIMULATE_OK,
	SIMULATE_ERROR
} SimulationStatus;

/* Forward declaration. */
struct Cell;

/* All particles are orginized as a double-linked list 
 * which starts from parent Cell. 
 */
typedef struct Particle {
	struct Cell *parent;
	struct Particle *pNext;
	struct Particle *pPrev;
	/* In case of different particle types or whatever. */
	void *payload;
} Particle;

/* Structure which represents a unit of crystal. Cells are orginized in
 * array and contain their position, mutex and two linked lists of particles:
 * the first one represents particles state before iteration;
 * the second one -- new particles, which are trapped in this cell at the 
 * end of iteration.
 * Mutex is required because access to list of new particles should be
 * synchronized. 
 */
typedef struct Cell {
	uint32_t position;
	pthread_mutex_t mutex;
	/* Linked lists of particles. */
	Particle *headParticle;
	Particle *newGeneration;
} Cell;

typedef struct Crystal {
	/* Array of pointers to cell. */
	Cell **cells;
	uint32_t length;
	uint32_t numbOfParticles;
	double probabilityToMoveLeft;
} Crystal;

/* Arguments which are passed to thread function. */
typedef struct ThreadArg {
	Crystal *crystal;
	Particle *particle;

} ThreadArg;

/* Parameters which are determine brownian motion simulation. */
typedef struct ProgramParameters {
	uint32_t crystalLength;
	uint32_t numbOfParticles;
	uint32_t numbOfThreads;
	uint32_t numbOfIterations;
	time_t executionTime;
	double probability;
	ExecutionMode mode;
} ProgramParameters;

/* Gloabl variables below are used to synchronize cycle-iteration routine.
 * All workers have to wait signal from master-thread to start iteration.
 * In the same way, they send to master signal when the work is done.
 * When the number of active threads equals to zero, master-thread
 * updates array of cells and starts new iteration.
 */

/* Threads which still in process of executing thread routine */
pthread_mutex_t activeWorkersMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t activeWorkersCond = PTHREAD_COND_INITIALIZER;
uint32_t activeWorkers;

/* Threads which have completed their routine and 
 * are waiting for master-thread. */
pthread_mutex_t currentlyIdleMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t currentlyIdleCond = PTHREAD_COND_INITIALIZER;
uint8_t currentlyIdle;

/* Variable which is used to signal threads to start thread routine. */
pthread_mutex_t workReadyMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t workReadyCond = PTHREAD_COND_INITIALIZER;
uint8_t workReady;

/* Varaible which is used to signal threads to end thread routine. */ 
pthread_mutex_t workFinishMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t workFinishCond = PTHREAD_COND_INITIALIZER;
uint8_t workFinish;

/* Variable which is used to completely stop thread routine. */
uint8_t workStop;

/* Default parameters. */
const uint32_t DEFAULT_CRYSTAL_LENGTH = 10;
const uint32_t DEFAULT_NUMB_OF_PARTICLES = 6;
const uint32_t DEFAULT_NUMB_OF_ITERATIONS = 10;
const double DEFAULT_PROBABILITY = 0.5;
const time_t DEFAULT_EXECUTION_TIME = 10;
const time_t DEFAULT_ITERATION_DELAY = 1;
const uint32_t DEFAULT_DISPLAY_TIMES = 4;

/* This function implemets routine for master-thread. 
 * To be more precise, it creates and joins all other threads and provides 
 * synchronization between cycles.
 */
void
runMasterThread(const ProgramParameters *);

/* Implemetation of one iteration of master-thread. */
static void
runIteration(Crystal *, uint32_t);

/* Display info about program parameters. */
void
info(const ProgramParameters *);

void*
initThreadFunction(void *);

static void
simulateBrownianMotion(Crystal *, Particle *); 

void
initSystem();

/* Destroying mutexes and conditional variables. */
void
finilizeSystem();

void
setParameters(const char *, ProgramParameters *);

static void
setDefaultParameters(ProgramParameters *);

/* Simply allocates threads on heap. */
static pthread_t*
createThreads(const uint32_t);

/* Simply allocates threads' args on heap. */
static ThreadArg*
createThreadArgs(const uint32_t);

void*
initThreadFunction(void *);

/* Prints greetings information to stdin. */
static void
greetings();

/* Prints helpful information to stdin. */
void
help();

/* Returns new instance of Crystal structure. */
Crystal*
createCrystal(const uint32_t, const uint32_t, const double);

static void
destroyCrystal(Crystal *);

/* Creates double-linked list of particles with root at cell. */
static Particle*
createParticleList(const uint32_t, Cell *);

static void
destroyParticleList(Particle *);

/* Move particle to the cell. Takes cell's list of new particles and
 * inserts particle into it.
 */
static SimulationStatus
moveParticle(Particle *, Cell *);

static SimulationStatus
simulateOneParticle(Crystal *, Particle*);

/* Returns last element from particle list */
static Particle*
getTail(const Particle *);

/* Nondeterministic function: returns random value in range [0, 1] */
double
getProb();

static double
convertIntToMantissa(long int);

/* Based on current position and length of crystal returns direction to move. */
static int
getDirectionToMove(const double, size_t, uint32_t);

/* Displays information about simulation and crystal state to stdin. */
void
displayCrystal(const Crystal *);

static void
flushInput();

#endif

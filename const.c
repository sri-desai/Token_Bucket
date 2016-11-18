/*******************************************************************
 * File Name : const.c
 *
 * Operations:
 * -----------
 * o Declares global variables
 ******************************************************************/


#include "commonheader.h"


/* General simulation variables */
int          globalStopTokenPacketServerThreads;
CLI_INFO     globalCLIINFO;
TIMER		 globalSimStartTime;
TIMER        globalSimEndTime;
E_SIM_STATUS globalSimStatus;
FILE        *globalFilePtr;
volatile int isIntruptServiced;
volatile int isIntruptOccured;

/* Packet variables*/
TIMER        globalPrevPacketTimer;


/* Token variables */
long long   	   globalTotalTokenCount;
unsigned long long globalTokenCount;


/* Queues used*/
My402List    globalQueueQ1;
My402List    globalQueueQ2;


/* Mutexes used */
MUTEX          globalTokenBucketMutex;
MUTEX          globalQ2Mutex;
MUTEX          globalQ1Mutex;
MUTEX          globalStatUpdateMutex;

/* Condition variables used */
CONDT 		   condPacketsAtQ1;
CONDT 		   condPacketsAtQ2;

/*Statistic variables*/
double             globalInterArrivalPacketTime;
double             globalServiceTimeMilli;
double             globalTimeSpentInSystemMilli;
double             globalTimeSpentAtQ1Micro;
double             globalTimeSpentAtQ2Micro;
double       	   globalTimeSpentAtS1ServerMilli;
double      	   globalTimeSpentAtS2ServerMilli;
double             globalStatTimeSpentInSystemMilli;
double             globalStatMeanOfTimeMilli;
double             globalStatVarOfTimeMilli;
unsigned long long globalPacketRemovedQ1;
unsigned long long globalPacketRemovedQ2;
unsigned long long globalDroppedTokenCount;
unsigned long long globalPacketCount;
unsigned long long globalDroppedPacketCount;

/* Global Pthread ID's */
pthread_t       tokenGeneratorThread;
pthread_t       packetGeneratorThread;
pthread_t       server1GeneratorThread;
pthread_t       server2GeneratorThread;
pthread_t 		sigIntHandlerThread;


/* Signal handler set*/
sigset_t          globalSignalSet;

/* Debug consts: Need to remove at submission*/
FILE *debug_filePtr;


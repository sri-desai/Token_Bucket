/*
 * commonheader.h
 *
 *  Created on: May 16, 2016
 *      Author: srinivas
 */

#ifndef COMMONHEADER_H_
#define COMMONHEADER_H_

#include "my402list.h"
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <ctype.h>
#include <sys/stat.h>
#include <math.h>

#ifndef TRUE
#define FALSE 0
#define TRUE 1
#endif /* ~TRUE */

typedef struct timeval  TIMER;
typedef pthread_mutex_t MUTEX;
typedef pthread_cond_t  CONDT;
typedef struct stat T_DIR_TYPE;

typedef struct CLI_INFO
{
	int       B;
	int       P;
	int       num;
	float     lambda;
	float     mu;
	float     r;
	char    * tsfile;
}CLI_INFO;

typedef struct T_PACKET
{
	int   packetNumber;
	int   tokensRequired;
	float serviceTime;
	TIMER generationTime;
	TIMER arrivalTimeQ1;
	TIMER arrivalTimeQ2;
	TIMER serviceStartTime;
}T_PACKET;

typedef enum E_SIM_STATUS
{
	E_PACKET_NO_ACTIVITY = 0,
	E_PACKET_GENERATION_COMPLETE,
	E_PACKET_TRANSFER_COMPLETE,
	E_PACKET_SERVICE_COMPLETE,
}E_SIM_STATUS;

typedef enum E_ERROR_CODE
{
	E_INPUT_DIR,
	E_STAT_ERROR,
	E_CMDARG_ERROR,
	E_CMDLINE_ERROR,
	E_NO_DIGIT_ERROR,
	E_MEMALLOC_ERROR,
	E_R_NEGATIVE_ERROR,
	E_INVALID_CHAR_ERROR,
	E_MU_NEGATIVE_ERROR,
	E_LEADING_SPACE_ERROR,
	E_FILE_NOT_EXIST_ERROR,
	E_NO_PACKET_INFO_ERROR,
	E_TRAILING_SPACE_ERROR,
	E_LAMBDA_NEGATIVE_ERROR,
	E_INVALID_LINE_CNT_ERROR,
	E_BUCKET_OVER_UNDER_ERROR,
	E_INVALID_FIRST_LINE_ERROR,
	E_PACKET_COUNT_OVER_UNDER_ERROR,
	E_REQUIRED_TOKEN_OVER_UNDER_ERROR,
}E_ERROR_CODE;

extern sigset_t     globalSignalSet;
extern CONDT 		condPacketsAtQ1;
extern CONDT 		condPacketsAtQ2;
extern FILE        *globalFilePtr;
extern volatile int isIntruptOccured;
extern volatile int isIntruptServiced;
extern int          globalStopTokenPacketServerThreads;
extern MUTEX        globalQ2Mutex;
extern MUTEX        globalQ1Mutex;
extern MUTEX        globalTokenBucketMutex;
extern MUTEX        globalStatUpdateMutex;
extern TIMER		globalSimStartTime;
extern TIMER        globalPrevPacketTimer;
extern TIMER        globalSimEndTime;
extern double       globalTimeSpentAtQ1Micro;
extern double       globalTimeSpentAtQ2Micro;
extern double       globalServiceTimeMilli;
extern double       globalTimeSpentInSystemMilli;
extern double       globalInterArrivalPacketTime;
extern double       globalTimeSpentAtS1ServerMilli;
extern double       globalTimeSpentAtS2ServerMilli;
extern CLI_INFO     globalCLIINFO;
extern My402List    globalQueueQ1;
extern My402List    globalQueueQ2;
extern long long   	       globalTotalTokenCount;
extern unsigned long long  globalDroppedPacketCount;
extern unsigned long long  globalPacketCount;
extern unsigned long long  globalDroppedTokenCount;
extern unsigned long long  globalTokenCount;
extern unsigned long long globalPacketRemovedQ1;
extern unsigned long long globalPacketRemovedQ2;
extern E_SIM_STATUS 	   globalSimStatus;
extern pthread_t       tokenGeneratorThread;
extern pthread_t       packetGeneratorThread;
extern pthread_t       server1GeneratorThread;
extern pthread_t       server2GeneratorThread;
extern pthread_t 	   sigIntHandlerThread;
extern double          globalStatTimeSpentInSystemMilli;
extern double          globalStatMeanOfTimeMilli;
extern double          globalStatVarOfTimeMilli;

/* Debug consts: Need to remove at submission*/
FILE *debug_filePtr;

extern void   printStatistics 		  (TIMER);
extern int    transferPackets         (int *);
extern int    getPacketCount          (FILE *);
extern void   checkIsAllDigits        (char *);
extern void   checkFileExist          (char *);
extern void   globalVarInit           (void);
extern void   checkTFileFormart       (void);
extern void   readInputFromFile       (void);
extern void * SigIntHandlerFunc       (void *);
extern void   processQueues           (void);
extern void   submitError             (E_ERROR_CODE);
extern void   processInit             (int, char **);
extern void * packetGenThread         (void * param);
extern void * tokenGenThread          (void * param);
extern void * server1ThreadSubRoutine (void * param);
extern void * server2ThreadSubRoutine (void * param);
extern void   getPacketInfo           (FILE *, int *, int * , int *);
extern double diffTimeinMicroSec      (TIMER, TIMER);


/* Debug info*/
extern void debug_exec(void const * const firstParam, void const * const secParam);

#endif /* COMMONHEADER_H_ */

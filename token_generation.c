/*
 * token_generation.c
 *
 *  Created on: Jun 2, 2016
 *      Author: srinivas
 */


#include "commonheader.h"

void TokenthreadCancelHandlerFunc (MUTEX *mutexToUnlock)
{
	pthread_mutex_trylock(mutexToUnlock);

	pthread_mutex_unlock(mutexToUnlock);

	pthread_exit(NULL);
}


void * tokenGenThread (void * param)
{
	int          hasToExit;
	TIMER        prevTimeRed;
	TIMER 	     currTimeRed;
	TIMER        currTime;
	double 	     requiredTimeMicro;
	double 	     timeDifferenceRed;
	unsigned int localTokenCount;

	requiredTimeMicro = (1.0/globalCLIINFO.r)*1000000.00F;

	pthread_cleanup_push(TokenthreadCancelHandlerFunc, &globalQ1Mutex);

	usleep(requiredTimeMicro);

	hasToExit = FALSE;

	while(globalSimStatus != E_PACKET_TRANSFER_COMPLETE && globalStopTokenPacketServerThreads == FALSE)
	{
		gettimeofday(&prevTimeRed, NULL);

		pthread_mutex_lock(&globalQ1Mutex);

		if(globalTokenCount < globalCLIINFO.B)
		{
			globalTokenCount = globalTokenCount + 1;

			localTokenCount = globalTokenCount;

			gettimeofday(&currTime, NULL);

			pthread_mutex_unlock(&globalQ1Mutex);

			printf("%012.3fms:  token %lld Arrived and token bucket now has %d tokens\n",
				  diffTimeinMicroSec (currTime, globalSimStartTime)/1000.00F,
				  ++globalTotalTokenCount, localTokenCount);
		}
		else
		{
			pthread_mutex_unlock(&globalQ1Mutex);

			gettimeofday(&currTime, NULL);

			globalDroppedTokenCount++;

			printf("%012.3fms:  token %lld Arrived, dropped\n",
				  diffTimeinMicroSec (currTime, globalSimStartTime)/1000.00F,
				  ++globalTotalTokenCount);
		}

		transferPackets(&hasToExit);

		gettimeofday(&currTimeRed, NULL);

		timeDifferenceRed = diffTimeinMicroSec(currTimeRed, prevTimeRed);

		usleep(requiredTimeMicro - timeDifferenceRed);

	}

	pthread_mutex_lock(&globalQ1Mutex);

	pthread_cond_broadcast(&condPacketsAtQ2);

	pthread_mutex_unlock(&globalQ1Mutex);

	pthread_cleanup_pop(0);

	return (void *)0;
}

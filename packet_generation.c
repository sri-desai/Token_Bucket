/*
 * packet_generation.c
 *
 *  Created on: May 31, 2016
 *      Author: srinivas
 */


#include "commonheader.h"


void threadCancelHandlerFunc (MUTEX *mutexToUnlock)
{
	pthread_mutex_trylock(mutexToUnlock);

	pthread_mutex_unlock(mutexToUnlock);

	pthread_exit(NULL);
}


void packetGenerator (int tokensRequired, float serviceTime, int bucketDepth)
{
	double         interArrivalTime;
	TIMER          currTime;
	TIMER          arrivalTime;
	T_PACKET      *packetElem;

	packetElem = (T_PACKET *)malloc(sizeof(T_PACKET));

	gettimeofday(&currTime, NULL);

	if(packetElem == NULL)
	{
		submitError(E_MEMALLOC_ERROR);
	}
	else
	{
		;
	}

	if(tokensRequired > bucketDepth)
	{
		interArrivalTime = diffTimeinMicroSec(currTime, globalPrevPacketTimer);

		printf("%012.3fms:  p%lld arrives, needs %d tokens, inter-arrival time = %.3lfms, dropped\n",
				diffTimeinMicroSec (currTime, globalSimStartTime)/1000.00F,
				++globalPacketCount,
				tokensRequired,
				interArrivalTime/1000.00F);

		free(packetElem);

		++globalDroppedPacketCount;

	}
	else
	{
		pthread_mutex_lock(&globalQ1Mutex);

		My402ListAppend(&globalQueueQ1, packetElem);

		gettimeofday(&arrivalTime, NULL);

		pthread_cond_signal(&condPacketsAtQ1);

		packetElem->generationTime = currTime;
		packetElem->serviceTime    = serviceTime;
		packetElem->packetNumber   = ++globalPacketCount;
		packetElem->tokensRequired = tokensRequired;
		packetElem->arrivalTimeQ1  = arrivalTime;

		interArrivalTime = diffTimeinMicroSec(currTime, globalPrevPacketTimer);

		pthread_mutex_unlock(&globalQ1Mutex);

		printf("%012.3fms:  p%d arrives, needs %d tokens, inter-arrival time = %.3fms\n",
			   diffTimeinMicroSec (currTime, globalSimStartTime)/1000.00F, packetElem->packetNumber,
			   packetElem->tokensRequired, interArrivalTime/1000.00F);

		printf("%012.3fms:  P%d enters Q1\n",
		       diffTimeinMicroSec (arrivalTime, globalSimStartTime)/1000.00F, packetElem->packetNumber);
	}

	globalPrevPacketTimer = currTime;

	globalInterArrivalPacketTime += interArrivalTime;
}


void cmdLinePacketGenCall(void)
{
	int    maxNumberOfPackets;
	int    isTransmissionStop;
	double requiredTimeMicro;
	double timeDifferenceRed;
	TIMER  prevTimeRed;
	TIMER  currTimeRed;

	isTransmissionStop = FALSE;

	maxNumberOfPackets = globalCLIINFO.num;

	requiredTimeMicro  = 1 / (double)globalCLIINFO.lambda;

	if(requiredTimeMicro > 10.0F)
	{
		requiredTimeMicro = 10.0F;
	}
	else
	{
		;
	}

	requiredTimeMicro  = requiredTimeMicro * 1000000.00F;

	usleep(requiredTimeMicro);

	while(maxNumberOfPackets-- >= 1 && globalStopTokenPacketServerThreads == FALSE)
	{
		gettimeofday(&prevTimeRed, NULL);

		packetGenerator(globalCLIINFO.P, 1.0F/globalCLIINFO.mu, globalCLIINFO.B);

		transferPackets (&isTransmissionStop);

		gettimeofday(&currTimeRed, NULL);

		timeDifferenceRed = diffTimeinMicroSec(currTimeRed, prevTimeRed);

		if(timeDifferenceRed > requiredTimeMicro)
		{
			timeDifferenceRed = requiredTimeMicro;
		}
		else
		{
			;
		}

		usleep(requiredTimeMicro - timeDifferenceRed);
	}

	globalSimStatus = E_PACKET_GENERATION_COMPLETE;

	while(globalSimStatus != E_PACKET_TRANSFER_COMPLETE && globalStopTokenPacketServerThreads == FALSE)
	{
		transferPackets (&isTransmissionStop);

		usleep((1.0/globalCLIINFO.r)*1000000.00F);
	}

	pthread_mutex_lock(&globalQ1Mutex);

	pthread_cond_broadcast(&condPacketsAtQ2);

	pthread_mutex_unlock(&globalQ1Mutex);
}

void tfilePacketGenCall (void)
{
	int isTransmissionStop;
	int totalNumberOfPackets;
	int interArrivalTime;
	int serviceTime;
	int tokenRequired;
	double timeDifferenceRed;
	FILE *filePtr;
	TIMER currTime;
	TIMER prevTime;

	gettimeofday(&prevTime, NULL);

	isTransmissionStop = FALSE;

	filePtr = fopen(globalCLIINFO.tsfile, "r");

	totalNumberOfPackets = getPacketCount(filePtr);

	globalCLIINFO.num = totalNumberOfPackets;

	getPacketInfo(filePtr, &interArrivalTime, &tokenRequired, &serviceTime);

	gettimeofday(&currTime, NULL);

	usleep((interArrivalTime * 1000) - diffTimeinMicroSec(currTime, prevTime));

	while(totalNumberOfPackets-- >= 1)
	{
		gettimeofday(&prevTime, NULL);

		packetGenerator(tokenRequired, (float)(serviceTime/1000.00F), globalCLIINFO.B);

		getPacketInfo(filePtr, &interArrivalTime, &tokenRequired, &serviceTime);

		gettimeofday(&currTime, NULL);

		timeDifferenceRed = diffTimeinMicroSec(currTime, prevTime);

		if(timeDifferenceRed > (double)(interArrivalTime * 1000))
		{
			timeDifferenceRed = (double)(interArrivalTime * 1000);
		}
		else
		{
			;
		}

		usleep((interArrivalTime * 1000) - timeDifferenceRed);
	}

	globalSimStatus = E_PACKET_GENERATION_COMPLETE;

	while(globalSimStatus != E_PACKET_TRANSFER_COMPLETE &&
		  globalStopTokenPacketServerThreads == FALSE)
	{
		transferPackets (&isTransmissionStop);

		usleep((1.0/globalCLIINFO.r)*1000000.00F);
	}

	pthread_mutex_lock(&globalQ1Mutex);

	pthread_cond_broadcast(&condPacketsAtQ2);

	pthread_mutex_unlock(&globalQ1Mutex);

}

void * packetGenThread (void * param)
{
	pthread_cleanup_push(threadCancelHandlerFunc, &globalQ1Mutex);

	if(globalCLIINFO.tsfile == NULL)
	{
		cmdLinePacketGenCall();
	}
	else
	{
		tfilePacketGenCall();
	}

	pthread_cleanup_pop(0);

	return (void *)0;
}

/*
 * server.c
 *
 *  Created on: Jun 2, 2016
 *      Author: srinivas
 */


#include "commonheader.h"

void service (int serverName)
{
	int            hasToExit;
	TIMER 		   currTime;
	double         serviceTimeMilli;
	double         timeInSystemMilli;
	double         timeInQ2Micro;
	T_PACKET       packetElem;
	My402ListElem *currElemPtr;

	hasToExit = FALSE;

	while(hasToExit == FALSE)
	{
		currElemPtr = NULL;

		pthread_mutex_lock(&globalQ1Mutex);

		while((currElemPtr = My402ListFirst(&globalQueueQ2)) == NULL && globalStopTokenPacketServerThreads == FALSE)
		{
			if(globalSimStatus == E_PACKET_TRANSFER_COMPLETE)
			{
				hasToExit = TRUE;

				currElemPtr = NULL;

				break;
			}
			else
			{
				pthread_cond_wait(&condPacketsAtQ2, &globalQ1Mutex);
			}
		}

		if(globalStopTokenPacketServerThreads == TRUE)
		{
			break;
		}
		else
		{
			;
		}

		if(currElemPtr != NULL)
		{
			packetElem = *(T_PACKET *)currElemPtr->obj;

			free(currElemPtr->obj);

			My402ListUnlink(&globalQueueQ2, currElemPtr);

			gettimeofday(&currTime, NULL);

			pthread_mutex_unlock(&globalQ1Mutex);

			timeInQ2Micro = diffTimeinMicroSec(currTime, packetElem.arrivalTimeQ2);

			globalTimeSpentAtQ2Micro += timeInQ2Micro;

			printf("%012.3fms:  p%d leaves Q2, time in Q2 = %.3fms\n",
					diffTimeinMicroSec (currTime, globalSimStartTime)/1000.00F,
					packetElem.packetNumber,
					timeInQ2Micro/1000.00F);

			gettimeofday(&currTime, NULL);

			printf("%012.3fms:  P%d begins service at S%d, requesting %.3fms of service\n",
					diffTimeinMicroSec (currTime, globalSimStartTime)/1000.00F,
					packetElem.packetNumber,
					serverName,
					packetElem.serviceTime * 1000.00F);

			packetElem.serviceStartTime = currTime;

			usleep(packetElem.serviceTime * 1000000.00F);

			gettimeofday(&currTime, NULL);

			serviceTimeMilli  = diffTimeinMicroSec(currTime, packetElem.serviceStartTime)/1000.00F;
			timeInSystemMilli = diffTimeinMicroSec(currTime, packetElem.generationTime)/1000.00F;

			printf("%012.3fms:  P%d departs from S%d, service time = %.3fms, time in system = %.3fms\n",
					diffTimeinMicroSec (currTime, globalSimStartTime)/1000.00F,
					packetElem.packetNumber,
					serverName,
					serviceTimeMilli,
					timeInSystemMilli);

			if(serverName == 1)
			{
				globalTimeSpentAtS1ServerMilli += serviceTimeMilli;
			}
			else
			{
				globalTimeSpentAtS2ServerMilli += serviceTimeMilli;
			}

			/* Update service info */
			pthread_mutex_lock(&globalStatUpdateMutex);
			globalServiceTimeMilli       += serviceTimeMilli;
			globalTimeSpentInSystemMilli += timeInSystemMilli;
			globalStatTimeSpentInSystemMilli += (timeInSystemMilli * timeInSystemMilli);
			pthread_mutex_unlock(&globalStatUpdateMutex);

		}
		else
		{
			pthread_mutex_unlock(&globalQ1Mutex);

		}

		if(globalStopTokenPacketServerThreads == TRUE)
		{
			break;
		}
		else
		{
			;
		}

	}

}



void * server1ThreadSubRoutine (void * param)
{
	service(1);

	return (void *)0;
}

void * server2ThreadSubRoutine (void * param)
{
	service(2);

	return (void *)0;

}


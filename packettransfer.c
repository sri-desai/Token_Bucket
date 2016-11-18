/*
 * packettransfer.c
 *
 *  Created on: Jun 4, 2016
 *      Author: srinivas
 */


#include "commonheader.h"

int transferPackets (int *hasToExit)
{
	int            remainingTokens;
	double         timeSpentAtQ1Micro;
	TIMER          leaveQ1Time;
	T_PACKET	  *packetRef;
	My402ListElem *firstListElem;

	pthread_mutex_lock(&globalQ1Mutex);

	firstListElem = My402ListFirst(&globalQueueQ1);

	if(firstListElem == NULL)
	{
		pthread_mutex_unlock(&globalQ1Mutex);

		if(globalSimStatus == E_PACKET_GENERATION_COMPLETE)
		{
			globalSimStatus = E_PACKET_TRANSFER_COMPLETE;
		}
		else
		{
			;
		}

		return FALSE;
	}
	else
	{
		packetRef = (T_PACKET *)firstListElem->obj;
	}

	if(globalTokenCount >= packetRef->tokensRequired)
	{
		My402ListUnlink(&globalQueueQ1, firstListElem);

		gettimeofday(&leaveQ1Time, NULL);

		globalTokenCount -= packetRef->tokensRequired;

		remainingTokens = globalTokenCount;

		My402ListAppend(&globalQueueQ2, packetRef);

		pthread_cond_broadcast(&condPacketsAtQ2);

		gettimeofday(&packetRef->arrivalTimeQ2, NULL);

		timeSpentAtQ1Micro = diffTimeinMicroSec(leaveQ1Time, packetRef->arrivalTimeQ1);

		globalTimeSpentAtQ1Micro += timeSpentAtQ1Micro;

		printf("%012.3fms:  P%d leaves Q1, time in Q1 = %.3fms, token bucket now has %d tokens\n",
				diffTimeinMicroSec(leaveQ1Time, globalSimStartTime)/1000.00F,
				packetRef->packetNumber,
				timeSpentAtQ1Micro/1000.00F,
				remainingTokens);

		printf("%012.3fms:  P%d enters Q2\n",
				diffTimeinMicroSec(packetRef->arrivalTimeQ2, globalSimStartTime)/1000.00F,
				packetRef->packetNumber);

		if(packetRef->packetNumber >= globalCLIINFO.num)
		{
			globalSimStatus = E_PACKET_TRANSFER_COMPLETE;
		}
		else
		{
			;
		}

		pthread_mutex_unlock(&globalQ1Mutex);
	}
	else
	{
		pthread_mutex_unlock(&globalQ1Mutex);
	}

	return TRUE;
}

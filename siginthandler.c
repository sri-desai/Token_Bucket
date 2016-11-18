/*
 * siginthandler.c
 *
 *  Created on: Jun 11, 2016
 *      Author: srinivas
 */


#include "commonheader.h"


void printStatistics (TIMER simEndTime)
{
	TIMER  currentTime;
	double totalEmulationTimeMicro;
	double averagePacketServiceTimeSec;
	double averageTimeSpentInSystem;

	currentTime = simEndTime;

	totalEmulationTimeMicro = diffTimeinMicroSec(currentTime, globalSimStartTime);

	printf("%012.3fms:  emulation ends\n",
		  diffTimeinMicroSec(currentTime, globalSimStartTime)/1000.00F);

	printf("\nStatistics:\n");

	averagePacketServiceTimeSec = (globalServiceTimeMilli/(globalPacketCount - globalDroppedPacketCount -
			  (globalPacketRemovedQ1 + globalPacketRemovedQ2)))/1000.00F;

	if(globalPacketCount != 0)
	{
		printf("average packet inter-arrival time = %.6gs\n",
			  (globalInterArrivalPacketTime/globalPacketCount)/1000000.00F);

		if(isnan(averagePacketServiceTimeSec) == 0)
		{
			printf("average packet service time = %.6gs\n\n",averagePacketServiceTimeSec);
		}
		else
		{
			printf("average packet service time = \"N/A\" as there were no packets to service\n");
		}

	}
	else
	{
		printf("average packet inter-arrival time = \"N/A\" as there were no packets\n");

		printf("average packet service time = \"N/A\" as there were no packets to service\n");
	}

	if(totalEmulationTimeMicro == 0.0F)
	{
		printf("average packet service time = \"N/A\" as emulation runs for 0 sec\n");
	}
	else
	{
		printf("average number of packets in Q1 = %.6g\n",
			  globalTimeSpentAtQ1Micro / totalEmulationTimeMicro);

		printf("average number of packets in Q2 = %.6g\n",
			  globalTimeSpentAtQ2Micro / totalEmulationTimeMicro);

		printf("average number of packets at S1 = %.6g\n",
			  (globalTimeSpentAtS1ServerMilli * 1000.00F) / totalEmulationTimeMicro);

		printf("average number of packets at S2 = %.6g\n\n",
			  (globalTimeSpentAtS2ServerMilli * 1000.00F) / totalEmulationTimeMicro);
	}

	averageTimeSpentInSystem = (globalTimeSpentInSystemMilli/(globalPacketCount - globalDroppedPacketCount -
			(globalPacketRemovedQ1 + globalPacketRemovedQ2)))/1000.00;

	if(isnan(averageTimeSpentInSystem) == 0)
	{
		printf("average time a packet spent in a system = %.6gs\n",averageTimeSpentInSystem);
	}
	else
	{
		printf("average time a packet spent in a system = \"N/A\" as there were no packets to calculate service time\n");
	}

	globalStatMeanOfTimeMilli = globalStatTimeSpentInSystemMilli / (globalPacketCount - globalDroppedPacketCount);

	globalStatVarOfTimeMilli = globalStatMeanOfTimeMilli - (
			                   (globalTimeSpentInSystemMilli/(globalPacketCount - globalDroppedPacketCount)) *
							   (globalTimeSpentInSystemMilli/(globalPacketCount - globalDroppedPacketCount))
	                           );

	if(isnan((sqrt(globalStatVarOfTimeMilli))/1000.00F) == 0)
	{
		printf("standard deviation for time spent in system : %.6g\n", (sqrt(globalStatVarOfTimeMilli))/1000.00F);
	}
	else
	{
		printf("standard deviation for time spent in system : \"NA\" as there are no valid packets\n");
	}


	if(globalTotalTokenCount == 0)
	{
		printf("token drop probability = \"NA\" as there are no packets generated\n");
	}
	else
	{
		printf("token drop probability = %.6g\n", (float)(globalDroppedTokenCount / (float)globalTotalTokenCount));
	}

	if(globalPacketCount == 0)
	{
		printf("packet drop probability = \"NA\" as there are no packets generated\n");
	}
	else
	{
		printf("packet drop probability = %.6g\n", (float)(globalDroppedPacketCount / (float)globalPacketCount));
	}

}

void displayDropPackets (void)
{
	TIMER          currentTime;
	My402ListElem *listElem;

	gettimeofday(&currentTime, NULL);

	for(listElem = globalQueueQ1.anchor.next; listElem != NULL && listElem != &globalQueueQ1.anchor;
		listElem = listElem->next)
	{
		printf("%012.3fms:  p%d removed from Q1\n",
			  diffTimeinMicroSec(currentTime, globalSimStartTime)/1000.00F,
			  ((T_PACKET *)listElem->obj)->packetNumber);

		++globalPacketRemovedQ1;

		free(listElem->obj);

		free(listElem);

		gettimeofday(&currentTime, NULL);
	}

	gettimeofday(&currentTime, NULL);

	for(listElem = globalQueueQ2.anchor.next; listElem != NULL && listElem != &globalQueueQ2.anchor;
		listElem = listElem->next)
	{
		printf("%012.3fms:  p%d removed from Q2\n",
				diffTimeinMicroSec(currentTime, globalSimStartTime)/1000.00F,
				((T_PACKET *)listElem->obj)->packetNumber);

		++globalPacketRemovedQ2;

		free(listElem->obj);

		free(listElem);

		gettimeofday(&currentTime, NULL);
	}

}


void * SigIntHandlerFunc(void * param)
{
	int signalGenerated;

	//pthread_sigmask(SIG_UNBLOCK, &globalSignalSet, NULL);

	while(1)
	{
		sigwait(&globalSignalSet, &signalGenerated);

		if(signalGenerated == SIGINT)
		{
			globalStopTokenPacketServerThreads = TRUE;

			pthread_cancel(packetGeneratorThread);

			pthread_cancel(tokenGeneratorThread);

			pthread_mutex_lock(&globalQ1Mutex);

			pthread_cond_broadcast(&condPacketsAtQ2);

			pthread_mutex_unlock(&globalQ1Mutex);

			pthread_join(server1GeneratorThread, 0);

			pthread_join(server2GeneratorThread, 0);

			displayDropPackets();

			gettimeofday(&globalSimEndTime, NULL);

			printStatistics(globalSimEndTime);

			isIntruptServiced = TRUE;

			exit(0);
		}
	}
}

/*
 * binary.c
 *
 *  Created on: May 31, 2016
 *      Author: srinivas
 */

#include "commonheader.h"

int main(int argc, char *argv[])
{
	globalVarInit();

	processInit(argc, argv);

	sigemptyset(&globalSignalSet);

	sigaddset(&globalSignalSet, SIGINT);

	pthread_sigmask(SIG_BLOCK, &globalSignalSet, NULL);

	gettimeofday(&globalSimStartTime, NULL);

	globalPrevPacketTimer = globalSimStartTime;

	printf("%012.3fms:  emulation begins\n", diffTimeinMicroSec(globalSimStartTime, globalSimStartTime)/1000.00F);

    pthread_create(&tokenGeneratorThread,   NULL, tokenGenThread,          NULL);
    pthread_create(&packetGeneratorThread,  NULL, packetGenThread, 		   NULL);
    pthread_create(&server1GeneratorThread, NULL, server1ThreadSubRoutine, NULL);
    pthread_create(&server2GeneratorThread, NULL, server2ThreadSubRoutine, NULL);
    pthread_create(&sigIntHandlerThread,    NULL, SigIntHandlerFunc,	   NULL);

	pthread_join  (packetGeneratorThread,  0);
	pthread_join  (tokenGeneratorThread,   0);
    pthread_join  (server1GeneratorThread, 0);
 	pthread_join  (server2GeneratorThread, 0);
 	pthread_cancel(sigIntHandlerThread);

 	gettimeofday(&globalSimEndTime, NULL);

 	if(globalStopTokenPacketServerThreads == FALSE)
 	{
 		printStatistics(globalSimEndTime);
 	}
 	else
 	{
 		while(isIntruptServiced == FALSE)
 		{
 			sleep(10000);
 		}
 	}


	return 0;

}

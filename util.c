/*
 * util.c
 *
 *  Created on: Jun 1, 2016
 *      Author: srinivas
 */


#include "commonheader.h"

void getPacketInfo (FILE *filePtr, int *interArrivalTime, int * tokenRequired, int *serviceTime)
{
	int  charCount;
	int  typeInfo;
	char buffer[1000];

	fgets(buffer, 1000, filePtr);

	*interArrivalTime = 0;
	*tokenRequired = 0;
	*serviceTime = 0;
	typeInfo = 0;

	for(charCount = 0; charCount < strlen(buffer) && buffer[charCount] != '\n'; ++charCount)
	{
		if(typeInfo == 0 && isdigit(buffer[charCount]))
		{
			*interArrivalTime = (*interArrivalTime * 10) + buffer[charCount] - '0';

			if(buffer[charCount + 1] == ' ' || buffer[charCount + 1] == '\t')
			{
				++typeInfo;
			}
			else
			{
				;
			}
		}
		else
		if(typeInfo == 1 && isdigit(buffer[charCount]))
		{
			*tokenRequired = (*tokenRequired * 10) + buffer[charCount] - '0';

			if(buffer[charCount + 1] == ' ' || buffer[charCount + 1] == '\t')
			{
				++typeInfo;
			}
			else
			{
				;
			}
		}
		else
		if(typeInfo == 2 && isdigit(buffer[charCount]))
		{
			*serviceTime = (*serviceTime * 10) + buffer[charCount] - '0';

			if(buffer[charCount + 1] == ' ' || buffer[charCount + 1] == '\t')
			{
				++typeInfo;
			}
			else
			{
				;
			}
		}
		else
		{
			if(buffer[charCount] != ' ' && buffer[charCount] != '\t')
			{
				submitError(E_INVALID_CHAR_ERROR);
			}
			else
			{

			}
		}
	}
}


int getPacketCount(FILE *filePtr)
{
	int  charCount;
	int  packetCount;
    char buffer[1000];

	packetCount = 0;

	fgets(buffer, 1000, filePtr);

	for(charCount = 0; buffer[charCount] != '\n'; ++charCount)
	{
		if(isdigit(buffer[charCount]) == FALSE)
		{
			submitError(E_INVALID_CHAR_ERROR);
		}
		else
		{
			packetCount = (packetCount * 10) + buffer[charCount] - '0';
		}
	}

	return packetCount;

}


double diffTimeinMicroSec (TIMER currTime, TIMER prevTime)
{
	double diffTime;

	diffTime = (((currTime.tv_sec - prevTime.tv_sec) * 1000000.00F) + (currTime.tv_usec - prevTime.tv_usec));

	return diffTime;
}


int checkFileFields (char *buffer, int isFirstLine)
{
	int   packetCount;
	int   charCount;
	char *endStrPtr;

	packetCount = 0;

	if(isspace(*buffer) != FALSE)
	{
		submitError(E_LEADING_SPACE_ERROR);
	}

	endStrPtr = buffer + strlen(buffer) - 1;

	if(isspace(*endStrPtr) != FALSE)
	{
		if(*endStrPtr == '\n')
		{
			;
		}
		else
		{
			submitError(E_TRAILING_SPACE_ERROR);
		}

	}

	for(charCount = 0; charCount < strlen(buffer); ++charCount)
	{
		if(isdigit(buffer[charCount]) == FALSE && buffer[charCount] != ' ' && buffer[charCount] != '\t')
		{
			if(charCount == (strlen(buffer) - 1) && buffer[charCount] == '\n')
			{
				;
			}
			else
			{
				submitError(E_INVALID_CHAR_ERROR);
			}

		}
		else
		{
			if(isFirstLine == TRUE)
			{
				packetCount = (packetCount * 10) + buffer[charCount] - '0';
			}
			else
			{
				;
			}
		}
	}

	return packetCount;
}

void checkTFileFormart (void)
{
	FILE  *filePtr;
	int   isFirstLine;
	int   lineCount;
	int   packetCount;
	int   infoPresent;
	char  *charBuffer;

	if(globalCLIINFO.tsfile != NULL)
	{
		lineCount = 0;

		packetCount = 0;

		infoPresent = FALSE;

		isFirstLine = TRUE;

		charBuffer = (char *)calloc(1, sizeof(char) * 1024);

		filePtr = fopen(globalCLIINFO.tsfile, "r");

		while(fgets(charBuffer, 1024, filePtr) != NULL)
		{
			if(isFirstLine == TRUE)
			{
				packetCount = checkFileFields(charBuffer, isFirstLine);
			}
			else
			{
				(void)checkFileFields(charBuffer, isFirstLine);

				++lineCount;
			}

			isFirstLine = FALSE;

			infoPresent = TRUE;
		}

		if(infoPresent == FALSE)
		{
			submitError(E_NO_PACKET_INFO_ERROR);
		}
		else
		{
			;
		}

		if(packetCount != lineCount)
		{
			submitError(E_INVALID_LINE_CNT_ERROR);
		}
		else
		{
			globalCLIINFO.num = packetCount;
		}
	}
	else
	{
		;
	}
}


void globalVarInit (void)
{
	globalFilePtr 					   = NULL;
	globalSimStatus					   = 0;
	globalTokenCount				   = 0;
	isIntruptOccured 				   = 0;
	globalPacketCount 				   = 0;
	isIntruptServiced				   = 0;
	globalTotalTokenCount 			   = 0;
	globalServiceTimeMilli      	   = 0.0F;
	globalDroppedTokenCount 	 	   = 0;
	globalDroppedPacketCount     	   = 0;
	globalTimeSpentAtQ1Micro 		   = 0.0F;
	globalTimeSpentAtQ2Micro		   = 0.0F;
	globalInterArrivalPacketTime       = 0.0F;
	globalTimeSpentInSystemMilli       = 0.0F;
	globalTimeSpentAtS1ServerMilli	   = 0.0F;
	globalTimeSpentAtS2ServerMilli	   = 0.0F;
	globalStatTimeSpentInSystemMilli   = 0.0F;
	globalStopTokenPacketServerThreads = 0;
	globalPacketRemovedQ1 = 0;
	globalPacketRemovedQ2 = 0;
	My402ListInit(&globalQueueQ1);
	My402ListInit(&globalQueueQ2);
	memset(&globalSimStartTime,    0, sizeof(TIMER));
	memset(&globalPrevPacketTimer, 0, sizeof(TIMER));
	pthread_mutex_init(&globalTokenBucketMutex, NULL);
	pthread_mutex_init(&globalQ2Mutex,          NULL);
	pthread_mutex_init(&globalQ1Mutex,		    NULL);
	pthread_mutex_init(&globalStatUpdateMutex,  NULL);
	pthread_cond_init(&condPacketsAtQ1, NULL);
	pthread_cond_init(&condPacketsAtQ2, NULL);

	isIntruptServiced = FALSE;

	globalCLIINFO.lambda = 1.0F;
	globalCLIINFO.mu = 0.35F;
	globalCLIINFO.r = 1.5F;
	globalCLIINFO.B = 10;
	globalCLIINFO.P = 3;
	globalCLIINFO.num = 20;
	globalCLIINFO.tsfile = NULL;
}



void checkFileExist (char *givenFilePath)
{
	FILE         *fileTypePtr;
	T_DIR_TYPE    dirCheck;

	fileTypePtr = fopen(givenFilePath, "r");

	if(fileTypePtr != NULL)
	{
		if((stat(givenFilePath, &dirCheck) == 0) && S_ISREG(dirCheck.st_mode) == FALSE)
		{
			submitError(E_INPUT_DIR);
		}

		else
		if(stat(givenFilePath, &dirCheck) != 0)
		{
			submitError(E_STAT_ERROR);
		}
		else
		{
			;
		}
	}
	else
	{
		submitError(E_FILE_NOT_EXIST_ERROR);
	}
}

void checkIsAllDigits (char *inputStr)
{
	int isCharDigit;
	int charCount;
	int strLen;

	strLen      = strlen(inputStr);
	isCharDigit = TRUE;

	for(charCount = 0; charCount < strLen && isCharDigit == TRUE; ++charCount)
	{
		if(isdigit(inputStr[charCount]) == FALSE)
		{
			if(inputStr[charCount] != '.')
			{
				isCharDigit = FALSE;
			}
			else
			{
				;
			}
		}
		else
		{
			;
		}
	}

	if(isCharDigit == FALSE)
	{
		submitError(E_NO_DIGIT_ERROR);
	}
	else
	{
		;
	}


}


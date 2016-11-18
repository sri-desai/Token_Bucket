/*
 * processInit.c
 *
 *  Created on: Jun 3, 2016
 *      Author: srinivas
 */


#include "commonheader.h"
#include "signal.h"

char *lambdaNumber = NULL;
char *muNumber     = NULL;
char *rNumber      = NULL;

void storeCmd (int argc, char * argument[], int totalArgs)
{
	if(totalArgs <= (argc + 1))
	{
		submitError(E_CMDARG_ERROR);
	}
	else
	if(strcmp(argument[argc], "-t") == 0)
	{
		checkFileExist(argument[argc + 1]);
	}
	else
	{
		checkIsAllDigits(argument[argc + 1]);
	}

	if(strcmp(argument[argc],"-lambda") == 0)
	{
		globalCLIINFO.lambda = atof(argument[argc + 1]);

		lambdaNumber = argument[argc + 1];
	}
	else
	if(strcmp(argument[argc], "-mu") == 0)
	{
		globalCLIINFO.mu = atof(argument[argc + 1]);

		muNumber = argument[argc + 1];
	}
	else
	if(strcmp(argument[argc], "-r") == 0)
	{
		globalCLIINFO.r = atof(argument[argc + 1]);

		rNumber = argument[argc + 1];
	}
	else
	if(strcmp(argument[argc], "-B") == 0)
	{
		globalCLIINFO.B = atoi(argument[argc + 1]);
	}
	else
	if(strcmp(argument[argc], "-P") == 0)
	{
		globalCLIINFO.P = atoi(argument[argc + 1]);
	}
	else
	if(strcmp(argument[argc], "-n") == 0)
	{
		globalCLIINFO.num = atoi(argument[argc + 1]);
	}
	else
	if(strcmp(argument[argc], "-t") == 0)
	{
		globalCLIINFO.tsfile = (char *)malloc(strlen(argument[argc + 1]) + 1);

		//strncpy(globalCLIINFO.tsfile, argument[argc + 1], strlen(argument[argc + 1]));

		snprintf(globalCLIINFO.tsfile, strlen(argument[argc + 1]) + 1, "%s", argument[argc + 1]);
	}
	else
	{
		submitError(E_CMDLINE_ERROR);
	}

}


void checkOrSetIfGlobalCmdEmpty (void)
{
	if(globalCLIINFO.lambda <= 0.0F)
	{
		submitError(E_LAMBDA_NEGATIVE_ERROR);
	}
	else
	if((1 / globalCLIINFO.lambda) > 10.0F)
	{
		globalCLIINFO.lambda = 0.1F;
	}
	else
	{
		;
	}


	if(globalCLIINFO.mu <= 0.0F)
	{
		submitError(E_MU_NEGATIVE_ERROR);
	}
	else
	if((1 / globalCLIINFO.mu) > 10.0F)
	{
		globalCLIINFO.mu = 0.1F;
	}
	else
	{
		;
	}

	if(globalCLIINFO.r <= 0.0F)
	{
		submitError(E_R_NEGATIVE_ERROR);
	}
	else
	{
		if((1/globalCLIINFO.r) > 10.0F)
		{
			globalCLIINFO.r = 0.1;
		}
		else
		{
			;
		}
	}

	if(globalCLIINFO.B < 0 || globalCLIINFO.B > 0x7FFFFFFF)
	{
		submitError(E_BUCKET_OVER_UNDER_ERROR);
	}
	else
	{
		;
	}

	if(globalCLIINFO.P < 0 || globalCLIINFO.P > 0x7FFFFFFF)
	{
		submitError(E_REQUIRED_TOKEN_OVER_UNDER_ERROR);
	}
	else
	{
		;
	}

	if(globalCLIINFO.num < 0 || globalCLIINFO.num > 0x7FFFFFFF)
	{
		submitError(E_PACKET_COUNT_OVER_UNDER_ERROR);
	}
	else
	{
		;
	}
}

void printSimInfoHeader (void)
{
	printf("Emulation Parameters:\n");

	printf("\tnumber to arrive = %d\n", globalCLIINFO.num);

	if(globalCLIINFO.tsfile == NULL)
	{
		if(lambdaNumber == NULL)
		{
			printf("\tlambda = %.6f\n", globalCLIINFO.lambda);
		}
		else
		{
			printf("\tlambda = %s\n", lambdaNumber);
		}

		if(muNumber == NULL)
		{
			printf("\tmu = %.6f\n", globalCLIINFO.mu);
		}
		else
		{
			printf("\tmu = %s\n", muNumber);
		}

		if(rNumber == NULL)
		{
			printf("\tr = %.6f\n", globalCLIINFO.r);
		}
		else
		{
			printf("\tr = %s\n", rNumber);
		}


		printf("\tB = %d\n", globalCLIINFO.B);
		printf("\tP = %d\n", globalCLIINFO.P);
	}
	else
	{
		printf("\ttsfile = %s\n", globalCLIINFO.tsfile);
	}

}


void processInit (int argc, char *argv[])
{
	int cmdLineArgCount;

	for(cmdLineArgCount = 1; cmdLineArgCount < argc; cmdLineArgCount = cmdLineArgCount + 2)
	{
		storeCmd(cmdLineArgCount, argv, argc);
	}

	checkOrSetIfGlobalCmdEmpty();

	checkTFileFormart();

	printSimInfoHeader();

}



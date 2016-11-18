/*******************************************************************
 * File Name : submiterror.c
 *
 * Operations:
 * -----------
 * o Prints the error messages to the console and exits out of
 *   the program
 ******************************************************************/


#include "commonheader.h"

void submitError (E_ERROR_CODE errorMessage)
{
	switch(errorMessage)
	{
		case E_CMDLINE_ERROR:
		{
			fprintf(stderr, "\n\n\tError: Command line arguments are not proper\n\n");
		}
		break;

		case E_BUCKET_OVER_UNDER_ERROR:
		{
			fprintf(stderr, "\n\n\tError: Value of B is either negative or more than 0x7FFFFFFF \n\n");
		}
		break;

		case E_REQUIRED_TOKEN_OVER_UNDER_ERROR:
		{
			fprintf(stderr, "\n\n\tError: Value of P is either negative or more than 0x7FFFFFFF \n\n");
		}
		break;

		case E_PACKET_COUNT_OVER_UNDER_ERROR:
		{
			fprintf(stderr, "\n\n\tError: Value of n is either negative or more than 0x7FFFFFFF \n\n");
		}
		break;

		case E_MEMALLOC_ERROR:
		{
			fprintf(stderr, "\n\n\tError: Could not allocate memory on heap \n\n");
		}
		break;

		case E_LAMBDA_NEGATIVE_ERROR:
		{
			fprintf(stderr, "\n\n\tError: Lambda is negative \n\n");
		}
		break;

		case E_MU_NEGATIVE_ERROR:
		{
			fprintf(stderr, "\n\n\tError: Mu is negative \n\n");
		}
		break;

		case E_R_NEGATIVE_ERROR:
		{
			fprintf(stderr, "\n\n\tError: R value is negative \n\n");
		}
		break;

		case E_LEADING_SPACE_ERROR:
		{
			fprintf(stderr, "\n\n\tError: Leading space characters present in the file \n\n");
		}
		break;

		case E_TRAILING_SPACE_ERROR:
		{
			fprintf(stderr, "\n\n\tError: Trailing space characters present in the file \n\n");
		}
		break;

		case E_INVALID_CHAR_ERROR:
		{
			fprintf(stderr, "\n\n\tError: Invalid characters present in the file \n\n");
		}
		break;

		case E_NO_PACKET_INFO_ERROR:
		{
			fprintf(stderr, "\n\n\tInput file does'nt contain packet info/its empty file \n\n");
		}
		break;

		case E_INVALID_LINE_CNT_ERROR:
		{
			fprintf(stderr, "\n\n\tPacket count is more/less than the actual packet info in the file \n\n");
		}
		break;

		case E_CMDARG_ERROR:
		{
			fprintf(stderr, "\n\n\tInvalid command found/value for option is not specified\n\n");
			fprintf(stderr, "\n\t Command should be : warmup2 [-lambda lambda] [-mu mu] [-r r] [-B B] [-P P] [-n num] [-t tsfile]\n");


		}
		break;

		case E_INPUT_DIR:
		{
			fprintf(stderr, "\n\n\tInput path/file is a directory \n\n");
		}
		break;

		case E_STAT_ERROR:
		{
			fprintf(stderr, "\n\tError in computing Stat for the input path/file \n\n");
		}
		break;

		case E_NO_DIGIT_ERROR:
		{
			fprintf(stderr, "\n\n\tInput character is not a number\n\n");
			fprintf(stderr, "\n\t Command should be : warmup2 [-lambda lambda] [-mu mu] [-r r] [-B B] [-P P] [-n num] [-t tsfile]\n");
		}
		break;

		case E_FILE_NOT_EXIST_ERROR:
		{
			fprintf(stderr, "\n\n\tFile Not exist or permission denied \n\n");
		}
		break;

		default:
		{
			fprintf(stderr, "\n\n\t Invalid Error Code\n\n");
		}
	}

	exit(1);
}

#define _CRT_SECURE_NP_WARNINGS
#define _CRT_NONSTDC_NP_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include "../card/card.h"
#include "terminal.h"


EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
	// ask for the transaction date and store it in terminal data

	printf("please enter the transaction date in form DD/MM/YYYY :\n");
	scanf_s("%s", &termData->transactionDate,11);

	// check the length of the transactionDate

	if (strlen(termData->transactionDate) != 10 || termData->transactionDate == NULL)
	{
		return WRONG_DATE;
	}
	else
	{
		// check the format of the transactionDate

		for (int l = 0;l < strlen(termData->transactionDate); l++)
		{
			if (l == 2 || l == 5)
			{
				if (!(termData->transactionDate[l] == '/'))
				{
					return WRONG_DATE;
				}
			}
			else
			{
				if (!(isdigit(termData->transactionDate[l])))
				{
					return WRONG_DATE;
				}
			}
		}
		return TERMINAL_OK;
	}
}

EN_terminalError_t isCardExpired(ST_cardData_t *cardData, ST_terminalData_t *termData)
{
	// create temp arrays to store substrings from the expiry & transaction dates  

	char sub_trans_m[2], sub_trans_y[2], sub_exp_m[2], sub_exp_y[2];

	// substrings from the transaction date

	for (int k = 0; k < 10;k++)
	{
		if (k == 3)
		{
			sub_trans_m[0] = termData->transactionDate[k];
		}
		else if (k == 4)
		{
			sub_trans_m[1] = termData->transactionDate[k];
		}
		else if (k == 8)
		{
			sub_trans_y[0] = termData->transactionDate[k];
		}
		else if (k == 9)
		{
			sub_trans_y[1] = termData->transactionDate[k];
		}
	}

	// substrings from the expiry date

	for (int x = 0; x < 5;x++)
	{
		if (x == 0)
		{
			sub_exp_m[0] = cardData->cardExpirationDate[x];
		}
		else if (x == 1)
		{
			sub_exp_m[1] = cardData->cardExpirationDate[x];
		}
		else if (x == 3)
		{
			sub_trans_y[0] = cardData->cardExpirationDate[x];
		}
		else if (x == 4)
		{
			sub_trans_y[1] = cardData->cardExpirationDate[x];
		}
	}

	// convert these substrings into numbers to compare between them

	int trans_m = atoi(sub_trans_m);
	int trans_y = atoi(sub_trans_y);
	int exp_m = atoi(sub_exp_m);
	int exp_y = atoi(sub_exp_y);

	// compares the card expiry date with the transaction date

	if (exp_m < trans_m && exp_y < trans_y)
	{
		return EXPIRED_CARD;
	}
	else
	{
		return TERMINAL_OK;
	}
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
	//asks for the transaction amount and saves it into terminal data

	printf("please enter the transaction amount :\n");
	scanf_s("%f", &termData->transAmount,10);

	// verify the sign of the entered amount

	if (termData->transAmount <= 0)
	{
		return INVALID_AMOUNT;
	}
	else
	{
		return TERMINAL_OK;
	}
}

EN_terminalError_t setMaxAmount(ST_terminalData_t* termData, float maxAmount)
{
	// takes the maximum allowed amount and stores it into terminal data
	printf("please enter the max transaction amount :\n");
	scanf_s("%f", &maxAmount);

	termData->maxTransAmount = maxAmount;

	if (termData->maxTransAmount <= 0)
	{
		return INVALID_MAX_AMOUNT;
	}
	else
	{
		return TERMINAL_OK;
	}
}


EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
	//  compares the transaction amount with the terminal max allowed amount

	if (termData->transAmount > termData->maxTransAmount)
	{
		return EXCEED_MAX_AMOUNT;
	}
	else
	{
		return TERMINAL_OK;
	}
}
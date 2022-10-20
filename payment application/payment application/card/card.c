#define _CRT_SECURE_NP_WARNINGS
#define _CRT_NONSTDC_NP_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include "card.h"


EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
	// ask for the cardholder's name and store it into card data

	printf("please enter the card holder name : \n");
	scanf_s("%[^\n]S", &cardData->cardHolderName, 25);

	// verify the size of the input name

	if (strlen(cardData->cardHolderName) > 24 || strlen(cardData->cardHolderName) < 20 || cardData->cardHolderName == NULL)
	{
		return WRONG_NAME;
	}
	else
	{
		// test the type of entered characters if they are alphapetical or not

		for (int i = 0; i < strlen(cardData->cardHolderName); i++)
		{
			if (!(isalpha(cardData->cardHolderName[i])) && !((cardData->cardHolderName[i]) == ' '))
			{
				return WRONG_NAME;
			}
		}
		return CARD_OK;
	}
}


EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{
	// ask for the card expiry date and store it in card data

	printf("please enter the CardExpiryDate in format mm/yy : \n");
	scanf_s("%s", &cardData->cardExpirationDate, 6);
	
	// verify the size of the input date

	if (strlen(cardData->cardExpirationDate) != 5 || cardData->cardExpirationDate == NULL)
	{
		return  WRONG_EXP_DATE;
	}
	else
	{
		// test the type of the entered characters & the date format

		for (int j = 0; j < strlen(cardData->cardExpirationDate); j++)
		{
			if (!(isdigit(cardData->cardExpirationDate[j])))
			{
				if (!(cardData->cardExpirationDate[j] == '/' && j == 2))
				{
					return WRONG_EXP_DATE;
				}
			}
			else
			{
				if ( j == 2)
				{
					return WRONG_EXP_DATE;
				}
			}
		}
		return CARD_OK;
	}
}

EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{
	// ask for the card's Primary Account Number and store it in card data

	printf("please enter the CardPAN : \n");
	scanf_s("%s", &cardData->primaryAccountNumber, 20);

	// verify the size of the input PAN

	if (strlen(cardData->primaryAccountNumber) > 19 || strlen(cardData->primaryAccountNumber) < 16 || (cardData->primaryAccountNumber) == NULL)
	{
		return  WRONG_PAN;
	}
	else
	{
		// test if the input characters are digits or not

		for (int k = 0; k < strlen(cardData->primaryAccountNumber); k++)
		{
			if (!(isdigit(cardData->primaryAccountNumber[k])))
			{
				return WRONG_PAN;
			}
		}
		return CARD_OK;
	}
}
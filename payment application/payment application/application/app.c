#define _CRT_SECURE_NP_WARNINGS
#define _CRT_NONSTDC_NP_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include "../card/card.h"
#include "../terminal/terminal.h"
#include "../server/server.h"
#include "app.h"


void appStart(void)
{
	// card module
	ST_cardData_t card_info;                  // object from structure
	ST_cardData_t *cardData = &card_info;     // pointer to this object

	// first , take the CardHolderName from the user
	EN_cardError_t CardHolderName_error = getCardHolderName(cardData);  // variable to store the value returned from getCardHolderName

	// check the valdity of the CardHolderName
	if (CardHolderName_error == WRONG_NAME)
	{
		printf("invalid name ! , please enter your name again :\n");
		return ;  // end the program
	}

	 //second , take the CardExpiryDate from the user
	EN_cardError_t CardExpiryDate_error = getCardExpiryDate(cardData);  // variable to store the value returned from getCardExpiryDate

	 //check the valdity of the CardExpiryDate

	if (CardExpiryDate_error == WRONG_EXP_DATE)
	{
		printf("invalid exp date ! , please enter it again :\n");
		return;  // end the program
	}

	 //third , take the CardPAN from the user
	EN_cardError_t CardPAN_error = getCardPAN(cardData);  // variable to store the value returned from getCardPAN

	 //check the valdity of the CardPAN
	if (CardPAN_error == WRONG_PAN)
	{
		printf("invalid PAN ! , please enter your PAN again :\n");
		return;  // end the program
	}

	//------------------------------------------------------------------------------------

	//terminal module
	ST_terminalData_t terminal_info;                  // object from structure
	ST_terminalData_t* termData = &terminal_info;     // pointer to this object

	//first , take the TransactionDate from the user
	EN_terminalError_t TransactionDate_error = getTransactionDate(termData);  // variable to store the value returned from getTransactionDate

	 //check the valdity of the TransactionDate
	if (TransactionDate_error == WRONG_DATE)
	{
		printf("invalid trans date ! , please enter it again :\n");
		return;  // end the program
	}

	 //second , the isCardExpired function
	EN_terminalError_t CardExpired_error = isCardExpired(cardData, termData);  // variable to store the value returned from isCardExpired

	 //check the valdity of the card
	if (CardExpired_error == EXPIRED_CARD)
	{
		printf("expired card ! , please enter your card again :\n");
		return;  // end the program
	}

	 //third ,  take the TransactionAmount from the user
	EN_terminalError_t TransactionAmount_error = getTransactionAmount(termData);  // variable to store the value returned from getTransactionAmount

	 //check the valdity of the TransactionAmount
	if (TransactionAmount_error == INVALID_AMOUNT)
	{
		printf("invalid trans amount ! , please enter it again :\n");
		return;  // end the program
	}

	 //fourth , take the maxTransactionAmount 
	
	EN_terminalError_t setMaxAmount_error = setMaxAmount(termData, max_amount);  // variable to store the value returned from getTransactionAmount

	 //check the valdity of the maxTransactionAmount
	if (setMaxAmount_error == INVALID_MAX_AMOUNT)
	{
		printf("invalid max trans amount ! , please enter it again :\n");
		return;  // end the program
	}

	 //fifth , the isBelowMaxAmount function 
	EN_terminalError_t BelowMaxAmount_error = isBelowMaxAmount(termData);  // variable to store the value returned from isBelowMaxAmount

	 //check if the transaction amount exceed the maxTransactionAmount or not
	if (BelowMaxAmount_error == EXCEED_MAX_AMOUNT)
	{
		printf("exceeded max amount for transaction ! , please enter a valid amount :\n");
		return;  // end the program
	}

	//-------------------------------------------------------------------------

	// server module
	ST_transaction_t transaction_info;                  // object from structure
	ST_transaction_t* transData = &transaction_info;     // pointer to this object
	
	// assign the defined card & terminal info to the decleared structure
	transData->cardHolderData = card_info;
	transData->terminalData = terminal_info;

	// take the TransactionDate from the user
	EN_transState_t recieveTransactionData_error = recieveTransactionData(transData);  // variable to store the value returned from recieveTransactionData

	if (recieveTransactionData_error == FRAUD_CARD)
	{
		saveTransaction(transData);
		printf("this account is not found ! \n");
		return;
	}
	else if(recieveTransactionData_error == DECLINED_STOLEN_CARD)
	{
		saveTransaction(transData);
		printf("this card is stolen & the account is blocked ! \n");
		return;
	}
	else if (recieveTransactionData_error == DECLINED_INSUFFECIENT_FUND)
	{
		saveTransaction(transData);
		printf("the transaction amount is greater than the account balance ! \n");
		return;
	}
	else if (recieveTransactionData_error == INTERNAL_SERVER_ERROR)
	{
		saveTransaction(transData);
		printf("there an internal error in the server , the transaction can't be saved , please try again ! \n");
		return;
	}
	else if (recieveTransactionData_error == APPROVED)
	{
		printf("thanks for using our application ! , your transaction is done successfully \n");
		return;
	}
	else
	{
		return;
	}
}
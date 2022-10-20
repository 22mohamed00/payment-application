#define _CRT_SECURE_NP_WARNINGS
#define _CRT_NONSTDC_NP_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include "../card/card.h"
#include "../terminal/terminal.h"
#include "server.h"

//global array of ST_accountsDB_t structures for the valid accounts database

ST_accountsDB_t accountsDB[255] = { 
{500.0 ,RUNNING,"0123456789123456"}, {1000.0,BLOCKED,"5465498791326549"},
{1500.0,RUNNING,"4897913216547897"}, {2000.0,BLOCKED,"1235465794653217"},
{3000.0,RUNNING,"2154896549879856"}, {3500.0,BLOCKED,"4568798132549879"},
{4000.0,RUNNING,"4567891478529639"}, {4500.0,BLOCKED,"1478529632584179"},};

// global counter to the no of accounts
int acc_counter = 0 ;

//global array of ST_transaction_t structures for the valid transactions database

ST_transaction_t transaction[255] = { 0 };

// global counter to the no of transaction
int trans_counter = 0;

EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
	
	// take all transaction data and validate its data

	EN_serverError_t valid_account_error;
	valid_account_error = isValidAccount(&transData->cardHolderData, &accountsDB[acc_counter]);

	if (valid_account_error == ACCOUNT_NOT_FOUND)
	{
		return FRAUD_CARD;
	}

	EN_serverError_t amount_avaliable_error;
	amount_avaliable_error = isAmountAvailable(&transData->terminalData, &accountsDB[acc_counter]);

	if (amount_avaliable_error == LOW_BALANCE)
	{
		return DECLINED_INSUFFECIENT_FUND;
	}

	EN_serverError_t blocked_account_error;
	blocked_account_error = isBlockedAccount(&accountsDB[acc_counter]);

	if (blocked_account_error == BLOCKED_ACCOUNT)
	{
		return DECLINED_STOLEN_CARD;
	}


	EN_serverError_t saving_transaction_error;
	saving_transaction_error = saveTransaction(transData);

	if (saving_transaction_error == SAVING_FAILED)
	{
		return INTERNAL_SERVER_ERROR;
	}
	// print the account balance before the transaction
	printf("your balance before the transaction is : %.4f\n\n", accountsDB[acc_counter].balance);

	//update the database with the new balance
	accountsDB[acc_counter].balance -= transData->terminalData.transAmount;
	printf("your new balance after the transaction is : %.4f\n\n", accountsDB[acc_counter].balance);

	return APPROVED;
}


EN_serverError_t isValidAccount(ST_cardData_t *cardData, ST_accountsDB_t *accountRefrence)
{
	//validate if the account related to this card exists or not in the server's database

	for ( acc_counter   ; acc_counter < 255; acc_counter++)
	{
		if (strcmp(accountRefrence->primaryAccountNumber, cardData->primaryAccountNumber) == 0) 
		{
		    accountRefrence = &accountsDB[acc_counter];
			return SERVER_OK;
		}
		else
		{
		    accountRefrence = NULL;
			return ACCOUNT_NOT_FOUND;
		}
	}
}

EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence)
{
	//verifies if the account is blocked or not

	if (accountRefrence->state == RUNNING)
	{
		return SERVER_OK;
	}
	else
	{
		return BLOCKED_ACCOUNT;
	}
}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, ST_accountsDB_t* accountRefrence)
{
	// check if the account has a sufficient amount to withdraw or not

	if (termData->transAmount > accountRefrence->balance)
	{
		return LOW_BALANCE;
	}
	else
	{
		return SERVER_OK;
	}
}

EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
	// update the new sequence number to a transaction

	transData->transactionSequenceNumber = trans_counter;

	// store all transaction data in the transactions database

	if (transData->transactionSequenceNumber < 255)   // if the no of accounts is less than the allowed number 
	{
		transaction[transData->transactionSequenceNumber].cardHolderData = transData->cardHolderData;
		transaction[transData->transactionSequenceNumber].terminalData = transData->terminalData;
		transaction[transData->transactionSequenceNumber].transState = transData->transState;
		transaction[transData->transactionSequenceNumber].transactionSequenceNumber = transData->transactionSequenceNumber + 1;


		// update the number of transactions
		trans_counter++;

		printf("\n");
		// prints all transactions found in the transactions DB
		listSavedTransactions();
		printf("\n");

		return SERVER_OK;
	}
	return SAVING_FAILED;
}

void listSavedTransactions(void)
{
	// prints all transactions found in the transactions DB

	for (int c = 0;c < trans_counter;c++)
	{
		printf("######################### \n");
		printf("Transaction Sequence Number: %i\n", transaction[c].transactionSequenceNumber);
		printf("Transaction Date: %s\n", transaction[c].terminalData.transactionDate);
		printf("Transaction Amount: %f\n", transaction[c].terminalData.transAmount);
		printf("Transaction State: %i\n", transaction[c].transState);
		printf("Terminal Max Amount:%f \n", transaction[c].terminalData.maxTransAmount);
		printf("Cardholder Name: %s\n", transaction[c].cardHolderData.cardHolderName);
		printf("PAN: %s\n", transaction[c].cardHolderData.primaryAccountNumber);
		printf("Card Expiration Date: %s\n", transaction[c].cardHolderData.cardExpirationDate);
		printf("######################### \n");
	}
}
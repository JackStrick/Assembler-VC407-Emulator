//
// Created by Jonathan Strickland on 11/7/22.
//
#include "stdafx.h"
#include "Errors.h"

// Vector used to store all error messages
vector<string> Errors::m_ErrorMsgs;


/*
NAME

	InitErrorReporting - Initialize Errors

SYNOPSIS

	void Errors::InitErrorReporting()
		- no arguments

DESCRIPTION

	Initializes Error Reporting within the object instance. Clears the entire vector of any data from initialization.

RETURNS

	- no return
*/
void Errors::InitErrorReporting()
{
	m_ErrorMsgs.clear();
}


/*
NAME

	DsiplayErrors - Display's Errors

SYNOPSIS

	void Errors::DisplayErrors()
		- no arguments

DESCRIPTION

	Displays all errors stored inside m_ErrosMsgs vector

RETURNS

	- no return
*/
void Errors::DisplayErrors()
{
	//Iterates through entire vector 
	for (int i = 0; i < m_ErrorMsgs.size(); i++)
	{
		//Displays each item in the vector
		cout << m_ErrorMsgs[i] << endl;
	}
}




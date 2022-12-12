//
// Created by Jonathan Strickland on 11/7/22.
//
#include "stdafx.h"
#include "Errors.h"

vector<string> Errors::m_ErrorMsgs;

void Errors::InitErrorReporting()
{
	m_ErrorMsgs.clear();
}



void Errors::DisplayErrors()
{
	for (int i = 0; i < m_ErrorMsgs.size(); i++)
	{
		cout << m_ErrorMsgs[i] << endl;
	}
}





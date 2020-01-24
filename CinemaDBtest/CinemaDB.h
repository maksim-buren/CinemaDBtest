#pragma once
#import <C:\\Program Files\\Common Files\\system\\ado\\msado15.dll> rename( "EOF", "AdoNSEOF" )

#include<iostream>
#include <typeinfo>
#include<sql.h>
#include<sqlext.h>
#include <oledb.h>  
#include <stdio.h>  
#include <conio.h>  
#include "icrsint.h"  
#include <string>
#include"Function.h"
#include<list>

using namespace std;


inline void TESTHR(HRESULT _hr)
{
	if FAILED(_hr)
	_com_issue_error(_hr);
}


class CinemaDB
{
private:
	ADODB::_ConnectionPtr pConnection = NULL;
	ADODB::_RecordsetPtr pRecordset = NULL;
	ADODB::_CommandPtr pComand = NULL;
	_bstr_t bstrConnect;
	HRESULT hr;
public:
	CinemaDB(const char* conectPath)
	{
		CoInitialize(NULL);
		_bstr_t bstrConnect = conectPath;
		TESTHR(pConnection.CreateInstance(__uuidof(ADODB::Connection)));
		hr = pConnection->Open(bstrConnect, "", "", ADODB::adConnectUnspecified);
		if (SUCCEEDED(hr)) {
			cout << "Successfully connected to database." << endl;
		}
		else
		{
			cout << "Conection to data base error" << endl;
		}
	}

	~CinemaDB()
	{
		pConnection->Close();
	}

	string getListStringNameOfFilm()
	{
		string listOfFilms;
		TESTHR(pRecordset.CreateInstance(__uuidof(ADODB::Recordset)));
		_bstr_t query = "SELECT title FROM Films;";
		hr = pRecordset->Open(query, _variant_t((IDispatch *)pConnection, true), ADODB::adOpenUnspecified, ADODB::adLockUnspecified, ADODB::adCmdText);
		if (SUCCEEDED(hr))
		{
			ADODB::Fields* pFields = NULL;
			hr = pRecordset->get_Fields(&pFields);
			if (pFields && pFields->GetCount() <= 0)
			{
				cout << ": Error: Number of fields in the result set is 0." << endl;
			}
			while (!pRecordset->AdoNSEOF) {
				for (long nIndex = 0; nIndex < pFields->GetCount(); nIndex++) {

					string temp = bstr_to_str(_bstr_t(pFields->GetItem(nIndex)->GetValue()));
					listOfFilms += temp + "|";
				}
				pRecordset->MoveNext();
			}
		}
		else
		{
			cout << "error" << endl;
		}
		pRecordset->Close();
		listOfFilms += "\0";
		return listOfFilms;
	}

	list<string> getListOfUsers()
	{
		list<string> listOfUsers;
		TESTHR(pRecordset.CreateInstance(__uuidof(ADODB::Recordset)));
		_bstr_t query = "SELECT Nickname FROM Users;";
		hr = pRecordset->Open(query, _variant_t((IDispatch *)pConnection, true), ADODB::adOpenUnspecified, ADODB::adLockUnspecified, ADODB::adCmdText);
		if (SUCCEEDED(hr))
		{
			ADODB::Fields* pFields = NULL;
			hr = pRecordset->get_Fields(&pFields);
			if (pFields && pFields->GetCount() <= 0)
			{
				cout << ": Error: Number of fields in the result set is 0." << endl;
			}
			while (!pRecordset->AdoNSEOF) {
				for (long nIndex = 0; nIndex < pFields->GetCount(); nIndex++) {

					string temp = bstr_to_str(_bstr_t(pFields->GetItem(nIndex)->GetValue()));
					listOfUsers.push_back(temp);
				}
				pRecordset->MoveNext();
			}
		}
		else
		{
			cout << "error" << endl;
		}
		pRecordset->Close();
		return listOfUsers;
	}
	
	string getListOfSessionByFileTitle(const char* title)
	{
		string listOfSession;
		TESTHR(pRecordset.CreateInstance(__uuidof(ADODB::Recordset)));
		string request = "SELECT Num FROM Sessions WHERE Film=\'" + string(title) + "\';";
		_bstr_t query = request.c_str();
		hr = pRecordset->Open(query, _variant_t((IDispatch *)pConnection, true), ADODB::adOpenUnspecified, ADODB::adLockUnspecified, ADODB::adCmdText);
		if (SUCCEEDED(hr))
		{
			ADODB::Fields* pFields = NULL;
			hr = pRecordset->get_Fields(&pFields);
			if (pFields && pFields->GetCount() <= 0)
			{
				cout << ": Error: Number of fields in the result set is 0." << endl;
			}
			while (!pRecordset->AdoNSEOF) {
				for (long nIndex = 0; nIndex < pFields->GetCount(); nIndex++) {

					string temp = bstr_to_str(_bstr_t(pFields->GetItem(nIndex)->GetValue()));
					listOfSession += temp + "|";
				}
				pRecordset->MoveNext();
			}
		}
		else
		{
			cout << "error" << endl;
		}
		pRecordset->Close();
		listOfSession += "\0";
		return listOfSession;
	}

	int getHallByNumOfSession(int numOfSession)
	{
		string numOfHall;
		TESTHR(pRecordset.CreateInstance(__uuidof(ADODB::Recordset)));
		string request = "SELECT Hall FROM Sessions WHERE Num = " + std::to_string(numOfSession) + ";";
		_bstr_t query = request.c_str();
		hr = pRecordset->Open(query, _variant_t((IDispatch *)pConnection, true), ADODB::adOpenUnspecified, ADODB::adLockUnspecified, ADODB::adCmdText);
		if (SUCCEEDED(hr))
		{
			ADODB::Fields* pFields = NULL;
			hr = pRecordset->get_Fields(&pFields);
			if (pFields && pFields->GetCount() <= 0)
			{
				cout << ": Error: Number of fields in the result set is 0." << endl;
			}
			while (!pRecordset->AdoNSEOF) {
				for (long nIndex = 0; nIndex < pFields->GetCount(); nIndex++) {

					string temp = bstr_to_str(_bstr_t(pFields->GetItem(nIndex)->GetValue()));
					numOfHall += temp;
				}
				pRecordset->MoveNext();
			}
		}
		else
		{
			cout << "error" << endl;
		}
		pRecordset->Close();
		numOfHall += "\0";
		return std::stoi(numOfHall);
		
	}

	
	
	int InsertUser(const char* nickname, const char* lastname, const char* firstname)
	{
		try
		{
			hr = pComand.CreateInstance(__uuidof(ADODB::Command));
			pComand->ActiveConnection = pConnection;
			string temp = "INSERT INTO Users (Nickname,Last_name,First_name) VALUES (\'" + string(nickname) + "\',\'" + string(lastname) + "\',\'" + string(firstname) + "\');";
			_bstr_t query = temp.c_str();
			pComand->CommandText = query;
			pComand->Execute(NULL, NULL, ADODB::adCmdText);
			return 0;
		}
		catch (exception e)
		{
			return -11;
		}
	}

};


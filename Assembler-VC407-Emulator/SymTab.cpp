//
//		Implementation of the symbol table class.  This is the format I want for commenting functions.
//
#include "stdafx.h"
#include "SymTab.h"

/*
NAME

    AddSymbol - adds a new symbol to the symbol table.

SYNOPSIS

    void AddSymbol( const string &a_symbol, int a_loc );
    	a_symbol	-> The name of the symbol to be added to the symbol table.
    	a_loc		-> the location to be associated with the symbol.

DESCRIPTION

    This function will place the symbol "a_symbol" and its location "a_loc"
    in the symbol table.
*/
void SymbolTable::AddSymbol( const string &a_symbol, int a_loc )
{
    // If the symbol is already in the symbol table, record it as multiply defined.
    map<string, int>::iterator st = m_symbolTable.find( a_symbol );
    if( st != m_symbolTable.end() ) {

        st->second = multiplyDefinedSymbol;
        return;
    }
    // Record the location in the symbol table.
    m_symbolTable[a_symbol] = a_loc;
}

/*NAME

    LookupSymbol - Checks the Map For Symbol

SYNOPSIS

    bool SymbolTable::LookupSymbol(const string& a_symbol, int& a_loc)
        a_symbol	-> The name of the symbol to check against the symbol table.
        a_loc		-> the location associated with the symbol.

DESCRIPTION

    This function will check if the symbol "a_symbol" and its location "a_loc" are
    in the symbol table.

RETURNS
    
    True    ->  The symbol exists
    False   ->  The symbol does NOT exist
*/
bool SymbolTable::LookupSymbol(const string& a_symbol, int& a_loc)
{
    map<string, int>::iterator st = m_symbolTable.find(a_symbol);
    if (!st->first.empty())
    {
        return true;
    }
}


/*NAME

    GetValue - Finds the Value of the Map at a given Key

SYNOPSIS

    int SymbolTable::GetValue(const string& a_symbol)
        a_symbol    ->  The name of the symbol to check against the symbol table.

DESCRIPTION

    This function will check if the symbol "a_symbol" is in the symbol table by Iterating through the map
    and using a_symbol as the key. If the key exists in the table, the function will return the value at the key

RETURNS

    Integer:
        Value: Location stored at Key
*/
int SymbolTable::GetValue(const string& a_symbol)
{
    map<string, int>::iterator st = m_symbolTable.find(a_symbol);
    if (!st->first.empty())
    {
        return st->second;
    }
    else
    {
        return -1;
    }
}

/*NAME

    DisplaySymbolTable - Displays contents of the Symbol Table Map

SYNOPSIS

    void SymbolTable::DisplaySymbolTable()
        - No Arguments

DESCRIPTION

    This function will displays the entire contents of the Symbol Table Map 
    to the user via the console.

RETURNS

    None

*/
void SymbolTable::DisplaySymbolTable()
{
    cout << "Symbol Table: \n" << endl;
    cout << "Symbol # \tSymbol \t\tLocation" << endl;
    auto it = m_symbolTable.begin();
    
    //Iterate through each pair in the map and display 
    //the key and the value along with its place in order ("i")
    for (int i = 0; i < m_symbolTable.size(); i++)
    {
        string sym = it->first;
        int loc = it->second;
        //cout << i << "          " << sym << "       " << loc << endl;
        cout << i << "\t\t" << sym << "\t\t" << loc << endl;
        it++;
       
    }
   
   cout << "\n------------------------------------------------" << endl;
   cout << "\n\nPress Enter to Continue . . ." << endl;
   cin.ignore();
}

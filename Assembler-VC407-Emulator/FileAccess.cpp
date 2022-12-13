//
//  Implementation of file access class.
//
#include "stdafx.h"
#include "FileAccess.h"



/*NAME
   
    FileAccess - runs the instructions stored in memory

SYNOPSIS

    FileAccess::FileAccess( int argc, char *argv[] )
        argc -> number of command line arguments | *argv -> pointer to char array which should contain the name of the file we are processing

DESCRIPTION

    The FileAccess class contains all functions dealing with reading data from file specified through
    the command line arguments.

*/
FileAccess::FileAccess( int argc, char *argv[] )
{
    // Check that there is exactly one run time parameter.
    if( argc != 2 ) {
        cerr << "Usage: Assem <FileName>" << endl;
        exit( 1 );
    }
    // Open the file.  One might question if this is the best place to open the file.
    // One might also question whether we need a file access class.
    m_sfile.open( argv[1], ios::in );

    // If the open failed, report the error and terminate.
    if( ! m_sfile ) {
        cerr << "Source file could not be opened, assembler terminated."
            << endl;
        exit( 1 ); 
    }
}
FileAccess::~FileAccess( )
{
    // Not that necessary in that the file will be closed when the program terminates, but good form.
    m_sfile.close( );
}

/*NAME

    GetNextLine - Get the next line from the source file. Some error checking.

SYNOPSIS

    bool GetNextLine( string &a_line )
        &a_line   ->  line from file.

DESCRIPTION

    This function retrieves the lines from the file. It returns a boolean depending on whether there are
    any lines remaining in the file.

RETURNS

        True -> Next line exists
        False -> No more lines are left in the file
*/
bool FileAccess::GetNextLine( string &a_line )
{
    // If there is no more data, return false.
    if( m_sfile.eof() ) {
    
        return false;
    }
    getline( m_sfile, a_line );
    
    // Return indicating success.
    return true;
}

/*NAME

    Rewind - Get the next line from the source file. Some error checking.

SYNOPSIS

    bool Rewind()
       no arguments

DESCRIPTION

    This function points back to the beginning of the file.

RETURNS

        no return
*/
void FileAccess::rewind( )
{
    // Clean all file flags and go back to the beginning of the file.
    m_sfile.clear();
    m_sfile.seekg( 0, ios::beg );
}
    

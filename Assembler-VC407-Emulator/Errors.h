//
// Class to manage error reporting. Note: all members are static so we can access them anywhere.
// What other choices do we have to accomplish the same thing?
//
#ifndef _ERRORS_H
#define _ERRORS_H

#include <string>
#include <vector>

class Errors {

public:
    
    // Initializes error reports.
    static void InitErrorReporting( );

    // Records an error message.
    static void RecordError( string a_emsg )
    {
        m_ErrorMsgs.push_back(a_emsg);
    }

    // Displays the collected error message.
    static void DisplayErrors( );

    // Checks if Errors Exist
    static bool IsError()
    {
        if (!m_ErrorMsgs.empty())//Vector Not Empty, Return True
        {
            return true;
        }
        return false;
    }

private:

    static vector<string> m_ErrorMsgs;  // This must be declared in the .cpp file.  Why?
};
#endif
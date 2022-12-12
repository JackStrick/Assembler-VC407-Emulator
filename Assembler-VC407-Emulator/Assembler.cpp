//
//      Implementation of the Assembler class.
//
#include "stdafx.h"
#include "Assembler.h"
#include "Errors.h"

// Constructor for the assembler.  Note: we are passing argc and argv to the file access constructor.
// See main program.  
Assembler::Assembler( int argc, char *argv[] )
: m_facc( argc, argv )
{
    // Nothing else to do here at this point.
}  
// Destructor currently does nothing.  You might need to add something as you develope this project.  If not, we can delete it.
Assembler::~Assembler( )
{
}
// Pass I establishes the location of the labels.  You will write better function comments according to the coding standards.
void Assembler::PassI( ) 
{
    int loc = 0;        // Tracks the location of the instructions to be generated.

    // Successively process each line of source code.
    for( ; ; ) {

        // Read the next line from the source file.
        string line; 
        if( ! m_facc.GetNextLine( line ) ) {

            // If there are no more lines, we are missing an end statement.
            // We will let this error be reported by Pass II.
            return;
        }
        // Parse the line and get the instruction type.
        Instruction::InstructionType st =  m_inst.ParseInstruction( line );

        // If this is an end statement, there is nothing left to do in pass I.
        // Pass II will determine if the end is the last statement and report an error if it isn't.
        if( st == Instruction::ST_End ) return;

        // Labels can only be on machine language and assembler language
        // instructions.  So, skip comments.
        if( st == Instruction::ST_Comment )  
        {
        	continue;
	    }
        // If the instruction has a label, record it and its location in the
        // symbol table.
        if( m_inst.isLabel( ) ) {

            m_symtab.AddSymbol( m_inst.GetLabel( ), loc );
        }
        // Compute the location of the next instruction.
        loc = m_inst.LocationNextInstruction( loc );
    }
}


/*
NAME

    PassII - Second Pass On File, with Error Reporting

SYNOPSIS

    void Assembler::PassII();

DESCRIPTION

    Translates each line into Instruction Code
    Checks and reports on errors in file passed
*/
void Assembler::PassII()
{
    //Point Back To Start of File
    m_facc.rewind();
    //Error Reporting
    Errors::InitErrorReporting();

    cout << "Translation of Program\n" << endl;
    cout << "Location\tContents\tOriginal Statement" << endl;
    
    //Second Pass
    int loc = 0; 
    
    for (; ; ) {

        // Read the next line from the source file.
        string line;

        //If no END Statement Report Error
        if (!m_facc.GetNextLine(line)) {

            Errors::RecordError("ERROR - missing END statement. Unable to continue process...");
            return;
        }

        // Parse the line and get the instruction type.
        Instruction::InstructionType st = m_inst.ParseInstruction(line);

        
        //Determine if the end is the last statement and report an error if it isn't.
        if (st == Instruction::ST_End)
        {
            if (m_facc.GetNextLine(line)) 
            {
                Errors::RecordError("Instruction Following End Statement - Assembler.cpp: Line 108");//Make a check for commented out lines
            }
            m_inst.DisplayOutput(loc, st);
            return;
        }
            
        // Labels can only be on machine language and assembler language
        // instructions.  So, skip comments.
        else if (st == Instruction::ST_Comment)
        {
            m_inst.DisplayOutput(loc, st);
            continue;
        }
        else if (st == Instruction::ST_AssemblerInstr)
        {
            m_inst.DisplayOutput(loc, st);
        }
        else if (st == Instruction::ST_MachineLanguage)
        {
            /*if (!m_symtab.LookupSymbol(m_inst.GetLabel(), loc))
            {
                Errors::RecordError("Symbol Does Not Exist - Assembler.cpp: Line 129");
            }*/
            string operand = m_inst.GetOperand();
            string opcode = m_inst.GetOpCode();
            int OperandLoc = 0;
            if (operand != "") {
                OperandLoc = m_symtab.GetValue(operand);
                if (OperandLoc == -1)
                {
                    Errors::RecordError("Operand Does Not Exist In Symbol Table - Assmebler.cpp: Line 137");
                }
            }
            

            string OpCheck = opcode;
            transform(OpCheck.begin(), OpCheck.end(), OpCheck.begin(), ::toupper);
            int NumericOpcode = m_inst.GetOpCodeValue(OpCheck);
            if (NumericOpcode == -1)
            {
                Errors::RecordError("The Attempted Operation Does Not Exist - Assmebler.cpp: Line 143");
            }
            
            m_inst.DisplayMLOutput(loc, OperandLoc, NumericOpcode);
        }
        
        // Compute the location of the next instruction.
        loc = m_inst.LocationNextInstruction(loc);
    }



    


    
}


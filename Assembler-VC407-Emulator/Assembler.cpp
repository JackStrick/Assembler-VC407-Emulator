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
            //Display output for Assem Instruction
            m_inst.DisplayOutput(loc, st);
        }
        else if (st == Instruction::ST_MachineLanguage)
        {
            //Get Operand and Opcode
            string operand = m_inst.GetOperand();
            string opcode = m_inst.GetOpCode();

            //Initialize to hold operand location
            int OperandLoc = 0;

            //If operand is not empty, get its location from symbol table
            if (operand != "") 
            {
                //Pass Operand as key to return the value in SymTab map
                OperandLoc = m_symtab.GetValue(operand);

                //OperandLoc returns as -1 if it was not in the table
                //Record Error
                if (OperandLoc == -1)
                {
                    Errors::RecordError("Operand Does Not Exist In Symbol Table - Assmebler.cpp: Line 145");
                }
            }
            
            //Convert to Uppercase
            string OpCheck = opcode;
            transform(OpCheck.begin(), OpCheck.end(), OpCheck.begin(), ::toupper);

            //Find numeric opcode and set variable
            int NumericOpcode = m_inst.GetOpCodeValue(OpCheck);

            //If OpCode didn't exist, returns -1 and records an error
            if (NumericOpcode == -1)
            {
                Errors::RecordError("The Attempted Operation Does Not Exist - Assmebler.cpp: Line 159");
            }
            
            //Display Output for Machine Language Instruction
            m_inst.DisplayMLOutput(loc, OperandLoc, NumericOpcode);

            //create contents variable to store in emulator memory
            int contents = (NumericOpcode * 10000) + OperandLoc;
            
            //If memory insert returns false then record error
            if (!m_emul.insertMemory(loc, contents))
            {
                Errors::RecordError("Memory Already Exists in Attempted Location or Location is out of bounds - Assembler.cpp: Line 171");
            }
        }
        // Compute the location of the next instruction.
        loc = m_inst.LocationNextInstruction(loc);
    }
}

void Assembler::RunProgramInEmulator()
{
    //Check if errors exist in error vector
    if (Errors::IsError())
    {
        //If true, display all errors
        Errors::DisplayErrors();
    }
    //Run Emulation
    if (!m_emul.runProgram())
    {
        cout << "Unknown Operation Code - Assembler.cpp: Line 190\nTerminating Program....\n" << endl;
    }
    cout << "\nEnd of emulation" << endl;
}


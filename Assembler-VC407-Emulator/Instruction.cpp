//
// Created by Jonathan Strickland on 11/7/22.
//
#include "stdafx.h"
#include "Instruction.h"
#include "Errors.h"



/*NAME

    ParseInstruction - Seperates the current line into types

SYNOPSIS

    Instruction::InstructionType Instruction::ParseInstruction(string a_line)
        string a_line   ->  current line from file.

DESCRIPTION

    The Instruction Class contains all functions dealing with parsing line input into labels,
    op codes, and operands. After parsing the line, it can determine whether an instruction is an 
    assembly language instruction, machine language, comment, or an end instruction by returning as such. Includes some error checking.

RETURNS
    
    Instruction Type:
    
        ST_Comment - If Comment
        ST_AssemblerInstr - If Assembly Language Instruction
        ST_End - If END Statement
        ST_MachineLanguage - If Machine Lanuage Instruction

        
*/
Instruction::InstructionType Instruction::ParseInstruction(string a_line)
{
    //Set the the original instruction
    m_instruction = a_line;

    //Remove Comments
    RemoveComment(a_line);
 
    //Parse the line
    bool rv = ParseLine(a_line, m_Label, m_OpCode, m_Operand);

    //Check if Operand Is Numeric - Sets m_IsNumericOperand
    IsOperandNumeric(m_Operand);

    //If Operand is Numeric : Set Operand Value
    if (m_IsNumericOperand)
    {
        m_OperandValue = stoi(m_Operand);
    }

    //Make the OpCodes Case Sensitive
    OpCheck = m_OpCode;
    transform(OpCheck.begin(), OpCheck.end(), OpCheck.begin(), ::toupper);

    //Checks For Possible Errors or Instruction Type 
    //If Label isn't empty but opcode is empty - Throw and error
    if (!m_Label.empty() && m_OpCode.empty())
    {
        Errors::RecordError("A label exists but an Operation Code Does Not: Instruction.cpp Line: 64");
    }

    //If Label and OpCode is empty - Return Comment Type
    else if (m_Label.empty() && m_OpCode.empty())
    {
        return ST_Comment;
    }

    //If OpCode is DS or DC or ORG - Return Assembly Type
    else if (OpCheck == "DS" || OpCheck == "DC" || OpCheck == "ORG")
    {
        return ST_AssemblerInstr;
    }

    //If OpCode is END - Return End Type
    else if (OpCheck == "END")
    {
        return ST_End;
    }

    //If not caught above then it is a Machine Language Instruction
    return ST_MachineLanguage;
}



/*NAME

    RemoveComment - Removes Comments

SYNOPSIS

    void Instruction::RemoveComment(string& a_line)
        string& a_line   ->  current line from file.

DESCRIPTION

    This function takes in the current line in the file and removes anything following a semicolon (;)
    in the line

RETURNS

    none

*/
void Instruction::RemoveComment(string& a_line)
{
    size_t pos = a_line.find(';');
    if( pos == string::npos) return;

    a_line.erase(pos);
}


/*NAME

    ParseLine - Parses Line Into 3 segments

SYNOPSIS

    bool Instruction::ParseLine(const string& a_line, string& a_label, string& a_opcode, string& a_operand)
       const string& a_line   ->  current line from file.
       string& a_label        ->  Label 
       string& a_opcode       ->  OpCode
       string& a_operand      ->  Operand

DESCRIPTION

    This function is responsible for examining the line and seperates each part of the instruction

RETURNS

    True   ->  Empty Line or Too Many Arguments
    False  ->  Parsed Correctly with Correct Arguments

*/
bool Instruction::ParseLine(const string& a_line, string& a_label, string& a_opcode, string& a_operand)
{
    a_label = a_opcode = a_operand = "";

    if (a_line.empty())
    {
        return true;
    }

    istringstream ins(a_line);
    if (a_line[0] != ' ' && a_line[0] != '\t')
    {
        ins >> a_label;
    }
    string temp;
    ins >> a_opcode >> a_operand >> temp;

    if (!temp.empty())
    {
        Errors::RecordError("Too Many Arguments On Line: Instruction.cpp - Line 160");
    }

    return temp == "";
}

/*NAME

    LocationNextInstruction - Finds Next Instruction Location

SYNOPSIS

    int Instruction::LocationNextInstruction(int a_loc)
       int a_loc   ->  current location.

DESCRIPTION

    This function takes in the current location and returns the new location based on the instruction

RETURNS

    Integer:
        The new location for the next instruction

*/
int Instruction::LocationNextInstruction(int a_loc)
{
    //Check if the OpCode is allocating space
    //If ORG, return operand
    if (OpCheck == "ORG")
    {
        //The label should be empty - If not add error
        if (!m_Label.empty())
        {
            Errors::RecordError("ORG Command Preceded By Label - Instruction.cpp: Line 194");
        }
        return stoi(m_Operand);
    }

    //If DS, increment the save location with the operand
    else if (OpCheck == "DS")
    {   
        return a_loc + stoi(m_Operand);
    }

    //If the instruction is completely empty - Return the same a_loc
    else if (m_Label.empty() && m_OpCode.empty() && m_Operand.empty())
    {
        return a_loc;
    }

    //Anything else, just increment by 1
    else {
        return a_loc + 1;
    }
}


/*NAME

    IsOperandNumeric - Checks Operand for Numerical Value

SYNOPSIS

    void Instruction::IsOperandNumeric(string m_Operand)
        string m_Operand   ->  current Operand from line

DESCRIPTION

    This function takes in the current operand and checks that it is a numerical value. This should only be numeric 
    if it is an Assembly Language instruction.

RETURNS

    none

*/
void Instruction::IsOperandNumeric(string m_Operand)
{
    // Check that Operand is not empty
    if (!m_Operand.empty())
    {
        //Loop through the Operand string and check that the value is a digit
        for (int i = 0; i < m_Operand.length(); i++)
        {
            //If Digit set Operand Value and set Numeric to true
            if (isdigit(m_Operand[i]))
            {
                m_IsNumericOperand = true;
                m_OperandValue = m_Operand[i];
            }
            else
            {
                //Not Numeric
                m_IsNumericOperand = false;
            }
        }
    }
    //If not caught anywhere then default to false
    m_IsNumericOperand = false;
}


/*NAME

    GetOpCodeValue - Gets the value stored in OpCode Map

SYNOPSIS

    int Instruction::GetOpCodeValue(const string& a_symbol)
        const string& a_symbol  ->  the OpCode string symbol (i.e. ADD, SUB, etc)

DESCRIPTION

    This function takes in the OpCode symbol which is the key in the OpCode Map. 
    The Key is used to return the value which is the equivalent numerical OpCode Value (1-13)

RETURNS

    Integer:
        m_NumOpCode - The numerical value of the given OpCode

*/
int Instruction::GetOpCodeValue(const string& a_symbol) {
    map<string, int>::iterator st = m_NumOpMap.find(a_symbol);
    if (!st->first.empty())
    {
        m_NumOpCode = st->second;
        return m_NumOpCode;
    }
    else
    {
        return -1;
    }
}


/*NAME

    DisplayOutput - Displays Output To User

SYNOPSIS

        void Instruction::DisplayOutput(int a_loc, InstructionType type)
        int a_loc   ->  the location of the current instruction
        InstructionType type    ->  The type of instruction given (END, Assembly Language, Comment)

DESCRIPTION

    This function takes in the location of the current instruction and the type of instruction detemrined by the Parse Instruction
    function. It checks what type of instruction it received and then prints out accordingly.

RETURNS

    - None

*/
void Instruction::DisplayOutput(int a_loc, InstructionType type)
{
    if (type == ST_Comment)
    {
        cout << "\t\t\t\t" << m_instruction << endl;
    }
    else if (type == ST_End)
    {
        cout << "\t\t\t\t" << m_instruction << endl;
    }
    else if (type == ST_AssemblerInstr)
    { 
        if (OpCheck == "ORG") 
        {
            cout << a_loc << "\t\t\t\t" << m_instruction << endl;
        }
        else if (OpCheck != "DS")
        {
            cout << a_loc << "\t\t" << setfill('0') << setw(6) << right << m_Operand << "\t\t" << m_instruction << endl;
        }
        else
        {
            cout << a_loc << "\t\t\t\t" << m_instruction << endl;
        }
    }
}


/*NAME

    DisplayMLOutput - Displays Output for Machine Language Instruction Type 

SYNOPSIS

        void Instruction::DisplayMLOutput(int a_loc, int OperandLoc, int a_NumericOpCode)
        int a_loc               ->  the location of the current instruction
        int OperandLoc          ->  The location of the Operand In Symbol Table
        int a_NumericOpCode     ->  The Numerical Equivalent OpCode Value

DESCRIPTION

    This function takes in the location of the current instruction, lhe location of the Operand In Symbol Table, 
    and the Numerical Equivalent OpCode Value. It then prints out the Machine Language line accordingly.

RETURNS

    - None

*/
void Instruction::DisplayMLOutput(int a_loc, int OperandLoc, int a_NumericOpCode)
{
    cout << a_loc << "\t\t" << setfill('0') << setw(2) << right << a_NumericOpCode << setfill('0') << setw(4) << right << OperandLoc << "\t\t" << m_instruction << endl;
}



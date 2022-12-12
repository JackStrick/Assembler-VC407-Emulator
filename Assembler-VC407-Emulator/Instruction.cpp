//
// Created by Jonathan Strickland on 11/7/22.
//
#include "stdafx.h"
#include "Instruction.h"
#include "Errors.h"

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
    if (!m_Label.empty() && m_OpCode.empty())
    {
        Errors::RecordError("A label exists but an Operation Code Does Not: Instruction.cpp Line: 18");
    }
    else if (m_Label.empty() && m_OpCode.empty())
    {
        return ST_Comment;
    }
    else if (OpCheck == "DS" || OpCheck == "DC" || OpCheck == "ORG")
    {
        return ST_AssemblerInstr;
    }
    else if (OpCheck == "END")
    {
        return ST_End;
    }
    //If not caught above then it is a Machine Language Instruction
    return ST_MachineLanguage;
}
void Instruction::RemoveComment(string& a_line)
{
    size_t pos = a_line.find(';');
    if( pos == string::npos) return;

    a_line.erase(pos);
}
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
        Errors::RecordError("Too Many Arguments On Line: Instruction.cpp - Line 65");
    }

    return temp == "";
}

int Instruction::LocationNextInstruction(int a_loc)
{
    //Check if the OpCode is allocating space
    //If yes, increment the save location with the operand
    //If the instruction is completely empty
    
    if (m_Label.empty() && (OpCheck == "ORG"))
    {
        return stoi(m_Operand);
    }
    else if (OpCheck == "DS")
    {
        return a_loc + stoi(m_Operand);
    }
    else if (m_Label.empty() && m_OpCode.empty() && m_Operand.empty())
    {
        return a_loc;
    }
    else {

        return a_loc + 1;
    }
}

void Instruction::IsOperandNumeric(string m_Operand)
{
    if (!m_Operand.empty())
    {
        for (int i = 0; i < m_Operand.length(); i++)
        {
            if (isdigit(m_Operand[i]))
            {
                m_IsNumericOperand = true;
                m_OperandValue = m_Operand[i];
            }
            else
            {
                m_IsNumericOperand = false;
            }
        }
    }
    m_IsNumericOperand = false;
}

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

void Instruction::DisplayMLOutput(int a_loc, int OperandLoc, int NumericOpcode)
{
    cout << a_loc << "\t\t" << setfill('0') << setw(2) << right << NumericOpcode << setfill('0') << setw(4) << right << OperandLoc << "\t\t" << m_instruction << endl;
}



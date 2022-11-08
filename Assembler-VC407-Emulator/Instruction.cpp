//
// Created by Jonathan Strickland on 11/7/22.
//
#include "stdafx.h"
#include "Instruction.h"
#include "Errors.h"

Instruction::InstructionType Instruction::ParseInstruction(string a_line)
{
    m_instruction = a_line;

    //RRemove Comments
    RemoveComment(a_line);

    //Parse the line
    bool rv = ParseLine(a_line, m_Label, m_OpCode, m_Operand);

    if (!m_Label.empty() && m_OpCode.empty())
    {
        Errors::RecordError("A label exists but an Operation Code Does Not: Instruction.cpp Line: 17");
    }

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

    return temp == "";
}


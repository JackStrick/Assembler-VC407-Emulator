//
// Class to parse and provide information about instructions.  Note: you will be adding more functionality.
//
#pragma once

// The elements of an instruction.
class Instruction {

public:

    Instruction() :m_IsNumericOperand{ false }, m_OperandValue{ 0 }, m_type{ ST_Comment }, m_NumOpCode{ 0 }{};
    ~Instruction() {};

    // Codes to indicate the type of instruction we are processing.  Why is this inside the
    // class?  We should make this an enum class.  We will do this during a lecture.
    enum InstructionType {
        ST_MachineLanguage, 	// A machine language instruction.
        ST_AssemblerInstr,      // Assembler Language instruction.
        ST_Comment,             // Comment or blank line
        ST_End                  // end instruction.
    };
    // Parse the Instruction.
    InstructionType ParseInstruction(string a_line);

    // Compute the location of the next instruction.
    int LocationNextInstruction(int a_loc);

    //Display The Output
    void DisplayOutput(int a_loc, InstructionType);

    void DisplayMLOutput(int a_loc, int OperandLoc, int NumericOpcode);

    void IsOperandNumeric(string m_Operand);
        

    // To access the label
    inline string &GetLabel( ) {//Inline not neccesary because its in a .h file

        return m_Label;
    };

    string& GetOpCode() {

        return m_OpCode;
    };

    string& GetOperand() {
        return m_Operand;
    };

    // To determine if a label is blank.
    inline bool isLabel( ) {

        return ! m_Label.empty();
    };

    int& GetNumOpCode() {
        return m_NumOpCode;
    };

    int GetOpCodeValue(const string& a_symbol);

private:


    // The elements of an instruction
    string m_Label;        // The label.
    string m_OpCode;       // The symbolic op code.
    string m_Operand;     // The first operand.

    string OpCheck;       //Used to check for caseless Op Code

    string m_instruction;    // The original instruction.

    // This is the Numerical OpCode equivalency table.  The symbol is the key to the map.  The value is the numerical OpCode.
    map<string, int> m_NumOpMap = { {"ADD", 1}, {"SUB", 2}, {"MULT", 3}, {"DIV", 4},
                                    {"LOAD", 5}, {"STORE", 6}, {"READ", 7}, {"WRITE", 8},
                                    {"B", 9}, {"BM", 10}, {"BZ", 11}, {"BP", 12}, {"HALT", 13} };

    // Derived values.
    int m_NumOpCode;     // The numerical value of the op code for machine language equivalents.
    InstructionType m_type; // The type of instruction.

    bool m_IsNumericOperand;// == true if the operand 1 is numeric.
    int m_OperandValue;   // The value of the operand 1 if it is numeric.

    void RemoveComment(string& a_line);
    bool ParseLine(const string &a_line, string &a_label, string &a_opcode, string &a_operand);

};



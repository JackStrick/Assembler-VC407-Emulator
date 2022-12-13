#include "stdafx.h"
#include "Emulator.h"



/*
NAME

	insertMemory - Inserts memory at the location

SYNOPSIS

	bool Emulator::insertMemory(int a_loc, int a_contents)
		a_loc		->	location of instruction in memory
		a_contents	-> decimal representation of instruction

DESCRIPTION

	Inserts memory into m_memory vector at specified location

RETURNS
	True	-> if memory was able to be inserted
	False	-> if memory was not able to be inserted (some error)
*/
bool Emulator::insertMemory(int a_loc, int a_contents)
{
	// If Memory exists at this location, Do Not add new memory
	if (!m_memory[a_loc])
	{
		// If not, check location within bounds
		if (a_loc < MEMSZ)
		{
			// If true, add new memory
			m_memory[a_loc] = a_contents;
			return true;
		}
	}
	//If false, record insertion error
	return false;
}


/*
NAME

	runProgram - Runs Emulator

SYNOPSIS

	bool Emulator::runProgram()
		- no arguments

DESCRIPTION

	Uses translated machine/assembly language and runs the emulation

RETURNS

	True	-> if ran successfully
	False	-> if emulator failed or no HALT command was found
*/
bool Emulator::runProgram()
{
	cout << "\n\nPress Enter to Continue . . ." << endl;
	cin.ignore();
	cout << "\nResults from emulating program:\n" << endl;

	// Initialized For Later Input Needs
	int input = 0;

	// Used to access vector
	int memLoc = 0;
	
	// Loop through entire vector and follow translated commands
	while (memLoc < m_memory.size())
	{
		// The contents stored in memory at memLoc
		int contents = m_memory[memLoc];

		// The First 2 digits of contents - OpCode Numerical Value (Should be 0-13)
		int NumOpCode = contents / MEMSZ;

		// The last few digits of contents - Locations of values in memory
		int location = contents % MEMSZ;
		
		// If a potential OpCode - Enters and begins switch
		if (NumOpCode != 0)
		{
			// Checks each op code as it is referenced in Assembly language
			switch (NumOpCode)
			{
				case 1: // ADD
					m_acc += m_memory[location];
					break;

				case 2: // SUB
					m_acc -= m_memory[location];
					break;

				case 3: // MULT
					m_acc *= m_memory[location];
					break;

				case 4: // DIV
					m_acc /= m_memory[location];
					break;

				case 5: // LOAD
					m_acc = m_memory[location];
					break;

				case 6: // STORE
					m_memory[location] = m_acc;
					break;

				case 7: // READ
					cout << "? ";
					cin >> input;
					m_memory[location] = input % MEMSZ;
					break;

				case 8: // WRITE
					cout << m_memory[location] << endl;
					break;

				case 9: //B
					memLoc = location;
					continue;

				case 10: // BM
					if (m_acc < 0)
					{
						memLoc = location;
						continue;
					}
					break;

				case 11: // BZ
					if (m_acc == 0)
					{
						memLoc = location;
						continue;
					}
					break;

				case 12: // BP
					if (m_acc > 0)
					{
						memLoc = location;
						continue;
					}
					break;

				case 13: // HALT
					return true;

				default: //IF NOT AN OPCODE - TERMINATE
					return false;
			}
		}
		memLoc++;
	}
	//IF HALT NEVER REACHED - TERMINATE
	return false;
}


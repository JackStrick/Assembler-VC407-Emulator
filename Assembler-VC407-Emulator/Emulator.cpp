#include "stdafx.h"
#include "Emulator.h"




bool Emulator::insertMemory(int loc, int contents)
{
	// If Memory exists at this location, Do Not add new memory
	if (!m_memory[loc])
	{
		// If not, check location within bounds
		if (loc < MEMSZ)
		{
			// If true, add new memory
			m_memory[loc] = contents;
			return true;
		}
	}
	//If false, record insertion error
	return false;
}



bool Emulator::runProgram()
{
	cout << "\n\nPress Enter to Continue . . ." << endl;
	cin.ignore();
	cout << "\nResults from emulating program:\n" << endl;


	int input = 0;
	int memLoc = 0;
	
	while (memLoc < m_memory.size())
	{
		int contents = m_memory[memLoc];
		int NumOpCode = contents / MEMSZ;
		int location = contents % MEMSZ;
		
		if (NumOpCode != 0)
		switch (NumOpCode)
		{
			case 1:
				m_acc += m_memory[location];
				break;
			case 2:
				m_acc -= m_memory[location];
				break;
			case 3:
				m_acc *= m_memory[location];
				break;
			case 4:
				m_acc /= m_memory[location];
				break;
			case 5:
				m_acc = m_memory[location];
				break;
			case 6:
				m_memory[location] = m_acc;
				break;
			case 7:
				cout << "? ";
				cin >> input;
				m_memory[location] = input % MEMSZ;
				break;
			case 8: 
				cout << m_memory[location] << endl;
				break;
			case 9:
				memLoc = location;
				continue;
			case 10:
				if (m_acc < 0)
				{
					memLoc = location;
					continue;
				}
				break;
			case 11:
				if (m_acc == 0)
				{
					memLoc = location;
					continue;
				}
				break;
			case 12: 
				if (m_acc > 0)
				{
					memLoc = location;
					continue;
				}
				break;
			case 13:
				return true;
			default:
				return false;
		}
		memLoc++;
	}
	return false;
}


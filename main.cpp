#include "simulator.hpp"
int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "Required argument: file_name\n./MIPS_interpreter <file name>\n";
		return 0;
	}
	std::ifstream file(argv[1]);
	Simulator *mips;
	if (file.is_open())
		mips = new Simulator(file,2);
	else
	{
		std::cerr << "File could not be opened. Terminating...\n";
		return 0;
	}
	mips->executeCommands();
	return 0;
}

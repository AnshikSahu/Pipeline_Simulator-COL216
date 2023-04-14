#include "simulator.hpp"
int main(int argc, char *argv[])
{
	// if (argc != 2)
	// {
	// 	std::cerr << "Required argument: file_name\n./MIPS_interpreter <file name>\n";
	// 	return 0;
	// }
	std::ifstream file("sample.asm");
	Simulator *mips;
	if (file.is_open())
		mips = new Simulator(file,1);
	else
	{
		std::cerr << "File could not be opened. Terminating...\n";
		return 0;
	}
	mips->executeCommands();
	// vector<int> reg(32,0);
	// unordered_map<int,int> memDelta;
	// vector<int> temp=mips->memoryupdatequeue->peek();
	// 		if(temp[2]<=220){
	// 			memDelta[temp[1]]=temp[2];
	// 			mips->memoryupdatequeue->move_left();
	// 		}
	// mips->printRegistersAndMemoryDelta(1,mips->pipeline->registerfile->state_at_time(220,reg,0),memDelta);
	//mips->pipeline->print_table();
	return 0;
}

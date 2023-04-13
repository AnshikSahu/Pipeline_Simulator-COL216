#include "simulator.hpp"
void print_table(Pipeline* pipeline){
	for(int i=0; i<(int)pipeline->history.size(); i++){
		int x= pipeline->history[i]->stages[0][0];
		int j=x/220;
		for(;j>0;j--){
			std::cout<<"  ";
		}
		for(int k=0; k<(int)pipeline->history[i]->stages.size(); k++){
			std::cout<<pipeline->history[i]->stagenames[k]<<"";
			int y= pipeline->history[i]->stages[k][1]-pipeline->history[i]->stages[k][0]-220;
			int l=y/220;
			for(;l>0;l--){
				std::cout<<"* ";
			}
		}
		std::cout<<""<<endl;
	}
}
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
		mips = new Simulator(file,1);
	else
	{
		std::cerr << "File could not be opened. Terminating...\n";
		return 0;
	}
	
	mips->executeCommands();
	print_table(mips->pipeline);
	return 0;
}
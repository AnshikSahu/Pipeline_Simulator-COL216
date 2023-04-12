#include "parser.hpp"
#include "requirements.hpp"

using namespace std;

int main(){
    ifstream file("sample.asm");
    struct MIPS_Architecture* parser = new MIPS_Architecture(file,1);
    vector<struct Command*> commands = parser->parametric_commands;
    for(int i = 0; i < (int)commands.size(); i++){
        commands[i]->print_command();
    }
    struct Pipeline* pipeline = new Pipeline(5,false,true,32,0,{"IF","ID","EX","MEM","WB"});
    cout << "Pipeline:" << endl;
    for(int i = 0; i < (int)commands.size(); i++){
        Runtimedata* runtime=pipeline->run_command(commands[i]);
        pipeline->save();
    }
    pipeline->print_pipeline();
    return 0;
}
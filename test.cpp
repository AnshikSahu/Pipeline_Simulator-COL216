#include "parser.hpp"
#include "requirements.hpp"

using namespace std;

int main(){
    ifstream file("sample.asm");
    struct MIPS_Architecture* parser = new MIPS_Architecture(file,1);
    vector<struct Command*> commands = parser->parametric_commands;
    for(int i = 0; i < (int)commands.size(); i++){
        print_command(commands[i]);
    }
    struct Pipeline* pipeline = new Pipeline(5,false,true,32,0,{"IF","ID","EX","MEM","WB"});
    cout << "Pipeline:" << endl;
    for(int i = 0; i < (int)commands.size(); i++){
        Runtimedata* runtime=run_command(pipeline,commands[i]);
        save(pipeline);
    }
    print_pipeline(pipeline);
    return 0;
}
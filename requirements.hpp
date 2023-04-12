#ifndef COMMAND_H
#define COMMAND_H
#include <stdlib.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <iostream>
using namespace std;
struct Command{
    public:
    bool intermediatelatchesactive;
    int intermediatelatchlength;
    int numberofstages;
    vector<int> stagelengths;
    int bypassindex;
    int readindex;
    int writeindex;
    int destinationregister;
    int sourceregister1;
    int sourceregister2;
    vector<string> stagenames;
    string opcode;
    int value;
    int constant;
    Command(bool in1, int in2, vector<int> in3, vector<int> in4, vector<int> in5, vector<string> in6, string in7, int in8, int in9) {
        intermediatelatchesactive = in1;
        intermediatelatchlength = in2;
        numberofstages = (int)in3.size();
        stagelengths = in3;
        bypassindex = in4[0];
        readindex = in4[1];
        writeindex = in4[2];
        destinationregister = in5[0];
        sourceregister1 = in5[1];
        sourceregister2 = in5[2];
        stagenames = in6;
        opcode = in7;
        value = in8;
        constant = in9;
    }
    void print_command() {
    cout << "intermediatelatchesactive: " << intermediatelatchesactive << endl;
    cout << "intermediatelatchlength: " << intermediatelatchlength << endl;
    cout << "numberofstages: " << numberofstages << endl;
    cout << "stagelengths: ";
    for (int i = 0; i < (int)stagelengths.size(); i++) {
        cout << stagelengths[i] << " ";
    }
    cout << endl;
    cout << "bypassindex: " << bypassindex << endl;
    cout << "readindex: " << readindex << endl;
    cout << "writeindex: " << writeindex << endl;
    cout << "destinationregister: " << destinationregister << endl;
    cout << "sourceregister1: " << sourceregister1 << endl;
    cout << "sourceregister2: " << sourceregister2 << endl;
    cout << "stagenames: ";
    for (int i = 0; i < (int)stagenames.size(); i++) {
        cout << stagenames[i] << " ";
    }
    cout << endl;
    cout << "opcode: " << opcode << endl;
    cout << "value: " << value << endl;
    cout << "constant: " << constant << endl;
}
};
struct Registerfile{
    int size;
    vector<int> updatetime;
    vector<int> intermediateupdatetime;
    vector<int> values;
    Registerfile(int in1){
        size = in1;
        vector<int> temp1(in1, 0),temp2(in1,0),temp3(in1,0);
        updatetime.resize(size);
        updatetime = temp1;
        intermediateupdatetime.resize(size);
        intermediateupdatetime = temp2;
        values.resize(size);
        values = temp3;
    }
    struct vector<int> copy_vector(vector<int> in1) {
    struct vector<int> newvector(in1.size());
    for (int i = 0; i < (int)in1.size(); i++) {
        newvector[i] = in1[i];
    }
    return newvector;
    }
    struct Registerfile* copy_file() {
    struct Registerfile* newregisterfile = new Registerfile(size);
    newregisterfile->size = size;
    newregisterfile->updatetime = copy_vector(updatetime);
    newregisterfile->intermediateupdatetime = copy_vector(intermediateupdatetime);
    newregisterfile->values = copy_vector(values);
    return newregisterfile;
    }
};
struct Runtimedata{
    Command* command;
    int starttime;
    vector<string> stagenames;
    vector<vector<int> > stages;

    Runtimedata(Command* in1, int in2, int in3) {
        command = in1;
        starttime = in2;
        stagenames.resize(in3);
        vector<string> temp3(in3,"");
        stagenames = temp3;
        stages.resize(in3);
        vector<vector<int> > temp1(in3);
        for (int i = 0; i < in3; i++) {
            vector<int> temp2(2,0);
            temp1[i] = temp2;
        }
        stages = temp1;
    }
    void print_runtime() {
    for (int i = 0; i < (int)stagenames.size(); i++) {
        cout << stagenames[i] << " " << stages[i][0] << " " << stages[i][1] << endl;
    }
    cout << endl;
    }
};
struct Pipeline{
    int numberofstages;
    bool bypassactive;
    bool symmetryactive;
    vector<int> stageemptytime;
    vector<int> pseudostageemptytime;
    int numberofregisters;
    struct Registerfile* registerfile;
    struct Registerfile* pseudoregisterfile;
    vector<Runtimedata*> pseudoruntimelist;
    vector<Runtimedata*> history;
    int starttime;
    unordered_map<string, int> stagemap;

    Pipeline(int in1, bool in2, bool in3, int in4, int in5, vector<string> in6) {
        numberofstages = in1;
        bypassactive = in2;
        symmetryactive = in3;
        numberofregisters = in4;
        registerfile = new Registerfile(in4);
        pseudoregisterfile = new Registerfile(in4);
        stageemptytime.resize(in1);
        vector<int> temp1(in1,0);
        stageemptytime = temp1;
        pseudostageemptytime.resize(in1);
        vector<int> temp2(in1,0);
        pseudostageemptytime = temp2;
        starttime = in5;
        for (int i = 0; i < (int)in6.size(); i++) {
            stagemap[in6[i]] = i;
        }
        history.resize(0);
        pseudoruntimelist.resize(0);
    }
    struct Runtimedata* run_command(Command* in1){
    vector<int> v = {in1->destinationregister,in1->sourceregister1,in1->sourceregister2};
    vector<int> v1 = {in1->bypassindex,in1->readindex,in1->writeindex};
    struct Command* command = new Command(false,in1->intermediatelatchlength,in1->stagelengths,v1,v,in1->stagenames,in1->opcode,in1->value,in1->constant);
    if (in1->intermediatelatchesactive){
        for(int i=0;i<(int)command->stagelengths.size()-1;i++){
            command->stagelengths[i]+=command->intermediatelatchlength;
        }
    }
    if (symmetryactive){
        vector<int> temp1(command->numberofstages,*max_element(command->stagelengths.begin(),command->stagelengths.end()));
        command->stagelengths= temp1;
    }
    struct Runtimedata* runtime = new Runtimedata(command,stageemptytime[0],command->numberofstages);
    runtime->stagenames[0]=command->stagenames[0];
    runtime->stages[0]={pseudostageemptytime[stagemap[command->stagenames[0]]],-1};
    for (int j=0;j<command->numberofstages-1;j++){

        int endtime = runtime->stages[j][0]+command->stagelengths[j];
        if (endtime<pseudostageemptytime[stagemap[command->stagenames[j+1]]]){
            endtime= pseudostageemptytime[stagemap[command->stagenames[j+1]]];

        }
        if (j+1==command->bypassindex){
            if (bypassactive){
                if (command->sourceregister1!=-1){
                    if(pseudoregisterfile->intermediateupdatetime[command->sourceregister1]>endtime){
                        endtime=pseudoregisterfile->intermediateupdatetime[command->sourceregister1];
                    }
                }
                if (command->sourceregister2!=-1){
                    if(pseudoregisterfile->intermediateupdatetime[command->sourceregister2]>endtime){
                        endtime=pseudoregisterfile->intermediateupdatetime[command->sourceregister2];
                    }
                }
            }
            else{
                if (command->sourceregister1 != -1){
                    if (pseudoregisterfile->updatetime[command->sourceregister1] > endtime){
                        endtime = pseudoregisterfile->updatetime[command->sourceregister1];
                    }
                }
                if (command->sourceregister2 != -1){
                    if (pseudoregisterfile->updatetime[command->sourceregister2] > endtime){
                        endtime = pseudoregisterfile->updatetime[command->sourceregister2];
                    }
                }              
            }
        }
        if(command->destinationregister!=-1){
            if(j==command->readindex){
                pseudoregisterfile->intermediateupdatetime[command->destinationregister]=endtime;}
                // new next two lines
            if (j==command->writeindex){
                pseudoregisterfile->updatetime[command->destinationregister]=endtime;
            }
        }
        runtime->stages[j][1]=endtime;
        runtime->stagenames[j+1] =command->stagenames[j+1];
        runtime->stages[j+1] = {endtime,-1};
        pseudostageemptytime[stagemap[command->stagenames[j]]]=endtime;
    }
    pseudostageemptytime[stagemap[command->stagenames[command->numberofstages-1]]]=runtime->stages[command->numberofstages-1][0]+command->stagelengths[command->numberofstages-1];
    // next two lines are new
    if(command->destinationregister!=-1){
        if(command->writeindex==command->numberofstages-1){
            pseudoregisterfile->updatetime[command->destinationregister]=pseudostageemptytime[stagemap[command->stagenames[command->numberofstages-1]]];
        }
        if(command->readindex==command->numberofstages-1){
            pseudoregisterfile->intermediateupdatetime[command->destinationregister]=pseudostageemptytime[stagemap[command->stagenames[command->numberofstages-1]]];
        }
        registerfile->values[command->destinationregister]=command->value;
    }
    runtime->stages[command->numberofstages-1][1]=pseudostageemptytime[stagemap[command->stagenames[command->numberofstages-1]]];
    pseudoruntimelist.push_back(runtime);
    return runtime;
    }
    void save() {
    history.insert(history.end(), pseudoruntimelist.begin(), pseudoruntimelist.end());
    if(pseudoruntimelist.size() > 0){
        pseudoruntimelist.clear();
    }
    registerfile = pseudoregisterfile->copy_file();
    stageemptytime = pseudoregisterfile->copy_vector(pseudostageemptytime);
    }
    void restore(Pipeline* pipeline ) {
        if(pseudoruntimelist.size() > 0){
            pseudoruntimelist.clear();
        }
        pseudoregisterfile = registerfile->copy_file();
        pseudostageemptytime = registerfile->copy_vector(stageemptytime);
    }
    void insert_halt(Command* command, Pipeline* pipeline) {
        stageemptytime[0] = history.back()->stages[command->readindex][2];
        if(pseudoruntimelist.size() > 0){
            pseudoruntimelist.clear();
        }
        pseudoregisterfile = registerfile->copy_file();
        pseudostageemptytime = registerfile->copy_vector(stageemptytime);
    }
    void print_pipeline() {
        for (int i = 0; i < (int)history.size(); i++) {
            history[i]->print_runtime();
        }
    }
};
#endif
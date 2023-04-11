#include "./requirements.hpp"
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
using namespace std;
// subi
// value has to be made dynamically implied in run, stage reordering
// value contains the value in register
// branch predictor
struct Command* new_Command(bool in1, int in2, vector<int> in3, vector<int> in4, vector<int> in5, vector<string> in6, string in7, int in8, int in9) {
        struct Command* newcommand = (struct Command*) malloc(sizeof( struct Command));
        newcommand->intermediatelatchesactive = in1;
        newcommand->intermediatelatchlength = in2;
        newcommand->numberofstages = (int)in3.size();
        newcommand->stagelengths = in3;
        newcommand->bypassindex = in4[0];
        newcommand->readindex = in4[1];
        newcommand->writeindex = in4[2];
        newcommand->destinationregister = in5[0];
        newcommand->sourceregister1 = in5[1];
        newcommand->sourceregister2 = in5[2];
        newcommand->stagenames = in6;
        newcommand->opcode = in7;
        newcommand->value = in8;
        newcommand->constant = in9;
        return newcommand;
    }

void print_command(Command* in1) {
    cout << "intermediatelatchesactive: " << in1->intermediatelatchesactive << endl;
    cout << "intermediatelatchlength: " << in1->intermediatelatchlength << endl;
    cout << "numberofstages: " << in1->numberofstages << endl;
    cout << "stagelengths: ";
    for (int i = 0; i < (int)in1->stagelengths.size(); i++) {
        cout << in1->stagelengths[i] << " ";
    }
    cout << endl;
    cout << "bypassindex: " << in1->bypassindex << endl;
    cout << "readindex: " << in1->readindex << endl;
    cout << "writeindex: " << in1->writeindex << endl;
    cout << "destinationregister: " << in1->destinationregister << endl;
    cout << "sourceregister1: " << in1->sourceregister1 << endl;
    cout << "sourceregister2: " << in1->sourceregister2 << endl;
    cout << "stagenames: ";
    for (int i = 0; i < (int)in1->stagenames.size(); i++) {
        cout << in1->stagenames[i] << " ";
    }
    cout << endl;
    cout << "opcode: " << in1->opcode << endl;
    cout << "value: " << in1->value << endl;
    cout << "constant: " << in1->constant << endl;
}

struct Registerfile* new_Registerfile(int in1) {
    cout << "size: " << in1 << endl;
    struct Registerfile* newregisterfile = (struct Registerfile*) malloc(sizeof( struct Registerfile));
    newregisterfile->size = in1;
    vector<int> temp1(in1, 0),temp2(in1,0),temp3(in1,0);
    cout << "size: " << newregisterfile->size << endl;
    newregisterfile->updatetime.resize(in1);
    newregisterfile->updatetime = temp1;
    newregisterfile->intermediateupdatetime.resize(in1);
    newregisterfile->intermediateupdatetime = temp2;
    newregisterfile->values.resize(in1);
    newregisterfile->values = temp3;
    cout << "size: " << newregisterfile->size << endl;
    return newregisterfile;
}

struct Runtimedata* new_Runtimedata(Command* in1, int in2, int in3) {
    struct Runtimedata* newruntimedata = (struct Runtimedata*) malloc(sizeof( struct Runtimedata));
    newruntimedata->command = in1;
    newruntimedata->starttime = in2;
    vector<string> temp3(in3,"");
    newruntimedata->stagenames = temp3;
    vector<vector<int> > temp1(in3);
    for (int i = 0; i < in3; i++) {
        vector<int> temp2(2,0);
        temp1[i] = temp2;
    }
    newruntimedata->stages = temp1;
    return newruntimedata;
}

struct vector<int> copy_vector(vector<int> in1) {
    struct vector<int> newvector(in1.size());
    for (int i = 0; i < (int)in1.size(); i++) {
        newvector[i] = in1[i];
    }
    return newvector;
}

struct Registerfile* copy_file(Registerfile* in1) {
    struct Registerfile* newregisterfile = (struct Registerfile*) malloc(sizeof( struct Registerfile));
    newregisterfile->size = in1->size;
    newregisterfile->updatetime = copy_vector(in1->updatetime);
    newregisterfile->intermediateupdatetime = copy_vector(in1->intermediateupdatetime);
    newregisterfile->values = copy_vector(in1->values);
    return newregisterfile;
}

struct Pipeline* new_Pipeline(int in1, bool in2, bool in3, int in4, int in5, vector<string> in6) {
    struct Pipeline* newpipeline = (struct Pipeline*) malloc(sizeof( struct Pipeline));
    newpipeline->numberofstages = in1;
    newpipeline->bypassactive = in2;
    newpipeline->symmetryactive = in3;
    vector<int> temp1(in1, 0), temp2(in1, 0);
    newpipeline->stageemptytime = temp1;
    newpipeline->pseudostageemptytime = temp2;
    newpipeline->numberofregisters = in4;  
    cout << "in4: " << in4 << endl;
    newpipeline->registerfile = new_Registerfile(in4);
    newpipeline->pseudoregisterfile = new_Registerfile(in4);
    cout << "in5: " << in5 << endl;
    newpipeline->pseudoruntimelist = vector<Runtimedata*>();
    newpipeline->history = vector<Runtimedata*>();
    newpipeline->starttime = in5;
    std::unordered_map<std::string, int> stringIndexMap;
    int index = 0;
    for (const auto& str : in6) {
        stringIndexMap[str] = index;
        index++;
    }
    newpipeline->stagemap= stringIndexMap;
    return newpipeline;
}

struct Runtimedata* run_command(Pipeline* pipeline, Command* in1){
    vector<int> v = {in1->destinationregister,in1->sourceregister1,in1->sourceregister2};
    vector<int> v1 = {in1->bypassindex,in1->readindex,in1->writeindex};
    struct Command* command = (struct Command*) new_Command(false,in1->intermediatelatchlength,in1->stagelengths,v1,v,in1->stagenames,in1->opcode,in1->value,in1->constant);
    if (in1->intermediatelatchesactive){
        int x = command->intermediatelatchlength;
        for (int j=0; j<command->numberofstages; j++){
            x+=command->stagelengths[j];
        }
        command->stagelengths= {x};
        command->stagelengths[command->numberofstages-1]=command->stagelengths[command->numberofstages-1]-command->intermediatelatchlength;
    }
    if (pipeline->symmetryactive){
        vector<int> temp1(command->numberofstages,*max_element(command->stagelengths.begin(),command->stagelengths.end()));
        command->stagelengths= temp1;
    }
    struct Runtimedata* runtime = (struct Runtimedata*) new_Runtimedata(command,pipeline->stageemptytime[0],command->numberofstages);
    runtime->stagenames[0]=command->stagenames[0];
    runtime->stages[0]={pipeline->pseudostageemptytime[pipeline->stagemap[command->stagenames[0]]],-1};
    for (int j=0;j<command->numberofstages-1;j++){

        int endtime = runtime->stages[j][0]+command->stagelengths[j];
        if (endtime<pipeline->pseudostageemptytime[pipeline->stagemap[command->stagenames[j+1]]]){
            endtime= pipeline->pseudostageemptytime[pipeline->stagemap[command->stagenames[j+1]]];

        }
        if (j+1==command->bypassindex){
            if (pipeline->bypassactive){
                if (command->sourceregister1!=-1){
                    if(pipeline->pseudoregisterfile->intermediateupdatetime[command->sourceregister1]>endtime){
                        endtime=pipeline->pseudoregisterfile->intermediateupdatetime[command->sourceregister1];
                    }
                }
                if (command->sourceregister2!=-1){
                    if(pipeline->pseudoregisterfile->intermediateupdatetime[command->sourceregister2]>endtime){
                        endtime=pipeline->pseudoregisterfile->intermediateupdatetime[command->sourceregister2];
                    }
                }
            }
            else{
                if (command->sourceregister1 != -1){
                    if (pipeline->pseudoregisterfile->updatetime[command->sourceregister1] > endtime){
                        endtime = pipeline->pseudoregisterfile->updatetime[command->sourceregister1];
                    }
                }
                if (command->sourceregister2 != -1){
                    if (pipeline->pseudoregisterfile->updatetime[command->sourceregister2] > endtime){
                        endtime = pipeline->pseudoregisterfile->updatetime[command->sourceregister2];
                    }
                }              
            }
        }
        if(command->destinationregister!=-1){
            if(j==command->readindex){
                pipeline->pseudoregisterfile->intermediateupdatetime[command->destinationregister]=endtime;}
                // new next two lines
            if (j==command->writeindex){
                pipeline->pseudoregisterfile->updatetime[command->destinationregister]=endtime;
            }
        }
        runtime->stages[j][1]=endtime;
        runtime->stagenames[j+1] =command->stagenames[j+1];
        runtime->stages[j+1] = {endtime,-1};
        pipeline->pseudostageemptytime[pipeline->stagemap[command->stagenames[j]]]=endtime;
        }
    pipeline->pseudostageemptytime[pipeline->stagemap[command->stagenames[command->numberofstages-1]]]=runtime->stages[command->numberofstages-1][1]+command->stagelengths[command->numberofstages-1];
    // next two lines are new
    if(command->destinationregister!=-1){
        if(command->writeindex==command->numberofstages-1){
            pipeline->pseudoregisterfile->updatetime[command->destinationregister]=pipeline->pseudostageemptytime[pipeline->stagemap[command->stagenames[command->numberofstages-1]]];
        }
        if(command->readindex==command->numberofstages-1){
            pipeline->pseudoregisterfile->intermediateupdatetime[command->destinationregister]=pipeline->pseudostageemptytime[pipeline->stagemap[command->stagenames[command->numberofstages-1]]];
        }
        pipeline->registerfile->values[command->destinationregister]=command->value;
    }
    runtime->stages[command->numberofstages-1][2]=pipeline->pseudostageemptytime[pipeline->stagemap[command->stagenames[command->numberofstages-1]]];
    pipeline->pseudoruntimelist.push_back(runtime);
    return runtime;
}

void save(Pipeline* pipeline ) {
    pipeline->history.insert(pipeline->history.end(), pipeline->pseudoruntimelist.begin(), pipeline->pseudoruntimelist.end());
    pipeline->pseudoruntimelist.clear();
    pipeline->registerfile = copy_file(pipeline->pseudoregisterfile);
    pipeline->stageemptytime = copy_vector(pipeline->pseudostageemptytime);
}

void restore(Pipeline* pipeline ) {
    pipeline->pseudoruntimelist.clear();
    pipeline->pseudoregisterfile = copy_file(pipeline->registerfile);
    pipeline->pseudostageemptytime = copy_vector(pipeline->stageemptytime);
}

void insert_halt(Command* command, Pipeline* pipeline) {
    pipeline->stageemptytime[0] = pipeline->history.back()->stages[command->readindex][2];
    pipeline->pseudoruntimelist.clear();
    pipeline->pseudoregisterfile = copy_file(pipeline->registerfile);
    pipeline->pseudostageemptytime = copy_vector(pipeline->stageemptytime);
}

void print_runtime(Runtimedata* runtime) {
    for (int i = 0; i < (int)runtime->stagenames.size(); i++) {
        cout << runtime->stagenames[i] << " " << runtime->stages[i][0] << " " << runtime->stages[i][1] << endl;
    }
    cout << endl;
}

void print_pipeline(Pipeline* pipeline) {
    for (int i = 0; i < (int)pipeline->history.size(); i++) {
        print_runtime(pipeline->history[i]);
    }
}

// Path: requirements.h

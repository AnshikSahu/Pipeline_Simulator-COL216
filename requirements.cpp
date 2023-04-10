#include "./requirements.hpp"
#include <stdlib.h>
#include <vector>
#include <string>
using namespace std;

struct Command* new_Command(bool in1, int in2, vector<int> in3, vector<int> in4, vector<int> in5, vector<string> in6, string in7, int in8, int in9) {
        struct Command* newcommand = (struct Command*) malloc(sizeof( struct Command));
        newcommand->intermediatelatchesactive = in1;
        newcommand->intermediatelatchlength = in2;
        newcommand->numberofstages = in3.size();
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

struct Registerfile* new_Registerfile(int in1) {
    struct Registerfile* newregisterfile = (struct Registerfile*) malloc(sizeof( struct Registerfile));
    newregisterfile->size = in1;
    newregisterfile->updatetime = vector<int>(in1,0);
    newregisterfile->intermediateupdatetime = vector<int>(in1,0);
    newregisterfile->value = vector<int>(in1,0);
    return newregisterfile;
}

struct Runtimedata* new_Runtimedata(Command* in1, int in2, int in3) {
    struct Runtimedata* newruntimedata = (struct Runtimedata*) malloc(sizeof( struct Runtimedata));
    newruntimedata->command = in1;
    newruntimedata->starttime = in2;
    newruntimedata->stagenames = vector<string>(in3,"");
    newruntimedata->stages = vector<vector<int> >(in3, vector<int>(2,0));
    return newruntimedata;
}

struct vector<int> copy_vector(vector<int> in1) {
    struct vector<int> newvector = vector<int>(in1.size());
    for (int i = 0; i < in1.size(); i++) {
        newvector[i] = in1[i];
    }
    return newvector;
}

struct Registerfile* copy_file(Registerfile* in1) {
    struct Registerfile* newregisterfile = (struct Registerfile*) malloc(sizeof( struct Registerfile));
    newregisterfile->size = in1->size;
    newregisterfile->updatetime = copy_vector(in1->updatetime);
    newregisterfile->intermediateupdatetime = copy_vector(in1->intermediateupdatetime);
    newregisterfile->value = copy_vector(in1->value);
    return newregisterfile;
}

struct Pipeline* new_Pipeline(int in1, bool in2, bool in3, int in4, int in5) {
    struct Pipeline* newpipeline = (struct Pipeline*) malloc(sizeof( struct Pipeline));
    newpipeline->numberofstages = in1;
    newpipeline->bypassactive = in2;
    newpipeline->symmetryactive = in3;
    newpipeline->stageemptytime = vector<int>(in1,0);
    newpipeline->pseudostageemptytime = vector<int>(in1,0); 
    newpipeline->numberofregisters = in4;  
    newpipeline->registerfile = new_Registerfile(in4);
    newpipeline->pseudoregisterfile = new_Registerfile(in4);
    newpipeline->pseudoruntimelist = vector<Runtimedata*>();
    newpipeline->history = vector<Runtimedata*>();
    newpipeline->starttime = in5;
    return newpipeline;
}

struct Runtimedata* run_command(Pipeline* pipeline, Command* in1){
    vector<int> v = {in1->destinationregister,in1->sourceregister1,in1->sourceregister2};
    vector<int> v1 = {in1->bypassindex,in1->readindex,in1->writeindex};
    struct Command* command = (struct Command*) new_Command(false,in1->intermediatelatchlength,in1->stagelengths,v1,v,in1->stagenames,in1->opcode,in1->value,in1->constant);
    if (in1->intermediatelatchesactive){
        int x = command->intermediatelatchlength;
        for (int j=0; j<command->stagelengths.size(); j++){
            x+=command->stagelengths[j];
        }
        command->stagelengths= {x};
        command->stagelengths[command->numberofstages-1]=command->stagelengths[command->numberofstages-1]-command->intermediatelatchlength;
    }
    if (pipeline->symmetryactive){
        command->stagelengths= vector<int>(command->numberofstages,*max_element(command->stagelengths.begin(),command->stagelengths.end()));
    }
    struct Runtimedata* runtime = (struct Runtimedata*) new_Runtimedata(command,pipeline->stageemptytime[0],command->numberofstages);
    runtime->stagenames[0]=command->stagenames[0];
    runtime->stages[0]={pipeline->stageemptytime[0],-1};
    for (int j=0;j<command->numberofstages-1;j++){

        int endtime = runtime->stages[j][1]+command->stagelengths[j];
        if (endtime<pipeline->stageemptytime[j+1]){
            endtime= pipeline->stageemptytime[j+1];

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
        if(j==command->readindex){
            pipeline->pseudoregisterfile->intermediateupdatetime[command->destinationregister]=endtime;
            // new next two lines
                if (j==command->writeindex){
                    pipeline->pseudoregisterfile->updatetime[command->destinationregister]=endtime;
                }
            }
        runtime->stages[j][2]=endtime;
        runtime->stagenames[j+1] =command->stagenames[j+1];
        runtime->stages[j+1] = {endtime,-1};
        pipeline->pseudostageemptytime[j]=endtime;
        }
    pipeline->pseudostageemptytime[command->numberofstages-1]=runtime->stages[command->numberofstages-1][1]+command->stagelengths[command->numberofstages-1];
    // next two lines are new
    if(command->writeindex==command->numberofstages-1){
        pipeline->pseudoregisterfile->updatetime[command->destinationregister]=pipeline->pseudostageemptytime[command->numberofstages-1];
    }
    runtime->stages[command->numberofstages-1][2]=pipeline->pseudostageemptytime[command->numberofstages-1];
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

struct Runtimedata* run(Pipeline* pipeline, Command* command){
}

int main(){
    return 0;
}


// Path: requirements.h

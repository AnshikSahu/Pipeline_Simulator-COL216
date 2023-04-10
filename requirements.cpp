#include "./requirements.hpp"
#include <stdlib.h>
#include <vector>
#include <string>
using namespace std;

struct Command* new_Command(bool in1, int in2, vector<int> in3, vector<int> in4, vector<int> in5, vector<string> in6, int in7, int in8, int in9) {
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
        newcommand->stagenames = in7;
        newcommand->opcode = in8;
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
    newruntimedata->stages = vector<vector<int> >(in3, vector<int>(3,0));
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


// Path: requirements.h

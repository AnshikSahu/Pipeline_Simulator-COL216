#ifndef COMMAND_H
#define COMMAND_H
#include <stdlib.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
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
};

//struct Command* new_Command(bool in1, int in2, vector<int> in3, vector<int> in4, vector<int> in5, vector<string> in6, string in7, int in8, int in9);

void print_command(Command* in1);

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
};

// struct Registerfile* new_Registerfile(int in1);

struct Registerfile* copy_file(Registerfile* in1);

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
};

struct vector<int> copy_vector(vector<int> in1);

// struct Runtimedata* new_Runtimedata(Command* in1, int in2, int in3);

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
};

// struct Pipeline* new_Pipeline(int in1, bool in2, bool in3, int in4, int in5, vector<string> in6);

struct Runtimedata* run_command(Pipeline* pipeline ,Command* in1);

void save(Pipeline* pipeline );

void restore(Pipeline* pipeline );

void insert_halt( Command* command, Pipeline* pipeline );

void print_runtime(Runtimedata* runtime);

void print_pipeline(Pipeline* pipeline);

#endif

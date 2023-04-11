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
};

struct Command* new_Command(bool in1, int in2, vector<int> in3, vector<int> in4, vector<int> in5, vector<string> in6, string in7, int in8, int in9);

struct Registerfile{
    int size;
    vector<int> updatetime;
    vector<int> intermediateupdatetime;
    vector<int> value;
};

struct Registerfile* new_Registerfile(int in1);

struct Registerfile* copy_file(Registerfile* in1);

struct Runtimedata{
    Command* command;
    int starttime;
    vector<string> stagenames;
    vector<vector<int> > stages;
};

struct vector<int> copy_vector(vector<int> in1);

struct Runtimedata* new_Runtimedata(Command* in1, int in2, int in3);

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
};

struct Pipeline* new_Pipeline(int in1, bool in2, bool in3, int in4, int in5, vector<string> in6);

struct Runtimedata* run(Pipeline* pipeline ,Command* in1);

void save(Pipeline* pipeline );

void restore(Pipeline* pipeline );

void insert_halt( Command* command, Pipeline* pipeline );

#endif

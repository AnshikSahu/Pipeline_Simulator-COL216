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
#endif
#ifndef __PIPELINE_H__
#define __PIPELINE_H__
#include <stdlib.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include "requirements.hpp"
using namespace std;
struct Pipeline{

    int numberofstages;
    bool firstinfirstoutactive;
    bool bypassactive;
    bool symmetryactive;
    vector<int> stageemptytime;
    vector<int> pseudostageemptytime;
    vector<int> stagestarttime;
    vector<int> pseudostagestarttime;
    int numberofregisters;
    struct Registerfile* registerfile;
    struct Registerfile* pseudoregisterfile;
    vector<Runtimedata*> pseudoruntimelist;
    vector<Runtimedata*> history;
    int starttime;
    int timetaken;
    unordered_map<string, int> stagemap;
    int cycle;

    Pipeline(int in1, bool in2, bool in3, int in4, int in5, vector<string> in6,bool in7) {
        numberofstages = in1;
        firstinfirstoutactive= in7;
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
        stagestarttime.resize(in1);
        vector<int> temp3(in1,0);
        stagestarttime = temp3;
        pseudostagestarttime.resize(in1);
        vector<int> temp4(in1,0);
        pseudostagestarttime = temp4;
        starttime = in5;
        timetaken = 0;
        for (int i = 0; i < (int)in6.size(); i++) {
            stagemap[in6[i]] = i;
        }
        history.resize(0);
        pseudoruntimelist.resize(0);
        cycle=-1;
    }

    struct Runtimedata* run_command(Command* in1){

    vector<int> v = {in1->destinationregister,in1->sourceregister1,in1->sourceregister2};
    vector<int> v1 = {in1->bypassindex1,in1->bypassindex2,in1->readindex,in1->writeindex};
    struct Command* command = new Command(false,in1->intermediatelatchlength,in1->stagelengths,v1,v,in1->stagenames,in1->opcode,in1->value,in1->constant);
    if (in1->intermediatelatchesactive){
        for(int i=0;i<(int)command->stagelengths.size()-1;i++){
            command->stagelengths[i]+=command->intermediatelatchlength;
        }
    }

    if (symmetryactive){
        vector<int> temp1(command->numberofstages,*max_element(command->stagelengths.begin(),command->stagelengths.end()));
        command->stagelengths= temp1;
        if(cycle==-1){
            cycle=temp1[0];
        }
        else{
            if(cycle!=temp1[0]){
                cycle=-2;
            }
        }
    }

    bool firstinfirstouttemp=firstinfirstoutactive;
    if(command->destinationregister!=-1){
        firstinfirstouttemp=false;
    }
    struct Runtimedata* runtime = new Runtimedata(command,stageemptytime[0],command->numberofstages);
    runtime->stagenames[0]=command->stagenames[0];
    runtime->stages[0]={pseudostageemptytime[stagemap[command->stagenames[0]]],-1};
    pseudostagestarttime[stagemap[command->stagenames[0]]]=pseudostageemptytime[stagemap[command->stagenames[0]]];

    for (int j=0;j<command->numberofstages-1;j++){
        int endtime = runtime->stages[j][0]+command->stagelengths[j];
        if (j+1==command->bypassindex1){
            if (bypassactive){
                if (command->sourceregister1!=-1){
                    if(pseudoregisterfile->intermediateupdatetime[command->sourceregister1]>endtime){
                        endtime=pseudoregisterfile->intermediateupdatetime[command->sourceregister1];
                    }
                }
            }
            else{
                if (command->sourceregister1 != -1){
                    if (pseudoregisterfile->updatetime[command->sourceregister1] > endtime){
                        endtime = pseudoregisterfile->updatetime[command->sourceregister1];
                    }
                }
            }
        }
        if(j+1==command->bypassindex2){
            if(bypassactive){
                if (command->sourceregister2!=-1){
                    if(pseudoregisterfile->intermediateupdatetime[command->sourceregister2]>endtime){
                        endtime=pseudoregisterfile->intermediateupdatetime[command->sourceregister2];
                    }
                }
            }
            else{
                if (command->sourceregister2 != -1){
                    if (pseudoregisterfile->updatetime[command->sourceregister2] > endtime){
                        endtime = pseudoregisterfile->updatetime[command->sourceregister2];
                    }
                }
            }
        }
        if (endtime<pseudostageemptytime[stagemap[command->stagenames[j+1]]] and (endtime>pseudostagestarttime[stagemap[command->stagenames[j+1]]] or firstinfirstouttemp)){
            endtime= pseudostageemptytime[stagemap[command->stagenames[j+1]]];
        }
        if(command->destinationregister!=-1){
            if(j==command->readindex){
                pseudoregisterfile->intermediateupdatetime[command->destinationregister]=endtime;
            }
                // new next two lines
            if (j==command->writeindex){
                pseudoregisterfile->updatetime[command->destinationregister]=endtime;
                pseudoregisterfile->queue->enqueue(command->destinationregister,command->value,endtime);
            }
        }
        runtime->stages[j][1]=endtime;
        runtime->stagenames[j+1] =command->stagenames[j+1];
        runtime->stages[j+1] = {endtime,-1};
        pseudostageemptytime[stagemap[command->stagenames[j]]]=endtime;
        pseudostagestarttime[stagemap[command->stagenames[j+1]]]=endtime;
    }
    pseudostageemptytime[stagemap[command->stagenames[command->numberofstages-1]]]=runtime->stages[command->numberofstages-1][0]+command->stagelengths[command->numberofstages-1];
    if(pseudostageemptytime[stagemap[command->stagenames[command->numberofstages-1]]]>timetaken){
        timetaken=pseudostageemptytime[stagemap[command->stagenames[command->numberofstages-1]]];
    }
    if(command->destinationregister!=-1){
        if(command->writeindex==command->numberofstages-1){
            pseudoregisterfile->updatetime[command->destinationregister]=pseudostageemptytime[stagemap[command->stagenames[command->numberofstages-1]]];
            pseudoregisterfile->queue->enqueue(command->destinationregister,command->value,pseudostageemptytime[stagemap[command->stagenames[command->numberofstages-1]]]);
        }
        if(command->readindex==command->numberofstages-1){
            pseudoregisterfile->intermediateupdatetime[command->destinationregister]=pseudostageemptytime[stagemap[command->stagenames[command->numberofstages-1]]];
        }
        registerfile->values[command->destinationregister]=command->value;
    }
    runtime->stages[command->numberofstages-1][1]=pseudostageemptytime[stagemap[command->stagenames[command->numberofstages-1]]];
    registerfile->values[command->destinationregister]=command->value;
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

    void restore() {
        if(pseudoruntimelist.size() > 0){
            pseudoruntimelist.clear();
        }
        pseudoregisterfile = registerfile->copy_file();
        pseudostageemptytime = registerfile->copy_vector(stageemptytime);
    }

    void insert_halt(Command* com) {
        stageemptytime[0] = history[(int)history.size()-1]->stages[com->readindex][1];
        pseudostageemptytime[0] = history[(int)history.size()-1]->stages[com->readindex][1];
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

    void print_table(){
        if(cycle<0){
            cerr << "Error: Pipeline is too asymetric to print a table" << endl;
        }
        int numcycles=timetaken/cycle;
        cout<<"   ||";
        for (int i = 0; i < numcycles; i++) {
            cout<<i<<"";
            if(i<10){
                cout<<"   ";
            }
            else if(i<100){
                cout<<"  ";
            }
            else if(i<1000){
                cout<<" ";
            }
            cout<<"|";
        }
        cout<<endl;
        for(int i=0; i<(int)history.size(); i++){
            cout<<i<<"";
            if(i<10){
                cout<<"  ";
            }
            else if(i<100){
                cout<<" ";
            }
            cout<<"||";
            int x= history[i]->stages[0][0];
            int j=x/cycle;
            for(;j>0;j--){
                std::cout<<"    |";
            }
            for(int k=0; k<(int)history[i]->stages.size(); k++){
                string name=history[i]->stagenames[k];
                if (name.length() >= 4) {
                    name=name.substr(0, 4);
                } else {
                    name.append(4 - name.length(), ' ');
                }
                std::cout<<name<<"|";
                int y= history[i]->stages[k][1]-history[i]->stages[k][0]-cycle;
                int l=y/cycle;
                for(;l>0;l--){
                    std::cout<<"  * |";
                }
            }
            int z=timetaken-history[i]->stages[(int)history[i]->stages.size()-1][1];
            int m=z/cycle;
            for(;m>0;m--){
                std::cout<<"    |";
            }
            std::cout<<""<<endl;
        }
    }
};

#endif
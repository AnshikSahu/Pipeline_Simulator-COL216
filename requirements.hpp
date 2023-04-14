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
    int bypassindex1;
    int bypassindex2;
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
        bypassindex1 = in4[0];
        bypassindex2 = in4[1];
        readindex = in4[2];
        writeindex = in4[3];
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
    cout << "bypassindex1: " << bypassindex1 << endl;
    cout << "bypassindex2: " << bypassindex2 << endl;
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

struct QueueNode {
    int register_id;
    int value;
    int updatetime;
    QueueNode* next;
    QueueNode* prev;
};

struct UpdateQueue {

    QueueNode* head;
    QueueNode* tail;
    QueueNode* pointer;
    int size;

    UpdateQueue() {
        head = nullptr;
        tail = nullptr;
        pointer = nullptr;
        size = 0;
    }

    void enqueue(int in1,int in2,int in3) {
        QueueNode* newnode = new QueueNode;
        newnode->register_id = in1;
        newnode->value = in2;
        newnode->updatetime = in3;
        newnode->next = nullptr;
        newnode->prev = nullptr;
        if (head == nullptr) {
            head = newnode;
            pointer = newnode;
            tail = newnode;
        }
        else {
            tail->next = newnode;
            newnode->prev = tail;
            tail = newnode;
        }
        size++;
    }

    vector<int> dequeue() {
        if (head == nullptr) {
            return {-1,-1};
        }
        QueueNode* oldhead = head;
        vector<int> data= {oldhead->value,oldhead->register_id};
        head = head->next;
        if(pointer == oldhead){
            pointer = head;
        }
        if (head == nullptr) {
            tail = nullptr;
        }
        delete oldhead;
        return data;
        size--;
    }

    vector<int> peek() {
        if (pointer == nullptr) {
            return {-1,-1,-1};
        }
        vector<int> data = {pointer->value,pointer->register_id,pointer->updatetime};
        return data;
    }

    void move_left() {
        if (pointer == nullptr) {
            return;
        }
        pointer = pointer->prev;
    }

    void move_right() {
        if (pointer == nullptr) {
            return;
        }
        pointer = pointer->next;
    }

    bool is_empty() {
        return head == nullptr;
    }

    struct UpdateQueue* copy_queue() {
        struct UpdateQueue* newqueue = new UpdateQueue();
        QueueNode* temp = head;
        while (temp != nullptr) {
            newqueue->enqueue(temp->register_id, temp->value, temp->updatetime);
            temp = temp->next;
        }
        return newqueue;
    }
};

struct Registerfile{

    int size;
    vector<int> updatetime;
    vector<int> intermediateupdatetime;
    vector<int> values;
    struct UpdateQueue* queue;

    Registerfile(int in1){
        size = in1;
        vector<int> temp1(in1, 0),temp2(in1,0),temp3(in1,0);
        updatetime.resize(size);
        updatetime = temp1;
        intermediateupdatetime.resize(size);
        intermediateupdatetime = temp2;
        values.resize(size);
        values = temp3;
        queue = new UpdateQueue();
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
        newregisterfile->queue = queue->copy_queue();
        return newregisterfile;
    }

    void make_update(int in1,int in2, int in3){
        updatetime[in1] = in3;
        values[in1] = in2;
    }

    vector<int> state_at_time(int in1, vector<int> file, int in2){
        vector<int> newregisterfile = copy_vector(file);
        queue->pointer = queue->head;
        vector<int> temp = queue->peek();
        while(temp[0]!=-1){
            if(temp[2]<=in2){
                queue->move_right();
                temp = queue->peek();
            }
            else if(temp[2]<=in1){
                newregisterfile[temp[1]] = temp[0];
                queue->move_right();
                temp = queue->peek();
            }
            else{
                break;
            }
        }
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
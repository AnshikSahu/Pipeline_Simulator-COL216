#include <fstream>
#include <iostream>
#include "branchpredictor.hpp"
using namespace std;
int main(int argc, char** argv) {

    // Open input file
    ifstream input(argv[1]);

    // Create branch predictor
    int size= (1<<16);
    int start=0;
    SaturatingBHRBranchPredictor bp(start,size);

    // Read branches from file and verify predictions
    uint32_t pc;
    string outcome;
    int count=0;
    int total=0;
    while (input >> hex >> pc >> outcome) {
        bool expected = (outcome == "1");
        bool predicted = bp.predict(pc);
        if (predicted == expected) {
            count+=1;
        }
        total+=1;
        bp.update(pc, expected);
    }
    double fraction = static_cast<double>(count) / total;
    cout<<(100*fraction)<<"%"<<endl;

    return 0;
}
#include <fstream>
#include <iostream>
#include "branchP.cpp"
using namespace std;
int main(int argc, char** argv) {

    // Open input file
    ifstream input(argv[1]);

    // Create branch predictor
    BHRBranchPredictor bp(2);

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
    cout<<(fraction)<<endl;

    return 0;
}
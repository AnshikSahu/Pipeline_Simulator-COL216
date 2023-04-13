#ifndef __BRANCH_PREDICTOR_HPP__
#define __BRANCH_PREDICTOR_HPP__

#include <vector>
#include <bitset>
#include <cassert>
using namespace std;
struct BranchPredictor {
    virtual bool predict(uint32_t pc) = 0;
    virtual void update(uint32_t pc, bool taken) = 0;
};

struct SaturatingBranchPredictor : public BranchPredictor {
    vector<bitset<2>> table; 
    // table = {"10","11"}
    SaturatingBranchPredictor(int value) : table(1 << 14, value) {}

    bool predict(uint32_t pc) {
        // last 14 bits of the branch instruction 
        uint32_t index = pc & ((1 << 14) - 1); 
        bitset<2> value = table[index];
        // branch is taken for 10,11
        return (value[0] == 1);
    }
    void update(uint32_t pc, bool taken) {
        uint32_t index = pc & ((1 << 14) - 1);
        bitset<2> value = table[index];
        if (taken) {
            if (value != bitset<2>("11")) {
                value = value.to_ulong() + 1;
            }
        } else {
            if (value != bitset<2>("00")) {
                value = value.to_ulong() - 1;
            }
        }
        table[index] = value;
    }
};
struct BHRBranchPredictor : public BranchPredictor {
    vector<bitset<2>> bhrTable;
    bitset<2> bhr;
    BHRBranchPredictor(int value) : bhrTable(1 << 2, value), bhr(value) {}
    bool predict(uint32_t pc) {
        // last two bits of the branch history register
        uint32_t index = bhr.to_ulong() & ((1 << 2) - 1);
        bitset<2> value = bhrTable[index];
        // branch is taken for 01,11
        return (value[1] == 1);
    }
    void update(uint32_t pc, bool taken) {
        // shift in the new taken/not-taken bit into the branch history register
        bhr <<= 1;
        bhr[0] = taken;
        // update the corresponding entry in the BHR table
        uint32_t index = bhr.to_ulong() & ((1 << 2) - 1);
        bitset<2> value = bhrTable[index];
        if (value == bitset<2>("11")) {
            if (!taken){
                value=bitset<2>("10");
            }
        } 
        if (value == bitset<2>("00")) {
            if (taken){
                value=bitset<2>("01");
            }
        } 
        if (value == bitset<2>("10"))  {
            if (!taken){
                value=bitset<2>("00");
            }
            else{
                value=bitset<2>("01");
            }
        } else {
            if (!taken){
                value=bitset<2>("10");
            }
            else{
                value=bitset<2>("11");
            }
        }
        bhrTable[index] = value;
    }
};

struct SaturatingBHRBranchPredictor : public BranchPredictor {
    vector<bitset<2>> bhrTable;
    bitset<2> bhr;
    vector<bitset<2>> table;
    vector<bitset<2>> combination;
    SaturatingBHRBranchPredictor(int value, int size) : bhrTable(1 << 2, value), bhr(value), table(1 << 14, value), combination(size, value) {
        assert(size <= (1 << 16));
    }

    bool predict(uint32_t pc) {
        // your code here
        return false;
    }

    void update(uint32_t pc, bool taken) {
        // your code here
    }
};

#endif
#ifndef _BRANCH_PREDICTOR_HPP_
#define _BRANCH_PREDICTOR_HPP_

#include <vector>
#include <bitset>
#include <iostream>
#include <cassert>
using namespace std;
struct BranchPredictor {
    virtual bool predict(uint32_t pc) = 0;
    virtual void update(uint32_t pc, bool taken) = 0;
};

struct SaturatingBranchPredictor : public BranchPredictor {
    vector<bitset<2>> table; 
    SaturatingBranchPredictor(int value) : table(1 << 14, value) {}

    bool predict(uint32_t pc) {
        // last 14 bits of the branch instruction 
        uint32_t index = pc & ((1 << 14) - 1); 
        bitset<2> value = table[index];
        // branch is taken for 10,11
        return (value[1] == 1);
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
// for Saturating Counter
// start = 00; 0.790146. 
// start = 01; 0.839416. 
// start = 10; 0.879562. (best)
// start = 11; 0.866788. 

struct BHRBranchPredictor : public BranchPredictor {
    vector<bitset<2>> bhrTable;
    // having 4 counters, eacg=h mapped to 00, 01, 10, 11
    bitset<2> bhr;
    BHRBranchPredictor(int value) : bhrTable(1 << 2, value), bhr(value) {}
    bool predict(uint32_t pc) {
        // last two bits of the branch history register
        uint32_t index = bhr.to_ulong() ;
        // DISCUSS| (pc & 3); 
        bitset<2> value = bhrTable[index];
        // reached the counter of the specific bhr and then that counter has to be 10 and 11 
        return (value[1] == 1);
    }
    void update(uint32_t pc, bool taken) {
        // shift in the new taken/not-taken bit into the branch history register
        uint32_t index = bhr.to_ulong() ;
        // DISCUSS| (pc & 3); 
        uint32_t value = bhrTable[index].to_ulong();
        if (taken){
            if (bhrTable[index]!=bitset<2>("11")){
                bhrTable[index]=bitset<2>(value+1);
        }
        }
        else{
            if (bhrTable[index]!=bitset<2>("00")){
                bhrTable[index]=bitset<2>(value-1);
        }
        }
        bhr <<= 1;
        bhr[0] = taken;
 
        // update the corresponding entry in the BHR table
        // bhrTable[index] = value;
    }
};
// for BHR
// start = 00; 0.715328. 
// start = 01; 0.722628. 
// start = 10; 0.726277. 
// start = 11; 0.728102. (best)


// Let's try or
struct SaturatingBHRBranchPredictor : public BranchPredictor {
    vector<bitset<2>> bhrTable;
    bitset<2> bhr;
    vector<bitset<2>> table;
    vector<bitset<2>> combination;
    SaturatingBHRBranchPredictor(int value, int size) : bhrTable(1 << 2, value), bhr(value), table(1 << 14, value), combination(size, value) {
        assert(size <= (1 << 16));
        // a table of size 2^14 corresponding to combinatons of bhr = 00,01,10,11
    }

    bool predict(uint32_t pc) {
        uint32_t bhrIndex = bhr.to_ulong() ;
        uint32_t tableIndex = pc & ((1 << 14) - 1);
        uint32_t combinedIndex= bhrIndex | tableIndex;
        bitset<2> combinedEntry= combination[combinedIndex];
        return (combinedEntry).to_ulong() >= 2;
    }

    void update(uint32_t pc, bool taken) {
        uint32_t bhrIndex = bhr.to_ulong() ;

        bhr <<= 1;
        bhr.set(0, taken);

        uint32_t tableIndex = (pc) & ((1 << 14) - 1);
        uint32_t combinedIndex= bhrIndex | tableIndex;
        bitset<2> value = combination[combinedIndex];
        if (taken) {
            if (value != 3) {
                value = value.to_ulong() + 1;
            }
        } else {
            if (value != 0) {
                value = value.to_ulong() - 1;
            }
        }
        combination[combinedIndex] = value;
    }
};
// for BHR
// start = 00; 77.0073%. 
// start = 01; 82.2993%. 
// start = 10; 87.5912%. (best)
// start = 11; 86.1314%. 


// let's have 2^16 counters and with those choose which counter would be used for prediction 
// struct SaturatingBHRBranchPredictor2 : public BranchPredictor {
//     vector<bitset<2>> bhrTable;
//     bitset<2> bhr;
//     vector<bitset<2>> table;
//     vector<bitset<2>> combination;
//     SaturatingBHRBranchPredictor2(int value, int size) : bhrTable(1 << 2, value), bhr(value), table(1 << 14, value), combination(size, value) {
//         assert(size <= (1 << 16));
//         // a table of size 2^14 corresponding to combinatons of bhr = 00,01,10,11
//     }

//     bool predict(uint32_t pc) {
        
//         uint32_t index = pc & ((1 << 14) - 1); 
//         // Get the index into the BHR table using the low 2 bits of the BHR
//         uint32_t bhrIndex = bhr.to_ulong();
//         uint16_t combinedIndex = (index << 2) | bhrIndex;
//         bitset<2> value= combination[combinedIndex];
//         if (value==bitset<2>("00") or value==bitset<2>("01")){
//             value = table[index];
//             return (value[1] == 1);
//         }
//         else{
//             value = bhrTable[bhrIndex];
//             return (value[1] == 1);
//         }

//     }
//     void update(uint32_t pc, bool taken) {
//         uint32_t index = pc & ((1 << 14) - 1); 
//         // Get the index into the BHR table using the low 2 bits of the BHR
//         uint32_t bhrIndex = bhr.to_ulong();
//         uint32_t valueb=bhrTable[bhrIndex].to_ulong();
//         bitset<2> value= combination[(index << 2) | bhrIndex];
//         bitset<2> valuet= table[index];
//         // Update the BHR table with the current BHR value
//         // Shift the BHR to the left and add the taken/not-taken bit to the right
//         bhr <<= 1;
//         bhr.set(0, taken);
        
//         if (taken) {
//             if (bhrTable[bhrIndex]!=bitset<2>("11")){
//                 bhrTable[bhrIndex]=bitset<2>(valueb+1);
//             }
//             if (valuet != bitset<2>("11")) {
//                 valuet = valuet.to_ulong() + 1;
//             }
//             if (value == bitset<2>("10")) {
//                 value = bitset<2>("11");
//             }
//             if (value == bitset<2>("01")){
//                 value = bitset<2>("00");
//             }
//         } else {
//             if (bhrTable[index]!=bitset<2>("00")){
//                 bhrTable[index]=bitset<2>(valueb-1);
//                 }
//             if (valuet != bitset<2>("00")) {
//                 valuet = valuet.to_ulong() - 1;
//             }
//             if (value == bitset<2>("00")) {
//                 value = bitset<2>("01");
//             }
//             if (value == bitset<2>("01")) {
//                 value = bitset<2>("10");
//             }
//             if (value == bitset<2>("10")) {
//                 value = bitset<2>("01");
//             }            
//             if (value == bitset<2>("11")) {
//                 value = bitset<2>("10");
//             } 
//         }
//         table[index] = valuet;
//         combination[(index << 2) | bhrIndex]=value;
//     }
// };
// for BHR
// start = 00; 79.0146%. 
// start = 01; 83.9416%. 
// start = 10; 87.2263%. (best)
// start = 11; 85.7664%. 
#endif
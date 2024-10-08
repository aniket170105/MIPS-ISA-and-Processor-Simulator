#include <bits/stdc++.h>
#include "parseingAndCleaningMIPS.cpp"
#include "convertingInstructionToBinary.cpp"
#include "takingCareOfDataSection.cpp"
#include "executingTheInstructions.cpp"


using namespace std;
/// Assumption we are going to implement a single cycle Processor
// Also we are only working with main cpu and not coprocessor (fpu) because there instruction formata are different and 
// it complicate thinks


const int MEMORY_SIZE = 1e6;
vector <uint8_t> MEMORY(MEMORY_SIZE,0);
int POINTING_TO_MEMORY = 0, END_OF_DATA_SECTION = 0;
// After end of data section will be used to store any variable read from syscall 

int main(){
    cout<<"gg"<<endl;
    auto it = readAndClearCode2("code.txt");
    // vector <string> ans = convertInstructionToBinary(it.second);
    map <string, int> label_address = takingCareOfDataSection(it.first, MEMORY, &POINTING_TO_MEMORY);
    // This same map will be used in .data section.
    // for(auto &x : mp){
    //     cout<<x.first<<" "<<x.second<<endl;
    // }
    // cout<<POINTING_TO_MEMORY<<endl; /// Now after .data we are going to store .text in this
    // POINTING_TO_MEMORY Must be alligned to be a multiple of 4
    // POINTING_TO_MEMORY += (POINTING_TO_MEMORY)%4;
    
    END_OF_DATA_SECTION = POINTING_TO_MEMORY; // Necessary to read something from syscall!!! Actually it is not needed....
    POINTING_TO_MEMORY += 1000;

    while(POINTING_TO_MEMORY%4 != 0){
        POINTING_TO_MEMORY++;
    }
    int STARTING_INSTRUCTION = convertInstructionToBinary(it.second, label_address, MEMORY, &POINTING_TO_MEMORY);

    for(auto &x : label_address){
        cout<<x.first<<" "<<x.second<<endl;
    }

    for(int i=STARTING_INSTRUCTION; i<=124+STARTING_INSTRUCTION; i+=4){
        bitset <32> gg(lw2(i, MEMORY));
        cout<<gg.to_string()<<endl;
    }

    CPU* mycpu = new CPU();

    mycpu->executeInstruction(MEMORY, STARTING_INSTRUCTION);
    cout<<"gg"<<endl;
}
#include <bits/stdc++.h>
#include "miscFunc.cpp"
using namespace std;

// Completed

/// Problems ??
// How exactly am I going to simulate stack, should I use int array or char array --> If i use char array then alignment issuse 
// will not come i.e. Lab 4 in which double needed to be aligned.

// Assumption when implementing jump instruction, we do PC+4+label, but here since i am storing it vector<string> i can 
// also use index to create this offset. But it is not neccessary.

int STARTING_POINT_OF_INSTRUCTIONS;

map <string , string> registerNameToNumberMatching = {{"$zero", "$0"}, {"$at", "$1"}, {"$v0", "$2"}, {"$v1", "$3"},
        {"$a0", "$4"},   {"$a1", "$5"}, {"$a2", "$6"}, {"$a3", "$7"},
        {"$t0", "$8"},   {"$t1", "$9"}, {"$t2", "$10"}, {"$t3", "$11"},
        {"$t4", "$12"},  {"$t5", "$13"}, {"$t6", "$14"}, {"$t7", "$15"},
        {"$s0", "$16"},  {"$s1", "$17"}, {"$s2", "$18"}, {"$s3", "$19"},
        {"$s4", "$20"},  {"$s5", "$21"}, {"$s6", "$22"}, {"$s7", "$23"},
        {"$t8", "$24"},  {"$t9", "$25"}, {"$k0", "$26"}, {"$k1", "$27"},
        {"$gp", "$28"},  {"$sp", "$29"}, {"$fp", "$30"}, {"$ra", "$31"},
        {"$0", "$0"}, {"$1", "$1"}, {"$2", "$2"}, {"$3", "$3"},
        {"$4", "$4"}, {"$5", "$5"}, {"$6", "$6"}, {"$7", "$7"},
        {"$8", "$8"}, {"$9", "$9"}, {"$10", "$10"}, {"$11", "$11"},
        {"$12", "$12"}, {"$13", "$13"}, {"$14", "$14"}, {"$15", "$15"},
        {"$16", "$16"}, {"$17", "$17"}, {"$18", "$18"}, {"$19", "$19"},
        {"$20", "$20"}, {"$21", "$21"}, {"$22", "$22"}, {"$23", "$23"},
        {"$24", "$24"}, {"$25", "$25"}, {"$26", "$26"}, {"$27", "$27"},
        {"$28", "$28"}, {"$29", "$29"}, {"$30", "$30"}, {"$31", "$31"}
    };

map <string, string> registerNumberToBinary = {
        {"$0", "00000"}, {"$1", "00001"}, {"$2", "00010"}, {"$3", "00011"},
        {"$4", "00100"}, {"$5", "00101"}, {"$6", "00110"}, {"$7", "00111"},
        {"$8", "01000"}, {"$9", "01001"}, {"$10", "01010"}, {"$11", "01011"},
        {"$12", "01100"}, {"$13", "01101"}, {"$14", "01110"}, {"$15", "01111"},
        {"$16", "10000"}, {"$17", "10001"}, {"$18", "10010"}, {"$19", "10011"},
        {"$20", "10100"}, {"$21", "10101"}, {"$22", "10110"}, {"$23", "10111"},
        {"$24", "11000"}, {"$25", "11001"}, {"$26", "11010"}, {"$27", "11011"},
        {"$28", "11100"}, {"$29", "11101"}, {"$30", "11110"}, {"$31", "11111"}
    };

const set <string> instructionNameBeingUsed = {"syscall", "add", "sub", "mul", "ori","sll", "slt", "lw", "beq", "addi", "j", "jal", "jr", "li", "lui", "sw"};
map <string, int> instructionSize = {{"add", 1}, {"sub", 1}, {"addi", 1}, {"mul", 1}, {"lw", 1}, {"sw", 1}, {"slt", 1}, {"sll", 1},
{"move", 1}, {"lui", 1}, {"ori", 1}, {"li", 2}, {"beq", 1},{"j", 1}, {"jal", 1}, {"jr", 1},{"syscall", 1}};

// Load word (4 bytes)
uint32_t lw2(uint32_t address, vector <uint8_t> &memory) {
    uint32_t word;
    std::memcpy(&word, &memory[address], sizeof(word));
    return word;
}

// Store word (4 bytes)
void sw2(uint32_t address, uint32_t value, vector <uint8_t> &memory) {
    std::memcpy(&memory[address], &value, sizeof(value));
}

vector<uint32_t> clearAdd(vector <string> &instruction){
    if(instruction.size() != 4){
        cout<<"Error Encoutered"; // Throw error because it's not a valid add instruction
        return {0};
    }
    string opcode, rs, rt, rd, shamt, func, temp;
    opcode = "000000"; func = "100000"; shamt = "00000";
    temp = instruction[3];
    if(registerNameToNumberMatching.find(temp) == registerNameToNumberMatching.end()){
        cout<<"Error Encoutered"<<endl;
        return {0};
    }
    rt = registerNumberToBinary[registerNameToNumberMatching[temp]];

    temp = instruction[2];
    if(registerNameToNumberMatching.find(temp) == registerNameToNumberMatching.end()){
        cout<<"Error Encoutered"<<endl;
        return {0};
    }
    rs = registerNumberToBinary[registerNameToNumberMatching[temp]];

    temp = instruction[1];
    if(registerNameToNumberMatching.find(temp) == registerNameToNumberMatching.end()){
        cout<<"Error Encoutered"<<endl;
        return {0};
    }
    rd = registerNumberToBinary[registerNameToNumberMatching[temp]];    

    bitset <32> answer(opcode+rs+rt+rd+shamt+func);
    return {answer.to_ulong()}; 
}

vector<uint32_t> clearAddi(vector <string> &instruction){
    if(instruction.size() != 4){
        cout<<"Error Encoutered"; // Throw error because it's not a valid add instruction
        return {0};
    }
    string opcode, rs, rt, immediate, temp;
    opcode = "001000";
    int imme_int = stringToInteger(instruction[3]); /// stoi will give error if no number is present in the string
    bitset <16> binary_repr(imme_int);  ///// If number is negative we need to store it in 2's complement
    // So, when we will be implemeting ALU we have to use BITSET for easier sailing

    immediate = binary_repr.to_string();

    temp = instruction[2];
    if(registerNameToNumberMatching.find(temp) == registerNameToNumberMatching.end()){
        cout<<"Error Encoutered"<<endl;
        return {0};
    }
    rs = registerNumberToBinary[registerNameToNumberMatching[temp]];

    temp = instruction[1];
    if(registerNameToNumberMatching.find(temp) == registerNameToNumberMatching.end()){
        cout<<"Error Encoutered"<<endl;
        return {0};
    }
    rt = registerNumberToBinary[registerNameToNumberMatching[temp]];    

    bitset <32> answer(opcode+rs+rt+immediate);
    return {answer.to_ulong()}; 
}

vector<uint32_t> clearSub(vector <string> &instruction){
    if(instruction.size() != 4){
        cout<<"Error Encoutered"; // Throw error because it's not a valid add instruction
        return {0};
    }
    string opcode, rs, rt, rd, shamt, func, temp;
    opcode = "000000"; func = "100010"; shamt = "00000";
    temp = instruction[3];
    if(registerNameToNumberMatching.find(temp) == registerNameToNumberMatching.end()){
        cout<<"Error Encoutered"<<endl;
        return {0};
    }
    rt = registerNumberToBinary[registerNameToNumberMatching[temp]];

    temp = instruction[2];
    if(registerNameToNumberMatching.find(temp) == registerNameToNumberMatching.end()){
        cout<<"Error Encoutered"<<endl;
        return {0};
    }
    rs = registerNumberToBinary[registerNameToNumberMatching[temp]];

    temp = instruction[1];
    if(registerNameToNumberMatching.find(temp) == registerNameToNumberMatching.end()){
        cout<<"Error Encoutered"<<endl;
        return {0};
    }
    rd = registerNumberToBinary[registerNameToNumberMatching[temp]];    

    bitset <32> answer(opcode+rs+rt+rd+shamt+func);
    return {answer.to_ulong()};
}

vector<uint32_t> clearMul(vector <string> &instruction){
    if(instruction.size() != 4){
        cout<<"Error Encoutered"; // Throw error because it's not a valid add instruction
        return {0};
    }
    string opcode, rs, rt, rd, shamt, func, temp;
    opcode = "000000"; func = "000010"; shamt = "00000";
    temp = instruction[3];
    if(registerNameToNumberMatching.find(temp) == registerNameToNumberMatching.end()){
        cout<<"Error Encoutered"<<endl;
        return {0};
    }
    rt = registerNumberToBinary[registerNameToNumberMatching[temp]];

    temp = instruction[2];
    if(registerNameToNumberMatching.find(temp) == registerNameToNumberMatching.end()){
        cout<<"Error Encoutered"<<endl;
        return {0};
    }
    rs = registerNumberToBinary[registerNameToNumberMatching[temp]];

    temp = instruction[1];
    if(registerNameToNumberMatching.find(temp) == registerNameToNumberMatching.end()){
        cout<<"Error Encoutered"<<endl;
        return {0};
    }
    rd = registerNumberToBinary[registerNameToNumberMatching[temp]];    

    bitset <32> answer(opcode+rs+rt+rd+shamt+func);
    return {answer.to_ulong()};
}

vector<uint32_t> clearSlt(vector <string> &instruction){
    if(instruction.size() != 4){
        cout<<"Error Encoutered"; // Throw error because it's not a valid add instruction
        return {0};
    }
    string opcode, rs, rt, rd, shamt, func, temp;
    opcode = "000000"; func = "101010"; shamt = "00000";
    temp = instruction[3];
    if(registerNameToNumberMatching.find(temp) == registerNameToNumberMatching.end()){
        cout<<"Error Encoutered"<<endl;
        return {0};
    }
    rt = registerNumberToBinary[registerNameToNumberMatching[temp]];

    temp = instruction[2];
    if(registerNameToNumberMatching.find(temp) == registerNameToNumberMatching.end()){
        cout<<"Error Encoutered"<<endl;
        return {0};
    }
    rs = registerNumberToBinary[registerNameToNumberMatching[temp]];

    temp = instruction[1];
    if(registerNameToNumberMatching.find(temp) == registerNameToNumberMatching.end()){
        cout<<"Error Encoutered"<<endl;
        return {0};
    }
    rd = registerNumberToBinary[registerNameToNumberMatching[temp]];    

    bitset <32> answer(opcode+rs+rt+rd+shamt+func);
    return {answer.to_ulong()};
}

vector<uint32_t> clearSll(vector <string> &instruction){
    if(instruction.size() != 4){
        cout<<"Error Encoutered"; // Throw error because it's not a valid add instruction
        return {0};
    }
    string opcode, rs, rt, rd, shamt, func, temp;
    opcode = "000000"; func = "000000"; shamt = "00000";
    int imme_int = stringToInteger(instruction[3]);
    // cout<<imme_int<<endl;
    if(imme_int<0 || imme_int > pow(2,5)){
        cout<<"Error Encountered"<<endl;
        return {0};
    }
    bitset <5> imme_bit(imme_int);

    rs = "00000";
    temp = instruction[2];
    if(registerNameToNumberMatching.find(temp) == registerNameToNumberMatching.end()){
        cout<<"Error Encoutered"<<endl;
        return {0};
    }
    rt = registerNumberToBinary[registerNameToNumberMatching[temp]];

    temp = instruction[1];
    if(registerNameToNumberMatching.find(temp) == registerNameToNumberMatching.end()){
        cout<<"Error Encoutered"<<endl;
        return {0};
    }
    rd = registerNumberToBinary[registerNameToNumberMatching[temp]];    

    bitset <32> answer(opcode+rs+rt+rd+imme_bit.to_string()+func);
    return {answer.to_ulong()};
}

vector<uint32_t> clearMove(vector <string> &instruction){
    if(instruction.size() != 3){
        cout<<" Error Encountered"<<endl;
        return {0};
    }
    /// It is a pseudo instruction : move $t1, $t2  == add $t1, $t2, $zero
    vector <string> temp = {"add", instruction[1], instruction[2], "$zero"};
    return clearAdd(temp);
}

vector<uint32_t> clearLui(vector <string> &instruction){
    if(instruction.size() != 3){
        cout<<"Error"<<endl;
        return {0};
    }
    string opcode, rs, rt, immediate, temp;
    opcode = "001111";
    int imme_int = stringToInteger(instruction[2]); /// stoi will give error if no number is present in the string
    bitset <16> binary_repr(imme_int);  ///// If number is negative we need to store it in 2's complement
    // So, when we will be implemeting ALU we have to use BITSET for easier sailing

    immediate = binary_repr.to_string();

    temp = instruction[1];
    if(registerNameToNumberMatching.find(temp) == registerNameToNumberMatching.end()){
        cout<<"Error Encoutered"<<endl;
        return {0};
    }
    rt = registerNumberToBinary[registerNameToNumberMatching[temp]];    

    bitset <32> answer(opcode+"00000"+rt+immediate);
    return {answer.to_ulong()}; 

}

vector<uint32_t> clearOri(vector <string> &instruction){
    if(instruction.size() != 4){
        cout<<"Error"<<endl;
        return {0};
    }
    string opcode, rs, rt, immediate, temp;
    opcode = "001101";
    int imme_int = stringToInteger(instruction[3]); /// stoi will give error if no number is present in the string
    bitset <16> binary_repr(imme_int);  ///// If number is negative we need to store it in 2's complement
    // So, when we will be implemeting ALU we have to use BITSET for easier sailing

    immediate = binary_repr.to_string();

    temp = instruction[2];
    if(registerNameToNumberMatching.find(temp) == registerNameToNumberMatching.end()){
        cout<<"Error Encoutered"<<endl;
        return {0};
    }
    rs = registerNumberToBinary[registerNameToNumberMatching[temp]];  

    temp = instruction[1];
    if(registerNameToNumberMatching.find(temp) == registerNameToNumberMatching.end()){
        cout<<"Error Encoutered"<<endl;
        return {0};
    }
    rt = registerNumberToBinary[registerNameToNumberMatching[temp]];    

    bitset <32> answer(opcode+rs+rt+immediate);
    return {answer.to_ulong()}; 
}

vector<uint32_t> clearLi(vector <string> &instruction){
    // Because it is a pseudo instruction lui and ori
    if(instruction.size() != 3){
        cout<<"Error"<<endl;
        return {0};
    }
    int imme_int = stringToInteger(instruction[2]);
    string rs = instruction[1];
    bitset <16> lower_bit(imme_int);   /// Yes working fine !!!!!!!
    vector <string> inst_for_ori = {"ori", rs, rs, to_string(lower_bit.to_ulong())};
    vector <string> inst_for_lui = {"lui", rs, to_string(imme_int>>16)};

    vector <uint32_t> ans;
    for(auto &x : clearLui(inst_for_lui)){
        ans.push_back(x);
    }
    for(auto &x : clearOri(inst_for_ori)){
        ans.push_back(x);
    }
    return ans;
}

vector<uint32_t> clearLwAndSw(vector <string> &instruction, bool isLw){
    if(instruction.size() != 3){
        cout<<"Error"<<endl;
        return {0};
    }
    string temp = instruction[2], rs, rt;
    int offset;
    stringstream s(temp);
    s >> offset; s >> rs;  // rs = ($rs)
    rs.pop_back(); rs = rs.substr(1); // rs = ($rs
    bitset <16> offset_bit(offset);

    temp = instruction[1];
    if(registerNameToNumberMatching.find(temp) == registerNameToNumberMatching.end()){
        cout<<"Error Encoutered"<<endl;
        return {0};
    }
    rt = registerNumberToBinary[registerNameToNumberMatching[temp]];

    if(registerNameToNumberMatching.find(rs) == registerNameToNumberMatching.end()){
        cout<<"Error Encoutered"<<endl;
        return {0};
    }
    rs = registerNumberToBinary[registerNameToNumberMatching[rs]];

    if(isLw){
        bitset <32> answer("100011" + rs + rt + offset_bit.to_string());
        return {answer.to_ulong()};
    }
    else{
        bitset <32> answer("101011" + rs + rt + offset_bit.to_string());
        return {answer.to_ulong()};
    }

}

vector<uint32_t> clearBeq(vector <string> &instruction, map <string, int> &label_address, int* POINTING_TO_MEMORY_Pointer){
    /*
        PC + 4 + offset*4 = label   -- But i have store it already in index format   // NOT ANYMORE, NOW IT ACTUALLY HOLD ITS ADDRESS

        PC/4 + 1 + offset = label/4 -- Dividing both side by 4

        offset = label_address - instructionNUmber - 1;
    */
    if(instruction.size() != 4){
        cout<<"Error BEQ"<<endl;
        return {0};
    }

    string rt, rs, label, temp;

    temp = instruction[1];
    if(registerNameToNumberMatching.find(temp) == registerNameToNumberMatching.end()){
        cout<<"Error Encoutered"<<endl;
        return {0};
    }
    rs = registerNumberToBinary[registerNameToNumberMatching[temp]];

    temp = instruction[2];
    if(registerNameToNumberMatching.find(temp) == registerNameToNumberMatching.end()){
        cout<<"Error Encoutered"<<endl;
        return {0};
    }
    rt = registerNumberToBinary[registerNameToNumberMatching[temp]];

    label = instruction[3];
    if(label_address.find(label) == label_address.end()){
        cout<<"Error BEQ no such label"<<endl;
        return {0};
    }

    int labelInt = label_address[label], offset;
    
    offset = (labelInt - *POINTING_TO_MEMORY_Pointer - 4)/4;
    bitset <16> inBinary(offset);

    bitset <32> answer("000100" + rs + rt + inBinary.to_string());
    return {answer.to_ulong()};
}

vector<uint32_t> clearJAndJal(vector <string> &instruction, map <string, int> &label_address, bool isJ){
    /*
        j label == "000010" + label_address/4; i.e. 26bit + 4bit i.e. 256 MB jump
        But label is in byte. So, we convert it to word by dividing by 4
        i.e. in ourcase we do not have to do any thing
    */
    if(instruction.size() != 2){
        cout<<"Error J"<<endl;
        return {0};
    }

    string label;

    label = instruction[1];
    if(label_address.find(label) == label_address.end()){
        cout<<"Error J no such label"<<endl;
        return {0};
    }

    int labelInt = label_address[label]/4;
    bitset <26> inBinary(labelInt);

    if(isJ) {
        bitset <32> answer("000010" + inBinary.to_string());
        return {answer.to_ulong()};
    }
    else {
        bitset <32> answer("000011" + inBinary.to_string());
        return {answer.to_ulong()};
    }
}

vector<uint32_t> clearJr(vector <string> &instruction){
    if(instruction.size() != 2){
        cout<<"Error Jr"<<endl;
        return {0};
    }

    string rs, temp;
    temp = instruction[1];
    if(registerNameToNumberMatching.find(temp) == registerNameToNumberMatching.end()){
        cout<<"Error Encoutered"<<endl;
        return {0};
    }
    rs = registerNumberToBinary[registerNameToNumberMatching[temp]];

    bitset <32> answer("000000" + rs + "00000" + "00000" + "00000" + "001000");
    return {answer.to_ulong()};
}

vector<uint32_t> clearSyscall(vector <string> &instruction){
    if(instruction.size() != 1){
        cout<<"Error syscall"<<endl;
        return {0};
    }
    string opcode = "000000";
    bitset <32> answer(opcode + "00000" + "00000" + "00000" + "00000" + "001100");
    return {answer.to_ulong()};
}


vector <uint32_t> convertOneLine(vector <string> &instruction, map <string, int> &label_address, int* POINTING_TO_MEMORY_Pointer){
    // Here this instruction number is just index !!!!!
    if(instructionNameBeingUsed.find(instruction[0]) == instructionNameBeingUsed.end()) {
        cout<<"Error Encoutered No such Instruction"<<endl; /// Throw an error because no instruction with such name
        return {0};
    }
    if(instruction[0] == "add"){
        return clearAdd(instruction);
    }
    else if(instruction[0] == "addi"){
        return clearAddi(instruction);
    }
    else if(instruction[0] == "sub"){
        return clearSub(instruction);
    }
    else if(instruction[0] == "mul"){
        return clearMul(instruction);
    }
    else if(instruction[0] == "slt"){
        return clearSlt(instruction);
    }
    else if(instruction[0] == "sll"){
        return clearSll(instruction);
    }
    else if(instruction[0] == "move"){
        return clearMove(instruction);
    }
    else if(instruction[0] == "lui"){
        return clearLui(instruction);
    }
    else if(instruction[0] == "ori"){
        return clearOri(instruction);
    }
    else if(instruction[0] == "li"){
        return clearLi(instruction);
    }
    else if(instruction[0] == "lw"){
        return clearLwAndSw(instruction, true);
    }
    else if(instruction[0] == "sw"){
        return clearLwAndSw(instruction, false);
    }
    else if(instruction[0] == "beq"){
        return clearBeq(instruction, label_address, POINTING_TO_MEMORY_Pointer);
    }
    else if(instruction[0] == "j"){
        return clearJAndJal(instruction,label_address, true);
    }
    else if(instruction[0] == "jal"){
        return clearJAndJal(instruction, label_address, false);
    }
    else if(instruction[0] == "jr"){
        return clearJr(instruction);
    }
    else if(instruction[0] == "syscall"){
        return clearSyscall(instruction);
    }
    else{
        return {0};
    }

}

int convertInstructionToBinary(vector <vector<string>> &data, map <string, int> &label_address, vector <uint8_t> &MEMORY, int *POINTING_TO_MEMORY_Pointer){
    // Remember that here label_address store it's index i.e. in words not in byte so don't forget to multiply by 4
    // To label each index with there instruction number
    STARTING_POINT_OF_INSTRUCTIONS = *POINTING_TO_MEMORY_Pointer;

    int num_of_instruction = 0; vector <vector <string>> onlyInstructionNoLabels;
    // map <string, int> label_address;
    for(int i=0; i<data.size(); i++){
        if(data[i].size() == 1 && data[i][0] != "syscall"){
            if(label_address.find(data[i][0]) != label_address.end()){
                cout<<"Same Label Name already Used"<<endl;
                return {};
            }
            label_address[data[i][0]] = num_of_instruction*4 + STARTING_POINT_OF_INSTRUCTIONS;
        }
        else{
            if(instructionNameBeingUsed.find(data[i][0]) == instructionNameBeingUsed.end()){
                cout<<"Error Encountered This Instruction is not Allowed : "<<data[i][0]<<endl;
                return {};
            }
            onlyInstructionNoLabels.push_back(data[i]);
            num_of_instruction += instructionSize[data[i][0]];
        }
    }

    for(auto &x : label_address){
        cout<<x.first<<" "<<x.second<<endl;
    }

    for(auto &x : onlyInstructionNoLabels){
        for(auto &y : x){
            cout<<'"'<<y<<'"'<<" ";
        }
        cout<<endl;
    }

    cout<<"Finished Parsing"<<endl;
    flush(cout);

    // vector <string> ans;
    // num_of_instruction = 0;
    for(auto &instruction : onlyInstructionNoLabels){
        for(auto &x : convertOneLine(instruction, label_address, POINTING_TO_MEMORY_Pointer)){
            sw2(*POINTING_TO_MEMORY_Pointer, x, MEMORY);
            *POINTING_TO_MEMORY_Pointer += 4;
            // ans.push_back(x);
        }
        // num_of_instruction += instructionSize[instruction[0]];
    }
    return STARTING_POINT_OF_INSTRUCTIONS;
}

// int main(){
//     vector <vector <string>> temp = {{"lui", "$t8", ""}};
// }
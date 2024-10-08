#include <bits/stdc++.h>
#include <cstdlib>
using namespace std;

// I am also implementing JAL, JR and LI so control signal will look a little different
// https://ocw.mit.edu/courses/6-823-computer-system-architecture-fall-2005/964e8d2c1085754ba5ed2eba48269a0b_l05_singlecycle.pdf
// Tweaking processor to also include $jr, $jal, $lui, $ori and $addi
// 

// I was not able to find any good source which by taking opcode + func and employing combinational circuit 
// generate control signal. So, for my specific use case I will generate control signal accordingly.

class CPU{
private:
    map<string, uint32_t> register_file = {
        {"00000", 0}, {"00001", 0}, {"00010", 0}, {"00011", 0},
        {"00100", 0}, {"00101", 0}, {"00110", 0}, {"00111", 0},
        {"01000", 0}, {"01001", 0}, {"01010", 0}, {"01011", 0},
        {"01100", 0}, {"01101", 0}, {"01110", 0}, {"01111", 0},
        {"10000", 0}, {"10001", 0}, {"10010", 0}, {"10011", 0},
        {"10100", 0}, {"10101", 0}, {"10110", 0}, {"10111", 0},
        {"11000", 0}, {"11001", 0}, {"11010", 0}, {"11011", 0},
        {"11100", 0}, {"11101", 0}, {"11110", 0}, {"11111", 0}
    };

    uint32_t lw(uint32_t address, vector <uint8_t> &MEMORY) {
        uint32_t word;
        std::memcpy(&word, &MEMORY[address], sizeof(word));
        return word;
    }
    /* So if intially stack point to 9000 when we call sw it will store that in 9000 - 9004
    And if we call lw it will load from 9000 - 9004
    */
    // Store word (4 bytes)
    void sw(uint32_t address, uint32_t value, vector <uint8_t> &MEMORY) {
        std::memcpy(&MEMORY[address], &value, sizeof(value));
    }
public:
    CPU(){
        register_file["11101"] = 65000; /// Stack will grow from higher address to lower address. So, intially it will point to higher address
    }

    string generateControlSignal(string &instruction){
        /// [0-1] RegDst, [2-3] PCSrc, [4] Branch, [5] MemRead, [6-7] MemToReg, [8-10] ALUOp, [11] MemWrite, [12] AluSrc, [13] RegWrite
        string opcode = instruction.substr(0,6);
        string funct = instruction.substr(26,6);
        if(opcode == "000000"){
            // R - type instructions
            if(funct == "100000"){
                // Add
                return "10000001000001";
            }
            else if(funct == "100010"){
                // Sub
                return "10000001001001";
            }
            else if(funct == "000010"){
                // mul
                return "10000001010001";
            }
            else if(funct == "101010"){
                // slt
                return "10000001101001";
            }
            else if(funct == "001000"){
                // jr
                return "00100000000000";
            }
            else{
                cout<<"NO such type of R - type instruction"<<endl;
                return "00000000000000";
            }
        }
        else if(opcode == "001101"){
            // ori
            return "00000001011011";
        }
        else if(opcode == "001111"){
            // lui
            return "00000001100011";
        }
        else if(opcode == "100011"){
            // lw
            return "00000100000011";
        }
        else if(opcode == "000100"){
            // beq
            return "00001000000000";
        }
        else if(opcode == "001000"){
            // addi
            return "00000001000011";
        }
        else if(opcode == "000010"){
            // j
            return "00010000000000";
        }
        else if(opcode == "000011"){
            // jal
            return "01010010000001";
        }
        else if(opcode == "101011"){
            // sw
            return "00000000000110";
        }
        else {
            cout<<"No Such instruction found to generate Control signal"<<endl;
            return "00000000000000";
        }

    }

    void executeInstruction(vector <uint8_t> &MEMORY, int STARTING_INSTRUCTION){
        while(lw(STARTING_INSTRUCTION, MEMORY) != 0){
            bitset <32> inst(lw(STARTING_INSTRUCTION, MEMORY));
            string inst_str = inst.to_string();
            if(inst_str.substr(0,6) == "000000" && inst_str.substr(26,6) == "001100"){
                // Code for syscall
                handleSyscall();
                STARTING_INSTRUCTION += 4;
            }
            else{
                // We will take care of the PC in last
                int incremented_PC = STARTING_INSTRUCTION + 4;
                // Instruction Fetch Phase
                string control_signal = generateControlSignal(inst_str);
                /// [0-1] RegDst, [2-3] PCSrc, [4] Branch, [5] MemRead, [6-7] MemToReg, [8-10] ALUOp, [11] MemWrite, [12] AluSrc, [13] RegWrite
                string RegDst = control_signal.substr(0,2), PCSrc = control_signal.substr(2,2), Branch = control_signal.substr(4,1),
                MemRead = control_signal.substr(5,1), MemToReg = control_signal.substr(6,2), ALUOp = control_signal.substr(8,3),
                MemWrite = control_signal.substr(11,1), ALUSrc = control_signal.substr(12,1), RegWrite = control_signal.substr(13,1);
                // Instruction Fetch Phase Done

                // Now Register access
                string ReadRegister1 = inst_str.substr(6,5), ReadRegister2 = inst_str.substr(11,5),
                WriteRegister = writeRegister(ReadRegister2, inst_str.substr(16,5), RegDst);

                uint32_t ReadData1 = register_file[ReadRegister1], ReadData2 = register_file[ReadRegister2],
                signExtended = generateSignExtend(inst_str.substr(16,16)), alu1 = ReadData1, alu2 = aluSecondSource(ReadData2, signExtended, ALUSrc);

                // ALU operations
                pair<bool, int> aluresultREG = aluOperation(alu1, alu2, ALUOp);
                uint32_t readDataMemory = readDataFromMemory(aluresultREG.second, MemRead, MEMORY);

                writeDataToMemory(aluresultREG.second, ReadData2, MemWrite, MEMORY);

                writeToRegister(WriteRegister, whichDataToPassToRegisterWrite(readDataMemory, aluresultREG.second, incremented_PC, MemToReg), RegWrite);
                
                // Now handling PC value
                // PC[31-28] MSB's will always be zero because my MEMORY is not that big;
                string JUMP_26TO28 = shiftLeftTwoONLYFORPC(inst_str.substr(6,26));
                string ccc = "0000" + JUMP_26TO28;
                bitset <32> WHOLE_JUMP_PC(ccc);

                uint32_t co_BEQ_PC4 = choiceBetweenBEQandPC4(incremented_PC, (incremented_PC + (signExtended<<2)), Branch, aluresultREG.first);

                STARTING_INSTRUCTION = finalNextPC(WHOLE_JUMP_PC.to_ulong(), co_BEQ_PC4, ReadData1, PCSrc);
            }
        }
        cout<<"Reached End of Instruction"<<endl;
    }

    uint32_t finalNextPC(int WHOLE_JUMP_PC, int co_BEQ_PC4, int jr, string PCSrc){
        if(PCSrc == "00"){
            return co_BEQ_PC4;
        }
        else if(PCSrc == "01"){
            return WHOLE_JUMP_PC;
        }
        else if(PCSrc == "10"){
            return jr;
        }
        else{
            cout<<"No such PCSrc control Signal found"<<endl;
            terminateStatusReceived();
            return 0;
        }
    }

    uint32_t choiceBetweenBEQandPC4(int incremented_PC, int beq, string Branch, bool zero){
        if(Branch == "1" && zero){
            return beq;
        }
        return incremented_PC;
    }

    void writeToRegister(string writeRegister, uint32_t dataToWrite, string RegWrite){
        if(RegWrite == "0") return;
        register_file[writeRegister] = dataToWrite;
    }

    uint32_t whichDataToPassToRegisterWrite(uint32_t readDataMemory, uint32_t ALUresult, uint32_t incremented_PC, string MemToReg){
        if(MemToReg == "00"){
            return readDataMemory;
        }
        else if(MemToReg == "01"){
            return ALUresult;
        }
        else if(MemToReg == "10"){
            return incremented_PC;
        }
        else{
            cout<<"Unexpected MemToReg control signal"<<endl;
            terminateStatusReceived();
            return 0;
        }
    }

    uint32_t readDataFromMemory(int ALUresult, string MemRead, vector <uint8_t> &MEMORY){
        if(MemRead == "0") return 0;
        return lw(ALUresult, MEMORY);
    }

    void writeDataToMemory(int ALUresult, uint32_t ReadData2, string MemWrite, vector <uint8_t> &MEMORY){
        if(MemWrite == "0") return;
        sw(ALUresult, ReadData2, MEMORY);
    }
    
    pair<bool, int> aluOperation(int a, int b, string ALUOp){
        if(ALUOp == "000"){
            return {a==b, a+b};
        }
        else if(ALUOp == "001"){
            return {a==b, a-b};
        }
        else if(ALUOp == "010"){
            return {a==b, a*b};
        }
        else if(ALUOp == "011"){
            return {a==b, (a|b)};
        }
        else if(ALUOp == "100"){
            return {a==b, a<<2};
        }
        else if(ALUOp == "101"){
            return {a==b, a<b};
        }
        else{
            cout<<"No such ALUop found"<<endl;
            terminateStatusReceived();
            return {0,0};
        }
    }

    uint32_t aluSecondSource(uint32_t ReadData2, uint32_t SignExtend, string ALUSrc){
        if(ALUSrc == "0") return ReadData2;
        else return SignExtend;
    }

    string writeRegister(string inst_20_16, string inst_15_11, string RegDST){
        if(RegDST == "00"){
            return inst_20_16;
        }
        else if(RegDST == "01"){
            return "11111";
        }
        else if(RegDST == "10"){
            return inst_15_11;
        }
        else{
            cout<<"No such control for write register"<<endl;
            terminateStatusReceived();
            return "11111";
        }
    }

    uint32_t generateSignExtend(string str){
        if(str.size() != 16){
            cout<<"Sign extension received length other than 16"<<endl;
            terminateStatusReceived();
        }
        string temp;
        for(int i=0; i<16; i++){
            temp.push_back(str[0]);
        }
        bitset <32> answer(temp+str);
        return answer.to_ulong();
    }

    string shiftLeftTwoONLYFORPC(string str){
        return str + "00";
    }

    void handleSyscall(){
        // Only hande read and write - string and int and terminate program
        // Can not implement read or write string for that we need "la"
        int v0_val = register_file["00010"];  // $v0 = 2
        if(v0_val == 1){
            // print $a0 
            cout<<(int)register_file["00100"];
        }
        else if(v0_val == 5){
            // Read integer and store in $v0
            int temp; cin>>temp;
            register_file["00010"] = temp;
        }
        else if(v0_val == 10){
            terminateStatusReceived();
        }
        else{
            cout<<"We do not support this type of syscall"<<endl;
        }

    }

    void terminateStatusReceived(){
        // What to do ???????
        exit(0);
    }
};

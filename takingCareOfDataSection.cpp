#include <bits/stdc++.h>
// #include "miscFunc.cpp"
using namespace std;

// Problems
// Here Labels should also be allocated address. So, how to do it?? -- One think can be start after .text labels

// In .data section we are only going to include .asciiz, .ascii (No because not safe), .space (To allocate space Static)
using DataType = variant<vector <uint8_t>,string>; 
// std::get<std::string>(
int stringToInteger2(string &str){
    stringstream s(str);
    int ans = 0;
    s >> ans;
    return ans;
}

// Store string
void storeString(uint32_t address, const std::string& str, vector <uint8_t> &memory) {
    if (address + str.size() >= 1e4) {
        std::cerr << "Error: String exceeds memory bounds." << std::endl;
        return;
    }
    std::memcpy(&memory[address], str.c_str(), str.size() + 1); // +1 to include null terminator
}

// Load word (4 bytes)
uint32_t lw(uint32_t address, vector <uint8_t> &memory) {
    uint32_t word;
    std::memcpy(&word, &memory[address], sizeof(word));
    return word;
}

// Store word (4 bytes)
void sw(uint32_t address, uint32_t value, vector <uint8_t> &memory) {
    std::memcpy(&memory[address], &value, sizeof(value));
}

string parseEscapeSequences(string& input) {
    string result;
    for (size_t i = 0; i < input.length(); i++) {
        if (input[i] == '\\' && i + 1 < input.length()) {
            switch (input[i + 1]) {
                case 'n':
                    result += '\n';
                    break;
                case 't':
                    result += '\t';
                    break;
                case '\\':
                    result += '\\';
                    break;
                case '"':
                    result += '"';
                    break;
                case '0':
                    result += '\0';
                    break;
                default:
                    // Unrecognized escape sequence, keep it as is
                    result += input[i];
                    result += input[i + 1];
            }
            i++; // Skip the next character as we've already processed it
        } else {
            result += input[i];
        }
    }
    return result;
}

map <string,int> takingCareOfDataSection(vector <vector <string>> &data, vector <uint8_t> &MEMORY, int* POINTING_TO_MEMORY_Pointer){
    map <string,int> mp; // label, <type, datatype>
    for(auto &x : data){
        if(x.size() != 3){
            cout<<"Error Incomplete Data Section"<<endl;
            return mp;
        }
        if(x[1] == ".asciiz"){
            string temp = x[2];
            if(temp[0] == '"' && temp.back() == '"'){
                temp = temp.substr(1);
                temp.pop_back();

                temp = parseEscapeSequences(temp);

                mp[x[0]] = *POINTING_TO_MEMORY_Pointer;
                storeString(*POINTING_TO_MEMORY_Pointer, temp, MEMORY);
                *POINTING_TO_MEMORY_Pointer += temp.size() + 1;
            }
            else{
                cout<<"Error in .asciiz not found "" correctly "<<endl;
                return mp;
            }
        }
        else if(x[1] == ".word"){
            int word = stringToInteger2(x[2]);
            // cout<<word<<endl;
            mp[x[0]] = *POINTING_TO_MEMORY_Pointer;
            sw(*POINTING_TO_MEMORY_Pointer, word,MEMORY);
            *POINTING_TO_MEMORY_Pointer += 4;
            // cout<<(int)lw(31, MEMORY)<<endl; For testing Purpose how to convert a usig to sig

        }
        else if(x[1] == ".space"){
            int size = stringToInteger2(x[2]);
            vector <uint8_t> temp(size);
            mp[x[0]] = *POINTING_TO_MEMORY_Pointer;    // We will use this as byte space to read and write in this when needed.
            *POINTING_TO_MEMORY_Pointer += size;
        }
        else{
            cout<<"Currently Does Not Support This : "<<x[1]<<endl;
            return mp;
        }
    }
    return mp;
}

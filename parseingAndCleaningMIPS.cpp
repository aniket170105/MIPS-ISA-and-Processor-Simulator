#include <bits/stdc++.h>
using namespace std;

//////// This section is completely done now
/// Some Assumption Instruction will always come in format add $1, $2, $3
//  It cannot be something like add, $t1 ---->>>> Because this is also valid according to MARS.
// But I can make some changes to include this also

vector<string> splitString(string& str, char delimiter) {
    vector<string> result;
    stringstream ss(str);
    string item;
    while (getline(ss, item, delimiter)) {
        if(item != "")result.push_back(item);
    }
    return result;
}

void removeSpaces(string &str){
    while(str.size() && (str.back() == ' ' || str.back() == 9)){ //// Here 9 represent "Tab"
        str.pop_back();
    }
    for(int i=0; i<str.size(); i++){
        if(str[i] != ' ' && str[i] != 9){
            str.erase(str.begin(), str.begin() + i);
            break;
        }
    }
}

vector<string> splitString(string& str) {
    vector<string> result;
    // stringstream ss(str);
    removeSpaces(str);
    string temp1, temp2; // one store .--- and other store it's value
    int i = 0;
    for(; i<str.size(); i++){
        if(str[i] == ' ' || str[i] == '"'){
            break;
        }
    }
    temp1 = str.substr(0, i);
    temp2 = str.substr(i);
    removeSpaces(temp1); removeSpaces(temp2);
    return {temp1, temp2};
}

int doesLabelExist(string &str, char check){
    for(int i=0; i<str.size(); i++){
        if(str[i] == check) return i;
    }
    return -1;
}

string clearString(string temp){
    if(temp.size() == 0) return "";
    if(temp.back() == '\n') temp.pop_back();
    int it = doesLabelExist(temp, '#'); /// To remove any comment from code
    if(it != -1) temp.erase(temp.begin() + it, temp.end());
    // cout<<temp<<endl;
    removeSpaces(temp);
    return temp;
}


pair<string, string> instructionAndFirstRegister(string &str){
    stringstream s(str);
    string instr, regi;
    s >> instr; s >> regi;
    return {instr, regi};
}

void readAndClearCode(string filename){
    fstream newfile;
    newfile.open(filename,ios::in);
    vector <vector <string>> new_code;
    string temp;
    while(getline(newfile,temp)){
        // cout<<temp<<endl;
        // flush(cout);
        string cleaned_string = clearString(temp);
        if(cleaned_string.size()){ //// To check if it contain any instruction But why 1 so '\n' does not get included not needed more
        // No it was not due to that it was due to "Tab"
            // cout<<cleaned_string<<endl;
            // flush(cout);
            int doesLabExist = doesLabelExist(cleaned_string, ':'); /// just checking if label and instruction are in same line
            // cout<<doesLabExist<<endl;
            if(doesLabExist != -1){ 
                string label = cleaned_string.substr(0 , doesLabExist); /// If label exist create a new line
                new_code.push_back({label});
                cleaned_string = cleaned_string.substr(doesLabExist + 1);
            }
            if(cleaned_string.size()){
                vector <string> part_of_instruction = splitString(cleaned_string, ',');
                if(part_of_instruction.size() > 3 || part_of_instruction.size() < 1){
                    //// throw error
                    throw "error";
                    return;
                }
                vector <string> one_instruction;
                auto partone = instructionAndFirstRegister(part_of_instruction[0]);
                one_instruction.push_back(partone.first); one_instruction.push_back(partone.second);
                for(int i=1; i<part_of_instruction.size(); i++){
                    string copy = part_of_instruction[i];
                    removeSpaces(copy);
                    one_instruction.push_back(copy);
                }
                new_code.push_back(one_instruction);
            }
        }
    }
    for(auto &x : new_code){
        for(auto &y : x){
            cout<<y<<" ";
        }
        cout<<endl;
    }
}


vector <vector<string>> readDataSection(int start, int end, vector <string> &data){
    vector <vector <string>> ans;
    for(int i = start; i<end; i++){
        string cleaned_string = clearString(data[i]);
        // cout<<cleaned_string<<endl;
        if(cleaned_string.size()){
            vector <string> temp;
            int doesLabExist = doesLabelExist(cleaned_string, ':');
            if(doesLabExist == -1) throw "Error"; /// Throw error because some sentence exist without any data type
            string label = cleaned_string.substr(0 , doesLabExist);
            cleaned_string = cleaned_string.substr(doesLabExist + 1);

            // We cannot just directly use this say .asciiz "Input an string to check whether it is pali or not : " 
            // It will try to split at all ' ', which is not what we want
            vector <string> splited =  splitString(cleaned_string);
            if(splited.size() != 2) throw "Error";
            temp.push_back(clearString(label));
            for(auto &x : splited){
                temp.push_back(clearString(x));
            }
            ans.push_back(temp);
        }
    }
    return ans;
}


vector <vector<string>> readTextSection(int start, int end, vector <string> &data){
    vector <vector <string>> new_code;
    for(int i = start; i<end; i++){
        string cleaned_string = clearString(data[i]);
        if(cleaned_string.size()){ //// To check if it contain any instruction But why 1 so '\n' does not get included not needed more
        // No it was not due to that it was due to "Tab"
            // cout<<cleaned_string<<endl;
            // flush(cout);
            int doesLabExist = doesLabelExist(cleaned_string, ':'); /// just checking if label and instruction are in same line
            // cout<<doesLabExist<<endl;
            if(doesLabExist != -1){ 
                string label = cleaned_string.substr(0 , doesLabExist); /// If label exist create a new line
                new_code.push_back({label});
                cleaned_string = cleaned_string.substr(doesLabExist + 1);
            }
            if(cleaned_string.size()){
                vector <string> part_of_instruction = splitString(cleaned_string, ',');
                if(part_of_instruction.size() > 3 || part_of_instruction.size() < 1){
                    //// throw error
                    throw "error";
                }
                vector <string> one_instruction;
                auto partone = instructionAndFirstRegister(part_of_instruction[0]);
                if(partone.first.size()) one_instruction.push_back(partone.first); 
                if(partone.second.size()) one_instruction.push_back(partone.second); /// For somethig like add, $1, $2, $3
                for(int i=1; i<part_of_instruction.size(); i++){
                    string copy = part_of_instruction[i];
                    removeSpaces(copy);
                    one_instruction.push_back(copy);
                }
                new_code.push_back(one_instruction);
            }
        }
    }
    return new_code;
}

pair<vector <vector<string>>,vector <vector<string>>> readAndClearCode2(string filename){
    fstream newfile;
    newfile.open(filename,ios::in);
    vector <vector <string>> new_code;
    string str; vector <string> file;
    while(getline(newfile, str)){
        // if(str.back() == '\n') str.pop_back(); /// Handling it in clearString()
        file.push_back(str);
    }
    if(file.size() == 0) return{{{""}},{{""}}};

    vector <vector <string> > text_section, data_section;
    for(int i=0; i<file.size(); i++){
        string temp = clearString(file[i]);
        if(temp == ".data"){
            int j = i;
            while(j<file.size() && clearString(file[j++]) != ".text");
            j--; // Because it will increment one even if we don't want
            data_section = readDataSection(i+1, j, file);
            break;
        }
    }
    for(int i=0; i<file.size(); i++){
        string temp = clearString(file[i]);
        if(temp == ".text"){
            text_section = readTextSection(i+1, file.size(), file);
            break;
        }
    }
    return {data_section, text_section};
    // for(auto &x : text_section){
    //     for(auto &y : x){
    //         cout<<y<<" ";
    //     }
    //     cout<<endl;
    // }
    // for(auto &x : data_section){
    //     for(auto &y : x){
    //         cout<<y<<" ";
    //     }
    //     cout<<endl;
    // }
}
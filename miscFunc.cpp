#include <bits/stdc++.h>
using namespace std;

int stringToInteger(string &str){
    stringstream s(str);
    int ans = 0;
    s >> ans;
    return ans;
}
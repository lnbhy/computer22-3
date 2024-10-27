//#include<string>
//#include<stdbool.h>
//#include<vector>
//#include<iostream>
#include<bits/stdc++.h>
using namespace std;
typedef struct _token {
    string word;
    int type;
 } Token;

int DFA[4][5] = {
    1,2,0,3,3,
    1,1,4,4,4,
    4,2,4,4,4,
    4,4,4,-1,4
 };
vector<Token> Tokens;
string inf = "sum=tmp1*30;";
int getCol(char ch)
{
    if(isalpha(ch))
        return 0;
    else if(isdigit(ch))
        return 1;
    else if(isspace(ch))
        return 2;
    else if(ch=='+' || ch=='-' || ch=='*' || ch=='/' || ch=='=')
        return 3;
    else if (ch==';')
        return 4;
    else
        return -1;
}
bool isover(int inNum)
{
    if(inNum == 4)
        return true;
    else
        return false;
}
bool Analysis() {
    int row = 0, col = 0;
    string tmp;
    int cur = 0;
    Token t;
    while (cur < inf.length()) {
        char ch = inf[cur];
        col = getCol(ch);
        if (col < 0)
            return false;
        row = DFA[row][col];
        if (isover(row)) {
            t.word = tmp;
            if (isalpha(tmp[0]))
                t.type = 1;
            else if (isdigit(tmp[0]))
                t.type = 2;
            else if (tmp[0] == '=')
                t.type = 3;
            else if (tmp[0] == '+')
                t.type = 4;
            else if (tmp[0] == '-')
                t.type = 5;
            else if (tmp[0] == '*')
                t.type = 6;
            else if (tmp[0] == '/')
                t.type = 7;
            Tokens.push_back(t);
            tmp.clear();
            // tmp.append(1, ch);
            row = 0;
            // cur++;
        } else {
            if (!isspace(ch))
                tmp.append(1, ch);
            cur++;
        }
    }
    return true;
}
int main() {
    string tmp;
    Analysis();
    for (int i = 1; i <= Tokens.size(); i++) {
        cout << "(" << Tokens[i - 1].word << "\t" << Tokens[i - 1].type << ")" << endl;
    }
    return 0;
}

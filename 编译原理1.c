/*
第69行代码的文件路径改成自己的文件路径就行
*/
#include <iostream>
#include <string.h>
#include <stdio.h>
using namespace std;
//全局变量
char* inf;
//删除特殊字符
char* DelSpace(char* instr)
{
    static char buf[512] = {0};
    int j = 0;
    memset(buf, 0, sizeof(buf));

    for(int i = 0; i < strlen(instr); i++)
    {
        if(instr[i] != 0x17 && instr[i] != 0x20)
        {
            buf[j] = instr[i];
            j++;
        }
    }

    return buf;
}
//删除注释
char* DelComment(char* instr)
{
    for(int i = 0; i < strlen(instr) - 1; i++)
    {
        if(instr[i] == '/' && instr[i+1] == '/')
        {
            instr[i] = '\0';
            break;
        }
    }
    return instr;
}
//删除换行
char* Del(char*instr)
{
	int i=strlen(instr)-1;
	instr[i]='\0';
	return instr;
 } 
//读取文件
char* GetString(const char* filename)
{
    char buf[512]={0};
    FILE* rdfile= fopen(filename,"r");
    if(rdfile== NULL) return NULL;
    while(fgets(buf, sizeof(buf), rdfile)!= NULL)
    {
        if(strlen(buf)<= 1) //空行不处理
            continue;
        strcat(inf,Del(DelComment(DelSpace(buf)))); // 这里假设inf是一个已经定义的字符串变量
        memset(buf,0,sizeof(buf));
    }
    fclose(rdfile);
    return inf; // 这里假设inf是一个已经定义的字符串变量
}
int main()
{
    // 为全局变量inf分配内存（假设一个合理的大小，例如1024）
    inf = new char[1024];  
    memset(inf, 0, 1024);  // 清空内存
    const char* filename ="1.cpp"; //1.cpp修改成自己的文件路径
    // 调用 GetString 函数，读取文件内容
    char* result = GetString(filename); 
    cout << result << endl; // 输出处理后的字符串

    // 释放分配的内存
    delete[] inf; 

    return 0; // 返回0表示程序正常结束
}
 

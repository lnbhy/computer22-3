/*使用说明：
  第49行测试文件修改成自己的测试文件，或者使用我提供的样例，代码文件和测试文件须在同一目录下*/
#include<windows.h>
#include<fstream>
#include <stdlib.h>
#include<stdio.h>
#include<string.h>
#include<conio.h>
#include<iostream>
using namespace std;
#define MAX_BUFFER_NUM 10 
#define INTE_PER_SEC 1000
#define MAX_THREAD_NUM 64
struct ThreadInfo
{
int serial; //线程序列号
char entity; //是 P 还是 C
double delay; //线程延迟
int thread_request[MAX_THREAD_NUM]; //线程请求队列
int n_request; //请求个数
};
CRITICAL_SECTION PC_Critical[MAX_BUFFER_NUM];
int Buffer_Critical[MAX_BUFFER_NUM]; //缓冲区声明，用于存放产品；
HANDLE h_Thread[MAX_THREAD_NUM]; //用于存储每个线程句柄的数组；
ThreadInfo Thread_Info[MAX_THREAD_NUM]; //线程信息数组；
HANDLE empty_semaphore; //一个信号量；
HANDLE h_mutex; 
DWORD n_Thread = 0; 
DWORD n_Buffer_or_Critical; 
HANDLE h_Semaphore[MAX_THREAD_NUM]; 
void Produce(void *p);
void Consume(void *p);
bool IfInOtherRequest(int);
int FindProducePositon();
int FindBufferPosition(int);
int main(void)
{
DWORD wait_for_all;
ifstream inFile;
for(int i=0;i< MAX_BUFFER_NUM;i++)
Buffer_Critical[i] = -1;
for(int j=0;j<MAX_THREAD_NUM;j++){
for(int k=0;k<MAX_THREAD_NUM;k++)
Thread_Info[j].thread_request[k] = -1;
Thread_Info[j].n_request = 0;
}
for(int i =0;i< MAX_BUFFER_NUM;i++) 
InitializeCriticalSection(&PC_Critical[i]);
inFile.open("1.txt");//修改成自己的测试文件
inFile >> n_Buffer_or_Critical;
inFile.get();
printf("输入文件是:\n");
printf("%d \n",(int) n_Buffer_or_Critical);
while(inFile){
inFile >> Thread_Info[n_Thread].serial;
inFile >> Thread_Info[n_Thread].entity;
inFile >> Thread_Info[n_Thread].delay;
char c;
inFile.get(c);
while(c!='\n'&& !inFile.eof()){
inFile>> 
Thread_Info[n_Thread].thread_request[Thread_Info[n_Thread].n_request++];
inFile.get(c);
}
n_Thread++;
} 

for(int j=0;j<(int) n_Thread;j++){
        int Temp_serial = Thread_Info[j].serial;
        char Temp_entity = Thread_Info[j].entity;
        double Temp_delay = Thread_Info[j].delay;
        printf(" \n thread%2d %c %f ",Temp_serial,Temp_entity,Temp_delay);
        int Temp_request = Thread_Info[j].n_request;
        for(int k=0;k<Temp_request;k++)
            printf(" %d ", Thread_Info[j].thread_request[k]);
        cout<<endl;
}
printf("\n\n");

empty_semaphore=CreateSemaphore(NULL,n_Buffer_or_Critical,n_Buffer_or_Critical, "semaphore_for_empty");
h_mutex = CreateMutex(NULL,FALSE,"mutex_for_update");

for(int j=0;j<(int)n_Thread;j++){ 
std::string lp ="semaphore_for_produce_";
int temp =j;
while(temp){
char c = (char)(temp%10);
lp+=c;
temp/=10;
}
h_Semaphore[j+1]=CreateSemaphore(NULL,0,n_Thread,lp.c_str());
}

for(int i =0;i< (int) n_Thread;i++){
if(Thread_Info[i].entity =='P')
h_Thread[i]= 
CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)(Produce),&(Thread_Info[i]),0,NULL);
else
 
h_Thread[i]=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)(Consume),&(Thread_Info[i]),0,NULL);
}
//主程序等待各个线程的动作结束；
wait_for_all = WaitForMultipleObjects(n_Thread,h_Thread,TRUE,-1);
printf(" \n \nALL Producer and consumer have finished their work. \n");
printf("Press any key to quit!\n");
_getch();
return 0;
}
//确认是否还有对同一产品的消费请求未执行；
bool IfInOtherRequest(int req)
{
for(int i=0;i<n_Thread;i++)
for(int j=0;j<Thread_Info[i].n_request;j++)
if(Thread_Info[i].thread_request[j] == req)
return TRUE;
return FALSE;
}
//找出当前可以进行产品生产的空缓冲区位置；
int FindProducePosition()
{
int EmptyPosition;
for (int i =0;i<n_Buffer_or_Critical;i++)
if(Buffer_Critical[i] == -1){
EmptyPosition = i;
//用下面这个特殊值表示本缓冲区正处于被写状态；
Buffer_Critical[i] = -2;
break;
}
return EmptyPosition;
}
//找出当前所需生产者生产的产品的位置；
int FindBufferPosition(int ProPos)
{
int TempPos=0;
for (int i =0 ;i<n_Buffer_or_Critical;i++)
if(Buffer_Critical[i]==ProPos){
TempPos = i;
break;
}
return TempPos;
}
//生产者进程
void Produce(void *p)
{
//局部变量声明；
DWORD wait_for_semaphore,wait_for_mutex,m_delay;
int m_serial;
//获得本线程的信息；
m_serial = ((ThreadInfo*)(p))->serial;
m_delay = (DWORD)(((ThreadInfo*)(p))->delay *INTE_PER_SEC);
Sleep(m_delay);
//开始请求生产
printf("Producer %2d sends the produce require.\n",m_serial);

wait_for_semaphore = WaitForSingleObject(empty_semaphore,-1);
//互斥访问下一个可用于生产的空临界区，实现写写互斥；
wait_for_mutex = WaitForSingleObject(h_mutex,-1);
int ProducePos = FindProducePosition();
 ReleaseMutex(h_mutex);
//生产者在获得自己的空位置并做上标记后，以下的写操作在生产者之间可以并发；
//核心生产步骤中，程序将生产者的 ID 作为产品编号放入，方便消费者识别;
printf("Producer %2d begin to produce at position %2d.\n",m_serial,ProducePos);
Buffer_Critical[ProducePos] = m_serial;
printf("Producer %2d finish producing :\n ",m_serial);
printf(" position[ %2d ]:%3d \n" ,ProducePos,Buffer_Critical[ProducePos]);
//使生产者写的缓冲区可以被多个消费者使用，实现读写同步；
ReleaseSemaphore(h_Semaphore[m_serial],n_Thread,NULL);
}
//消费者进程
void Consume(void * p)
{
//局部变量声明；
DWORD wait_for_semaphore,m_delay;
int m_serial,m_requestNum; //消费者的序列号和请求的数目；
int m_thread_request[MAX_THREAD_NUM];//本消费线程的请求队列；
//提取本线程的信息到本地；
m_serial = ((ThreadInfo*)(p))->serial;
m_delay = (DWORD)(((ThreadInfo*)(p))->delay *INTE_PER_SEC);
m_requestNum = ((ThreadInfo *)(p))->n_request;
for (int i = 0;i<m_requestNum;i++)
m_thread_request[i] = ((ThreadInfo*)(p))->thread_request[i];
Sleep(m_delay);
//循环进行所需产品的消费
for(int i =0;i<m_requestNum;i++){ 
 //请求消费下一个产品
 printf("Consumer %2d request to consume %2d product\n",m_serial,m_thread_request[i]);

wait_for_semaphore=WaitForSingleObject(h_Semaphore[m_thread_request[i]],-1); 
 //查询所需产品放到缓冲区的号
 int BufferPos=FindBufferPosition(m_thread_request[i]); 

 EnterCriticalSection(&PC_Critical[BufferPos]);
 printf("Consumer%2d begin to consume %2d product \n",m_serial,m_thread_request[i]);
 ((ThreadInfo*)(p))->thread_request[i] =-1;
 if(!IfInOtherRequest(m_thread_request[i])){
Buffer_Critical[BufferPos] = -1;//标记缓冲区为空；
printf("Consumer%2d finish consuming %2d:\n ",m_serial,m_thread_request[i]);
printf(" position[ %2d ]:%3d \n" ,BufferPos,Buffer_Critical[BufferPos]);
ReleaseSemaphore(empty_semaphore,1,NULL);
 }
 else{
printf("Consumer %2d finish consuming product %2d\n ",m_serial,m_thread_request[i]);
 }
//离开临界区来扩大势力发
 LeaveCriticalSection(&PC_Critical[BufferPos]);
}
}

/*测试文件
1.txt
  5
0 P 1
1 C 1 0
2 P 1
3 C 1 0 2
*/


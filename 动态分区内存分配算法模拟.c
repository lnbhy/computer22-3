#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<conio.h>
int memoryStartAddress = -1;
int memorySize = -1;
typedef struct jobList
{
    int id; /* 作业 ID */
    int size; /* 作业大小（需要的存储空间大小） */
    int status;
    /* 作业状态 0 : new job ,1 : in the memory , 2 : finished . */
    struct jobList *next; /* 作业链表指针 */
}jobList;
typedef struct freeList
{
    int startAddress; /* 分区起始地址 */
    int size; /* 分区大小 */
    struct freeList *next; /* 分区链表指针 */
}freeList;
typedef struct usedList 
{
    int startAddress; /* 分区起始地址 */
    int jobID; /* 分区中存放作业 ID */
    struct usedList *next; /* 分区链表指针 */
}usedList;
void errorMessage(void) /*出现严重错误时显示信息并结束程序*/
{
    printf("\n\tError !\a");
    printf("\nPress any key to exit !");
    getch();
    exit(1);
}
void openFile(FILE **fp,char *filename,char *mode) 
 /*以要求的方式打开文件*/
{
    if((*fp = fopen(filename,mode)) == NULL)
    {
        printf("\nCan't open %s in mode %s.",filename,mode);
        errorMessage();
    }
}
void makeFreeNode(struct freeList **empty,int startAddress,int size) 
/*根据参数 startAddress、size 创建空闲节点，由 empty 指针返回*/
{
    if((*empty = malloc(sizeof(struct freeList))) == NULL)
    {
        printf("\nNot enough to allocate for the free node .");
        errorMessage();
    }
    (*empty)->startAddress = startAddress;
    (*empty)->size = size;
    (*empty)->next = NULL;
}
void iniMemory(void) /*初始化存储空间起始地址、大小*/
{
    char MSA[10],MS[10];
    printf("\nPlease input the start address of the memory !");
    scanf("%s",MSA);
    memoryStartAddress = atoi(MSA);
    printf("\nPlease input the size of the memory !");
    scanf("%s",MS);
    memorySize = atoi(MS);
}
char selectFitMethod(void) /*选择适应算法*/
{
    FILE *fp;
    char fitMethod;
    do{
        printf("\n\nPlease input a char as fallow to select the fit method !\
        \n 1 (Best fit) \
        \n 2 (Worst fit) \
        \n 3 (First fit) \
        \n 4 (Last fit)\n");
        fitMethod = getche();
    }while(fitMethod < '1' || fitMethod > '4');
    openFile(&fp,"d:\\result.cl","a");
    switch(fitMethod)
    {
        case '1':
        fprintf(fp,"\n\n\n\n\tBest fit");
        fprintf(fp,"\n**********************************************");
        break;
        case '2':
        fprintf(fp,"\n\n\n\n\tWorst fit");
        
        fprintf(fp,"\n**********************************************");
        break;
        case '3':
        fprintf(fp,"\n\n\n\n\tFirst fit");
        
        fprintf(fp,"\n**********************************************");
        break;
        case '4': fprintf(fp,"\n\n\n\n\tLast fit");
        fprintf(fp,"\n**********************************************");
        break;
    }
    fclose(fp);
    return fitMethod;
}
void inputJob(void) /*从键盘输入作业到 D 盘的 JOB 文件*/
{
int /*id,size, */status = 0,jobnum = 0;
FILE *fp;
char id[10],size[10];
openFile(&fp,"d:\\job.cl","w");
fprintf(fp,"job_ID\tsize\tstatus");
printf("\n\n\n\nPlease input the jobs as fallow !\
\nEnter a integer smaller than 1 to quit .\njob_ID\tsize\n");
do{
/* scanf("%d%d",&id,&size); */
scanf("%s\t%s",id,size);
if(atoi(id) > 0 && atoi(size) > 0)
{
fprintf(fp,"\n%s\t%s\t%d",id,size,status);
/* fprintf(fp,"\n%d\t%d\t%d",id,size,status); */
jobnum++;
}
else break;
}while(1);
if(jobnum)
printf("\nFinished to input the jobs !");
else
{
printf("\nNo job was given .");
errorMessage();
}
fclose(fp);
}
int makeJobList(struct jobList **jobs) 
 /*从 JOB 文件中读出作业并创建作业链表*/
{
char jobID[10],size[10],status[10];
struct jobList *rear;
FILE *fp;
openFile(&fp,"d:\\job.cl","r");
fscanf(fp,"%s%s%s",jobID,size,status);
if((*jobs = malloc(sizeof(struct jobList))) == NULL)
{
printf("\nNot enough to allocate for the job .");
fclose(fp);
errorMessage();
}
rear = *jobs;
(*jobs)->next = NULL;
while(!feof(fp))
{
struct jobList *p;
fscanf(fp,"%s%s%s",jobID,size,status);
if((p = malloc(sizeof(struct jobList))) == NULL)
{
printf("\nNot enough to allocate for the job .");
fclose(fp);
errorMessage();
}
p -> next = rear -> next;
rear -> next = p;
rear = rear -> next;
rear -> id = atoi(jobID);
rear -> size = atoi(size);
rear -> status = atoi(status);
}
fclose(fp);
return 0;
}
int updateJobFile(struct jobList *jobs) /*更新作业链表中作业的状态*/
{
FILE *fp;
struct jobList *p;
openFile(&fp,"d:\\job.cl","w");
fprintf(fp,"job_ID\tsize\tstatus");
for(p = jobs -> next;p;p = p -> next)
fprintf(fp,"\n%d\t%d\t%d",p->id,p->size,p->status);
fclose(fp);
return 0;
}
int showFreeList(struct freeList *empty) /*空闲分区队列显示*/
{
FILE *fp;
struct freeList *p = empty -> next;
int count = 0;
openFile(&fp,"d:\\result.cl","a");
fprintf(fp,"\n\nNow show the free list...");
printf("\n\nNow show the free list...");
if(p)
{
fprintf(fp,"\nnumber\tsize\tstartAddress");
printf("\nnumber\tsize\tstartAddress");
for(;p;p = p -> next)
{
fprintf(fp,"\n%d\t%d\t%d",++count,p -> size,p -> startAddress);
printf("\n%d\t%d\t%d",count,p -> size,p -> startAddress);
}
fclose(fp);
return 1;
}
else
{
fprintf(fp,"\nThe memory was used out !");
printf("\nThe memory was used out !");
 fclose(fp);
return 0;
}
}
void getJobInfo(struct jobList *jobs,int id,int *size,int *status) 
 /*获取作业的信息*/
{
struct jobList *p = jobs->next;
while(p && p->id != id)
p = p->next;
if(p == NULL)
{
printf("\nCan't find the job which id is : %d .",id);
errorMessage();
}
else
{
*size = p -> size;
*status = p -> status;
}
}
void updateJobStatus(struct jobList **jobs,int id,int status)
{
struct jobList *p = (*jobs)->next;
while(p && p->id != id)
p = p->next;
if(p == NULL)
{
printf("\nCan't find the job which id is : %d .",id);
errorMessage();
}
else
p -> status = status;
}
int showUsedList(struct jobList *jobs,struct usedList *used) 
/*作业占用链表显示*/
{
FILE *fp;
struct usedList *p = used -> next;
int count = 0,size,status;
openFile(&fp,"d:\\result.cl","a");
fprintf(fp,"\n\nNow show the used list...");
printf("\n\nNow show the used list...");
if(p)
{
fprintf(fp,"\nnumber\tjobID\tsize\tstartAddress");
printf("\nnumber\tjobID\tsize\tstartAddress");
for(;p;p = p -> next)
{
getJobInfo(jobs,p -> jobID,&size,&status);
fprintf(fp,"\n%d\t%d\t%d\t%d",++count,p->jobID,size,p-> startAddress);
printf("\n%d\t%d\t%d\t%d",count,p->jobID,size,p-> startAddress);
}
fclose(fp);
return 1;
}
else
{
fprintf(fp,"\nNo job in the memory ! You should input some jobs to it.");
printf("\nNo job in the memory ! You should input some jobs to it.");
fclose(fp);
return 0;
}
}
int showJobList(struct jobList *jobs) /*显示作业链表*/
{
struct jobList *p;
p = jobs->next;
if(p == NULL)
{
printf("\nNo job in the list ! Try again next time.");
return 0;
}
printf("\n\nThe job list is as fallow :\njob_ID\tsize\tstatus");
while(p)
{
printf("\n%d\t%d\t%d",p->id,p->size,p->status);
p = p->next;
}
return 1;
}
void moveFragment(struct jobList *jobs,struct freeList **empty,struct usedList **used)
{
int size,status;
struct usedList *p;
int address = memoryStartAddress;
/*全局变量，初始化时分配存储空间始址*/
if((*empty)->next == NULL) /* 空闲分区链表为空，提示并返回 */
{
printf("\nThe memory was used out at all.\nMay be you should finish some jobs first or press any key to try again !");
getch();
return;
}
for(p = (*used) -> next;p;p = p-> next) 
/* 循环的修改占用分区的始址 */
{
p -> startAddress = address;
getJobInfo(jobs,p -> jobID,&size,&status);
/* 由作业 ID 获得作业大小 */
address += size;
}
(*empty)->next->startAddress = address;
/*修改空闲分区的首节点始址、大小*/
(*empty) -> next -> size = memorySize - (address - memoryStartAddress);
(*empty) -> next -> next = NULL; 
/* 删除首节点后的所有节点 */
}
void order(struct freeList **empty,int bySize,int inc)
{
struct freeList *p,*q,*temp;
int startAddress,size;
for(p = (*empty) -> next;p;p = p -> next)
{ /* 按 bySize 和 inc 两个参数寻找合适的节点，用 temp 指向它 */
for(temp = q = p;q;q = q -> next)
{
switch(bySize)
{
case 0 : switch(inc)
{
case 0:if(q->size < temp->size)
temp = q;break;
default:if(q->size > temp->size)
temp = q;break;
} break;
default: switch(inc)
{
case 0:if(q->startAddress < temp->startAddress)
temp = q;break;
default:if(q->startAddress > temp->startAddress)
temp = q;break;
} break;
}
} /* 交换节点的成员值 */ 
if(temp != p)
{ 
startAddress = p->startAddress;
size = p->size;
p->startAddress = temp->startAddress;
p->size = temp->size;
temp->startAddress = startAddress;
temp->size = size;
}
}
}
int allocate(struct freeList **empty,int size) 
/*为作业分配存储空间、状态必须为 0*/
{
struct freeList *p,*prep;
int startAddress = -1;
p = (*empty) -> next;
while(p && p->size < size)
p = p -> next;
if(p != NULL)
{
if(p -> size > size)
{
startAddress = p -> startAddress;
p -> startAddress += size;
p -> size -= size;
}
else
{
 startAddress = p -> startAddress;
prep = *empty;
while(prep -> next != p)
prep = prep -> next;
prep -> next = p -> next;
free(p);
}
}
else printf("\nMay be you should move the fragment together ."); /* Unsuccessful ! 
*/
return startAddress;
}
void insertUsedNode(struct usedList **used,int id,int startAddress) /*插入释放的空间
到 used 链表中（作业号为 id，startAddress 由函数 13 返回）*/
{
struct usedList *q,*r,*prer;
if((q = malloc(sizeof(struct usedList))) == NULL)
{
printf("\nNot enough to allocate for the used node .");
errorMessage();
}
q -> startAddress = startAddress;
q -> jobID = id;
prer = *used;
r = (*used) -> next;
while(r && r->startAddress < startAddress)
{
prer = r;
r = r -> next;
}
q -> next = prer -> next;
prer -> next = q;
}
int finishJob(struct usedList **used,int id,int *startAddress) 
/*结束一个作业号为 id 的作业，释放存储空间（由*startAddress 返回空间的起始地址）
*/
{
struct usedList *p,*prep;
prep = *used;
p = prep -> next;
while(p && p -> jobID != id)
{
prep = p;
p = p -> next;
}
if(p == NULL)
{
printf("\nThe job which id is : %d is not in the memory !",id);
return 0;
}
else
{
*startAddress = p->startAddress;
prep -> next = p -> next;
free(p);
return 1;
}
}
void insertFreeNode(struct freeList **empty,int startAddress,int size)
/*插入回收的空节点分区，处理回收分区与空闲分区的四种邻接关系。*/
{
struct freeList *p,*q,*r;
 for(p = *empty;p -> next;p = p -> next) ; 
/* 处理链表尾部的邻接情况 */
if(p == *empty || p -> startAddress + p -> size < startAddress)
/* 与尾部不相邻*/
{
makeFreeNode(&r,startAddress,size);
/* 通过 r 指针返回创建的空闲节点*/
r -> next = p -> next; /* 插入独立的空闲节点 */
p -> next = r;
return ;
}
if(p -> startAddress + p -> size == startAddress) /* 与尾部上邻 */
{
p -> size += size; /* 合并尾部节点 */
return ;
}
q = (*empty) -> next; /* 处理链表首节点的邻接情况 */
if(startAddress + size == q -> startAddress) /* 与首节点下邻 */
{
q -> startAddress = startAddress; /* 合并首节点 */
q -> size += size;
}
else if(startAddress + size < q -> startAddress) 
/* 与首节点不相邻 */
{
makeFreeNode(&r,startAddress,size);
r -> next = (*empty) -> next;
(*empty) -> next = r;
}
else 
{ /* 处理链表中间的邻接情况 */
while(q -> next && q->startAddress < startAddress)
{
p = q;
q = q -> next;
}
if(p->startAddress+p->size == startAddress &&\
 q->startAddress == startAddress+size) /* 上下邻，合并节点 */
{
p->size+= size+q->size;
p->next =q->next;
free(q); /* 删除多余节点 */
}
else if(p -> startAddress + p -> size == startAddress &&\
 q -> startAddress != startAddress + size) 
/*上邻，增加节点的大小*/
{
p -> size += size;
}
else if(p -> startAddress + p -> size != startAddress &&\
 q -> startAddress == startAddress + size) /* 下邻 */
{
q -> startAddress = startAddress; /* 修改节点起始地址 */
q -> size += size; /* 修改节点的大小 */
}
else
{ /* 上下不相邻 */
makeFreeNode(&r,startAddress,size);
r -> next = p -> next;
p -> next = r;
}
}
}
void main(void)
{
    char fitMethod;
    FILE *fp;
    struct jobList *jobs;
    struct freeList *empty;
    struct usedList *used;
    if((used = malloc(sizeof(struct usedList))) == NULL)
    {
        printf("\nNot enough to allocate for the used node.");
        errorMessage();
    }
    used -> next = NULL;
    remove("d:\\result.cl");
    makeFreeNode(&empty,0,0);
    while(1)
    {
        char ch,step;
        int id,size,startAddress,status;
        struct jobList *q;
        printf("\n1-----------Initializiation.\
        \n2-----------Put job into memory(allocate memory).\
        \n3-----------Finish job(reuse memory).\
        \n4-----------Show current free list.\
        \n5-----------Show current memory used by jobs.\
        \n6-----------Move fragment together.\
        \n7-----------Exit.");
        printf("\nPlease select a digit to continue.\n");
        step = getche();
        printf("\n");
        switch(step)
        {
            case '1':
                openFile(&fp,"d:\\result.cl","a");
                fprintf(fp,"\n\n\tInitializiation :)");
                used -> next = NULL;
                empty->next = NULL;
                iniMemory();
                makeFreeNode(&(empty->next),memoryStartAddress,memorySize);
                fprintf(fp,"\n\n\nDo you want to use your job file directly ?\
                \nDefault is \'N\' . Y/N : ");
                printf("\n\n\nDo you want to use your job file directly ?\
                \nDefault is \'N\' . Y/N : \n");
                ch = getche();
                fprintf(fp,"\n%c",ch);
                fclose(fp);
                if(ch != 'Y'&& ch != 'y')
                {
                inputJob();
                }
                makeJobList(&jobs);
                if(ch == 'Y'|| ch == 'y')
                {
                for(q = jobs->next;q;q = q->next)
                {
                if(q->status == 1)
                {
                startAddress = allocate(&empty,q->size);
                if(startAddress != -1)
                {
                insertUsedNode(&used,q->id,startAddress);
                }
                }
                }
                }
                fitMethod = selectFitMethod();
            break;
            case '2':
            if(memoryStartAddress < 0 || memorySize < 1)
            {
            printf("\n\nBad memory allocated !\a");
            break;
            }
            openFile(&fp,"d:\\result.cl","a");
            fprintf(fp,"\n\n\tPut job into memory(allocate memory) ...");
            fprintf(fp,"\n\n\nDo you want to allocate for job from keyboard ?\
            \nDefault is \'N\' . Y/N : ");
            printf("\n\n\nDo you want to allocate for job from keyboard ?\
            \nDefault is \'N\' . Y/N : \n");
            ch = getche();
            fprintf(fp,"\n%c",ch);
            fclose(fp);
            if(ch != 'Y' && ch != 'y')
            {
            for(q = jobs->next;q;q = q->next)
            {
            if(q->status == 0)
            {
            switch(fitMethod)
            {
            case '1': order(&empty,0,0);break;
            case '2': order(&empty,0,1);break;
            case '3': order(&empty,1,0);break;
            case '4': order(&empty,1,1);break;
            }
            startAddress = allocate(&empty,q->size);
            if(startAddress != -1)
            {
            insertUsedNode(&used,q->id,startAddress);
            updateJobStatus(&jobs,q->id,1);
            }
            }
            }
            updateJobFile(jobs);
            }
            else
            {
            showJobList(jobs);
            openFile(&fp,"d:\\result.cl","a");
            fprintf(fp,"\nPlease input a job id from above .");
            printf("\nPlease input a job id from above .");
            scanf("%d",&id);
            fprintf(fp,"%d\n",id);
            getJobInfo(jobs,id,&size,&status);
            switch(status)
            {
            case 0: printf("\nOk !The job's status is correct !");
            fprintf(fp,"\nOk !The job's status is correct !");fclose(fp);break;
            case 1: printf("\nThe job was in the memory !");
            fprintf(fp,"\nThe job was in the memory !");fclose(fp);goto label;
            case 2: printf("\nThe job was finished !");
            fprintf(fp,"\nThe job was finished !");fclose(fp);goto label;
            default:printf("\nUnexpected job status .Please check you job file.");
            fprintf(fp,"\nUnexpected job status .Please check you job file.");
            fclose(fp);errorMessage();
            }
            switch(fitMethod)
            {
            case '1': order(&empty,0,0);break;
            case '2': order(&empty,0,1);break;
            case '3': order(&empty,1,0);break;
            case '4': order(&empty,1,1);break;
            }
            startAddress = allocate(&empty,size);
            if(startAddress != -1)
            {
            insertUsedNode(&used,id,startAddress);
            updateJobStatus(&jobs,id,1);
            updateJobFile(jobs);
            }
            }
            label : ;
            break;
            case '3':
            if(memoryStartAddress < 0 || memorySize < 1)
            {
            printf("\n\nBad memory allocated !\a");
            break;
            }
            do{
            int i;
            openFile(&fp,"d:\\result.cl","a");
            fprintf(fp,"\n\n\tFinish job(reuse memory). ...");
            fclose(fp);
            if(showUsedList(jobs,used) == 0)
            break;
            openFile(&fp,"d:\\result.cl","a");
            fprintf(fp,"\nPlease input the id from above .\nInput -1 to end the finish job task .");
            printf("\nPlease input the id from above .\nInput -1 to end the finish job task .");
            scanf("%d",&id);
            fprintf(fp,"%d\n",id);
            fclose(fp);
            if(id == -1)
            break;
            getJobInfo(jobs,id,&size,&status);
            if(status == 1)
            {
                i = finishJob(&used,id,&startAddress);
                if(i)
                {
                    insertFreeNode(&empty,startAddress,size);
                    updateJobStatus(&jobs,id,2);
                    updateJobFile(jobs);
                }
            }
            else
            {
                if(status == 0 || status == 2)
                {
                    if(status == 0)
                    printf("\nThe job was not in the memory !");
                    else printf("\nThe job was finished !");
                }
                else
                {
                    printf("\nUnexpected job status .\
                    Please check you job file.");
                    errorMessage();
                }
            }
            }while(1);
            break;
            case '4':
                openFile(&fp,"d:\\result.cl","a");
                fprintf(fp,"\n\n\tShow current free list. ...");
                fclose(fp);
                showFreeList(empty);
            break;
            case '5':
                openFile(&fp,"d:\\result.cl","a");
                fprintf(fp,"\n\n\tShow current memory used by jobs. ...");
                fclose(fp);
                showUsedList(jobs,used);
            break;
            case '6':
                openFile(&fp,"d:\\result.cl","a");
                fprintf(fp,"\n\n\tMove fragment together. ...");
                fclose(fp);
                moveFragment(jobs,&empty,&used);
            break;
            case '7':
                openFile(&fp,"d:\\result.cl","a");
                fprintf(fp,"\n\n\tExit :(");
                fclose(fp);
                exit(0);
            default: printf("\nWrong choice !");
        }
    }
    getch();
}

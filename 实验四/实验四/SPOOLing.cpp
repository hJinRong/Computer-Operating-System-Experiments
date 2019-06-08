#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "iostream"
using namespace std;

#define PROCESSNUM 2   //输出进程个数
#define BUFFERNUM  100  //内存缓冲字节个数
#define OUTBUFFERNUM  200  //输出井存储字节个数
#define REQBLOCKNUM   10

struct pcb //进程控制块PCB
{
	int id;		//进程标识 
	int status;	//状态 
	int length;//输出长度 
	int buf[BUFFERNUM];//输出缓冲 
}PCB[PROCESSNUM + 1];

struct req   //请求输出块
{
	int reqid;//要求输出的进程 
	int length;//输出长度 
	int addr;//输出首地址 
}ReqBlock[REQBLOCKNUM];

struct outbuf  //输出井结构
{
	int buf[OUTBUFFERNUM];   //输出井缓冲区
	int usedNum;     //输出井缓冲区已使用的数目
	int head;  //指示输出井空闲块首地址
	//int tail;  //指示输出井信息块（有信息的部分）尾地址
}OutBuffer[PROCESSNUM];

int UsedReqBlockNum = 0;   //记录当前已使用的请求块数目
int head = 0, tail = 0;  //指示当前使用的输出请求块，request从tail开始取，spooling从head开始取
int FileNum[PROCESSNUM];

void input()//输入函数 
{
	for (int i = 0; i < PROCESSNUM; i++) {
		cout << "输入第" << i + 1 << "个用户需要输出的文件数目：";
		cin >> FileNum[i];
	}
}

void init()//初始化函数 
{
	int i, j;
	for (i = 0; i < PROCESSNUM; i++) {
		OutBuffer[i].head = 0;
		OutBuffer[i].usedNum = 0;
		for (j = 0; j < OUTBUFFERNUM; j++)
			OutBuffer[i].buf[j] = 0;
	}
	for (i = 0; i < REQBLOCKNUM; i++) {
		ReqBlock[i].reqid = -1;
		ReqBlock[i].length = 0;
		ReqBlock[i].addr = 0;
	}
	for (i = 0; i < PROCESSNUM + 1; i++) {
		PCB[i].id = i;
		PCB[i].status = 0;
		PCB[i].length = 0;
		for (j = 0; j < BUFFERNUM; j++)
			PCB[i].buf[j] = 0;
	}
	PCB[PROCESSNUM].status = 2;  //spooling进程的状态置2（输出井空）
}


void request(int i) {
	int j, length = 0;

	if (PCB[i].length == 0)   //判断上次的输出是否处理完
	{
		FileNum[i] = FileNum[i] - 1;

		//srand((unsigned)time(NULL));
		while (1) {
			j = rand() % 10;
			if ((j == 0) && (length != 0))    //以0结束此次输出
			{
				PCB[i].length = length;
				break;
			}
			PCB[i].buf[length] = j;
			length++;
		}
	}

	if (OutBuffer[i].usedNum + length > OUTBUFFERNUM)   //判断输出井是否满
	{
		PCB[i].status = 1;   //输出井满，进程状态置1
		return;
	}

	if (UsedReqBlockNum == REQBLOCKNUM)    //判断是否有空闲的请求块
	{
		PCB[i].status = 3;  //没有空闲的请求块，进程状态置3
		return;
	}

	//填写请求块
	ReqBlock[tail].reqid = i;
	ReqBlock[tail].addr = OutBuffer[i].head;
	ReqBlock[tail].length = PCB[i].length;
	UsedReqBlockNum++;

	//将数据写到输出井
	int k;
	for (k = 0; k < PCB[i].length; k++)
		OutBuffer[i].buf[(OutBuffer[i].head + k) % OUTBUFFERNUM] = PCB[i].buf[k];
	OutBuffer[i].head = (OutBuffer[i].head + PCB[i].length) % OUTBUFFERNUM;
	OutBuffer[i].usedNum += PCB[i].length;
	PCB[i].length = 0;

	if (PCB[PROCESSNUM].status == 2)  //若spooling进程阻塞，则修改其状态为可执行（0）
		PCB[PROCESSNUM].status = 0;
	tail = (tail + 1) % REQBLOCKNUM;

	if (FileNum[i] == 0)
		PCB[i].status = 4;
}

void spooling() {
	if (UsedReqBlockNum !=0 ) {
		//取数据--打印
		PCB[0].length=0;
		PCB[0].status=4;
		PCB[1].length = 0;
		PCB[1].status = 4;
		if (ReqBlock[0].addr!=0) {
			ReqBlock[0].reqid=0;
		}
		if (ReqBlock[1].addr != 0) {
			ReqBlock[1].reqid = 0;
		}
	} else {
		if (sizeof(OutBuffer[0].buf)==0 && sizeof(OutBuffer[1].buf) == 0) {
			PCB[3].status=4;
		} else {
			PCB[3].status=2;
		}
	}

	return;
}

void work()//模拟进程调度 
{
	int i;
	bool isFinish;

	srand((unsigned)time(NULL));
	while (1) {
		i = rand() % 100;
		if (i <= 45) {
			if (PCB[0].status == 0)
				request(0);
		} else if (i <= 90) {
			if (PCB[1].status == 0)
				request(1);
		} else
			spooling();

		isFinish = true;
		for (i = 0; i < PROCESSNUM + 1; i++)  //判断是否所有进程都结束
			if (PCB[i].status != 4)
				isFinish = false;
		if (isFinish)     //若所有进程都结束，则退出
			return;
	}
}
int main() //主程序
{
	printf("\n>>>>>>>>>>>>>>>> SPOOLing系统模拟程序 <<<<<<<<<<<<<<<<<\n");

	init();
	input();
	work();
	return 0;
}

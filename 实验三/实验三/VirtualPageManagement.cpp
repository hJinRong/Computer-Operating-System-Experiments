#include<stdio.h> 
#include<string.h> 
#include<iostream>
using namespace std;

const int MAXSIZE = 1000;//定义最大页面数 
const int NUM = 3;//定义页框数（物理块数）

typedef struct node {
	int loaded;   //记录该物理块存储的页面
	int time;     //记录该物理块没有被使用的时间
}page;

page pages[NUM]; //定义页框表 （物理块表）
int queue[MAXSIZE];
int quantity;

//初始化结构函数 
void initial() {
	int i;
	for (i = 0; i < NUM; i++) {
		pages[i].loaded = -1;
	}
	for (i = 0; i < MAXSIZE; i++)
		queue[i] = -1;
	quantity = 0;
}

//读入页面流
void readData() {
	FILE* fp;
	char fname[20];
	int i;
	cout << "请输入页面流文件名:";
	cin >> fname;
	if ((fp = fopen(fname, "r")) == NULL) {
		cout << "错误,文件打不开,请检查文件名";
	} else {
		while (!feof(fp)) {
			fscanf(fp, "%d ", &queue[quantity]);
			quantity++;
		}
	}
	cout << "读入的页面流:";
	for (i = 0; i < quantity; i++) {
		cout << queue[i] << " ";
	}
}

//FIFO调度算法
void FIFO() {
	int i, j, p, flag;
	int absence = 0;  //记录缺页次数
	p = 0;
	cout << endl << "----------------------------------------------------" << endl;
	cout << "先进先出调度算法(FIFO)页面调出流:";
	for (i = 0; i < NUM; i++)  //前3个进入内存的页面
	{
		pages[p].loaded = queue[i];
		cout << pages[p].loaded << " ";
		p = (p + 1) % NUM;
	}
	absence = 3;
	for (i = NUM; i < quantity; i++) {
		flag = 0;
		for (j = 0; j < NUM; j++)  //判断当前需求的页面是否在内存
		{
			if (pages[j].loaded == queue[i])
				flag = 1;
		}
		if (flag == 0) {
			cout << pages[p].loaded << " ";
			pages[p].loaded = queue[i];
			p = (p + 1) % NUM;
			absence++;
		}
	}
	cout << endl << "总缺页数:" << absence << endl;
}

//最近最少使用调度算法（LRU）
void LRU() {

	//请补充LRU算法的代码
}


void main() {
	cout << "     /**********虚拟存储管理器的页面调度**************/" << endl;
	initial();
	readData();
	FIFO();
	// LRU();
}

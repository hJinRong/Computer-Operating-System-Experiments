#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <vector>
using namespace std;

#define MAX_PROCESS 32              //最大进程数
#define MAX_RESOURCE 64              //最大资源类别

int PROCESS_NUM;              //实际总进程数
int RESOURCE_NUM;               //实际资源类别数
int Available[MAX_RESOURCE];                 //可利用资源向量
int Max[MAX_PROCESS][MAX_RESOURCE];          //最大需求矩阵
int Allocation[MAX_PROCESS][MAX_RESOURCE];   //分配矩阵
int Need[MAX_PROCESS][MAX_RESOURCE];         //需求矩阵

int Request_PROCESS;                       //发出请求的进程
int Request_RESOURCE_NEMBER[MAX_RESOURCE];     //请求资源数

void Read_Available_list();      //读入可用资源Available
void Read_Max_list();           //读入最大需求矩阵Max
void Read_Allocation_list();    //读入已分配矩阵Allocation
void PrintInfo();               //打印各数据结构信息
void Read_Request();			//输入请求向量
void Allocate_Source();         //开始正式分配资源（修改Allocation_list.txt）
void Recover_TryAllocate();     //恢复试分配前状态
int Test_Safty();               //安全性检测
void RunBanker();               //执行银行家算法


//读入可用资源Available
void Read_Available_list()
{
	FILE* fp;
	if ((fp = fopen("Available_list.txt", "r")) == NULL)
	{
		cout << "错误,文件打不开,请检查文件名" << endl;
		exit(0);
	}
	fscanf(fp, "%d", &RESOURCE_NUM);
	int i = 0;
	while (!feof(fp))
	{
		fscanf(fp, "%d", &Available[i]);
		i++;
	}
	fclose(fp);
}

//读入最大需求矩阵Max
void Read_Max_list()
{
	FILE* fp;
	if ((fp = fopen("Max_list.txt", "r")) == NULL)
	{
		cout << "错误,文件打不开,请检查文件名" << endl;
		exit(0);
	}
	fscanf(fp, "%d", &PROCESS_NUM);
	for (int i = 0; i < PROCESS_NUM; i++)
		for (int j = 0; j < RESOURCE_NUM; j++)
			fscanf(fp, "%d", &Max[i][j]);
	fclose(fp);
}

//读入已分配矩阵Allocation
void Read_Allocation_list()
{
	FILE* fp;
	if ((fp = fopen("Allocation_list.txt", "r")) == NULL)
	{
		cout << "错误,文件打不开,请检查文件名" << endl;
		exit(0);
	}
	for (int i = 0; i < PROCESS_NUM; i++)
		for (int j = 0; j < RESOURCE_NUM; j++)
			fscanf(fp, "%d", &Allocation[i][j]);
	fclose(fp);
}

//设置需求矩阵Need
void Set_Need_Available()
{
	for (int i = 0; i < PROCESS_NUM; i++)
		for (int j = 0; j < RESOURCE_NUM; j++)
		{
			Need[i][j] = Max[i][j] - Allocation[i][j];
			Available[j] = Available[j] - Allocation[i][j];
		}
}

//打印各数据结构信息
void PrintInfo()
{
	cout << "进程个数： " << PROCESS_NUM << "\t" << "资源个数： " << RESOURCE_NUM << endl;
	cout << "可用资源向量Available：" << endl;
	int i, j;
	for (i = 0; i < RESOURCE_NUM; i++)
		cout << Available[i] << "\t";
	cout << endl;
	cout << "最大需求矩阵Max：" << endl;
	for (i = 0; i < PROCESS_NUM; i++)
	{
		for (j = 0; j < RESOURCE_NUM; j++)
			cout << Max[i][j] << "\t";
		cout << endl;
	}
	cout << "已分配矩阵Allocation：" << endl;
	for (i = 0; i < PROCESS_NUM; i++)
	{
		for (j = 0; j < RESOURCE_NUM; j++)
			cout << Allocation[i][j] << "\t";
		cout << endl;
	}
	cout << "需求矩阵Need：" << endl;
	for (i = 0; i < PROCESS_NUM; i++)
	{
		for (j = 0; j < RESOURCE_NUM; j++)
			cout << Need[i][j] << "\t";
		cout << endl;
	}
}

//输入请求向量
void Read_Request()
{
	cout << "输入发起请求的进程（0－" << PROCESS_NUM - 1 << ")：";
	cin >> Request_PROCESS;

	cout << "输入请求资源的数目：按照这样的格式输入 x x x：";
	for (int i = 0; i < RESOURCE_NUM; i++)
		cin >> Request_RESOURCE_NEMBER[i];
}

//开始正式分配资源（修改Allocation_list.txt）
void Allocate_Source()
{
	cout << '\n' << "开始给第" << Request_PROCESS << "个进程分配资源..." << endl;
	FILE* fp;
	if ((fp = fopen("Allocation_list.txt", "w")) == NULL)
	{
		cout << "错误,文件打不开,请检查文件名" << endl;
		exit(0);
	}
	for (int i = 0; i < PROCESS_NUM; i++)
	{
		for (int j = 0; j < RESOURCE_NUM; j++)
			fprintf(fp, "%d  ", Allocation[i][j]);
		fprintf(fp, "\n");
	}
	cout << "分配完成，已更新Allocation_list.txt" << endl;
	fclose(fp);
}

//恢复试分配前状态
void Recover_TryAllocate()
{
	for (int i = 0; i < RESOURCE_NUM; i++)
	{
		Available[i] = Available[i] + Request_RESOURCE_NEMBER[i];
		Allocation[Request_PROCESS][i] = Allocation[Request_PROCESS][i] - Request_RESOURCE_NEMBER[i];
		Need[Request_PROCESS][i] = Need[Request_PROCESS][i] + Request_RESOURCE_NEMBER[i];
	}
}

//安全性检测
//返回值：0：未通过安全性测试； 1：通过安全性测试
int Test_Safty()
{
	int m=sizeof(Allocation); //m 资源种类数
	int n=sizeof(Need);  //n 进程数
	vector<int> Work(m);
	for (int i = 0; i < m; i++) {
		Work[i] = Available[i];
	}
	vector<bool> Finish(n);
	for (int i = 0; i < n; i++) {
		Finish[i] = false;
	}

	int compared_time = 0; //已经循环全组比较次数
	int max_compare_time = 0;  //最大循环全组比较次数
	int _n=n;
	while (_n != 1) {
		max_compare_time *= _n;
		_n--;
	}

	while (compared_time != max_compare_time) {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				if (Finish[i] == false && Need[i][j] <= Work[j]) {
					Work[j] = Work[j] + Allocation[i][j];
					if (j == 3) {
						Finish[i] = true;
						i++;
						break;
					}
				}
			}
		}
	}

	for (int i = 0; i < n; i++) {
		if (Finish[i] == true) {
			return 1;
		} else {
			return 0;
		}
	}
}


void RunBanker() {              //执行银行家算法
	cout << endl;
	cout << "开始执行银行家算法..." << endl;


	for (int i = 0; i < RESOURCE_NUM; i++)  //检查是否满足条件Request<=Need
		if (Request_RESOURCE_NEMBER[i] > Need[Request_PROCESS][i])
		{
			cout << "\n第" << Request_PROCESS << "个进程请求资源不成功" << endl;
			cout << "原因：超出该进程尚需的资源的最大数量!" << endl;
			return;
		}
	for (int i = 0; i < RESOURCE_NUM; i++)   //检查是否满足条件Request<=Available
		if (Request_RESOURCE_NEMBER[i] > Available[i])
		{
			cout << "\n第" << Request_PROCESS << "个进程请求资源不成功" << endl;
			cout << "原因：系统中无足够的资源!" << endl;
			return;
		}
		else {
			//试分配，更新各相关数据结构
			Available[i] = Available[i] - Request_RESOURCE_NEMBER[i];
			Allocation[Request_PROCESS][i] = Allocation[Request_PROCESS][i] + Request_RESOURCE_NEMBER[i];
			Need[Request_PROCESS][i] = Need[Request_PROCESS][i] - Request_RESOURCE_NEMBER[i];
		}
	cout << endl << "试分配完成..." << endl;

	if (Test_Safty())    //使用安全性算法检查，若满足，则正式分配
		Allocate_Source();
	else                //否则恢复试分配前状态
		Recover_TryAllocate();
}



void main()
{
	char c;
	Read_Available_list();
	Read_Max_list();
	Read_Allocation_list();
	Set_Need_Available();
	PrintInfo();
	while (1)
	{
		Read_Request();
		RunBanker();
		cout << "\n\n需要继续吗？（y-继续；n-终止）";
		cin >> c;
		if (c == 'n')
			break;
		cout << endl << endl;
		PrintInfo();
	}
}

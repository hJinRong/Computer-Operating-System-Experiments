#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <vector>
using namespace std;

#define MAX_PROCESS 32              //��������
#define MAX_RESOURCE 64              //�����Դ���

int PROCESS_NUM;              //ʵ���ܽ�����
int RESOURCE_NUM;               //ʵ����Դ�����
int Available[MAX_RESOURCE];                 //��������Դ����
int Max[MAX_PROCESS][MAX_RESOURCE];          //����������
int Allocation[MAX_PROCESS][MAX_RESOURCE];   //�������
int Need[MAX_PROCESS][MAX_RESOURCE];         //�������

int Request_PROCESS;                       //��������Ľ���
int Request_RESOURCE_NEMBER[MAX_RESOURCE];     //������Դ��

void Read_Available_list();      //���������ԴAvailable
void Read_Max_list();           //��������������Max
void Read_Allocation_list();    //�����ѷ������Allocation
void PrintInfo();               //��ӡ�����ݽṹ��Ϣ
void Read_Request();			//������������
void Allocate_Source();         //��ʼ��ʽ������Դ���޸�Allocation_list.txt��
void Recover_TryAllocate();     //�ָ��Է���ǰ״̬
int Test_Safty();               //��ȫ�Լ��
void RunBanker();               //ִ�����м��㷨


//���������ԴAvailable
void Read_Available_list()
{
	FILE* fp;
	if ((fp = fopen("Available_list.txt", "r")) == NULL)
	{
		cout << "����,�ļ��򲻿�,�����ļ���" << endl;
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

//��������������Max
void Read_Max_list()
{
	FILE* fp;
	if ((fp = fopen("Max_list.txt", "r")) == NULL)
	{
		cout << "����,�ļ��򲻿�,�����ļ���" << endl;
		exit(0);
	}
	fscanf(fp, "%d", &PROCESS_NUM);
	for (int i = 0; i < PROCESS_NUM; i++)
		for (int j = 0; j < RESOURCE_NUM; j++)
			fscanf(fp, "%d", &Max[i][j]);
	fclose(fp);
}

//�����ѷ������Allocation
void Read_Allocation_list()
{
	FILE* fp;
	if ((fp = fopen("Allocation_list.txt", "r")) == NULL)
	{
		cout << "����,�ļ��򲻿�,�����ļ���" << endl;
		exit(0);
	}
	for (int i = 0; i < PROCESS_NUM; i++)
		for (int j = 0; j < RESOURCE_NUM; j++)
			fscanf(fp, "%d", &Allocation[i][j]);
	fclose(fp);
}

//�����������Need
void Set_Need_Available()
{
	for (int i = 0; i < PROCESS_NUM; i++)
		for (int j = 0; j < RESOURCE_NUM; j++)
		{
			Need[i][j] = Max[i][j] - Allocation[i][j];
			Available[j] = Available[j] - Allocation[i][j];
		}
}

//��ӡ�����ݽṹ��Ϣ
void PrintInfo()
{
	cout << "���̸����� " << PROCESS_NUM << "\t" << "��Դ������ " << RESOURCE_NUM << endl;
	cout << "������Դ����Available��" << endl;
	int i, j;
	for (i = 0; i < RESOURCE_NUM; i++)
		cout << Available[i] << "\t";
	cout << endl;
	cout << "����������Max��" << endl;
	for (i = 0; i < PROCESS_NUM; i++)
	{
		for (j = 0; j < RESOURCE_NUM; j++)
			cout << Max[i][j] << "\t";
		cout << endl;
	}
	cout << "�ѷ������Allocation��" << endl;
	for (i = 0; i < PROCESS_NUM; i++)
	{
		for (j = 0; j < RESOURCE_NUM; j++)
			cout << Allocation[i][j] << "\t";
		cout << endl;
	}
	cout << "�������Need��" << endl;
	for (i = 0; i < PROCESS_NUM; i++)
	{
		for (j = 0; j < RESOURCE_NUM; j++)
			cout << Need[i][j] << "\t";
		cout << endl;
	}
}

//������������
void Read_Request()
{
	cout << "���뷢������Ľ��̣�0��" << PROCESS_NUM - 1 << ")��";
	cin >> Request_PROCESS;

	cout << "����������Դ����Ŀ�����������ĸ�ʽ���� x x x��";
	for (int i = 0; i < RESOURCE_NUM; i++)
		cin >> Request_RESOURCE_NEMBER[i];
}

//��ʼ��ʽ������Դ���޸�Allocation_list.txt��
void Allocate_Source()
{
	cout << '\n' << "��ʼ����" << Request_PROCESS << "�����̷�����Դ..." << endl;
	FILE* fp;
	if ((fp = fopen("Allocation_list.txt", "w")) == NULL)
	{
		cout << "����,�ļ��򲻿�,�����ļ���" << endl;
		exit(0);
	}
	for (int i = 0; i < PROCESS_NUM; i++)
	{
		for (int j = 0; j < RESOURCE_NUM; j++)
			fprintf(fp, "%d  ", Allocation[i][j]);
		fprintf(fp, "\n");
	}
	cout << "������ɣ��Ѹ���Allocation_list.txt" << endl;
	fclose(fp);
}

//�ָ��Է���ǰ״̬
void Recover_TryAllocate()
{
	for (int i = 0; i < RESOURCE_NUM; i++)
	{
		Available[i] = Available[i] + Request_RESOURCE_NEMBER[i];
		Allocation[Request_PROCESS][i] = Allocation[Request_PROCESS][i] - Request_RESOURCE_NEMBER[i];
		Need[Request_PROCESS][i] = Need[Request_PROCESS][i] + Request_RESOURCE_NEMBER[i];
	}
}

//��ȫ�Լ��
//����ֵ��0��δͨ����ȫ�Բ��ԣ� 1��ͨ����ȫ�Բ���
int Test_Safty()
{
	int m=sizeof(Allocation); //m ��Դ������
	int n=sizeof(Need);  //n ������
	vector<int> Work(m);
	for (int i = 0; i < m; i++) {
		Work[i] = Available[i];
	}
	vector<bool> Finish(n);
	for (int i = 0; i < n; i++) {
		Finish[i] = false;
	}

	int compared_time = 0; //�Ѿ�ѭ��ȫ��Ƚϴ���
	int max_compare_time = 0;  //���ѭ��ȫ��Ƚϴ���
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


void RunBanker() {              //ִ�����м��㷨
	cout << endl;
	cout << "��ʼִ�����м��㷨..." << endl;


	for (int i = 0; i < RESOURCE_NUM; i++)  //����Ƿ���������Request<=Need
		if (Request_RESOURCE_NEMBER[i] > Need[Request_PROCESS][i])
		{
			cout << "\n��" << Request_PROCESS << "������������Դ���ɹ�" << endl;
			cout << "ԭ�򣺳����ý����������Դ���������!" << endl;
			return;
		}
	for (int i = 0; i < RESOURCE_NUM; i++)   //����Ƿ���������Request<=Available
		if (Request_RESOURCE_NEMBER[i] > Available[i])
		{
			cout << "\n��" << Request_PROCESS << "������������Դ���ɹ�" << endl;
			cout << "ԭ��ϵͳ�����㹻����Դ!" << endl;
			return;
		}
		else {
			//�Է��䣬���¸�������ݽṹ
			Available[i] = Available[i] - Request_RESOURCE_NEMBER[i];
			Allocation[Request_PROCESS][i] = Allocation[Request_PROCESS][i] + Request_RESOURCE_NEMBER[i];
			Need[Request_PROCESS][i] = Need[Request_PROCESS][i] - Request_RESOURCE_NEMBER[i];
		}
	cout << endl << "�Է������..." << endl;

	if (Test_Safty())    //ʹ�ð�ȫ���㷨��飬�����㣬����ʽ����
		Allocate_Source();
	else                //����ָ��Է���ǰ״̬
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
		cout << "\n\n��Ҫ�����𣿣�y-������n-��ֹ��";
		cin >> c;
		if (c == 'n')
			break;
		cout << endl << endl;
		PrintInfo();
	}
}

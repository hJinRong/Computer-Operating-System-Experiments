#include<stdio.h> 
#include<string.h> 
#include<iostream>
using namespace std;

const int MAXSIZE = 1000;//�������ҳ���� 
const int NUM = 3;//����ҳ���������������

typedef struct node {
	int loaded;   //��¼�������洢��ҳ��
	int time;     //��¼�������û�б�ʹ�õ�ʱ��
}page;

page pages[NUM]; //����ҳ��� ��������
int queue[MAXSIZE];
int quantity;

//��ʼ���ṹ���� 
void initial() {
	int i;
	for (i = 0; i < NUM; i++) {
		pages[i].loaded = -1;
	}
	for (i = 0; i < MAXSIZE; i++)
		queue[i] = -1;
	quantity = 0;
}

//����ҳ����
void readData() {
	FILE* fp;
	char fname[20];
	int i;
	cout << "������ҳ�����ļ���:";
	cin >> fname;
	if ((fp = fopen(fname, "r")) == NULL) {
		cout << "����,�ļ��򲻿�,�����ļ���";
	} else {
		while (!feof(fp)) {
			fscanf(fp, "%d ", &queue[quantity]);
			quantity++;
		}
	}
	cout << "�����ҳ����:";
	for (i = 0; i < quantity; i++) {
		cout << queue[i] << " ";
	}
}

//FIFO�����㷨
void FIFO() {
	int i, j, p, flag;
	int absence = 0;  //��¼ȱҳ����
	p = 0;
	cout << endl << "----------------------------------------------------" << endl;
	cout << "�Ƚ��ȳ������㷨(FIFO)ҳ�������:";
	for (i = 0; i < NUM; i++)  //ǰ3�������ڴ��ҳ��
	{
		pages[p].loaded = queue[i];
		cout << pages[p].loaded << " ";
		p = (p + 1) % NUM;
	}
	absence = 3;
	for (i = NUM; i < quantity; i++) {
		flag = 0;
		for (j = 0; j < NUM; j++)  //�жϵ�ǰ�����ҳ���Ƿ����ڴ�
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
	cout << endl << "��ȱҳ��:" << absence << endl;
}

//�������ʹ�õ����㷨��LRU��
void LRU() {

	//�벹��LRU�㷨�Ĵ���
}


void main() {
	cout << "     /**********����洢��������ҳ�����**************/" << endl;
	initial();
	readData();
	FIFO();
	// LRU();
}

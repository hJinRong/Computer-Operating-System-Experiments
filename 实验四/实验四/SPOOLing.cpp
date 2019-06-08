#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "iostream"
using namespace std;

#define PROCESSNUM 2   //������̸���
#define BUFFERNUM  100  //�ڴ滺���ֽڸ���
#define OUTBUFFERNUM  200  //������洢�ֽڸ���
#define REQBLOCKNUM   10

struct pcb //���̿��ƿ�PCB
{
	int id;		//���̱�ʶ 
	int status;	//״̬ 
	int length;//������� 
	int buf[BUFFERNUM];//������� 
}PCB[PROCESSNUM + 1];

struct req   //���������
{
	int reqid;//Ҫ������Ľ��� 
	int length;//������� 
	int addr;//����׵�ַ 
}ReqBlock[REQBLOCKNUM];

struct outbuf  //������ṹ
{
	int buf[OUTBUFFERNUM];   //�����������
	int usedNum;     //�������������ʹ�õ���Ŀ
	int head;  //ָʾ��������п��׵�ַ
	//int tail;  //ָʾ�������Ϣ�飨����Ϣ�Ĳ��֣�β��ַ
}OutBuffer[PROCESSNUM];

int UsedReqBlockNum = 0;   //��¼��ǰ��ʹ�õ��������Ŀ
int head = 0, tail = 0;  //ָʾ��ǰʹ�õ��������飬request��tail��ʼȡ��spooling��head��ʼȡ
int FileNum[PROCESSNUM];

void input()//���뺯�� 
{
	for (int i = 0; i < PROCESSNUM; i++) {
		cout << "�����" << i + 1 << "���û���Ҫ������ļ���Ŀ��";
		cin >> FileNum[i];
	}
}

void init()//��ʼ������ 
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
	PCB[PROCESSNUM].status = 2;  //spooling���̵�״̬��2��������գ�
}


void request(int i) {
	int j, length = 0;

	if (PCB[i].length == 0)   //�ж��ϴε�����Ƿ�����
	{
		FileNum[i] = FileNum[i] - 1;

		//srand((unsigned)time(NULL));
		while (1) {
			j = rand() % 10;
			if ((j == 0) && (length != 0))    //��0�����˴����
			{
				PCB[i].length = length;
				break;
			}
			PCB[i].buf[length] = j;
			length++;
		}
	}

	if (OutBuffer[i].usedNum + length > OUTBUFFERNUM)   //�ж�������Ƿ���
	{
		PCB[i].status = 1;   //�������������״̬��1
		return;
	}

	if (UsedReqBlockNum == REQBLOCKNUM)    //�ж��Ƿ��п��е������
	{
		PCB[i].status = 3;  //û�п��е�����飬����״̬��3
		return;
	}

	//��д�����
	ReqBlock[tail].reqid = i;
	ReqBlock[tail].addr = OutBuffer[i].head;
	ReqBlock[tail].length = PCB[i].length;
	UsedReqBlockNum++;

	//������д�������
	int k;
	for (k = 0; k < PCB[i].length; k++)
		OutBuffer[i].buf[(OutBuffer[i].head + k) % OUTBUFFERNUM] = PCB[i].buf[k];
	OutBuffer[i].head = (OutBuffer[i].head + PCB[i].length) % OUTBUFFERNUM;
	OutBuffer[i].usedNum += PCB[i].length;
	PCB[i].length = 0;

	if (PCB[PROCESSNUM].status == 2)  //��spooling�������������޸���״̬Ϊ��ִ�У�0��
		PCB[PROCESSNUM].status = 0;
	tail = (tail + 1) % REQBLOCKNUM;

	if (FileNum[i] == 0)
		PCB[i].status = 4;
}

void spooling() {
	if (UsedReqBlockNum !=0 ) {
		//ȡ����--��ӡ
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

void work()//ģ����̵��� 
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
		for (i = 0; i < PROCESSNUM + 1; i++)  //�ж��Ƿ����н��̶�����
			if (PCB[i].status != 4)
				isFinish = false;
		if (isFinish)     //�����н��̶����������˳�
			return;
	}
}
int main() //������
{
	printf("\n>>>>>>>>>>>>>>>> SPOOLingϵͳģ����� <<<<<<<<<<<<<<<<<\n");

	init();
	input();
	work();
	return 0;
}

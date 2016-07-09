/*
 * TYPEdef.h
 *
 *  Created on: 22.03.2016
 *      Author: admin
 */

#ifndef TYPEDEF_H_
#define TYPEDEF_H_
//------------------------------------------------------------------//
#define MASTER true //��� ���� - �������
#define SLAVE false //��� ���� - ����������
#define MAXname 16	//������������ ������ ��� TDM � DPU

using namespace std;

//��� ������ ��������� ����
class TDM;

struct DPU_Set{//���������� ������������� ���� ��������� ������
	char* DPUname;//��� ���� ���������
	bool mode;//����� ������������ ����: MASTER=true - ������ �� ����� ������ TDM, SLAVE=false - ������ �� ������� ���������� TDM_0(�������)
	double period;//������ ����� ������������ ����-MASTER
	int (*procedure)(vector <void *> &In, vector <void *> &Out);
	TDM *(*In)[];//������� ��������� ��� �������� ��������� �� ������ ������� �������� ������������ ������
	TDM *(*Out)[];//������ ���������� �� �������� ������� ������������ ������
};
/*
struct DPU_Set{//���������� ������������� ���� ��������� ������
	char* DPUname;//��� ���� ���������
	bool mode;//����� ������������ ����: MASTER=true - ������ �� ����� ������ TDM, SLAVE=false - ������ �� ������� ���������� TDM_0(�������)
	double period;//������ ����� ������������ ����-MASTER
	int (*procedure)(vector <void *> In, vector <void *> Out);
	TDM *(*In)[];//������� ��������� ��� �������� ��������� �� ������ ������� �������� ������������ ������
	TDM *(*Out)[];//������ ���������� �� �������� ������� ������������ ������
};*/
#endif /* TYPEDEF_H_ */

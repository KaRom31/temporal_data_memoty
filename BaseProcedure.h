/*
 * BaseProcedure.h
 *
 *  Created on: 22.03.2016
 *      Author: admin
 */

#ifndef BASEPROCEDURE_H_
#define BASEPROCEDURE_H_

#include <vector>

#include <time.h>

#include <cstdlib>
#include <ctime>


#include <fstream>
#include <iomanip>
using namespace std;

//��������� ���������� ���� DPU0

double L=0;

int DPU0_Proc(vector <void *> &Vec_pTD_In, vector <void *> &Vec_pTD_Out){;
//��������� ���������� TDM
//double In0_Value=*(double*)(Vec_pTD_In[0]);

long Out0_Value=-123;
	cout << "\n������� ��������� DPU0_Proc" << endl;
		//	cout	<< "DPU0_Proc: �������� ������� �������� Vec_pTD_In[0] = " << In0_Value << endl;

		sleep(1);//���������� ��������� ������
		//Out0_Value= (long)(1000.3*In0_Value);//��������� ���������� DPU0_Proc

		//��������� ��������� ��������� � �������� TDM-�������
		memcpy(Vec_pTD_Out[0], (void *)&Out0_Value, sizeof (Out0_Value));
//		cout 	<< "DPU0_Proc: ��������� ���������� Vec_pTD_Out[0] = " << *(long*)(Vec_pTD_Out[0]) <<endl;

		cout 	<< "             ��������� ��������� ��������� ���� " << std::endl;
errno=1;
if(errno)return EXIT_SUCCESS;
else return EXIT_FAILURE;
}

//���������� ������ ���� DPU2
int DPU2(vector <void *> &Vec_pTD_In, vector <void *> &Vec_pTD_Out){;
	//��������� ���������� TDM
	double delta=*(double*)(Vec_pTD_In[0]);
	double alpha=*(double*)(Vec_pTD_In[1]);
			//10-L/10;
			//

		double volume=0;
		volume=alpha*delta;

	memcpy(Vec_pTD_Out[0], (void *)&volume, sizeof (volume));

		cout<<"DPU2 OUT!!! -- "<<volume<<endl;
		cout 	<< "             ��������� ��������� ��������� ���� " << std::endl;
		sleep(2);
errno=1;
if(errno)return EXIT_SUCCESS;
else return EXIT_FAILURE;
}



//DPU6 ����������� ������ ���� L �� TM3 � TM4 � TM6
int DPU_Replicator(vector <void *> &Vec_pTD_In, vector <void *> &Vec_pTD_Out){;
	//��������� ���������� TDM
	double level=*(double*)(Vec_pTD_In[0]);
		//��������� ��������� ��������� � �������� TDM-�������

		memcpy(Vec_pTD_Out[0], (void *)&level, sizeof (level));
		memcpy(Vec_pTD_Out[1], (void *)&level, sizeof (level));

		cout<<"LEVEL REPLICATOR!!! -- "<<level<<endl;
		cout 	<< "             ��������� ��������� ��������� ���� " << std::endl;
		sleep(3);
errno=1;
if(errno)return EXIT_SUCCESS;
else return EXIT_FAILURE;
}


//DPU3 - ���������� ������ ����
//��������� TM2 � ����������� ���������� � TM3
int DPU3(vector <void *> &Vec_pTD_In, vector <void *> &Vec_pTD_Out){;
		//��������� ���������� TDM
		double volume=*(double*)(Vec_pTD_In[0]);
		L+=volume;
		//��������� ��������� ��������� � �������� TDM-�������
		cout<<"DPU3 IN!!! -- "<<volume<<endl;
		memcpy(Vec_pTD_Out[0], (void *)&L, sizeof (L));
		cout<<"DPU3 OUT SUM LEVEL!!! -- "<<L<<endl;
		cout 	<< "             ��������� ��������� ��������� ���� " << std::endl;
		sleep(2);
errno=1;
if(errno)return EXIT_SUCCESS;
else return EXIT_FAILURE;
}

//DPU4 - ���������� ����� ��� ������������� ������ ����
//�� ��������� �� L(TM4) � P(TM1) � ��������� � TM5
int DPU4(vector <void *> &Vec_pTD_In, vector <void *> &Vec_pTD_Out){;
		//��������� ���������� TDM
		double Level=*(double*)(Vec_pTD_In[0]);
		double Volume=//10.0;
				*(double*)(Vec_pTD_In[1]);
				//10;
				//

		double alpha=0;
		if(Volume!=0)
		alpha=(Volume-Level)/Volume;
		cout<<"DPU4!!! -- "<<Level<<" -- "<<Volume<<endl;
		//��������� ��������� ��������� � �������� TDM-�������

		memcpy(Vec_pTD_Out[0], (void *)&alpha, sizeof (alpha));
		cout<<"DPU4!! -- "<<alpha<<endl;
		cout 	<< "             ��������� ��������� ��������� ���� " << std::endl;
		sleep(3);
errno=1;
if(errno)return EXIT_SUCCESS;
else return EXIT_FAILURE;
}

//��������� ������ ����
//DPU0
int DPU0(vector <void *> &Vec_pTD_In, vector <void *> &Vec_pTD_Out){;
		//��������� ���������� TDM
		double delta=0.5;

		//��������� ��������� ��������� � �������� TDM-�������

		memcpy(Vec_pTD_Out[0], (void *)&delta, sizeof (delta));
		cout 	<< " DPUO OUT "<<*(double*)Vec_pTD_Out[0]<<endl;
		cout 	<< "             ��������� ��������� ��������� ���� ���������--" <<delta <<std::endl;
sleep(2);
errno=1;
if(errno)return EXIT_SUCCESS;
else return EXIT_FAILURE;
}

//DPU1 - ��������� ������� ������������� ������ ����
int DPU1(vector <void *> &Vec_pTD_In, vector <void *> &Vec_pTD_Out){;
		//��������� ���������� TDM
		double volume=10.0;
		cout<<"DPU1 !!! VOLUME -- "<<volume<<endl;
		//��������� ��������� ��������� � �������� TDM-�������
		memcpy(Vec_pTD_Out[0], (void *)&volume, sizeof (volume));
		cout 	<< " DPU1 OUT "<<*(double*)Vec_pTD_Out[0]<<endl;
		cout<<"Gen 1111!!!!! ---- "<<endl;
		cout 	<< "             ��������� ��������� ��������� ���� " << std::endl;
		sleep(2);
errno=1;
if(errno)return EXIT_SUCCESS;
else return EXIT_FAILURE;
}

//DPU5 - ��������, ����� ������ ���� �� �����
int DPU_Display(vector <void *> &Vec_pTD_In, vector <void *> &Vec_pTD_Out){;


//��������� ���������� TDM
double Volume=*(double*)(Vec_pTD_In[0]);

		//��������� ��������� ��������� � �������� TDM-�������

cout<<"Display!!! LEVEL -- "<<Volume<<endl;

		//memcpy(Vec_pTD_Out[0], (void *)&Volume, sizeof (Volume));
		cout 	<< "             ��������� ��������� ��������� ���� " << std::endl;
		sleep(2);
errno=1;
if(errno)return EXIT_SUCCESS;
else return EXIT_FAILURE;
}


#endif /* BASEPROCEDURE_H_ */

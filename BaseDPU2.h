/*
 * BaseDPU.h
 *
 *  Created on: 22.03.2016
 *      Author: admin
 */

#ifndef BASEDPU_H_
#define BASEDPU_H_

#include <string.h>
//----------------------------------------


class TDM;
/*************** DPU-��������� � ������ ����� ***************/


/*-----�������� ������� � �������� ������������ ������----*/

TDM* DPU5_IN[]={&tm0,NULL};
TDM* DPU5_OUT[]={NULL};



DPU_Set DPU0Gen_Set={(char *)"DPU_IN_terminat",//��������� ��� ����
				  MASTER,//����� ������������: ����-�����������
				  2.9876, //�������� ����� ���� (���)
				  &DPU_Display,//��������� ��������� ������ ����
				  (TDM*(*)[])DPU5_IN, //������� TDM ����
				  (TDM*(*)[])DPU5_OUT//�������� TDM ����
				 };

/*

//������ ���� DPU0 � ��������� �����������
DPUcreate DPU0Gen1_run(&DPU0Gen_Set,//��������� ��� ��������� ������ � ������� ����
				DPU_routine,//��������� ���� ��� ������� ������� � �������� TDM � ������� ����
				sysconf(_SC_PAGESIZE),//������ ����� ��������� � ���� �������� ������
				SCHED_FIFO,//���������� ��������������� "FIFO" - SCHED_FIFO, "�����������"-SCHED_RR
			20//��������� ���� ���������
				);
*/



//----------------------------------------------------------
//----------------------------







#endif /* BASEDPU_H_ */

/*
 * class_DPUcreate.h
 *
 *  Created on: 22.03.2016
 *      Author: admin
 */

#ifndef CLASS_DPUCREATE_H_
#define CLASS_DPUCREATE_H_

#include <pthread.h>
#include <unistd.h>
#include <limits.h>
#include <sys/neutrino.h>
#include <sched.h>
#include <string.h>

class DPUcreate{

	pthread_attr_t attr;//�������� ���� ���� ���������
	struct sched_param param;//�������� ��� ���������� ����
public:
		char *name;//��� ����
		pthread_t DPUtid;//���������� ���� ���� ���������

				// �����������
	DPUcreate(	DPU_Set *DPU,//��������� �������/�������� ������������ ������
				void* (*DPUroutine)(void*),//��������� ��������� ������
				size_t stacksize,//������ ����� ���������
				int policy=SCHED_RR,//���������� ���������������
				int priority=15 //��������� ���� ���������
				)try{

		name=DPU->DPUname;//��� ����
		std::cout << "������������� � �������� ����:  " << name << std::endl;

		//������������� ��������� ���� ����
		if(pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED)!=EOK)/*������ �������� ��������������*/
		throw (char*)"������ ������������� �������� �������������� ���� ����";
//getchar();
		if(pthread_attr_setinheritsched(&attr,	PTHREAD_EXPLICIT_SCHED)!=EOK)/*������ �������� ��������������*/
		throw (char*)"������ ������������� �������� �������������� ���� ����";//����� �� ������������ ������������ ���������
//getchar();
		if(pthread_attr_setschedpolicy(&attr,policy)!=EOK)//������ ���������� ���������������
		throw (char*)"������ ������������� �������� ���������� ��������������� ���� ����";
//getchar();
		param.sched_priority = priority;//����������� ���������
		if(pthread_attr_setschedparam(&attr, &param)!=EOK)//������ ���������
		throw (char*)"������ ������������� �������� ���������� ��������������� ���� ����";
//getchar();
			std::cout << "���� ��������: " << name << "\n";
//getchar();
		pthread_create(&DPUtid,&attr,DPUroutine,(void *)DPU);	//������ ����
	}
	//----------------------------------------------------------------------------------------
	catch(char* msg_error){
			std::cout << name << ": " << msg_error << std::endl;
			exit(EXIT_FAILURE);
	}


	~DPUcreate(){
		free(name);
	}
};

#endif /* CLASS_DPUCREATE_H_ */

/*
 * DPU_routine.h
 *
 *  Created on: 22.03.2016
 *      Author: admin
 */

#ifndef DPU_ROUTINE_H_
#define DPU_ROUTINE_H_

#include <time.h>
#include <sys/siginfo.h>
#include <vector>
#include <time.h>
#include <sys/netmgr.h>
#include <sys/neutrino.h>


#include "class_TimerClock.h"
using namespace std;
sigset_t set;//����� ��������
struct sigevent event;//��� �����������, ���������� - "������"
timer_t	timerid;//�������� �������

double sec;//����� ����� ������ � ������� ������������ ����
double nsec;//������� ����� ������ � ������� � ������������

struct itimerspec period0_alarm;//��������� ������ ������� �����������
struct itimerspec period_alarm;//������� ����������� �����������

//��������� ������������� � ������� ���� �� ����������� DPU_Set
void *DPU_routine(void *pDPU_Set)try{//������� ���� ����
	cout<<"copy load"<<endl;
	DPU_Set *DPUSet=(DPU_Set *)pDPU_Set;//�������������� void * � ��������� �� ��������� ��������� ����

	SIGEV_SIGNAL_INIT(&event, SIGALRM);//��������� ������� ��� ����������� ���� �������� SIGALRM
	timer_create(CLOCK_REALTIME, &event, &timerid);//�������� � ������������� �������
	sigemptyset(&set);//�������� ����� ��������
	sigaddset(&set,SIGALRM);//���������� ������ ����� ��� ������������ �������� ���� ������� SIGALRM
//��������� ����� ������������ �������
	SignalProcmask(0,//������� �������
				   0,//������� ����
				   SIG_SETMASK,//���������� �����
				   &set,
				   NULL);

	double fraction_sec = modf(DPUSet->period, &sec);//sec - ���������� ����� ������, fraction_sec - ���� ������� � �������

	period_alarm.it_value.tv_sec=sec;//��������� ������
	period_alarm.it_value.tv_nsec=fraction_sec*SECOND;

	period_alarm.it_interval.tv_sec=sec;//������ ������
	period_alarm.it_interval.tv_nsec=fraction_sec*SECOND;

//��������� ������� ����� ������� TDid � ������������ ������� �� TD
	vector <TDid*> VecCopy_pTDid_In;//������ ����� ������������ ������� TDM ���� ����
	vector <TDid*> VecCopy_pTDid_Out;//������ ����� ������������ �������� TDM_Out - TDMid_Out ���� ����
	vector <void *> Vec_pTD_In;//������ ��� ����� ���������� pTD ������� ������
	vector <void *> Vec_pTD_Out;//������ ��� ���������� �� ������� TD_In

//	std::cout << "DPU_routine(): ���� �������� �������� ����� ������� TDMid-�� ����: "/* << DPUSet->DPUname*/ << std::endl;

//	double minValid=1;
//	double TDM_In0_interval=((TDM*)(*(DPUSet->In))[0])->interval;//�������� ������������������ TDM_In0

	TDM *pTDM_In;//��������� �� ������� ������� TDM
	TDM *pTDM_Out;//��������� �� ������� �������� TDM

/*****<< ���� ��������� ����� TDid_copy �������� TDM_Out � ����������� �������� ���������� �������� TDid_copy � pTD >>***********/
	TDid *TDid_copy;
	for( unsigned int i=0;(pTDM_Out = (*(DPUSet->Out))[i])!=NULL; i++ ){//pTDM_In - �������� �� ������� TDM_In
//std::cout << "\n<<���� ��������� ����� TDid_copy �������� TDM_Out � ����������� �������� ���������� ������� pTDM_Out � pTD_Value>>" << std::endl;

		TDid_copy=new TDid(pTDM_Out->TDsize);//������� TDid_copy � �������  ������� ������ TDM
		pTDM_Out->Get_TDid(*TDid_copy);//��������� ����� ����������� TDMid �������� TDM_Out

		VecCopy_pTDid_Out.push_back(TDid_copy);//������������ ������� ����� ������������ �������� TDM_Out
		Vec_pTD_Out.push_back(TDid_copy->pTD_value);//������������ ������� ����� ���������� �������� TD_Out ��� ��������� ���������

	}



/*******************************<<<<< ���� ������������ ���� � ������ ������� ��� ����������� >>>**********************************************/
double 	Interval;//�������� �������� ������������ ���� � ����������� � mode
do{
	std::cout << "DPU_routine: �������� � ����� ��������� ����: " << DPUSet->DPUname << std::endl;

	VecCopy_pTDid_In.clear();//�������� �������

//	cout << "DPU_routine: ��� 1\n";

	Vec_pTD_In.clear();
	double minValid=1;//��� ���������� ���������� ������ ������� TD_In[i] ��� ���������� ��������� ����������

//���������� ������ ����������� ���� � ����������� � ������������� mode: MASTER/SLAVE

	if(DPUSet->mode){//����� MASTER
		timer_settime(timerid,
					  0,//���� �������������� ������� ��� ����������� ����� period
					  &period_alarm,//= period, ������������ ������� ������� ����� period
					  NULL);//
		int signo;
		do{
			signo = SignalWaitinfo(&set, NULL);
		}while(signo == -1);
	}
	else{//����� SLAVE
		if((pTDM_In = (*(DPUSet->In))[0])!=NULL){//��������� �� TDM_In[0]
			pTDM_In->Wait_TDM0rewrite_flag();//�������� ����� ���������� TDM0
		}
		else{
			sleep(3);// �������� ������� �� ������� SIGALRM
		}

	}

/*****<< ���� ��������� ����� TDid_copy ������� TDM_In � ����������� �������� ���������� ������� pTDM_In � pTD_Value >>***********/
//TDM *ptr=DPUSet->In;

	for( unsigned int i=0;(pTDM_In = (*(DPUSet->In))[i])!=NULL; i++ ){//pTDM_In - �������� �� ������� TDM_In
//		std::cout << "\n<<���� ��������� ����� TDid_copy ������� TDM_In � ����������� �������� ���������� ������� pTDM_In � pTD_Value>>" << std::endl;
			 TDid_copy=new TDid(pTDM_In->TDsize);//��� �������������� ����������� ����������� TDMid �� TDM ��� ������� � ���.����������
			pTDM_In->Get_TDid(*TDid_copy);//��������� ����� ����������� TDM ������� TDM_In
//		TDid_copy.print();
		/*	void *ptr=malloc(pTDM_In->TDsize);
			memcpy(ptr,pTDM_In->pTD_value,pTDM_In->TDsize);
*/
			cout << "\n��������� ������� ������: "<< pTDM_In->TDM_name << endl;
			pTDM_In->print();
//���������� ����������� ���������� �������� �����������
//������� ����� ����������� ������� ������, ������������ � ��������� ����������
			VecCopy_pTDid_In.push_back(TDid_copy);//������������ ������� ���������� ������� TDMid_copy

	//		std::cout<<ptr<<std::endl;
			//std::cout<<*(double *)TDid_copy->pTD_value<<std::endl;
			Vec_pTD_In.push_back(TDid_copy->pTD_value);//������������ ������� ���������� pTD_In ������� TD
			minValid=min(minValid,TDid_copy->valid);//minValid - ���������� �������� �����������
	}

//	std::cout << "minValid: " << minValid << std::endl;
//#if(0)

//���������� �������� ������������������ �������� ����������� � ������������ � mode ����
//���� ������ ��� �������� period ����-master, ���� ����� �� ����������� TD_id ������� TDM_0 (� �������� 0)
	if(DPUSet->mode) Interval=DPUSet->period;//master - �������� ������������ ����� ������������ �� ����������� ���� DPUSet
	else Interval =((TDM*)(*(DPUSet->In))[0])->interval;//slave - �������� ����������������� �� ����������� TDM_In0

//	std::cout << "interval: " << Interval << std::endl;

/************************************<<< ����� ����� ��������� ���������� >>>**********************************************************/
	std::cout << "DPU_routine(): ����� ����� ��������� ����������: " << DPUSet->DPUname << std::endl;
	//cout << "\n�������� Vec_pTD_Out[0]: " << *(long*)(Vec_pTD_Out[0])<< endl;


					DPUSet->procedure(Vec_pTD_In, Vec_pTD_Out);//
				/*	std::cout<<DPUSet->DPUname<<std::endl;
					std::cout<<"IN"<<std::endl;
					for(int i=0;i<Vec_pTD_In.size();i++){
						std::cout<<i<<" "<<*(double *)Vec_pTD_In[i]<<std::endl;
					}

					std::cout<<"OUT"<<std::endl;
					for(int i=0;i<Vec_pTD_Out.size();i++){
						std::cout<<i<<" "<<*(double *)Vec_pTD_Out[i]<<std::endl;
					}*/
//					cout << "\n��������� ������������ ���� - TD_Value_long : " << *(long*)(Vec_pTD_Out[0])<< endl;

/********<<< ���� ��������� ���������� �������� VecCopy_pTD_Out[i] � �������� TDM_Out >>>******************/

		for(int i=0;(pTDM_Out = (*(DPUSet->Out))[i])!=NULL; i++ ){//pTDM_Out - ��������� �� ������� TDM_Out

			 TDid_copy=new TDid(pTDM_Out->TDsize);//��� �������������� ����������� ����������� TDMid �� TDM ��� ������� � ���.����������
			pTDM_Out->Get_TDid(*TDid_copy);
			TDid_copy->Put_Value(Vec_pTD_Out[i]);
			TDid_copy->interval=Interval;//������� � ����� ����������� ���������� �������� ��������� ������������������
			TDid_copy->valid=minValid;//������� � ����� ����������� ���������� �������� ������������ ���������
//				TDid_copy.print();
				cout << *(double*)TDid_copy->Get_pValue() << endl;

			pTDM_Out->Put_TDid(TDid_copy);	//��������� ����������� ����������� TD ���������� � �������� TDM_Out

			pTDM_Out->print();//������ ����������� �������� TDM_Out
			cout << "\nDPU_routine(): ��������� ������������ ���� � �������� TDM_Out : " << *(double*)(pTDM_Out->pTD_value)<< endl;
		}
//#endif

}while(1);//����������� ����.  ������ ��������� main()
cout << "DPU_routine(): ���� �������� ������" << (char*)(DPUSet->DPUname) << std::endl;//�� ���������� �������

return NULL;//����� �� ����� ������������ ���� do{ ..... }while() �� ������� ���������� ������ ����������;
}
catch(char* msg_error){
		std::cout << "errno = " << errno << ": " << msg_error << std::endl;
return NULL;
}
/*-----------------------------------------------------------------------------------------------------------*/




#endif /* DPU_ROUTINE_H_ */

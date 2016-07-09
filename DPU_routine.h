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
sigset_t set;//набор сигналов
struct sigevent event;//тип уведомления, установить - "сигнал"
timer_t	timerid;//создание таймера

double sec;//целое число секунд в периоде срабатывания узла
double nsec;//дробная часть секунд в периоде в наносекундах

struct itimerspec period0_alarm;//начальный период первого уведомления
struct itimerspec period_alarm;//периоды последующих уведомлений

//процедура инициализации и запуска узла по дескриптору DPU_Set
void *DPU_routine(void *pDPU_Set)try{//функция нити узла
	cout<<"copy load"<<endl;
	DPU_Set *DPUSet=(DPU_Set *)pDPU_Set;//преобразование void * в указатель на структуру настройки узла

	SIGEV_SIGNAL_INIT(&event, SIGALRM);//установка таймера для уведомления узла сигналом SIGALRM
	timer_create(CLOCK_REALTIME, &event, &timerid);//создание и инициализация таймера
	sigemptyset(&set);//очистить маску сигналов
	sigaddset(&set,SIGALRM);//установить разряд маски для блокирования доставки нити сигнала SIGALRM
//установка маски блокирования сигнала
	SignalProcmask(0,//текущий процесс
				   0,//текущая нить
				   SIG_SETMASK,//установить маску
				   &set,
				   NULL);

	double fraction_sec = modf(DPUSet->period, &sec);//sec - количество целых секунд, fraction_sec - доля секунды в периоде

	period_alarm.it_value.tv_sec=sec;//начальный период
	period_alarm.it_value.tv_nsec=fraction_sec*SECOND;

	period_alarm.it_interval.tv_sec=sec;//период циклов
	period_alarm.it_interval.tv_nsec=fraction_sec*SECOND;

//формируем вектора копий входных TDid с собственными копиями их TD
	vector <TDid*> VecCopy_pTDid_In;//вектор копий дескрипторов входных TDM нити узла
	vector <TDid*> VecCopy_pTDid_Out;//вектор копий дескрипторов выходных TDM_Out - TDMid_Out нити узла
	vector <void *> Vec_pTD_In;//вектор для копий указателей pTD входных данных
	vector <void *> Vec_pTD_Out;//вектор для указателей на входные TD_In

//	std::cout << "DPU_routine(): Цикл создания векторов копий входных TDMid-ов узла: "/* << DPUSet->DPUname*/ << std::endl;

//	double minValid=1;
//	double TDM_In0_interval=((TDM*)(*(DPUSet->In))[0])->interval;//интервал репрезентативности TDM_In0

	TDM *pTDM_In;//указатель на текущую входную TDM
	TDM *pTDM_Out;//указатель на текущую выходную TDM

/*****<< Цикл получения копий TDid_copy выходных TDM_Out и простроения векторов указателей выходных TDid_copy и pTD >>***********/
	TDid *TDid_copy;
	for( unsigned int i=0;(pTDM_Out = (*(DPUSet->Out))[i])!=NULL; i++ ){//pTDM_In - указател на текущую TDM_In
//std::cout << "\n<<Цикл получения копий TDid_copy выходных TDM_Out и простроения векторов указателей входных pTDM_Out и pTD_Value>>" << std::endl;

		TDid_copy=new TDid(pTDM_Out->TDsize);//создать TDid_copy с буфером  размера буфера TDM
		pTDM_Out->Get_TDid(*TDid_copy);//получение копии дескриптора TDMid выходной TDM_Out

		VecCopy_pTDid_Out.push_back(TDid_copy);//формирование вектора копий дескрипторов выходных TDM_Out
		Vec_pTD_Out.push_back(TDid_copy->pTD_value);//формирование вектора копий указателей выходных TD_Out для процедуры обработки

	}



/*******************************<<<<< Цикл срабатывания узла в режиме мастера или подчинённого >>>**********************************************/
double 	Interval;//интервал ожидания срабатывания узла в соответсвии с mode
do{
	std::cout << "DPU_routine: Ожидание и вызов процедуры узла: " << DPUSet->DPUname << std::endl;

	VecCopy_pTDid_In.clear();//очистить вектора

//	cout << "DPU_routine: Шаг 1\n";

	Vec_pTD_In.clear();
	double minValid=1;//для вычисления валидности группы входных TD_In[i] как валидности выходного результата

//реализация режима активизации узла в соответсвии с установленной mode: MASTER/SLAVE

	if(DPUSet->mode){//РЕЖИМ MASTER
		timer_settime(timerid,
					  0,//флаг относительного времени для уведомления через period
					  &period_alarm,//= period, планирование посылки сигнала через period
					  NULL);//
		int signo;
		do{
			signo = SignalWaitinfo(&set, NULL);
		}while(signo == -1);
	}
	else{//РЕЖИМ SLAVE
		if((pTDM_In = (*(DPUSet->In))[0])!=NULL){//указатель на TDM_In[0]
			pTDM_In->Wait_TDM0rewrite_flag();//ожидание флага обновления TDM0
		}
		else{
			sleep(3);// ожидание сигнала от таймера SIGALRM
		}

	}

/*****<< Цикл получения копий TDid_copy входных TDM_In и простроения векторов указателей входных pTDM_In и pTD_Value >>***********/
//TDM *ptr=DPUSet->In;

	for( unsigned int i=0;(pTDM_In = (*(DPUSet->In))[i])!=NULL; i++ ){//pTDM_In - указател на текущую TDM_In
//		std::cout << "\n<<Цикл получения копий TDid_copy входных TDM_In и простроения векторов указателей входных pTDM_In и pTD_Value>>" << std::endl;
			 TDid_copy=new TDid(pTDM_In->TDsize);//для промежуточного копирования дескриптора TDMid из TDM без мутекса и усл.переменной
			pTDM_In->Get_TDid(*TDid_copy);//получение копии дескриптора TDM входной TDM_In
//		TDid_copy.print();
		/*	void *ptr=malloc(pTDM_In->TDsize);
			memcpy(ptr,pTDM_In->pTD_value,pTDM_In->TDsize);
*/
			cout << "\nПрочитана входная память: "<< pTDM_In->TDM_name << endl;
			pTDM_In->print();
//определить коэффициент валидности выходных результатов
//минимум среди валидностей входных данных, используемых в получении результата
			VecCopy_pTDid_In.push_back(TDid_copy);//формирование вектора указателей входных TDMid_copy

	//		std::cout<<ptr<<std::endl;
			//std::cout<<*(double *)TDid_copy->pTD_value<<std::endl;
			Vec_pTD_In.push_back(TDid_copy->pTD_value);//формирование вектора указателей pTD_In входных TD
			minValid=min(minValid,TDid_copy->valid);//minValid - валидность выходных результатов
	}

//	std::cout << "minValid: " << minValid << std::endl;
//#if(0)

//определить интервал репрезентативности выходных результатов в соответствии с mode узла
//либо задать как параметр period узла-master, либо взять из дескриптора TD_id входной TDM_0 (с индексом 0)
	if(DPUSet->mode) Interval=DPUSet->period;//master - интервал собственного цикла срабатывания из дескриптора узла DPUSet
	else Interval =((TDM*)(*(DPUSet->In))[0])->interval;//slave - интервал репезентативности из дескриптора TDM_In0

//	std::cout << "interval: " << Interval << std::endl;

/************************************<<< Вызов узлом процедуры вычисления >>>**********************************************************/
	std::cout << "DPU_routine(): Вызов узлом процедуры вычисления: " << DPUSet->DPUname << std::endl;
	//cout << "\nзначение Vec_pTD_Out[0]: " << *(long*)(Vec_pTD_Out[0])<< endl;


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
//					cout << "\nРезультат срабатывания узла - TD_Value_long : " << *(long*)(Vec_pTD_Out[0])<< endl;

/********<<< Цикл занесения полученных значений VecCopy_pTD_Out[i] в выходные TDM_Out >>>******************/

		for(int i=0;(pTDM_Out = (*(DPUSet->Out))[i])!=NULL; i++ ){//pTDM_Out - указатель на текущую TDM_Out

			 TDid_copy=new TDid(pTDM_Out->TDsize);//для промежуточного копирования дескриптора TDMid из TDM без мутекса и усл.переменной
			pTDM_Out->Get_TDid(*TDid_copy);
			TDid_copy->Put_Value(Vec_pTD_Out[i]);
			TDid_copy->interval=Interval;//занести в копию дескриптора результата значение интервала репрезентативности
			TDid_copy->valid=minValid;//занести в копию дескриптора результата значение коэффициента влидности
//				TDid_copy.print();
				cout << *(double*)TDid_copy->Get_pValue() << endl;

			pTDM_Out->Put_TDid(TDid_copy);	//занесение полученного дескриптора TD результата в выходную TDM_Out

			pTDM_Out->print();//печать дескриптора выходной TDM_Out
			cout << "\nDPU_routine(): Результат срабатывания узла в выходной TDM_Out : " << *(double*)(pTDM_Out->pTD_value)<< endl;
		}
//#endif

}while(1);//бесконечный цикл.  Работу завершает main()
cout << "DPU_routine(): Узел завершил работу" << (char*)(DPUSet->DPUname) << std::endl;//не выполнится никогда

return NULL;//выход из цикла срабатывания узла do{ ..... }while() по сигналу завершения работы приложения;
}
catch(char* msg_error){
		std::cout << "errno = " << errno << ": " << msg_error << std::endl;
return NULL;
}
/*-----------------------------------------------------------------------------------------------------------*/




#endif /* DPU_ROUTINE_H_ */

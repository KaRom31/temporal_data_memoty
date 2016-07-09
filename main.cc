/*
 * main.cc
 *
 *  Created on: 22.03.2016
 *      Author: admin
 */
#include <cstdlib>
#include <iostream>
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <stddef.h>
#include <mem.h>
#include <pthread.h>
#include <limits.h>
#include <sys/neutrino.h>
#include <sched.h>
#include <string.h>
#include <vector>
#include <errno.h>


//--------------------------
#include "TYPEdef.h"
//#include "class_TDM.h"
#include "class_TimerClock.h"
#include "BaseTDM.h"
#include "BaseProcedure.h"
#include "DPU_routine.h"
#include "class_DPUcreate.h"
#include "BaseDPU.h"
#include <fstream>


using namespace std;

//Дескрипторы для формирования темпоральных данных
long Value_long=1010;
//------------------------------------------
TDid TD_doubl(sizeof(double));
TDid TD_long(sizeof(long));
//-----------------------------------------


int main(int argc, char *argv[]){
	TimerClock::StartTimerClock();//СТАРТ ЧАСОВ ОТНОСИТЕЛЬНОГО РЕАЛЬНОГО ВРЕМЕНИ

/*
char *str=argv[2];
cout<<argv[1]<<endl;
cout<<argv[2]<<endl;
SharedMemory ggg(argv[1]);
int tt=atoi(argv[2]);
ggg.setLenghtMemory(tt);*/

/*
SharedMemory strr0("/trr0_d");
strr0.setLenghtMemory(sizeof(TDM)+sizeof(double));
SharedMemory strr1("/trr1_d");
strr1.setLenghtMemory(sizeof(TDM)+sizeof(double));

TDM *trr0=new TDM((char*)"tm0_double", sizeof(double),PTHREAD_PROCESS_PRIVATE,strr0.getDescriptorMemory());
TDM *trr2=new TDM((char*)"tm2_double", sizeof(double),PTHREAD_PROCESS_PRIVATE,strr1.getDescriptorMemory());

TDM* DPU0_IN[]={NULL};
TDM* DPU0_OUT[]={trr0,NULL};


TDM* DPU1_IN[]={trr2,NULL};
TDM* DPU1_OUT[]={NULL};



TDM* DPU2_IN[]={trr0,NULL};
TDM* DPU2_OUT[]={trr2,NULL};


DPU_Set DPU0Gen_Set={(char *)"DPU_IN_Generator",//системное имя узла
				  MASTER,//режим срабатывания: узел-подчинённый
				  1.5, //интервал цикла узла (сек)
				  &DPU0,//процедура обработки данных узла
				  (TDM*(*)[])DPU0_IN, //входные TDM узла
				  (TDM*(*)[])DPU0_OUT//выходные TDM узла
				 };

//запуск узла DPU_In_Out с заданными настройками
DPUcreate *DPU0Gen_run=new DPUcreate(&DPU0Gen_Set,//структура для установки связей и свойств узла
				DPU_routine,//системная нить для привзки входных и выходных TDM и запуска узла
				sysconf(_SC_PAGESIZE),//размер стека процедуры в одну страницу памяти
				SCHED_FIFO,//дисциплина диспетчеризации "FIFO" - SCHED_FIFO, "карусельная"-SCHED_RR
				8//приоритет узла обработки
				);


DPU_Set DPU1Gen_Set={(char *)"DPU_IN_Generator1",//системное имя узла
				  MASTER,//режим срабатывания: узел-подчинённый
				  1.5, //интервал цикла узла (сек)
				  &DPU_Display,//процедура обработки данных узла
				  (TDM*(*)[])DPU1_IN, //входные TDM узла
				  (TDM*(*)[])DPU1_OUT//выходные TDM узла
				 };

//запуск узла DPU_In_Out с заданными настройками
DPUcreate *DPU1Gen_run=new DPUcreate(&DPU1Gen_Set,//структура для установки связей и свойств узла
				DPU_routine,//системная нить для привзки входных и выходных TDM и запуска узла
				sysconf(_SC_PAGESIZE),//размер стека процедуры в одну страницу памяти
				SCHED_FIFO,//дисциплина диспетчеризации "FIFO" - SCHED_FIFO, "карусельная"-SCHED_RR
				8//приоритет узла обработки
				);


DPU_Set DPU2_Set={(char *)"DPU2",//системное имя узла
				  SLAVE,//режим срабатывания: узел-подчинённый
				  NULL, //интервал цикла узла (сек)
				  &DPU3,//процедура обработки данных узла
				  (TDM*(*)[])DPU2_IN, //входные TDM узла
				  (TDM*(*)[])DPU2_OUT//выходные TDM узла
				 };

//запуск узла DPU_In_Out с заданными настройками
DPUcreate *DPU2_run=new DPUcreate(&DPU2_Set,//структура для установки связей и свойств узла
				DPU_routine,//системная нить для привзки входных и выходных TDM и запуска узла
				sysconf(_SC_PAGESIZE),//размер стека процедуры в одну страницу памяти
				SCHED_FIFO,//дисциплина диспетчеризации "FIFO" - SCHED_FIFO, "карусельная"-SCHED_RR
				7//приоритет узла обработки
				);

*/
/*

//формироание структуры связей, процедуры обработки и режима срабатывания узла
DPU_Set DPU_IN_Set={(char *)"DPU_IN",//системное имя узла
				  MASTER,//режим срабатывания: узел-подчинённый
				  3, //интервал цикла узла (сек)
				  &DPU_OUT,//процедура обработки данных узла
				  (TDM*(*)[])DPU0_OUT_In, //входные TDM узла
				  (TDM*(*)[])EMPTY//выходные TDM узла
				 };

//запуск узла DPU_In_Out с заданными настройками
DPUcreate *DPU_In_run = new
 DPUcreate(&DPU_IN_Set,//структура для установки связей и свойств узла
				DPU_routine,//системная нить для привзки входных и выходных TDM и запуска узла
				sysconf(_SC_PAGESIZE),//размер стека процедуры в одну страницу памяти
				SCHED_FIFO,//дисциплина диспетчеризации "FIFO" - SCHED_FIFO, "карусельная"-SCHED_RR
				5//приоритет узла обработки
				);

*/

//Initialize();
//InitializationTDM::getInitialiation();


//shar1.setLenghtMemory(sizeof(TDM)+sizeof(double));
//		tdm1=new (shar1.putTDM())TDM((char*)"tdm1_double", sizeof(double),PTHREAD_PROCESS_SHARED,shar1.getDescriptorMemory());
		//tdm1->print();


		//TDM *ptr=shar1.putTDM();
				//(char *)mmap(0, sizeof(TDM), PROT_WRITE|PROT_READ, MAP_SHARED,shar1.getDescriptorMemory(), 0);
	/*	TDM tmempt((char*)"tm5_double", sizeof(double),PTHREAD_PROCESS_PRIVATE);

		TDM *DPU0_OUT_In[]={&tmempt,NULL};
		TDM *EMPTY[]={NULL};

		DPU_Set DPU_IN_Set={(char *)"DPU_IN",//системное имя узла
						  MASTER,//режим срабатывания: узел-подчинённый
						  1.101, //интервал цикла узла (сек)
						  &DPU_OUT,//процедура обработки данных узла
						  (TDM*(*)[])DPU0_OUT_In, //входные TDM узла
						  (TDM*(*)[])EMPTY//выходные TDM узла
						 };

		//запуск узла DPU_In_Out с заданными настройками
		DPUcreate *dcrin=new DPUcreate(&DPU_IN_Set,//структура для установки связей и свойств узла
				DPU_routine,//системная нить для привзки входных и выходных TDM и запуска узла
				sysconf(_SC_PAGESIZE),//размер стека процедуры в одну страницу памяти
				SCHED_FIFO,//дисциплина диспетчеризации "FIFO" - SCHED_FIFO, "карусельная"-SCHED_RR
				5//приоритет узла обработки
				);
*/
/*
		TDM tm10((char*)"tm10pppp", sizeof(double),PTHREAD_PROCESS_PRIVATE);
		TDM *DPU_OUT_In[]={tdm1,NULL};
		TDM *EMPTY1[]={NULL};
*/
		//формироание структуры связей, процедуры обработки и режима срабатывания узла
		/*DPU_Set DPU_IN_Set={(char *)"DPU_INpppp",//системное имя узла
						  MASTER,//режим срабатывания: узел-подчинённый
						  1.201, //интервал цикла узла (сек)
						  &DPU_OUT,//процедура обработки данных узла
						  (TDM*(*)[])DPU_OUT_In, //входные TDM узла
						  (TDM*(*)[])EMPTY1//выходные TDM узла
						 };*/

	/*	DPU_Set dsetin;
		dsetin.DPUname=(char *)"DPU_INpppp";
		dsetin.mode=MASTER;
		dsetin.period=1.201;
		dsetin.procedure=&DPU_OUT;
		dsetin.In=(TDM*(*)[])DPU_OUT_In;
		dsetin.Out=(TDM*(*)[])EMPTY1;
*/
		/*
		//запуск узла DPU_In_Out с заданными настройками
		DPUcreate DPU_In_run(&dsetin,//структура для установки связей и свойств узла
						DPU_routine,//системная нить для привзки входных и выходных TDM и запуска узла
						sysconf(_SC_PAGESIZE),//размер стека процедуры в одну страницу памяти
						SCHED_FIFO,//дисциплина диспетчеризации "FIFO" - SCHED_FIFO, "карусельная"-SCHED_RR
						5//приоритет узла обработки
						);
		*/
	/*	DPUcreate *dcrin=new DPUcreate(&dsetin,//структура для установки связей и свойств узла
				DPU_routine,//системная нить для привзки входных и выходных TDM и запуска узла
				sysconf(_SC_PAGESIZE),//размер стека процедуры в одну страницу памяти
				SCHED_FIFO,//дисциплина диспетчеризации "FIFO" - SCHED_FIFO, "карусельная"-SCHED_RR
				5//приоритет узла обработки
				);
*/


/*
TDM tm10((char*)"tm10pppp", sizeof(double),PTHREAD_PROCESS_PRIVATE);
TDM *DPU_OUT_In[]={&tm10,NULL};
TDM *EMPTY1[]={NULL};

//формироание структуры связей, процедуры обработки и режима срабатывания узла
DPU_Set DPU_IN_Set={(char *)"DPU_INpppp",//системное имя узла
				  MASTER,//режим срабатывания: узел-подчинённый
				  1.201, //интервал цикла узла (сек)
				  &DPU_OUT,//процедура обработки данных узла
				  (TDM*(*)[])DPU_OUT_In, //входные TDM узла
				  (TDM*(*)[])EMPTY1//выходные TDM узла
				 };
DPUcreate *dcrin=new DPUcreate(&DPU_IN_Set,//структура для установки связей и свойств узла
		DPU_routine,//системная нить для привзки входных и выходных TDM и запуска узла
		sysconf(_SC_PAGESIZE),//размер стека процедуры в одну страницу памяти
		SCHED_FIFO,//дисциплина диспетчеризации "FIFO" - SCHED_FIFO, "карусельная"-SCHED_RR
		5//приоритет узла обработки
		);*/
/*DPU_Set dsetin;
dsetin.DPUname=(char *)"DPU_INpppp";
dsetin.mode=MASTER;
dsetin.period=1.201;
dsetin.procedure=&DPU_OUT;
dsetin.In=(TDM*(*)[])DPU_OUT_In;
dsetin.Out=(TDM*(*)[])EMPTY1;
*/
/*
//запуск узла DPU_In_Out с заданными настройками
DPUcreate DPU_In_run(&dsetin,//структура для установки связей и свойств узла
				DPU_routine,//системная нить для привзки входных и выходных TDM и запуска узла
				sysconf(_SC_PAGESIZE),//размер стека процедуры в одну страницу памяти
				SCHED_FIFO,//дисциплина диспетчеризации "FIFO" - SCHED_FIFO, "карусельная"-SCHED_RR
				5//приоритет узла обработки
				);
*/



	/*shar1.setLenghtMemory(sizeof(TDM)+sizeof(double));
		char *ptr=(char *)mmap(0, sizeof(TDM), PROT_WRITE|PROT_READ, MAP_SHARED,shar1.getDescriptorMemory(), 0);
		TDM *tdm123=new TDM((char*)"tdm1_double", sizeof(double),PTHREAD_PROCESS_SHARED,shar1.getDescriptorMemory());
		tdm123->print();
		tdm123->print();
		tdm123=shar1.putTDM(tdm123);
		tdm123->print();*/
//do{
	/*std::cout<<"main 20"<<std::endl;
	sleep(20);
	ofstream ft;
	ft.open("temporal.txt");
	ft<<"hello";
	ft.close();
*/
//}while(1);
/*
int err=spawnl(P_NOWAIT,"/home/host/procedure1","/home/host/procedure1",NULL);
if(err==-1) perror("error");
err=spawnl(P_NOWAIT,"/home/host/procedure2","/home/host/procedure2",NULL);
if(err==-1) perror("error");*/
char *e;
cout<<"hello";
cin>>e;
system("pause");
return EXIT_SUCCESS;
}





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

//процедура вычислений узла DPU0

double L=0;

int DPU0_Proc(vector <void *> &Vec_pTD_In, vector <void *> &Vec_pTD_Out){;
//Прочитать содержимое TDM
//double In0_Value=*(double*)(Vec_pTD_In[0]);

long Out0_Value=-123;
	cout << "\nВызвана процедура DPU0_Proc" << endl;
		//	cout	<< "DPU0_Proc: Получено входное значение Vec_pTD_In[0] = " << In0_Value << endl;

		sleep(1);//Моделирует обработку данных
		//Out0_Value= (long)(1000.3*In0_Value);//результат выполнения DPU0_Proc

		//Поместить результат обработки в выходные TDM-объекты
		memcpy(Vec_pTD_Out[0], (void *)&Out0_Value, sizeof (Out0_Value));
//		cout 	<< "DPU0_Proc: Результат выполнения Vec_pTD_Out[0] = " << *(long*)(Vec_pTD_Out[0]) <<endl;

		cout 	<< "             Выполнена процедура обработки узла " << std::endl;
errno=1;
if(errno)return EXIT_SUCCESS;
else return EXIT_FAILURE;
}

//вычисление порции воды DPU2
int DPU2(vector <void *> &Vec_pTD_In, vector <void *> &Vec_pTD_Out){;
	//Прочитать содержимое TDM
	double delta=*(double*)(Vec_pTD_In[0]);
	double alpha=*(double*)(Vec_pTD_In[1]);
			//10-L/10;
			//

		double volume=0;
		volume=alpha*delta;

	memcpy(Vec_pTD_Out[0], (void *)&volume, sizeof (volume));

		cout<<"DPU2 OUT!!! -- "<<volume<<endl;
		cout 	<< "             Выполнена процедура обработки узла " << std::endl;
		sleep(2);
errno=1;
if(errno)return EXIT_SUCCESS;
else return EXIT_FAILURE;
}



//DPU6 Копирование уровня воды L из TM3 в TM4 и TM6
int DPU_Replicator(vector <void *> &Vec_pTD_In, vector <void *> &Vec_pTD_Out){;
	//Прочитать содержимое TDM
	double level=*(double*)(Vec_pTD_In[0]);
		//Поместить результат обработки в выходные TDM-объекты

		memcpy(Vec_pTD_Out[0], (void *)&level, sizeof (level));
		memcpy(Vec_pTD_Out[1], (void *)&level, sizeof (level));

		cout<<"LEVEL REPLICATOR!!! -- "<<level<<endl;
		cout 	<< "             Выполнена процедура обработки узла " << std::endl;
		sleep(3);
errno=1;
if(errno)return EXIT_SUCCESS;
else return EXIT_FAILURE;
}


//DPU3 - накопитель уровня воды
//получение TM2 и копирование результата в TM3
int DPU3(vector <void *> &Vec_pTD_In, vector <void *> &Vec_pTD_Out){;
		//Прочитать содержимое TDM
		double volume=*(double*)(Vec_pTD_In[0]);
		L+=volume;
		//Поместить результат обработки в выходные TDM-объекты
		cout<<"DPU3 IN!!! -- "<<volume<<endl;
		memcpy(Vec_pTD_Out[0], (void *)&L, sizeof (L));
		cout<<"DPU3 OUT SUM LEVEL!!! -- "<<L<<endl;
		cout 	<< "             Выполнена процедура обработки узла " << std::endl;
		sleep(2);
errno=1;
if(errno)return EXIT_SUCCESS;
else return EXIT_FAILURE;
}

//DPU4 - вычисление альфа для регулирования подачи воды
//по значениям из L(TM4) и P(TM1) и занесение в TM5
int DPU4(vector <void *> &Vec_pTD_In, vector <void *> &Vec_pTD_Out){;
		//Прочитать содержимое TDM
		double Level=*(double*)(Vec_pTD_In[0]);
		double Volume=//10.0;
				*(double*)(Vec_pTD_In[1]);
				//10;
				//

		double alpha=0;
		if(Volume!=0)
		alpha=(Volume-Level)/Volume;
		cout<<"DPU4!!! -- "<<Level<<" -- "<<Volume<<endl;
		//Поместить результат обработки в выходные TDM-объекты

		memcpy(Vec_pTD_Out[0], (void *)&alpha, sizeof (alpha));
		cout<<"DPU4!! -- "<<alpha<<endl;
		cout 	<< "             Выполнена процедура обработки узла " << std::endl;
		sleep(3);
errno=1;
if(errno)return EXIT_SUCCESS;
else return EXIT_FAILURE;
}

//генератор подачи воды
//DPU0
int DPU0(vector <void *> &Vec_pTD_In, vector <void *> &Vec_pTD_Out){;
		//Прочитать содержимое TDM
		double delta=0.5;

		//Поместить результат обработки в выходные TDM-объекты

		memcpy(Vec_pTD_Out[0], (void *)&delta, sizeof (delta));
		cout 	<< " DPUO OUT "<<*(double*)Vec_pTD_Out[0]<<endl;
		cout 	<< "             Выполнена процедура обработки узла ГЕНЕРАТОР--" <<delta <<std::endl;
sleep(2);
errno=1;
if(errno)return EXIT_SUCCESS;
else return EXIT_FAILURE;
}

//DPU1 - генератор задания максимального уровня воды
int DPU1(vector <void *> &Vec_pTD_In, vector <void *> &Vec_pTD_Out){;
		//Прочитать содержимое TDM
		double volume=10.0;
		cout<<"DPU1 !!! VOLUME -- "<<volume<<endl;
		//Поместить результат обработки в выходные TDM-объекты
		memcpy(Vec_pTD_Out[0], (void *)&volume, sizeof (volume));
		cout 	<< " DPU1 OUT "<<*(double*)Vec_pTD_Out[0]<<endl;
		cout<<"Gen 1111!!!!! ---- "<<endl;
		cout 	<< "             Выполнена процедура обработки узла " << std::endl;
		sleep(2);
errno=1;
if(errno)return EXIT_SUCCESS;
else return EXIT_FAILURE;
}

//DPU5 - темнатор, вывод уровня воды на экран
int DPU_Display(vector <void *> &Vec_pTD_In, vector <void *> &Vec_pTD_Out){;


//Прочитать содержимое TDM
double Volume=*(double*)(Vec_pTD_In[0]);

		//Поместить результат обработки в выходные TDM-объекты

cout<<"Display!!! LEVEL -- "<<Volume<<endl;

		//memcpy(Vec_pTD_Out[0], (void *)&Volume, sizeof (Volume));
		cout 	<< "             Выполнена процедура обработки узла " << std::endl;
		sleep(2);
errno=1;
if(errno)return EXIT_SUCCESS;
else return EXIT_FAILURE;
}


#endif /* BASEPROCEDURE_H_ */

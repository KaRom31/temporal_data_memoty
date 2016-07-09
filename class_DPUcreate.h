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

	pthread_attr_t attr;//атрибуты нити узла обработки
	struct sched_param param;//параметр для приоритета узла
public:
		char *name;//имя узла
		pthread_t DPUtid;//дескриптор нити узла обработки

				// Конструктор
	DPUcreate(	DPU_Set *DPU,//структура входной/выходной темпоральной памяти
				void* (*DPUroutine)(void*),//процедура обработки данных
				size_t stacksize,//размер стека процедуры
				int policy=SCHED_RR,//дисциплина диспетчеризации
				int priority=15 //приоритет узла обработки
				)try{

		name=DPU->DPUname;//имя узла
		std::cout << "Инициализация и загрузка узла:  " << name << std::endl;

		//инициализация атрибутов нити узла
		if(pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED)!=EOK)/*Задать свойство обособленности*/
		throw (char*)"Ошибка инициализации атрибута обособленности нити узла";
//getchar();
		if(pthread_attr_setinheritsched(&attr,	PTHREAD_EXPLICIT_SCHED)!=EOK)/*задать свойство обособленности*/
		throw (char*)"Ошибка инициализации атрибута обособленности нити узла";//отказ от наследования родительских атрибутов
//getchar();
		if(pthread_attr_setschedpolicy(&attr,policy)!=EOK)//задать дисциплину диспетчеризации
		throw (char*)"Ошибка инициализации атрибута дисциплины диспетчеризации нити узла";
//getchar();
		param.sched_priority = priority;//Подготовить приоритет
		if(pthread_attr_setschedparam(&attr, &param)!=EOK)//задать приоритет
		throw (char*)"Ошибка инициализации атрибута дисциплины диспетчеризации нити узла";
//getchar();
			std::cout << "Узел загружен: " << name << "\n";
//getchar();
		pthread_create(&DPUtid,&attr,DPUroutine,(void *)DPU);	//запуск узла
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

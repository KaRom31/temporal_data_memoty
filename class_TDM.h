/*
 * class_TDM.h
 *
 *  Created on: 22.03.2016
 *      Author: admin
 */

#ifndef CLASS_TDM_H_
#define CLASS_TDM_H_

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
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>


using namespace std;

#include "class_TimerClock.h"

/*******************************************<<<< Typedef.h >>>>*************************************/
//Структура TDid, не влючает mutex и condvar,
//используется для создания и печати содержимого TDid,
//используется объектами TDM для получения копии и обновления содержимого TDM

struct TDid{
		char TDM_name[MAXname];//имя TDM из которого извлекли дескриптор TDid
		char TD_name[MAXname];//имя полученного темпорального данного
		size_t TDsize;
		void *pTD_value;	//указатель выделенного буфера хранения TD_value
		double interval; 	//интервал репрезентативности TD
		double valid;       //степень валидности TD
		long double TickMark;//метка времени получения TD

//----------------------------
		TDid(size_t TD_size){
			TDsize = TD_size;

			pTD_value=malloc(TDsize);//создание динамической области для хранения TD

			int len=strlen("TDM_name blank");
			len >= MAXname ? len=MAXname-1 : len;
			strncpy(TDM_name, (char*)"TDM_name blank", len);
			*(TDM_name+len)='\0';

			len=strlen("TD_name blank");
			len >= MAXname ? len=MAXname-1 : len;
			strncpy(TD_name, (char*)"TD_name blank", len);
			*(TD_name+len)='\0';
		}
//-------------------------
		void SetTDid(char *TDMname,
				     char *TDname,
					 size_t TD_size,
					 void *pValue,
					 double intervl,
					 double vald=1,
					 long double TickMark=TimerClock::GetTickmark()
					)
		{
			int len=strlen(TDMname);
			len >= MAXname ? len=MAXname-1 : len;
			strncpy(TDM_name, (char*)TDMname, len);
			*(TDM_name+len)='\0';

			len=strlen(TDname);
			len >= MAXname ? len=MAXname-1 : len;
			strncpy(TD_name, (char*)TDname, len);
			*(TD_name+len)='\0';

			if(TDsize >= TD_size) memcpy(pTD_value,pValue,TDsize);
			else {cout << "Ошибка SetTDid" << endl; exit(EXIT_FAILURE);};

			TickMark = TimerClock::GetTickmark();
			interval = intervl;
			valid = vald;
		}
		//------------------
		void *Get_pValue(){//получить указатель на TD из дескриптора
			return (void*)pTD_value;
		}
		void print(){
			cout << "\n********Дескриптор темпорального данного**************"
			     << "\nTDM_name = " << TDM_name
			     << "\nTD_name = " << TD_name
			     << "\nTDsize = " << TDsize
				 << "\ninterval = " << interval
				 << "\nvalid = " << valid
				 << "\nTickMark = " << TickMark
				 << endl;
		}
		//------------------
		TDid* Put_Value(void* pTD_src){//заменить значение TD в дескрипторе TDid
			memcpy(pTD_value, pTD_src, TDsize);
		return this;
		}
//-----------------------------
		~TDid(){
			free(pTD_value);//удаление динамической области для хранения TD
		}
};

/*******************************************<<<< class_TDM.h >>>>*************************************/
class TDM {
public:

	//SharedMemory *shar;
	pthread_mutex_t *mutex;	//мутекс доступа к TDM
	pthread_cond_t *condvar;	//точки ожидания уведомления о доступе к TDM
	pthread_mutexattr_t mutex_attr;
	pthread_condattr_t condvar_attr;
	int pshared;//PTHREAD_PROCESS_PRIVATE/PTHREAD_PROCESS_SHARED
//-----------------------------------------------------------------------------
	char DPUprovider_name[MAXname];
	char TDM_name[MAXname];
	char TD_name[MAXname];
	size_t TDsize;
	void *pTD_value;	//буфер хранения TD_value
	double interval; 	//интервал репрезентативности TD
	double valid;       //степень валидности TD
	long double TickMark;//метка времени обновления TDM
	bool rewrite_flag;	//флаг обновления TD
//------------------------------------------------------------------------------------
//инициализация мутекса
void mutex_init(int pshared){
	//cout << "Инициализация мутекса TDM: " << TDM_name <<endl;// getchar();

		if(pthread_mutexattr_init(&mutex_attr)!=EOK)//инициализация атрибутов мутекса
				throw (char*)"Ошибка инициализации атрибутной записи мутекса";
		if(pthread_mutexattr_setpshared(&mutex_attr, pshared)!=EOK)//установка свойства мутекса
				throw (char*) "Ошибка установки свойств мутекса";
		if(pthread_mutex_init (mutex, &mutex_attr)!=EOK)//инициализация атрибутов мутекса
				throw (char*) "Ошибка инициализации блокировки мутекса";
//		pthread_mutexattr_destroy(&mutex_attr);//удаление атрибутной записи мутекса
		//cout << "Создан и инициализирован мутекс TDM: " << TDM_name <<endl;
return;
}

//инициализация точки ожидания уведомлений
void condvar_init(int pshared){
	//cout << "Инициализация condvar TDM: " << TDM_name <<endl;// getchar();
	int c1,c2,c3;
		if((c1=pthread_condattr_init(&condvar_attr))!=EOK)//инициализация атрибутов точки ожидания
				throw (char*)"Ошибка инициализации атрибутов точки ожидания уведомлений";
		if((c2=pthread_condattr_setpshared(&condvar_attr, pshared))!=EOK)//установка свойств точки ожидания
				throw (char*) "Ошибка установки свойств точки ожидания уведомлений";
		if((c3=pthread_cond_init(condvar, &condvar_attr))!=EOK)//инициализация точки ожидания
				throw (char*) "Ошибка инициализации condvar";
//		pthread_condattr_destroy(&condvar_attr);//удаление атрибутной записи точки ожидания

	//cout << "Создана и инициализирована condvar TDM: " << TDM_name <<endl;
return;
}

// конструкторы TDM_id --------------------------------------------------------

	//-----------------constructor sharmem-----------------


	TDM(char* TDMname,
		size_t TypeTD_size,
		int shared,//признак разделяемости: PTHREAD_PROCESS_PRIVATE/PTHREAD_PROCESS_SHARED
		bool rw_flag=0,
		double interv=0,
		double vld=0
	)try{


		int len=strlen("DPUprovider_name blank");
		len >= MAXname ? len=MAXname-1 : len;
		strncpy(DPUprovider_name, (char*)"DPUprovider_name blank", len);
		*(DPUprovider_name+len)='\0';

		len=strlen(TDMname);
		len >= MAXname ? len=MAXname-1 : len;
		strncpy(TDM_name, (char*)TDMname, len);
		*(TDM_name+len)='\0';

		len=strlen("TD_name");
		len >= MAXname ? len=MAXname-1 : len;
		strncpy(TD_name, (char*)"TD_name blank", len);
		*(TD_name+len)='\0';
		rewrite_flag=rw_flag;
		TickMark=0.0;
		interval=interv;
		valid=vld;
if(shared== PTHREAD_PROCESS_PRIVATE){

	mutex=(pthread_mutex_t *)malloc(sizeof(pthread_mutex_t ));
	condvar=(pthread_cond_t *)malloc(sizeof(pthread_cond_t ));
		TDsize=TypeTD_size;
		pTD_value= malloc(TypeTD_size);


			//cout << "Инициализация TDM: " << TDMname <<endl;// getchar();
					mutex_init(shared);
					condvar_init(shared);
			//cout << "Создана и инициализирована TDM: " << TDM_name <<endl;
			}
			else{
				//shar=new SharedMemory(TDMname);
				TDsize=TypeTD_size;
				int descriptor;
				int error;
				//if(
				descriptor=shm_open(TDM_name, O_CREAT|O_RDWR, S_IRWXO|S_IRWXG|S_IRWXU);
				if(descriptor==-1)perror("error shm_open");
					//perror("error shm_open");
				int size=sizeof(pthread_mutex_t)+sizeof(pthread_condattr_t)+TDsize;
				//if(error=ftruncate(descriptor,sizeof(pthread_mutex_t)+sizeof(pthread_condattr_t)+TDsize)==-1);
				ftruncate(descriptor,size);


				mutex=//(pthread_mutex_t *)shr->getMMAP(0,sizeof(pthread_mutex_t));
						(pthread_mutex_t *)mmap(0, sizeof(pthread_mutex_t), PROT_WRITE|PROT_READ, MAP_SHARED,descriptor, 0);
				condvar=//(pthread_cond_t *)shr->getMMAP(sizeof(pthread_mutex_t),sizeof(pthread_condattr_t));
						(pthread_cond_t *)mmap(0, sizeof(pthread_condattr_t), PROT_WRITE|PROT_READ, MAP_SHARED,descriptor,sizeof(pthread_mutex_t) );
				pTD_value=//shr->getMMAP(sizeof(pthread_mutex_t)+sizeof(pthread_cond_t),TDsize);
						mmap(0, TDsize, PROT_WRITE|PROT_READ, MAP_SHARED,descriptor,sizeof(pthread_mutex_t)+sizeof(pthread_cond_t) );



				pthread_cond_destroy(condvar);
				pthread_mutex_destroy(mutex);

				mutex_init(shared);

				condvar_init(shared);

			}
	}
//----------------------------------------------------------------------------------------
	catch(char* msg_error){
		std::cout << TDM_name << ": " << msg_error << std::endl;
		exit(EXIT_FAILURE);
	}


	// Методы TDM

/******************<< Wait_rewrite_flag() - ожидание флага обновления темпоральной памяти >>******************/
	void Wait_TDM0rewrite_flag()try{

		if((errno=pthread_mutex_lock(mutex))!=EOK)//ожидание и захват мутекса для записи
		throw (char*) "Ошибка захвата мутекса методом Wait_rewrite_flag()";

			while(!rewrite_flag){
				pthread_cond_wait(condvar, mutex);
			}

		if((errno=pthread_mutex_unlock(mutex))!=EOK)//освобождение мутекса
		throw (char*) "Ошибка освобождения мутекса методом Wait_rewrite_flag()";
	}
	catch(char* msg_error){
		std::cout << TDM_name << ": " << msg_error << std::endl;
		exit(EXIT_FAILURE);
	}


/*************<< Put_TDid() - занесение дескриптора темпорального данного в темпоральную память >>***********/

  	void Put_TDid(TDid *TD)try{

//  		std::cout << "Вызван метод Put_TDid():" << std::endl;
if((errno=pthread_mutex_lock(mutex))!=EOK)//ожидание и захват мутекса для записи
throw (char*) "Ошибка захвата мутекса методом Put_TDid(TDid TD)";

			strcpy(TD_name,TD->TD_name);

			if(TD->TDsize)memcpy(pTD_value,TD->pTD_value,TDsize);

			TickMark = TimerClock::GetTickmark();//метка времени обновления темпорального данного
			interval = TD->interval;//интервал репрезентативности темпорального данного
			valid = TD->valid;//валидность темпорального данного
			rewrite_flag=true;//установлен флаг обновления
			pthread_cond_broadcast(condvar);
if((errno=pthread_mutex_unlock(mutex))!=EOK)//освобождение мутекса
throw (char*) "Ошибка освобождения мутекса методом Put_TDid(TDid TD)";
//std::cout << "Освобождение мутекса методом PutTDinTDM()" << std::endl;
	}
  	catch(char* msg_error){
  		std::cout << "errno = " << errno << ": " << msg_error << std::endl;
		exit(EXIT_FAILURE);
  	}

/***************************<< Get_TDid(TDid &TD) - получить копию дескриптора TD из TDM >>***************************/

	void Get_TDid(TDid &TD)try{
		if((errno=pthread_mutex_lock(mutex))!=EOK)//ожидание и захват мутекса для записи
		throw (char*) "Ошибка захвата мутекса методом Get_TDid(TDid &TD)";

//		int len=strlen(TD.TDM_name);
//		len >= MAXname ? len=MAXname-1 : len;
//		strncpy(TD.TDM_name, TDM_name,  len);
		strcpy(TD.TDM_name, TDM_name);

/*		len=strlen(TD.TD_name);
		len >= MAXname ? len=MAXname-1 : len;
		strncpy(TD.TD_name, TD_name,  len); */
		strcpy(TD.TD_name, TD_name);

		memcpy(TD.pTD_value,pTD_value,TD.TDsize);
		//TD.pTD_value=pTD_value;
		TD.TickMark = TickMark;
		TD.interval = interval;
		TD.valid = valid;

		double now_time=TimerClock::GetTickmark(); //текущий момент времени
		if(now_time <= TickMark + interval)  TD.valid=valid;// текущее время внутри периода репрезентативности
 	 	else TD.valid=valid*(interval/(now_time - TickMark));// текущее время вышло из периода репрезентативности
		rewrite_flag=false;

		if((errno=pthread_mutex_unlock(mutex))!=EOK)//освобождение мутекса
		throw (char*) "Ошибка освобождения мутекса методом Get_TDid(TDid &TD)";
	}
  	catch(char* msg_error){
  		std::cout << "errno = " << errno << ": " << msg_error << std::endl;
		exit(EXIT_FAILURE);
  	}

/***************************<< GetTD_pValu() - получить копию значени TD из TDM >>***************************/
	void *GetTD_pValue()try{

		if((errno=pthread_mutex_lock(mutex))!=EOK)//ожидание и захват мутекса для записи
		throw (char*) "Ошибка захвата мутекса методом GetTD_pValue()";
			void *now_pTD_value=(void*)pTD_value;
		if((errno=pthread_mutex_unlock(mutex))!=EOK)//освобождение мутекса
		throw (char*) "Ошибка освобождения мутекса методом GetTD_pValue()";
	return now_pTD_value;
	}
  	catch(char* msg_error){
  		std::cout << "errno = " << errno << ": " << msg_error << std::endl;
		exit(EXIT_FAILURE);
  	}
  	//копирование значений полей темпоральной памяти
  			void copyInShared(TDM *value ){

  				memcpy(value->TDM_name,(char *)TDM_name,sizeof(TDM_name));
  				value->interval=interval;
  				value->rewrite_flag=rewrite_flag;
  				value->TDsize=TDsize;

  				value->mutex=mutex;
  				value->mutex_attr=mutex_attr;

  				value->pTD_value=malloc(TDsize);

  				pthread_mutexattr_setpshared(&value->mutex_attr,PTHREAD_PROCESS_SHARED);

  				/*value->mutex_init(PTHREAD_PROCESS_SHARED);
  				value->condvar_init(PTHREAD_PROCESS_SHARED);*/

  				/*value->mutex=mutex;
  				value->mutex_attr=mutex_attr;
  				pthread_mutexattr_setpshared(&value->mutex_attr,PTHREAD_PROCESS_SHARED);*/

  			}
  			void copyValueInShared(void *value){
  				memcpy(pTD_value,value,TDsize);
  			}
//-------------------------- распечатать содержимое дескриптора
		void print(){
			cout << "\n*********** Дескриптор темпоральной памяти *****************"
				 << "\nDPUprovider_name = " << DPUprovider_name
			     << "\nTDM_name = " << TDM_name
			     << "\nTD_name = " << TD_name
			     << "\nTDsize = " << TDsize
				 << "\ninterval = " << interval
				 << "\nvalid = " << valid
				 << "\nTickMark = " << TickMark
				 << "\nrewrite_flag = " << rewrite_flag
				 << endl;
		}
// деструктор TemporalData --------------------------------------
		~TDM(){
			free(pTD_value);//создание динамической области для хранения TD
		}
};



#endif /* CLASS_TDM_H_ */

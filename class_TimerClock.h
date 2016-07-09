/*
 * TimerClock.h
 *
 *  Created on: 22.03.2016
 *      Author: admin
 */

#ifndef TIMERCLOCK_H_
#define TIMERCLOCK_H_
/*******************************************<<<< TimerClock.h >>>>*************************************/
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

using namespace std;

#define SECOND  1000000000L	 //Наносекунд в секунде
#define MINUTE  60.0 //Секунд в минуте
#define HOUR  3600.0 //Минут в часе

class TimerClock{ //Класс "Системные часы"
	static struct timespec TimerClock0; //компьютерный ноль системных часов - "схваченный" текущий момент компьютерного времени
public:

	// Методы класса TimerClock
	static double StartTimerClock(){// пуск системных часов
	    if( clock_gettime( CLOCK_REALTIME, &TimerClock0) == -1 ) return -1.0;
	return (double)TimerClock0.tv_sec + (double)( TimerClock0.tv_nsec )/(double)SECOND;
	}
	//GetTickMark()() - получить текущую метку времени системных часов
	static double GetTickmark(){
		struct timespec t; //текущий момент компьютерного времени
			if( clock_gettime( CLOCK_REALTIME, &t) == -1 ) return -1.0;
	return (t.tv_sec - TimerClock0.tv_sec) + (double)( t.tv_nsec - TimerClock0.tv_nsec)/(double)SECOND;
	}
};
	struct timespec TimerClock::TimerClock0={-1,0};//инициализация статической переменной класса TimerClock


#endif /* TIMERCLOCK_H_ */

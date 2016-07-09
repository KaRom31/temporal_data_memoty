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

#define SECOND  1000000000L	 //���������� � �������
#define MINUTE  60.0 //������ � ������
#define HOUR  3600.0 //����� � ����

class TimerClock{ //����� "��������� ����"
	static struct timespec TimerClock0; //������������ ���� ��������� ����� - "����������" ������� ������ ������������� �������
public:

	// ������ ������ TimerClock
	static double StartTimerClock(){// ���� ��������� �����
	    if( clock_gettime( CLOCK_REALTIME, &TimerClock0) == -1 ) return -1.0;
	return (double)TimerClock0.tv_sec + (double)( TimerClock0.tv_nsec )/(double)SECOND;
	}
	//GetTickMark()() - �������� ������� ����� ������� ��������� �����
	static double GetTickmark(){
		struct timespec t; //������� ������ ������������� �������
			if( clock_gettime( CLOCK_REALTIME, &t) == -1 ) return -1.0;
	return (t.tv_sec - TimerClock0.tv_sec) + (double)( t.tv_nsec - TimerClock0.tv_nsec)/(double)SECOND;
	}
};
	struct timespec TimerClock::TimerClock0={-1,0};//������������� ����������� ���������� ������ TimerClock


#endif /* TIMERCLOCK_H_ */

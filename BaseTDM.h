/*
 * TDMBase.h
 *
 *  Created on: 22.03.2016
 *      Author: admin
 */

#ifndef TDMBASE_H_
#define TDMBASE_H_

#include "class_TDM.h"


/******* База TDM **********/


TDM tm0((char*)"/tsh0_double", sizeof(double),PTHREAD_PROCESS_SHARED);
TDM tm1((char*)"/tsh1_double", sizeof(double),PTHREAD_PROCESS_SHARED);
TDM tm2((char*)"/tsh2_double", sizeof(double),PTHREAD_PROCESS_SHARED);
TDM tm3((char*)"/tsh3_double", sizeof(double),PTHREAD_PROCESS_SHARED);
TDM tm4((char*)"/tsh4_double", sizeof(double),PTHREAD_PROCESS_SHARED);
TDM tm5((char*)"/tsh5_double", sizeof(double),PTHREAD_PROCESS_SHARED,1,INT_MAX,1.0);
TDM tm6((char*)"/tsh6_double", sizeof(double),PTHREAD_PROCESS_SHARED);


#endif /* TDMBASE_H_ */

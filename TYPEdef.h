/*
 * TYPEdef.h
 *
 *  Created on: 22.03.2016
 *      Author: admin
 */

#ifndef TYPEDEF_H_
#define TYPEDEF_H_
//------------------------------------------------------------------//
#define MASTER true //тип узла - главный
#define SLAVE false //тип узла - подчинённый
#define MAXname 16	//максимальный размер имён TDM и DPU

using namespace std;

//тип набора установок узла
class TDM;

struct DPU_Set{//дескриптор инициализации узла обработки данных
	char* DPUname;//имя узла обработки
	bool mode;//режим срабатывания узла: MASTER=true - запуск по циклу опроса TDM, SLAVE=false - запуск по событию обновления TDM_0(ведущая)
	double period;//период цикла срабатывания узла-MASTER
	int (*procedure)(vector <void *> &In, vector <void *> &Out);
	TDM *(*In)[];//рабочий указатель для хранения указателя на массив входных объектов темпоральной памяти
	TDM *(*Out)[];//вектор указателей на выходные объекты темпоральной памяти
};
/*
struct DPU_Set{//дескриптор инициализации узла обработки данных
	char* DPUname;//имя узла обработки
	bool mode;//режим срабатывания узла: MASTER=true - запуск по циклу опроса TDM, SLAVE=false - запуск по событию обновления TDM_0(ведущая)
	double period;//период цикла срабатывания узла-MASTER
	int (*procedure)(vector <void *> In, vector <void *> Out);
	TDM *(*In)[];//рабочий указатель для хранения указателя на массив входных объектов темпоральной памяти
	TDM *(*Out)[];//вектор указателей на выходные объекты темпоральной памяти
};*/
#endif /* TYPEDEF_H_ */

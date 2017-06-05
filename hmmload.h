
//#include "common.h"
#ifndef _HMMLOAD_H_
#define _HMMLOAD_H_


typedef struct Hmm
{
	char * hmmName;
	int stateNum;
	struct State ** pState;   //�˴��������ȡ���������Ķ�
	struct Transfer * pTransfer;
	struct Hmm * next;
}Hmm;

typedef struct Transfer 
{
	char * transferName;
	int dimension;
	float **data;
}Transfer;

typedef struct State 
{
	char * stateName;
	int gaussNum;
	struct Gaussian * pGaussian;
	struct State * next;
}State;

typedef struct Gaussian 
{
	float weight;
	float  mean[DIMENSION];
	float  variance[DIMENSION];
	float gconst;
	struct Gaussian * next;
}Gaussian;

Hmm * hmmLoad(FILE * pCommand,State ** stateHead,const char * modelPath);
void stateDataLoad(State * ps,FILE * fState);
void transferDataLoad(Transfer * pt,FILE * fTransfer);

#endif

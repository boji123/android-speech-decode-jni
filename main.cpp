/*
********************************************************************************************
*Function: command recognition
*Description:load Hmm data ,build a node net according to commands, then decode by
             using token passing algorithm, and print the result of recognition
*Author:chen qiyu
*Version：1.0.0
*DATE：2015.7.28
*********************************************************************************************
*/



#include "common.h"
//#include "main.h"
#include "hmmload.h"
#include "net.h"
#include "wav.h"
#include "feature.h"
#include "decode.h"
#include "free.h"




FILE * pCommand=NULL;
Hmm * hmmHead=NULL;        //指向Hmm链表
State * stateHead=NULL;   //指向State链表
Net netHead;              //完整的网络
float * wavData=NULL;
Vector mfccData=NULL;
Feature * pFeature=NULL;
int samplePointNum;
int frameNum;
int mfccDimension;
int returnId=-6;
int nodeNum;
char name[20];
Hmm *h1,*h2;
Net net1,net2;

void ACRLoadModelBuildNet(const char * modelPath,const char * cmdPath)
{
	//HMM数据加载
		pCommand=fopen(cmdPath,"r");
		hmmHead=hmmLoad(pCommand,&stateHead,modelPath);   //此处输入参数设为引用，方便以后通过pState释放state链表
	//网络构建
		fseek(pCommand,0,0);LOGE("TESTLOAD");
		netHead=netBuild(hmmHead,pCommand);
		fclose(pCommand);
		pCommand=NULL;LOGE("TESTNET");
}



int ACRWavDecode(const char * wavPath)
{
	//MFCC特征值提取
		openWavFile(wavPath,&wavData,&samplePointNum);LOGE("Num2=%d",samplePointNum);
		wav2mfc(wavData,samplePointNum,&mfccData,&frameNum,&mfccDimension);LOGE("TESTWAV2");
		pFeature=(Feature *)malloc(sizeof(Feature)*frameNum);LOGE("TESTFEATURE");
		mfccToFeature(pFeature,mfccData,frameNum,mfccDimension);LOGE("TESTMFCC");
	//token passing解码
		returnId=tokenPassingDecode(netHead,pFeature,frameNum);LOGE("TESTTOKEN");
	//释放动态内存
		freeWavTokenMemory(pFeature,frameNum,wavData,mfccData);
		pFeature=NULL;
		wavData=NULL;
		mfccData=NULL;

		return returnId;
}



void ACRFree()
{
	freeHmmNetMemory(hmmHead,stateHead,netHead);
}



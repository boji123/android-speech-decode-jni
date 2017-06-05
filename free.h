#ifndef _FREE_H_
#define _FREE_H_


void freeWavTokenMemory(Feature * pFeature,int frameNum,float * wavData,float * mfccData);
void freeHmmNetMemory(Hmm * pHmm,State * pState,Net netHead);

#endif

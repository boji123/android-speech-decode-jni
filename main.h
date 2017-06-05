#ifndef _ACR_H_
#define _ACR_H_

void ACRLoadModelBuildNet(const char * modelPath,const char * cmdPath);
int ACRWavDecode(const char * wavPath);
void ACRFree();

#endif

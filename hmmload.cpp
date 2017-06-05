#include "common.h"
#include "hmmload.h"



/*
Function : hmmLoad
Action : According to "command.txt",load hmm ,state ,transfer data to struct
Input : A pointer point to "command.txt" FILE
Output: A pointer point to the list of Hmm struct
*/
Hmm * hmmLoad(FILE * pCommand,State ** stateHead,const char * model)
{

	Hmm * pHmm=NULL;
	Hmm* h1=NULL;
	Hmm* h2=NULL;
	Hmm* h3=NULL;
	Transfer * t1=NULL;
	State * s1=NULL,* s2=NULL,* s3=NULL,* s4=NULL,** s5=NULL,*s6=NULL;
	Gaussian * g1=NULL;
	FILE * fp=NULL,* fs=NULL,* ft=NULL;
	char oneLine[1024];
	char * string;
	char name[NAME],a[SIZEA];
	int i,j,k;
	char * hDataPath,*sDataPath,*tDataPath;
	char * modelPath=new char[strlen(model)+1];
	strcpy(modelPath,model);



	hDataPath=(char *)malloc(sizeof(char)*(strlen(modelPath)+4));
	sDataPath=(char *)malloc(sizeof(char)*(strlen(modelPath)+4));
	tDataPath=(char *)malloc(sizeof(char)*(strlen(modelPath)+4));


	memcpy(hDataPath,modelPath,strlen(modelPath));
	memcpy(sDataPath,modelPath,strlen(modelPath));
	memcpy(tDataPath,modelPath,strlen(modelPath));


	string=hDataPath+strlen(modelPath);
	*string='\0';
	string=sDataPath+strlen(modelPath);
	*string='\0';
	string=tDataPath+strlen(modelPath);
	*string='\0';

	string=hDataPath;
	while(*string!='\0')
		string++;
	*string=47;
	string++;
	*string='h';
	string++;
	*string=47;
	string++;
	*string='\0';

	string=sDataPath;
	while(*string!='\0')
		string++;
	*string=47;
	string++;
	*string='s';
	string++;
	*string=47;
	string++;
	*string='\0';

	string=tDataPath;
	while(*string!='\0')
		string++;
	*string=47;
	string++;
	*string='t';
	string++;
	*string=47;
	string++;
	*string='\0';


    //��ȡcommand.txt�ļ���ΪHMM�����ṹ�岢����Hmm��
	i=0;
	while (!feof(pCommand))  
	{i++;
		memset(oneLine,'\0',1024);
		fgets(oneLine,1024,pCommand);
		string=oneLine;
		LOGE("test %d  %s",i,string);
		j=0;
		while (*string!='\0'&&*string!='\n'&&*string!='\r')
		{j++;
			while(*string==' ')
				string++;


			if (*string!='\0')
			{
				memset(name,'\0',NAME);
				sscanf(string,"%s",name);
				string=string+strlen(name);
				k=0;
				h3=pHmm;
				while(h3!=NULL)
				{
					if(strcmp(h3->hmmName,name)==0)  //���Ѿ����ع���Hmm����k=1
						k=1;
					h3=h3->next;
				}
				if (k==0)        //����Hmmδ���ع�������ء����򲻼���
				{
					if (pHmm==NULL)
					{
						pHmm=(Hmm *)malloc(sizeof(Hmm));
						h1=h2=pHmm;
						h2->next=NULL;
						h2->hmmName=NULL;
						h2->pState=NULL;
						h2->pTransfer=NULL;
					} 
					else
					{
						h2=(Hmm *)malloc(sizeof(Hmm));
						h1->next=h2;
						h1=h2;
						h2->next=NULL;
						h2->hmmName=NULL;
						h2->pState=NULL;
						h2->pTransfer=NULL;
					}
					h2->hmmName=(char *)malloc(strlen(name)+1);
					memcpy(h2->hmmName,name,strlen(name)+1);
					LOGE("loadhmm %s",name);
				}
			}
		}

	}
	//����ټ�һ��sil HMM
	h1=pHmm;
	while (h1->next!=NULL)
		h1=h1->next;
	h2=(Hmm *)malloc(sizeof(Hmm));
	h1->next=h2;
	h2->next=NULL;
	h2->pState=NULL;
	h2->pTransfer=NULL;
	memset(a,'\0',SIZEA);
	memcpy(a,"sil",strlen("sil"));
	h2->hmmName=(char *)malloc(sizeof(a)+1);
	memcpy(h2->hmmName,a,strlen(a)+1);
	LOGE("TESTc");
	//����������HMM����
	h1=pHmm;
	while(h1!=NULL)
	{
		memset(name,'\0',NAME);                     //��hmm.bin�ļ�
		memcpy(name,hDataPath,strlen(hDataPath));
		memcpy(&name[strlen(hDataPath)],h1->hmmName,strlen(h1->hmmName));
		memcpy(&name[strlen(h1->hmmName)+strlen(hDataPath)],".bin",4);
		fp=fopen(name,"rb");
		t1=(Transfer *)malloc(sizeof(Transfer));        //����transfer����
		h1->pTransfer=t1;
		fread(a,20,1,fp);
		memset(a,'\0',SIZEA);
		fread(a,20,1,fp);
		memset(name,'\0',NAME);                    
		memcpy(name,tDataPath,strlen(tDataPath)); //��t.bin�ļ�
		memcpy(&name[strlen(tDataPath)],a,strlen(a));
		memcpy(&name[strlen(a)+strlen(tDataPath)],".bin",4);
		ft=fopen(name,"rb");
		transferDataLoad(t1,ft);
		fclose(ft);
		ft=NULL;
		fread(&h1->stateNum,4,1,fp);                    //����state����
		h1->pState=(State **)malloc(sizeof(State *)*(h1->stateNum-2));
		for (i=0;i<(h1->stateNum)-2;i++)
		{
			memset(a,'\0',SIZEA);
			fread(a,20,1,fp);   //��ȡ״̬����a
			s3=NULL;
			s4=(*stateHead);
			while(s4!=NULL)      
			{
				if(strcmp(a,s4->stateName)==0)
				{
					s3=s4;
					break;
				}
				s4=s4->next;
			}
			if (s3!=NULL)     //����״̬�Ѿ����ع��������ظ�����
				h1->pState[i]=s3;
			else              //����״̬δ���ع�������ظ�״̬
			{
				if ((*stateHead)==NULL)
				{
					(*stateHead)=(State *)malloc(sizeof(State));
					(*stateHead)->next=NULL;
					(*stateHead)->pGaussian=NULL;
					s1=s2=(*stateHead);
				} 
				else
				{
					s2=(State *)malloc(sizeof(State));
					s2->next=NULL;
					s2->pGaussian=NULL;
					s1->next=s2;
					s1=s2;
				}
				h1->pState[i]=s2;           //��HMMָ��״̬

				memset(name,'\0',NAME);
				memcpy(name,sDataPath,strlen(sDataPath));
				memcpy(&name[strlen(sDataPath)],a,strlen(a));
				memcpy(&name[strlen(a)+strlen(sDataPath)],".bin",4);
				fs=fopen(name,"rb");
				stateDataLoad(s2,fs);
				fclose(fs);
				fs=NULL;
			}
		}
		LOGE("fopenhmm %s",h1->hmmName);
	/*	k=1;
		while(k<h1->stateNum-1)
		{
			s6=(h1->pState)[k];
			if(s6!=NULL&&s6->stateName!=NULL)
			LOGE("%s",s6->stateName);
			k++;
		}*/


		fclose(fp);
		fp=NULL;
		h1=h1->next;
	}

	LOGE("loadEND");
/*h1=pHmm;
while(h1->next->next!=NULL)
	h1=h1->next;
s6=h1->pState[0];
g1=s6->pGaussian;
while(g1->next!=NULL)
	g1=g1->next;
i=0;
while(i<39)
{
	LOGE("hmm=%s  mean %d =%f",h1->hmmName,i,g1->mean[i]);
	i++;
}
t1=h1->pTransfer;
LOGE("transferName=%s",t1->transferName);
for(i=0;i<t1->dimension;i++)
{
	for(j=0;j<t1->dimension;j++)
		LOGE("%f",t1->data[i][j]);
}*/

	return pHmm;
}




/*
Function : transferDataLoad
Action : According to Hmm struct list,load transfer data to struct
Input : A pointer point to Hmm struct list
Output: None
*/
void transferDataLoad(Transfer * pt,FILE * fTransfer)
{
	char a[SIZEA];
	int i,k;

	memset(a,'\0',SIZEA);
	fread(a,20,1,fTransfer);
	pt->transferName=(char *)malloc(strlen(a)+1);
	memcpy(pt->transferName,a,strlen(a)+1);
	fread(&pt->dimension,4,1,fTransfer);
	pt->data=(float **)calloc(pt->dimension,sizeof(float *));
	for (i=0;i<(pt->dimension);i++)
	{
		pt->data[i]=(float *)calloc(pt->dimension,sizeof(float));
		for (k=0;k<(pt->dimension);k++)
			fread((void *)&pt->data[i][k],sizeof(float),1,fTransfer);
	}
}



/*
Function : stateDataLoad
Action : According to Hmm struct list,load state data to struct
Input : A pointer point to Hmm struct list
Output: None
*/
void stateDataLoad(State * ps,FILE * fState)
{
	char a[SIZEA];
	int i,k;
	Gaussian * g1,* g2;
	float w;

	memset(a,'\0',SIZEA);
	fread(a,20,1,fState);
	ps->stateName=(char *)malloc(strlen(a)+1);
	memcpy(ps->stateName,a,strlen(a)+1);
	fread(&ps->gaussNum,4,1,fState);
	fread(a,4,1,fState);
	for (i=0;i<(ps->gaussNum);i++)
	{
		if (ps->pGaussian==NULL)
		{
			ps->pGaussian=(Gaussian *)malloc(sizeof(Gaussian));
			g1=g2=ps->pGaussian;
			g1->next=NULL;
		} 
		else
		{
			g1=(Gaussian *)malloc(sizeof(Gaussian));
			g2->next=g1;
			g2=g1;
			g1->next=NULL;
		}
		fread(&w,4,1,fState);
		g1->weight=log(w);                   //weightֵ����Ϊԭ���ݵĶ���ֵ
		for (k=0;k<DIMENSION;k++)
			fread(&g1->mean[k],4,1,fState);
		for(k=0;k<DIMENSION;k++)
		{
			fread(&g1->variance[k],4,1,fState);	
		}
		fread(&g1->gconst,4,1,fState);
	}
}

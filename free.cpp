#include "common.h"
#include "hmmload.h"
#include "net.h"
#include "feature.h"


/*
Function : freeMemory
Action : free the memory of Hmm,State,Transfer,Gaussian,Node,Link and Feature
Input : the pointer of all these struct,the number of frames
Output: none
*/




void freeWavTokenMemory(Feature * pFeature,int frameNum,float * wavData,float * mfccData)
{
	int i;
	//�ͷ�Feature
		for (i=0;i<frameNum;i++)
		{
			free(pFeature[i].data);
			pFeature[i].data=NULL;
		}
		free(pFeature);
		pFeature=NULL;


		free(wavData);
		free(mfccData);
}



void freeHmmNetMemory(Hmm * pHmm,State * pState,Net netHead)
{
	    State * s1=NULL;
		Gaussian * g1=NULL,* g2=NULL;
		Hmm * h1=NULL;
		Transfer * t1=NULL;
		Node * node1=NULL,* node2=NULL;
		Link * link1=NULL,* link2=NULL;
		int i;

		//�����ͷ�state����gaussian����
		while(pState!=NULL)
		{
			s1=pState;
			pState=pState->next;
			free(s1->stateName);
			s1->stateName=NULL;
			g1=s1->pGaussian;
			while (g1!=NULL)
			{
				g2=g1;
				g1=g1->next;
				free(g2);
				g2=NULL;
			}
			free(s1);
			s1=NULL;
		}
		//�ͷ�Hmm����,Transfer�ṹ��
		while(pHmm!=NULL)
		{
			h1=pHmm;
			pHmm=pHmm->next;
			t1=h1->pTransfer;
			free(t1->transferName);
			t1->transferName=NULL;
			for (i=0;i<(t1->dimension);i++)
			{
				free(t1->data[i]);
				t1->data[i]=NULL;
			}
			free(t1->data);
			t1->data=NULL;
			free(t1);
			t1=NULL;

			free(h1->hmmName);
			h1->hmmName=NULL;
			free(h1->pState);
			h1->pState=NULL;
			free(h1);
			h1=NULL;
		}


		//�ͷ�Node�����Link����
		node1=netHead.startNode;
		while (node1!=NULL)
		{
			node2=node1;
			node1=node1->next;
			link1=node2->pLink;
			while (link1!=NULL)
			{
				link2=link1;
				link1=link1->next;
				free(link2);
				link2=NULL;
			}
			free(node2);
			node2=NULL;
		}
}

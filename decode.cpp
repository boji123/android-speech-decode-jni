#include "common.h"
#include "hmmload.h"
#include "net.h"
#include "feature.h"
#include "decode.h"


/*
Function : tokenPassingDecode
Action : use token passing algorithm to decode and print the result of recognition
Input : the whole net of commands,a pointer point to the list of Feature struct,the number of frames
Output: none
*/
int tokenPassingDecode(Net netHead,Feature * pFeature,int frameNum)
{
	Token * oldToken=NULL,* newToken=NULL,* token1=NULL,* token2=NULL,* token3=NULL,*token4=NULL,*token5=NULL,*token6=NULL;
	Node * node1=NULL,* node2=NULL,*node3=NULL;
	Link * link1=NULL;
	int i,keyid=-3,tokenNum,j,k,m;
	float n;

	oldToken=(Token *)malloc(sizeof(Token));   //��ʼ����һ������
	oldToken->logLike=0;
	oldToken->pNode=netHead.startNode;
	oldToken->id=netHead.startNode->id;
	oldToken->next=NULL;
	newToken=NULL;
    LOGE("TOKENframeNum=%d",frameNum);
	for (i=0;i<frameNum;i++)   //��ÿһ֡���в���
	{
		token1=oldToken;	
		while (token1!=NULL)            //����������
		{
			node1=token1->pNode;
			link1=node1->pLink;
			while (link1!=NULL)
			{
				node2=link1->pNode;
				if(newToken==NULL)
				{
					newToken=(Token *)malloc(sizeof(Token));
					token2=newToken;
				}
				else
				{
					token2->next=(Token *)malloc(sizeof(Token));
					token2=token2->next;
				}
				if(node2->id>0)
					token2->id=node2->id;
				else
					token2->id=token1->id;
				token2->next=NULL;
				token2->pNode=node2;
				if(node2!=netHead.endNode)
					token2->logLike=token1->logLike+log(link1->weight)+outLike(node2,&pFeature[i]);//outlikeΪlogֵ
				else
					token2->logLike=token1->logLike+log(link1->weight);  //β������������

				link1=link1->next;
			}
			token1=token1->next;
		}

		token1=newToken;          //��newToken����ָ��ͬһ�������ƽ���������ֵ�ϴ���
		while(token1->next!=NULL)
		{
			token2=token1->next;
			while (token2!=NULL)
			{
				if (token1->pNode==token2->pNode)
				{
					if((token1->logLike)<(token2->logLike))
					{
						token1->id=token2->id;
						token1->logLike=token2->logLike;
					}
					token3=token1;
					while(token3->next!=token2)
						token3=token3->next;
					token3->next=token2->next;
					free(token2);
					token2=token3;
				}
				token2=token2->next;
			}
			token1=token1->next;
			if(token1==NULL)
				break;
		}


		token1=oldToken;            //�ͷ�oldToken��,����oldToken=newToken
		while(token1!=NULL)
		{
			token2=token1;
			token1=token1->next;
			free(token2);
		}
		oldToken=newToken;
		newToken=NULL;

		token4=oldToken;    //当前token数目
		tokenNum=0;
		while(token4!=NULL)
		{
			tokenNum++;
			token4=token4->next;
		}



		/*token4=oldToken;   //排序后输出oldToken链概率
		for(j=1;j<tokenNum;j++)
		{
			token5=token4->next;
			for(k=1;k<=tokenNum-j;k++)
			{
				if(token4->logLike<token5->logLike)
				{
					n=token5->logLike;
					token5->logLike=token4->logLike;
					token4->logLike=n;
					m=token5->id;
					token5->id=token4->id;
					token4->id=m;
					node3=token5->pNode;
					token5->pNode=token4->pNode;
					token4->pNode=node3;
				}
				token5=token5->next;
			}
			token4=token4->next;
		}
		token4=oldToken;
		while(token4!=NULL)
		{
			LOGE("frame=%d  TOKENNUM=%d  log=%f",i,tokenNum,token4->logLike);
			token4=token4->next;
		}*/


		//LOGE("frame=%d  TOKENNUM=%d",i,tokenNum);


		//根据Bin值丢令牌
		if(i>((frameNum/5)+10)&&i<((frameNum*4/5)-10))   //
		{
			token4=oldToken;
			for(j=1;j<tokenNum;j++)
			{
				token5=token4->next;
				for(k=1;k<=tokenNum-j;k++)
				{
					if(token4->logLike<token5->logLike)
					{
						n=token5->logLike;
						token5->logLike=token4->logLike;
						token4->logLike=n;
						m=token5->id;
						token5->id=token4->id;
						token4->id=m;
						node3=token5->pNode;
						token5->pNode=token4->pNode;
						token4->pNode=node3;
					}
					token5=token5->next;
				}
				token4=token4->next;
			}
			token5=oldToken;


			//n=(token5->logLike+token5->next->logLike+token5->next->next->logLike)/6;  //前三个令牌平均值的三分之二

			n=(token5->logLike+token5->next->logLike)/2;
			while(token5->next!=NULL)
			{
				token4=token5->next;
				if(token4->logLike<(n-100)&&token4->pNode!=netHead.endNode)//&&token4->pNode->id!=-3
				{
					token5->next=token4->next;
					free(token4);
				}
				else
					token5=token5->next;
			}

		}




		token4=token5=token6=NULL;
	}
	LOGE("TOKENB");



	token4=oldToken;   //排序后输出oldToken链概率
	for(j=1;j<tokenNum;j++)
	{
		token5=token4->next;
		for(k=1;k<=tokenNum-j;k++)
		{
			if(token4->logLike<token5->logLike)
			{
				n=token5->logLike;
				token5->logLike=token4->logLike;
				token4->logLike=n;
				m=token5->id;
				token5->id=token4->id;
				token4->id=m;
				node3=token5->pNode;
				token5->pNode=token4->pNode;
				token4->pNode=node3;
			}
			token5=token5->next;
		}
		token4=token4->next;
	}
	token4=oldToken;
	while(token4!=NULL)
	{
		if(token4->pNode==netHead.startNode)LOGE("nodename=start,id=%d,log=%f",token4->id,token4->logLike);
		else if(token4->pNode==netHead.endNode)LOGE("nodename=end,id=%d,log=%f",token4->id,token4->logLike);
		else
			LOGE("nodename=%s  id=%d  log=%f",token4->pNode->pState->stateName,token4->id,token4->logLike);
		token4=token4->next;
	}





	token3=oldToken;                 //ָ��ʶ�������
	node1=netHead.endNode;
	while(token3->pNode!=node1)
    {LOGE("tokenID=%d",token3->pNode->id);

		token3=token3->next;

		if(token3==NULL)
		break;
	}

	if(token3==NULL)
		keyid=6;
	else
	keyid=token3->id;

	LOGE("TOKENkeyid=%d",keyid);

	token1=oldToken;            //ʶ��������ͷ�oldToken������������token�����ͷ�
	while(token1!=NULL)
	{
		token2=token1;
		token1=token1->next;
		free(token2);
	}

	return keyid;
}



/*
Function : outLike
Action : calculate the log value of a node's outlike
Input : a pointer point to the node,a pointer point to Feature struct
Output: log value of the node's outlike
*/
float outLike(Node* pNode,Feature * pFeature)
{
	float outLike,logGaussLike;
	State * s1;
	Gaussian * g1;
	int i;
	
	outLike=LZERO;
	s1=pNode->pState;
	g1=s1->pGaussian;
	for(i=0;i<(s1->gaussNum);i++)	
	{
		logGaussLike = LogGaussProb(pFeature->data,g1,DIMENSION);	
		outLike=LogAdd(outLike, g1->weight+logGaussLike);	//g1->weight�ڼ���ʱ�Ѿ�ȡΪlogֵ
		g1=g1->next;
	}

	return outLike;
}



/*
Function : LogGaussProb
Action : calculate the log value of a gauss
Input : a pointer point to the data of feature,a pointer point to Gaussian struct,the dimension of gauss
Output: log value of the gauss
*/
float LogGaussProb(float *mfcc,Gaussian *guassian, int n_dim)
{
	float prob, factor = 0;
	int a;

	for(a=0; a<n_dim; a++)
		factor = factor + (mfcc[a] - guassian->mean[a])*(mfcc[a] - guassian->mean[a])/guassian->variance[a];

	prob = guassian->gconst + factor;

	//return the log Gaussian Prob
	return -0.5*prob;
}


/*
Function : LogAdd
Action : return sum x+y on log scale
Input : two log like
Output: sum x+y on log scale
*/
float LogAdd(float x, float y)
{
	float temp,diff,z;

	if (x<y) {
		temp = x; x = y; y = temp;
	}
	diff = y-x;
	if (diff<(-log(-LZERO))) 
		return  (x<LSMALL)?LZERO:x;
	else {
		z = exp(diff);
		return x+log(1.0+z);
	}
}

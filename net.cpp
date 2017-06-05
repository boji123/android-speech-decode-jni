#include "common.h"
#include "hmmload.h"
#include "net.h"




/*
Function : netBuild
Action : According to commands in "command.txt",build a net for token passing
Input : The list of Hmm struct,a file pointer point to "command.txt"
Output: A net that represent all the commands
*/
Net netBuild(Hmm * pHmm,FILE * pCommand)
{

	Net net1,net2,net3,net4;
	Hmm * h1=NULL;
	Node *node1;
	char oneLine[1024],name[NAMESIZE];
	char * string=NULL;
	int id=0,i;
	LOGE("neta");
	while(!feof(pCommand))          //��sil֮�����������,�õ�����net3
	{
		memset(oneLine,'\0',1024);
		fgets(oneLine,1024,pCommand);
		id++;
		string=oneLine;
		i=0;
		LOGE("net oneLine=%s",oneLine);
		while (*string!='\0'&&*string!='\n'&&*string!='\r')  //��һ��ָ�����connect���������õ�net1����
		{
			while(*string==' ')
				string++;
			if (*string!='\0'&&*string!='\n'&&*string!='\r')
			{
				memset(name,'\0',NAMESIZE);
				sscanf(string,"%s",name);
				string=string+strlen(name);
				h1=pHmm;
				while (strcmp(name,h1->hmmName)!=0)
					h1=h1->next;
				if (h1==NULL)
					printf("%s can not link to net\n",name);
				else
					net2=hmmToNet(h1,id);
				if(i==0)
					net1=net2;
				else
					net1=connectNet(net1,net2);
			}
			i++;
		}
		if((*oneLine!='\0'&&*oneLine!='\n'&&*oneLine!='\r'))//�˴�����ѭ������
		{
		if(id==1)
			net3=net1;
		else
			net3=unionNet(net3,net1);
		}
	}LOGE("netb");
	//����������β����sil�ӽ�����

	memset(name,'\0',NAMESIZE);
	memcpy(name,"sil",strlen("sil"));
	h1=pHmm;
	while (strcmp(name,h1->hmmName)!=0)
		h1=h1->next;
	if (h1==NULL)
		printf("%s can not link to net\n",name);
	else
	{
		id=-3;
		net2=hmmToNet(h1,id);
		id=-4;
		net4=hmmToNet(h1,id);
	}
	net3=connectNet(net2,net3);
	net3=connectNet(net3,net4);
	LOGE("netbB");





	/*node1=net3.startNode->next;
	while(node1->next!=NULL)
	{
		LOGE("NETTEST %s",node1->pState->stateName);
		node1=node1->next;
	}*/

	/*while(pCommand!=NULL)
	{
		memset(oneLine,'\0',1024);
		fgets(oneLine,1024,pCommand);
		id++;
		string=oneLine;
		i=0;
		LOGE("net oneLine=%s",oneLine);
		while (*string!='\0'&&*string!='\n'&&*string!='\r')  //��һ��ָ�����connect���������õ�net1����
		{
			while(*string==' ')
				string++;
			if (*string!='\0'&&*string!='\n'&&*string!='\r')
			{
				memset(name,'\0',NAMESIZE);
				sscanf(string,"%s",name);
				string=string+strlen(name);
				h1=pHmm;
				while (strcmp(name,h1->hmmName)!=0)
					h1=h1->next;
				if (h1==NULL)
					LOGE("%s can not link to net\n",name);
				else
					net2=hmmToNet(h1,id);
				if(i==0)
					net1=net2;
				else
					net1=connectNet(net1,net2);
			}
			i++;
		}
		if(id==1)
			net3=net1;
		else
			net3=unionNet(net3,net1);
	}*/
/*
	memset(name,'\0',NAMESIZE);
	memcpy(name,"sil",strlen("sil"));
	h1=pHmm;
	while (strcmp(name,h1->hmmName)!=0&&h1!=NULL)
		h1=h1->next;
	if (h1==NULL)
		LOGE("%s can not link to net\n",name);
	else
	{
		id=-3;
		net2=hmmToNet(h1,id);
		id=-4;
		net4=hmmToNet(h1,id);
	}
	net3=connectNet(net2,net3);
	net3=connectNet(net3,net4);

	node1=net3.startNode;
	while(node1!=net3.endNode)
	{
		LOGE("node=%s",node1->pState->stateName);
		node1=node1->next;
	}*/

	return net3;
}




/*
Function : hmmToNet
Action : According to the pointer point to Hmm,build a net for that Hmm
Input : A pointer point to a Hmm
Output: A net that represent that Hmm
*/
Net hmmToNet(Hmm * pH,int id)
{
	Net net1;
	Node * node1=NULL,* node2=NULL,* node3=NULL;
	Link * link1=NULL,* link2=NULL;
	Transfer * t1=NULL;
	int i,k,j;

	net1.startNode=(Node *)malloc(sizeof(Node));
	node1=net1.startNode;
	node1->next=NULL;
	node1->pLink=NULL;
	node1->pState=NULL;
	node1->id=-1;             //net��ͷβ����id=-1��sil��Ӧ��id=0������ָ����idΪ��Ӧ������
	net1.endNode=(Node *)malloc(sizeof(Node));
	node1=net1.endNode;
	node1->next=NULL;
	node1->pLink=NULL;
	node1->pState=NULL;
	node1->id=-1;


	for (i=0;i<(pH->stateNum)-2;i++)
	{
		if (i==0)
		{
			node1=(Node *)malloc(sizeof(Node));
			node1->pLink=NULL;
			node1->pState=NULL;
			node1->next=NULL;
			node3=net1.startNode;
			node3->next=node1;
			node1->id=id;
			node1->pState=pH->pState[i];
			node2=node1;
		}
		if (i==(pH->stateNum)-3)
		{
			node1=(Node *)malloc(sizeof(Node));
			node1->pLink=NULL;
			node1->pState=NULL;
			node1->next=NULL;
			node2->next=node1;
			node1->id=id;
			node1->pState=pH->pState[i];
			node1->next=net1.endNode;
			node2=node1;
		}
		if(i!=0&&i!=(pH->stateNum)-3)
		{
			node1=(Node *)malloc(sizeof(Node));
			node1->pLink=NULL;
			node1->pState=NULL;
			node1->next=NULL;
			node2->next=node1;
			node1->id=id;
			node1->pState=pH->pState[i];
			node2=node1;
		}
	}
	t1=pH->pTransfer;
	node1=net1.startNode;
	for (i=0;i<(pH->stateNum)-1;i++)
	{
		link1=link2=node1->pLink;
		for (k=0;k<(pH->stateNum);k++)
		{
			if (t1->data[i][k]!=0)
			{
				if(node1->pLink==NULL)
				{
					node1->pLink=(Link *)malloc(sizeof(Link));
					link1=link2=node1->pLink;
					link1->next=NULL;
					link1->pNode=NULL;
				}
				else
				{
					link2=(Link *)malloc(sizeof(Link));
					link1->next=link2;
					link1=link2;
					link1->next=NULL;
					link1->pNode=NULL;
				}
				link1->weight=t1->data[i][k];
				node2=net1.startNode;
				for (j=0;j<k;j++)
					node2=node2->next;
				link1->pNode=node2;
			}
		}
		node1=node1->next;
	}
	return net1;
}


/*
Function : connectNet
Action : connect two net to build a new net
Input : two net
Output: A net that connected by the two net
*/
Net connectNet(Net net1,Net net2)
{
	Node * node1=NULL,* node2=NULL,* node3=NULL;
	Link * link1=NULL,* link2=NULL,* link3=NULL;
	int linkNum,i,j;
	float weight;
    Net net3;

	node2=net2.startNode;
	link1=node2->pLink;
	linkNum=0;
	while(link1!=NULL)
	{
		link1=link1->next;
		linkNum++;
	}
	node1=net1.startNode;
	j=1;
	while (node1!=NULL)           //ʹָ��net1β����Link��������                              //�˴���ָ��net1β����linkʱ��������ұ�������Խ��Խ�࣬������ԸĽ�
	{
		link1=node1->pLink;
		while (link1!=NULL)
		{
			if (link1->pNode==net1.endNode)
			{
				node3=link1->pNode;
				if(node3->id!=(-1))
					printf("wrong");
				weight=link1->weight;
				link2=node2->pLink;
				link1->weight=weight*link2->weight;
				link1->pNode=link2->pNode;
				
				for (i=1;i<linkNum;i++)
				{
					link2=link2->next;                        //�˴�Ϊ�����������ھɰ汾�д������������һ��ѭ���⣬���³���
					link3=(Link *)malloc(sizeof(Link));
					link3->next=link1->next;
					link1->next=link3;
					link1=link3;
					link1->weight=weight*link2->weight;
					link1->pNode=link2->pNode;
				}
			}
			link1=link1->next;
		}
		node1=node1->next;
		j++;
	}
	node1=net1.startNode;          //ʹnet1��β���ָ��net2���׽�㣬�õ�����net3
	while (node1->next!=net1.endNode)
		node1=node1->next;
	node2=net2.startNode;
	node1->next=node2->next;
	net3.startNode=net1.startNode;
	net3.endNode=net2.endNode;


	node1=net1.endNode;    //�ͷ�net1β���
	link1=link2=node1->pLink;
	while (link1!=NULL)
	{
		link1=link1->next;
		free(link2);
		link2=link1;
	}
	free(node1);
	node1=NULL;

	node1=net2.startNode;   //�ͷ�net2�׽��
	link1=link2=node1->pLink;
	while (link1!=NULL)
	{
		link1=link1->next;
		free(link2);
		link2=link1;
	}
	free(node1);
	node1=NULL;

	return net3;
}


/*
Function : unionNet
Action : union two net to build a new net
Input : two net
Output: A net that union by the two net
*/
Net unionNet(Net net1,Net net2)
{
	Net net3;
	Link * link1=NULL,* link2=NULL,* link3=NULL;
	Node * node1=NULL,* node2=NULL,* node3=NULL;

	net3.startNode=(Node *)malloc(sizeof(Node));
	node1=net3.startNode;
	node1->next=NULL;
	node1->pLink=NULL;
	node1->pState=NULL;
	node1->id=-1;   
	net3.endNode=(Node *)malloc(sizeof(Node));
	node1=net3.endNode;
	node1->next=NULL;
	node1->pLink=NULL;
	node1->pState=NULL;
	node1->id=-1;

	LOGE("unionnet11 pass");
	node1=net1.startNode;
	node3=net3.startNode;
	link3=node1->pLink;      //����net1�׽���link
	while (link3!=NULL)
	{		
		if (node3->pLink==NULL)
		{
			node3->pLink=(Link *)malloc(sizeof(Link));
			link1=link2=node3->pLink;
			link1->next=NULL;
			link1->pNode=NULL;
		}
		else
		{
			link2=(Link *)malloc(sizeof(Link));
			link1->next=link2;
			link1=link2;
			link1->next=NULL;
			link1->pNode=NULL;
		}
		link1->weight=link3->weight;
		link1->pNode=link3->pNode;
		link3=link3->next;
	}
	LOGE("unionnet22 pass");

	node1=net2.startNode;
	link3=node1->pLink;      //����net2�׽���link
	while (link3!=NULL)
	{		
		if (node3->pLink==NULL)
		{
			node3->pLink=(Link *)malloc(sizeof(Link));
			link1=link2=node3->pLink;
			link1->next=NULL;
			link1->pNode=NULL;
		}
		else
		{
			link2=(Link *)malloc(sizeof(Link));
			link1->next=link2;
			link1=link2;
			link1->next=NULL;
			link1->pNode=NULL;
		}
		link1->weight=link3->weight;
		link1->pNode=link3->pNode;
		link3=link3->next;
	}
	LOGE("unionnet33 pass");


	node1=net1.startNode;                 //��������������н����������������net3����
	node2=net2.startNode;
	node3=net3.startNode;
	while(node1->next!=net1.endNode)
		node1=node1->next;
	node1->next=node2->next;
	node1=net1.startNode;
	node3->next=node1->next;

	node2=net2.startNode;
	while(node2->next!=net2.endNode)
		node2=node2->next;
	node2->next=net3.endNode;
	LOGE("unionnet34 pass");
	node1=net3.startNode;              //��ָ��net1��net2β����linkָ��net3β���
	while (node1!=net3.endNode)
	{
		link1=node1->pLink;
		while (link1!=NULL)
		{
			if (link1->pNode==net1.endNode||link1->pNode==net2.endNode)
				link1->pNode=net3.endNode;
			link1=link1->next;
		}
		node1=node1->next;
	}
	LOGE("unionnet44 pass");
	node1=net1.startNode;         //�ͷ�net1��net2��β��㼰link
	link1=link2=node1->pLink;
	while (link1!=NULL)
	{
		link1=link1->next;
		free(link2);
		link2=link1;
	}
	free(node1);
	node1=NULL;

	node1=net1.endNode;         
	link1=link2=node1->pLink;
	while (link1!=NULL)
	{
		link1=link1->next;
		free(link2);
		link2=link1;
	}
	free(node1);
	node1=NULL;

	node1=net2.startNode;
	link1=link2=node1->pLink;
	while (link1!=NULL)
	{
		link1=link1->next;
		free(link2);
		link2=link1;
	}
	free(node1);
	node1=NULL;

	node1=net2.endNode;
	link1=link2=node1->pLink;
	while (link1!=NULL)
	{
		link1=link1->next;
		free(link2);
		link2=link1;
	}
	free(node1);
	node1=NULL;
	LOGE("unionnet55 pass");
	return net3;
}

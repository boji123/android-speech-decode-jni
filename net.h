#ifndef _NET_H_
#define _NET_H_


typedef struct Node 
{
	//char * stateName;
	int id;
	struct Link * pLink;
	struct State * pState;
	struct Node * next;
}Node;

typedef struct Link
{
	struct Node * pNode;
	float weight;
	struct Link * next;
}Link;

typedef struct Net
{
	Node * startNode;
	Node * endNode;
}Net;

Net netBuild(Hmm * pHmm,FILE * pCommand);
Net hmmToNet(Hmm * pH,int id);
Net connectNet(Net net1,Net net2);
Net unionNet(Net net1,Net net2);

#endif
#ifndef _DECODE_H_
#define _DECODE_H_

typedef struct Token
{
	float logLike;
	struct Node * pNode;
	int id;
	struct Token * next;
}Token;


#define LZERO  (-1.0E10)		      /* ~log(0) ¸ºÎÞÇî*/
#define LSMALL (-0.5E10)			  /* log values < LSMALL are set to LZERO */
#define MINMIX  1.0E-5				  /* Min usable mixture weight */
#define LMINMIX -11.5129254649702     /* log(MINMIX) */

int tokenPassingDecode(Net netHead,Feature * pFeature,int frameNum);
float outLike(Node* pNode,Feature * pFeature);
float LogGaussProb(float *mfcc,Gaussian *guassian, int n_dim);
float LogAdd(float x, float y);

#endif


#ifndef LOCALMAP_H
#define LOCALMAP_H

#include "platform.h"

//memory localizing hash map with changing hash function cipher key

#define LOWEST0(x)			((~x) & (x + 1))
#define LOWEST1(x)			(x & ((~x) + 1))

#define HASHINT		unsigned __int8
#define HASHADDR	unsigned int
#define SHASHADDR	signed int
#define LOG2HASHBITS	2	//(log2(HASHINTBITS))
//#define HASHINTBITS		64
#define HASHINTBITS		8
#define MAXHASH		((((HASHINT)1)<<HASHINTBITS)-1)
#define HASHCROP(x)	((x)&(MAXHASH))
#define HASHROTL(x,n)	HASHCROP((x<<n)|(x>>(HASHINTBITS-n)))
#define HASHROTR(x,n)	HASHCROP((x>>n)|(x<<(HASHINTBITS-n)))
#define FITRESX		64
#define FITRESY		48
#define HASHLEVELBITS	8	
//#define HASHLEVELBITS	(FITRESX*FITRESY*3*8)
#define HASHLEVELS			((8))

#define HASHVALS		14

#define HASHOP1			|
#define HASHOP2			&
#define HASHOP3			~

#define SPARSITY		1

/*
what's the least possible mask you could have (greater than x) if you had to get this output, given any input?

fails at bottom, sends signal up to choose next possible mask
*/

struct Mask
{
	HASHINT mask[HASHLEVELBITS/HASHINTBITS];
	HASHINT ovaldown[HASHLEVELBITS/HASHINTBITS];
	//HASHINT maskup[HASHLEVELBITS/HASHINTBITS];
	Mask* maskup;
};

class Tried
{
public:
};

inline bool Ahead2(HASHINT current[HASHLEVELBITS/HASHINTBITS], 
				  HASHINT test[HASHLEVELBITS/HASHINTBITS])
{
	//for(SHASHADDR i=(HASHLEVELS-1); i>=0; --i)
	//for(uint8_t i=0; i&31; ++i)
	{	
		for(SHASHADDR j=HASHLEVELBITS/HASHINTBITS-1; j>=0; --j)
		{
			if(test[j] > current[j])
				return true;
			if(test[j] < current[j])
				return false;
		}
	}

	return false;
}

inline bool Ahead3(HASHINT current[HASHLEVELBITS/HASHINTBITS], 
				  HASHINT test[HASHLEVELBITS/HASHINTBITS],
				  HASHINT set[HASHLEVELBITS/HASHINTBITS])
{
	//for(SHASHADDR i=(HASHLEVELS-1); i>=0; --i)
	//for(uint8_t i=0; i&31; ++i)
	{	
		for(SHASHADDR j=HASHLEVELBITS/HASHINTBITS-1; j>=0; --j)
		{
			if(!set[j])
				continue;
			if((test[j]&set[j]) > (current[j]&set[j]))
				return true;
			if((test[j]&set[j]) < (current[j]&set[j]))
				return false;
		}
	}

	return false;
}

inline bool Ahead(HASHINT current[HASHLEVELS][HASHLEVELBITS/HASHINTBITS], 
				  HASHINT test[HASHLEVELS][HASHLEVELBITS/HASHINTBITS])
{
	for(SHASHADDR i=(HASHLEVELS-1); i>=0; --i)
	//for(uint8_t i=0; i&31; ++i)
	{	
		for(SHASHADDR j=HASHLEVELBITS/HASHINTBITS-1; j>=0; --j)
		{
			if(test[i][j] > current[i][j])
				return true;
			if(test[i][j] < current[i][j])
				return false;
		}
	}

	return false;
}

bool Down(std::list<Mask>* masksup, 
		  HASHINT mval[HASHLEVELS][HASHLEVELBITS/HASHINTBITS], 
		  HASHINT* maskup, 
		  HASHADDR level, 
		  HASHINT in[HASHVALS][HASHLEVELBITS/HASHINTBITS],
		  bool lowlim, 
		  HASHINT upin[HASHVALS][HASHLEVELBITS/HASHINTBITS], 
		  HASHINT upinmask[HASHVALS][HASHLEVELBITS/HASHINTBITS], 
		  HASHADDR nmc,
		  std::list<Tried>* tried,
		  HASHADDR downmci,
		  std::list<Tried>* wtried);
void Up(HASHINT in[HASHLEVELBITS/HASHINTBITS], HASHINT* inval, HASHINT* inmask);
void TestHash();
void Hash(HASHINT *m, HASHINT *i, HASHINT *o, HASHADDR nbits, HASHADDR boff);
//HASHINT Hash(HASHINT mval[HASHLEVELS][HASHLEVELBITS/HASHINTBITS], 
//			 HASHINT oval[HASHLEVELBITS/HASHINTBITS], 
//			 HASHADDR level);


void Sparsify(HASHINT from[HASHLEVELBITS/HASHINTBITS/SPARSITY],
			  HASHINT to[HASHLEVELBITS/HASHINTBITS]);
void Unsparsify(HASHINT from[HASHLEVELBITS/HASHINTBITS],
			  HASHINT to[HASHLEVELBITS/HASHINTBITS/SPARSITY]);

#endif
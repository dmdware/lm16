

#ifndef LOCALMAP_H
#define LOCALMAP_H

#include "platform.h"

//memory localizing hash map with changing hash function cipher key

#define LOWEST0(x)			((~x) & (x + 1))
#define LOWEST1(x)			(x & ((~x) + 1))

#define HASHINT		unsigned char
#define HASHADDR	unsigned short
#define SHASHADDR	signed short
#define LOG2HASHBITS	2	//(log2(HASHINTBITS))
#define HASHINTBITS		8
#define MAXHASH		((((HASHINT)1)<<HASHINTBITS)-1)
#define HASHCROP(x)	((x)&(MAXHASH))
#define HASHROTL(x,n)	HASHCROP((x<<n)|(x>>(HASHINTBITS-n)))
#define HASHROTR(x,n)	HASHCROP((x>>n)|(x<<(HASHINTBITS-n)))
#define FITRESX		640
#define FITRESY		480
#define HASHLEVELBITS	8	//(FITRESX*FITRESY*3*8)
#define HASHLEVELS			((8))

#define HASHVALS		1

/*
what's the least possible mask you could have (greater than x) if you had to get this output, given any input?

fails at bottom, sends signal up to choose next possible mask
*/

struct Mask
{
	HASHINT mask;
	HASHINT ovaldown;
	HASHINT maskup;
};

inline bool Ahead(HASHINT current[HASHLEVELS], HASHINT test[HASHLEVELS])
{
	for(SHASHADDR i=(HASHLEVELS-1); i>=0; --i)
	//for(uint8_t i=0; i&31; ++i)
	{	
		if(test[i] > current[i])
			return true;
		if(test[i] < current[i])
			return false;
	}

	return false;
}

//00 00
//10 01
//8b

//0000
//0001
//1000
//1001

//000m
//100m

//m0 0m
//4b

//m00m



//00 00
//11 11
//8b

//0000
//0011
//1100
//1111
//16b

//8b




//blob1
//00 00
//11 11

//blob2
//10 00
//11

//ublob
//10 00

//sblob
//11 00

//uublob
//10


//max entries * bits per entry = bit width of number * channels * 2 * bit width = max bits
//4 bit width * 2 * 1 ch = 8 max entries * 4 bits = 32 max bits
//2 bit width * 2 * 2 ch = 8 * 2 bits = = 16 max bits

//64*100 bit width * 2 * 1 ch * 64*100 bit width = 12800 entries * 6400 bits =  81920000 max bits = 78.125 MB
//64 bit width * 2 * 100 ch * * 64 bit width = 12800 entires * 64 bits = 819200 max bits = 800 kB

bool Down(std::list<Mask>* masksup, HASHINT mval[HASHLEVELS], HASHINT* maskup, HASHADDR level, HASHINT in, bool lowlim, HASHINT upin, HASHINT upinmask);
void Up(HASHINT in, HASHINT* inval, HASHINT* inmask);
void TestHash();
HASHINT Hash(HASHINT mval[HASHLEVELS], HASHINT oval, HASHADDR level);

#endif
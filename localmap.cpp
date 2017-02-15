

#include "utils.h"
#include "localmap.h"


int main(int argc, char** argv)
{
	OpenLog("log.txt", 0);
	TestHash();
	system("pause");
	return 0;
}

/*
i ini = 0100
m = 11
bit shft/level = 1
fin i = 00
fin s = 10
o0 = ~(1 & ~(0 & 1))
o1 = ~(1 & ~(0 & 0))
use bit len = tot bit len / 2

*/

/*
level 0:
	maintex:	01234567
	shifttex:	12345670

	outtex:			01,12,23,34,45,56,67,70

	every 2nd
	final 
	outtex:			01,23,45,67

	all
	final
	outtex:			01,12,23,34,45,56,67,70

level 1:

	maintex: 		01,12,23,34,45,56,67,70
	shifttex:		23,34,45,56,67,70,01,12

	every 2nd
	outtex:			

	all
	outtex:			(01,23),(12,34),(23,45),(34,56),(45,67),(56,70),(67,01),(70,12)

level 2:

	
*/



void Shift(HASHINT *i, HASHINT *s, int amt, int nbits)
{
	int amtB = amt/8;
	int amtb = amt%8;

	for(int bi=0; bi<nbits; bi+=8)
	{
		//if amtb>0, last [8-amtb,8] bits of i[bi/8] go to first [0,amtb] bits of s[bi/8+1+amtB]
		//and first [0,8-amtb] bits of i[bi/8] go to [amtb,8] bits of s[bi/8+amtB]
		if(amtb)
		{
			s[(bi/8+1+amtB)%(nbits/8)] |= ((i[bi/8]&(0xffffffff<<(8-amtb)))>>(8-amtb));
			s[(bi/8+amtB)%(nbits/8)] |= ((i[bi/8]&(0xffffffff>>amtb))<<amtb);
		}
		//else if amtb=0, [0,8] bits of i[bi/8] go to [0,8] bits of s[bi/8+amtB]
		else
		{
			s[(bi/8+amtB)%(nbits/8)] |= (i[bi/8]);
		}
	}
}

void Hash(HASHINT *m, HASHINT *i, HASHINT *o, int nbits, int boff)
{
	HASHINT s[HASHLEVELBITS/8];
	memset(s, 0, sizeof(HASHINT)*HASHLEVELBITS/8);
	Shift(i, s, boff, nbits);

	for(int bi=0; bi<nbits; bi+=8)
	{
		int Bi = bi/8;
		o[Bi] = ~(m[Bi] & ~(i[Bi] & s[Bi]));
	}
}

class Map
{
public:
	HASHINT in[HASHLEVELBITS/8];
	HASHINT out[HASHLEVELBITS/8];
};

void Train(std::list<Map> *map, HASHINT *m, int nlevels, int nbits)
{
}

void TestHash()
{
	HASHINT m[HASHLEVELS][HASHLEVELBITS/8];

	HASHINT imi[HASHLEVELBITS/8];
	for(int level=0; level<HASHLEVELS; ++level)
	{
		int boff = level?1:2*level;
		HASHINT imo[HASHLEVELBITS/8];
		Hash(&m[level][0], imi, imo, HASHLEVELBITS, boff);
		for(int B=0; B<HASHLEVELBITS/8; ++B)
			imi[B] = imo[B];
	}
	LoadedTex texout;
	AllocTex(&texout, FITRESX, FITRESY, 3);
	memcpy(texout.data, imi, FITRESX*FITRESY*3);
	SavePNG2("out.png", &texout);
}
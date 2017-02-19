

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

class Mask
{
public:
	Mask* parm;	//up out
	HASHINT m[HASHLEVELBITS/8];
	HASHINT i[HASHLEVELBITS/8];
};

//sub fn
void Train2(std::list<Map> *map, int nchan, //mapping channels
			std::list<Mask> **levelmasks, //[leveli][chan]
		   int nlevels, int leveli, int nbits, int bin, HASHINT *iset, HASHINT *mset,
		   Mask *parm, Mask* subm)
{
	//check all set
	for(int i=0; i<HASHLEVELBITS/8; ++i)
	{
		if(iset[i] != MAXHASH)
			goto notset;
		if(mset[i] != MAXHASH)
			goto notset;
	}

	levelmasks[leveli][0].push_back(Mask());
	Mask *addm = &*levelmasks[leveli][0].rbegin();
	addm->parm = parm;
	for(int i=0; i<HASHLEVELBITS/8; ++i)
	{
		addm->m[i] = subm->m[i];
		addm->i[i] = subm->i[i];
	}

	//go down level

	if(!leveli)
	{
		//check in

	}

	leveli--;

	return;

notset:

	Mask subm2;
	HASHINT subiset[HASHLEVELBITS/8];
	HASHINT submset[HASHLEVELBITS/8];
	memcpy(subm2.m, subm->m, sizeof(HASHINT) * HASHLEVELBITS/8);
	memcpy(subm2.i, subm->i, sizeof(HASHINT) * HASHLEVELBITS/8);
	memcpy(subiset, iset, sizeof(HASHINT) * HASHLEVELBITS/8);
	memcpy(submset, mset, sizeof(HASHINT) * HASHLEVELBITS/8);

	int nextbin = ((bin+HASHLEVELBITS-1)%HASHLEVELBITS);	//-1
	int shiftam = (leveli?(leveli*2):1);
	int altbin = ((bin+shiftam)%HASHLEVELBITS);

	//act on [bin] bit
	//supposed to give 1:
	if( (parm->i[bin/8] & (bin%8)) )
	{
		if( !( (iset[altbin/8])&(((HASHINT)1)<<(altbin%8)) ) )	//alt bin i not set?
		{
			//all inputs and mask=0, give 1

			submset[bin/8] |= (((HASHINT)1)<<(bin%8));
			subiset[bin/8] |= (((HASHINT)1)<<(bin%8));
			subiset[altbin/8] |= (((HASHINT)1)<<(altbin%8));
			//in 00 mask0
			Train2(map, nchan,
				levelmasks, nlevels,
				leveli, nbits,
				nextbin, 
				subiset,
				submset,
				parm,
				&subm2);
			//in 01 mask0
			parm->i[altbin/8] |= ((subm->i[altbin/8])&(((HASHINT)1)<<(altbin%8)));
			Train2(map, nchan,
				levelmasks, nlevels,
				leveli, nbits,
				nextbin, 
				subiset,
				submset,
				parm,
				&subm2);
			//in 10 mask0
			Train2(map, nchan,
				levelmasks, nlevels,
				leveli, nbits,
				nextbin, 
				subiset,
				submset,
				parm,
				&subm2);
			//in 11 mask0
			Train2(map, nchan,
				levelmasks, nlevels,
				leveli, nbits,
				nextbin, 
				subiset,
				submset,
				parm,
				&subm2);

			//or input=11 and mask=1, give 1
			Train2(map, nchan,
				levelmasks, nlevels,
				leveli, nbits,
				nextbin, 
				subiset,
				submset,
				parm,
				&subm2);

		}
		else	//alt bin i is set, give 1
		{
			HASHINT altbinv = ((subm->i[altbin/8])&(((HASHINT)1)<<(altbin%8)));
			if( !altbinv )	//alt i is 0
			{
				submset[bin/8] |= (((HASHINT)1)<<(bin%8));
				subiset[bin/8] |= (((HASHINT)1)<<(bin%8));
				//in 00
				//..mask0
				Train2(map, nchan,
					levelmasks, nlevels,
					leveli, nbits,
					nextbin, 
					subiset,
					submset,
					parm,
					&subm2);
				//in 10
				//..mask0
				subm2.i[bin/8] |= (((HASHINT)1)<<(bin%8));
				Train2(map, nchan,
					levelmasks, nlevels,
					leveli, nbits,
					nextbin, 
					subiset,
					submset,
					parm,
					&subm2);
			}
			else	//alt i is 1
			{
				//in 11
				//..mask1
				//give 1

				//in 11
				//..mask 0
				//give 1

				//in 01
				//..mask0
				//give 1

			}
		}
	}
	//supposed to give 0
	else
	{
		//either input=00,01,10 and mask=1
		
	}

	//sub call
	bin=((bin+HASHLEVELBITS-1)%HASHLEVELBITS);	//--

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
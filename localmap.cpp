

#include "utils.h"
#include "localmap.h"


int main(int argc, char * argv[])
{
	OpenLog("log12.txt", APPVERSION);
	TestHash();

	return 0;
}


void TestHash()
{
#if 1
	uint64_t t = GetTicks();

	//for(int times=0; times<10; ++times)
	{
		HASHINT m[HASHLEVELS][HASHLEVELBITS/HASHINTBITS];
		memset(m, 0, HASHLEVELS * sizeof(HASHINT) * HASHLEVELBITS / HASHINTBITS);

		struct Map
		{
			HASHINT i[HASHLEVELBITS/HASHINTBITS];
			HASHINT o[HASHLEVELBITS/HASHINTBITS];
		};

		std::list<Map> map;

		unsigned __int64 r = GetTicks();

		//for(HASHINT o=0; o<pow(2,HASHLEVELBITS)-1; ++o)
		//for(HASHINT o=0; o<pow(2,HASHLEVELBITS); ++o)
		//for(HASHINT o=0; o<pow(2,HASHLEVELBITS-2)-1; ++o)
		//for(HASHINT o=0; o<HASHLEVELBITS-1; ++o)
		for(HASHINT o=0; o<HASHVALS; ++o)
		{
			int Bi = 0;

			while(true)
			{
				r *= 0x12345;
				r += 98765;

				//HASHINT i = 12;
				//HASHINT i = 23;
				//HASHINT i = 74;
				//HASHINT i = 37235;
				//HASHINT i = 1024-o;
				HASHINT i = (1024-o) ^ r;
				//HASHINT i = o;
				//	HASHINT i = HASHCROP( o+1 );
				//HASHINT i = o * MAXJUMP;

				//i &= ((1<<1) | (1<<3) | (1<<4) | (1<<5));

				//i = (3-o)&(4-1);

				Map add;
				add.i[Bi] = HASHCROP(i);
				add.o[Bi] = HASHCROP(o);

				Bi++;

				if(Bi < HASHLEVELBITS/HASHINTBITS)
					continue;

				//if(o>1)
				//	add.o = HASHCROP( -1 );

#if 1
				bool have = false;

				for(std::list<Map>::iterator mit=map.begin(); mit!=map.end(); mit++)
				{
					if(mit->i != add.i)
						continue;

					have = true;
					break;
				}

				if(have)
					continue;
#endif
				map.push_back(add);
				break;
			}
		}

		//Log("start");

		std::list<Map>::iterator mit = map.begin();

		HASHINT in[HASHVALS][HASHLEVELBITS/HASHINTBITS] = {0};
		HASHINT inval[HASHVALS][HASHLEVELBITS/HASHINTBITS] = {0};
		HASHINT inmask[HASHVALS][HASHLEVELBITS/HASHINTBITS] = {0};
		
		std::list<Mask> masks[HASHVALS];

			std::list<Tried> tried;
			std::list<Tried> wtried;

		int mci = 0;
		while(mit != map.end())
		{
			HASHINT old[HASHLEVELS][HASHLEVELBITS/HASHINTBITS];
			HASHINT m2[HASHLEVELS][HASHLEVELBITS/HASHINTBITS];
			memcpy(old, m, sizeof(HASHINT) * HASHLEVELS * HASHLEVELBITS/HASHINTBITS);
			memcpy(m2, m, sizeof(HASHINT) * HASHLEVELS * HASHLEVELBITS/HASHINTBITS);

			//Log("%u->%u", (int)mit->i, (int)mit->o);

			Mask mask;
			//memcpy(mask.ovaldown, mit->o, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);
			Sparsify(mit->o, mask.ovaldown);
			//mask.ovaldown = mit->o;
			memset(mask.mask, 0, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);
			//mask.mask = 0;
			mask.maskup = NULL;
			masks[mci].push_back(mask);

			//Log("answer2:");

			///////TODO back: Up(mit->i, &inval, &inmask);

			
			//in[mci] = mit->i;
			memcpy(in[mci], mit->i, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);
			mci++;
			mit++;

			//Log("success %u->%u", (int)mit->i, (int)mit->o);

			//uint64_t p = GetTicks() - t;

			//char ms[123];
			//sprintf(ms, "passt %llu", p);
			//Log("%s", ms);

			//t = GetTicks();

#if 0

			auto mit2 = map.begin();

			for(; mit2!=map.end(); mit2++)
			{
				HASHINT o = Hash(m, mit2->i, 0);
				Log("gives %u->%u (correct: %u)", (int)mit2->i, (int)o, (int)mit2->o);
			}
			//break;

			for(SHASHADDR x=0; x<HASHLEVELS; ++x)
			{
				Log("\t mask[%d] = %llu", (int)x, m[x]);
			}
#endif

#if 0
			if((Ahead(old, m) || Ahead(m, old)) && mit != map.begin())
				mit = map.begin();
			else
#endif
				//mit++;
		}

#if 0
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
#endif

		HASHINT d[HASHLEVELBITS/HASHINTBITS];
		memset(d, -1, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);

		if(Down(masks, m, d, HASHLEVELS-1, 
			in, true, inval, inmask,
			HASHVALS, &tried, 0, &wtried))
			//if(Try12(m, mit->i, mit->o))
			;//InfoMess("s","s");
		else
		{
			//InfoMess("f","f");

			mit = map.begin();

			for(; mit!=map.end(); mit++)
			{
				HASHINT o[HASHLEVELBITS/HASHINTBITS] = {0};
				HASHINT im[HASHLEVELBITS/HASHINTBITS] = {0};
				memcpy(im, mit->i, sizeof(HASHINT) * HASHLEVELBITS/HASHINTBITS);
				for(int mli=0; mli<HASHLEVELS; ++mli)
				{
					Hash(m[mli], im, o, HASHLEVELBITS, (mli?(mli*2):1)%HASHLEVELBITS);
					memcpy(im, o, sizeof(HASHINT) * HASHLEVELBITS/HASHINTBITS);
				}
				HASHINT unsp[HASHLEVELBITS/HASHINTBITS/SPARSITY];
				Unsparsify(o, unsp);
				Log("fail gives %u->%u(%u) (correct: %u)", (int)mit->i[0], (int)unsp[0], (int)o[0], (int)mit->o[0]);
			}

			Log("answer:");

			Log("nor");

			for(SHASHADDR x=0; x<HASHLEVELS; ++x)
			{
				Log("\t wmask[%d] = %u", (int)x, (int)m[x]);
			}

			uint64_t p = GetTicks() - t;

			char ms[123];
			sprintf(ms, "fail %llu on %d", p, 123 /*(int)mit->o[0]*/ );
			MessageBox(NULL, ms, ms, NULL);

			return;
		}

		mit = map.begin();

		for(; mit!=map.end(); mit++)
		{
			HASHINT o[HASHLEVELBITS/HASHINTBITS] = {0};
			HASHINT im[HASHLEVELBITS/HASHINTBITS] = {0};
			memcpy(im, mit->i, sizeof(HASHINT) * HASHLEVELBITS/HASHINTBITS);
			for(int mli=0; mli<HASHLEVELS; ++mli)
			{
				Hash(m[mli], im, o, HASHLEVELBITS, (mli?(mli*2):1)%HASHLEVELBITS);
				memcpy(im, o, sizeof(HASHINT) * HASHLEVELBITS/HASHINTBITS);
			}
			HASHINT unsp[HASHLEVELBITS/HASHINTBITS/SPARSITY];
			Unsparsify(o, unsp);
			Log("gives %u->%u(%u) (correct: %u)", (int)mit->i[0], (int)unsp[0], (int)o[0], (int)mit->o[0]);
		}

		//__int128 i = 123ull;

		for(SHASHADDR x=0; x<HASHLEVELS; ++x)
		{
			Log("\t mask[%d] = %u", (int)x, (int)m[x][0]);
		}
	}

	uint64_t p = GetTicks() - t;

	char ms[123];
	sprintf(ms, "pass %llu", p);
	MessageBox(NULL, ms, ms, NULL);
	//InfoMess(ms,ms);
#endif
}

#if 1

bool UniqueMask(Mask a, Mask b)
{
	for(HASHADDR i=0; i<HASHLEVELBITS/HASHINTBITS; ++i)
	{
		if (a.ovaldown[i] == b.ovaldown[i])
			continue;
		return false;
	}
	for(HASHADDR i=0; i<HASHLEVELBITS/HASHINTBITS; ++i)
	{
		if (a.mask[i] == b.mask[i])
			continue;
		return false;
	}
	for(HASHADDR i=0; i<HASHLEVELBITS/HASHINTBITS; ++i)
	{
		//if (a.maskup[i] == b.maskup[i])
		if (a.maskup == b.maskup ||
			(a.maskup && b.maskup &&
			a.maskup->mask[i] == b.maskup->mask[i]))
			continue;
		return false;
	}
	return true;
}

//a goes before b? (a<b)?
bool CompareMask(Mask& a, Mask& b)
{
	bool maskupsame = true;

#if 0
	for(SHASHADDR i=HASHLEVELBITS/HASHINTBITS-1; i>=0; --i)
	{
		//if( (a.maskup[i] != b.maskup[i]) )
		if( (a.maskup != b.maskup) &&
			( (!a.maskup || !b.maskup) ||
			(a.maskup && b.maskup && a.maskup->mask[i] != b.maskup->mask[i]) 
			) )
		{
			maskupsame = false;
			break;
		}
	}
#else
	//if(a.maskup && b.maskup && 
	//	(Ahead2(a.maskup->mask, b.maskup->mask) || Ahead2(b.maskup->mask, a.maskup->mask)) )
	//	maskupsame = false;
#endif

#if 0
	for(SHASHADDR i=HASHLEVELBITS/HASHINTBITS-1; i>=0; --i)
	{
		//if( ( a.maskup[i] < b.maskup[i] ) )
		//	return true;
		if( (a.maskup && b.maskup && a.maskup->mask[i] < b.maskup->mask[i]) )
			continue;
		//if( ( a.maskup[i] == b.maskup[i] ) )
		//	continue;
		if( (a.maskup == b.maskup) &&
			( (!a.maskup && !b.maskup) ||
			(a.maskup && b.maskup && a.maskup->mask[i] == b.maskup->mask[i]) 
			) )
			continue;
		return false;	//?
	}
#else
	if(a.maskup && b.maskup && 
		(Ahead2(a.maskup->mask, b.maskup->mask)) )
		return true;
	if(a.maskup && b.maskup && 
		(Ahead2(b.maskup->mask, a.maskup->mask)) )
		return false;
#endif
	
	for(SHASHADDR i=HASHLEVELBITS/HASHINTBITS-1; i>=0; --i)
	{
		if( ( (a.mask[i] < b.mask[i]) ) )
			return true;
		if( ( (a.mask[i] == b.mask[i]) ) )
			continue;
		return false;	//?
	}
	
	//sort ivals from GREATEST to LEAST
	for(SHASHADDR i=HASHLEVELBITS/HASHINTBITS-1; i>=0; --i)
	{
		if( ( (a.ovaldown[i] > b.ovaldown[i]) ) )
			return true;
		if( ( (a.ovaldown[i] == b.ovaldown[i]) ) )
			continue;
		return false;	//?
	}

	return false;
}

void Up(HASHINT in[HASHLEVELBITS/HASHINTBITS], HASHINT* inval, HASHINT* inmask)
{

	//up0 m0 i[#-1]1 i[#]0 if left 0 right 1
	//up0 m0 i[#-1]1 i[#]1 if 0 right left 1
	//up0 m1 i[#-1]0 i[#]1 if both 1 mask set both 0 impossible
	//up0 m1 i[#-1]1 i[#]0 if left 0 mask set
	//up1 m0 i[#-1]0 i[#]0 if left 1 right also
	//up1 m0 i[#-1]0 i[#]1 if left 1 mask set
	//up1 m1 i[#-1]0 i[#]0if left 01 mask set11
	//up1 m1 i[#-1]1 i[#]1if right 0 left
	//then justget o by m below

#if 0000
	HASHINT shift = HASHROTL(in,1);

	*inval |= HASHCROP( ~shift & ~in );
	*inmask |= *inval;

	*inmask |= HASHCROP( shift & ~in );
#endif
}

void Sparsify(HASHINT from[HASHLEVELBITS/HASHINTBITS/SPARSITY],
			  HASHINT to[HASHLEVELBITS/HASHINTBITS])
{
	memset(to, 0, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);
	for(int bi=0; bi<HASHLEVELBITS/HASHINTBITS/SPARSITY; ++bi)
	{
		HASHINT v = (from[bi/HASHINTBITS]>>(bi%HASHINTBITS));
		to[(bi*SPARSITY)/HASHINTBITS] |= (((HASHINT)v)<<((bi*SPARSITY)%HASHINTBITS));

		//for(int sbi=1; sbi<SPARSITY; ++sbi)
		//{
		//}
	}
}

void Unsparsify(HASHINT from[HASHLEVELBITS/HASHINTBITS],
			  HASHINT to[HASHLEVELBITS/HASHINTBITS/SPARSITY])
{
	for(int bi=0; bi<HASHLEVELBITS/HASHINTBITS/SPARSITY; ++bi)
	{
		HASHINT v = (from[(bi*SPARSITY)/HASHINTBITS]>>((bi*SPARSITY)%HASHINTBITS));
		to[(bi)/HASHINTBITS] |= (((HASHINT)v)<<((bi)%HASHINTBITS));
	}

}

void Fill(HASHINT oval[HASHLEVELBITS/HASHINTBITS], 
		  HASHINT ovaldown[HASHLEVELBITS/HASHINTBITS], 
		  HASHINT oset[HASHLEVELBITS/HASHINTBITS], 
		  HASHADDR fullbin, 
		  HASHADDR setbin, 
		  HASHINT mlow[HASHLEVELBITS/HASHINTBITS], 
		  HASHINT mval[HASHLEVELBITS/HASHINTBITS], 
		  std::list<Mask>* mask, 
		  HASHINT mset[HASHLEVELBITS/HASHINTBITS], 
		  bool check, 
		  HASHINT in[HASHLEVELBITS/HASHINTBITS], 
		  HASHINT inmask[HASHLEVELBITS/HASHINTBITS], 
		  HASHINT high[HASHLEVELBITS/HASHINTBITS], 
		  //HASHINT maskup[HASHLEVELBITS/HASHINTBITS],
		  Mask* maskup,
		  HASHADDR leveli,
		  HASHINT sparse[HASHLEVELBITS/HASHINTBITS])
{
#if 0
	Log("ovaldown%d oval%d mset%d oset%d q%d mlow%d inmask%d h%d !HASHCROP(~oset)%d !HASHCROP(~mset)%d HASHCROP(~oset)%d HASHCROP(~mset)%d ~oset%d ~mset%d MSHHASH%d (int)HASHLEVELBITS%d, (int)(1<<HASHLEVELBITS)%d, (int)((1<<HASHLEVELBITS)-1)%d", 
		(int)ovaldown, (int)oval,
		(int)mset, (int)oset, (int)(!HASHCROP(~oset) && !HASHCROP(~mset)),
		(int)mlow, (int)inmask, (int)high,
		(int)(!HASHCROP(~oset)), (int)(!HASHCROP(~mset)), (int)(!!(int)HASHCROP(~oset)), (int)(!!HASHCROP(~mset)), (int)(!!(int)(~oset)), (int)(~mset), (int)MAXHASH, (int)HASHLEVELBITS, (int)(1<<HASHLEVELBITS), (int)((1<<HASHLEVELBITS)-1));
#endif

#if 00000
	//early reject TODO back
	if( ((mset & mval) < (mset & mlow)) || 
		((inmask & oset & ovaldown) != (inmask & oset & in)) ||
		((mset & mval) > (mset & high)) )
		return;
#elif 01
	if(	Ahead3(mval, mlow, mset) ||
		Ahead3(high, mval, mset) )
		return;
#endif

#if 0
	if(!lowlim && DidTry(tried, ovaldown, oset))
		return false;
	if(!lowlim && wtried && DidTry(wtried, ovaldown, oset))
		return true;
#endif

	HASHADDR bin = fullbin%HASHLEVELBITS;
	//HASHADDR backbin = (bin+HASHLEVELBITS-1)%HASHLEVELBITS;

#if 0
	Log("start l%d b%d bb%d d%d o%d l%d %d %dm", 
		(int)0, (int)bin, (int)backbin, (int)ovaldown, (int)oval, (int)mval, (int)oset, (int)mset);
#endif

	//HASHINT bm = ((HASHINT)1)<<bin;
	//HASHINT bbm = ((HASHINT)1)<<backbin;

	//HASHINT shifto = HASHROTL(ovaldown,1);
	//HASHINT shiftoset = HASHROTL(oset,1);

	HASHADDR shiftam = (leveli?(leveli*2):1);	//TODO var pass

#if 0001
	//if(!HASHCROP(~oset) && !HASHCROP(~mset))
	if(bin==(HASHLEVELBITS-1) && check)
	{
		HASHINT testo[HASHLEVELBITS/HASHINTBITS];
		Hash(mval, ovaldown, testo, HASHLEVELBITS, shiftam);
		if(Ahead3(testo, oval, sparse) || Ahead3(oval, testo, sparse))
			return;

		//check unique
		for(std::list<Mask>::iterator cm=mask->begin();
			cm!=mask->end();
			++cm)
		{
			if( !Ahead2(cm->mask, mval) && !Ahead2(mval, cm->mask) &&
				!Ahead2(cm->ovaldown, ovaldown) && !Ahead2(ovaldown, cm->ovaldown))
				return;
		}

		Mask m;
		//m.mask = mval;
		//m.ovaldown = ovaldown;
		//m.maskup = maskup;
		memcpy(m.mask, mval, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);
		memcpy(m.ovaldown, ovaldown, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);
		//memcpy(m.maskup, maskup, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);
		m.maskup = maskup;
		mask->push_back(m);

		//if(!lowlim)
		//	AddTried(tried, ovaldown);

#if 0
		Log("fill sz%d level%d m%d,%d i%d,%d o%d,%d high%u,%u low%u,%u mset%u,%u", 
			(int)mask->size(), (int)leveli, 
			(int)mval[1], (int)mval[0], 
			(int)ovaldown[1],(int)ovaldown[0], 
			(int)oval[1],(int)oval[0],
			(int)high[1],(int)high[0], (int)mlow[1],(int)mlow[0],
			(int)mset[1],(int)mset[0]);
#endif
		//Log("levelfill%d m%d for %d->%d", (int)leveli, (int)mval[0], (int)ovaldown[0], (int)oval[0]);

		return;
	}
#endif

	///oset |= bm;
	///oset |= bbm;
	///mset |= bm;

	//HASHADDR nextbin = backbin;
	//HASHADDR nextbin = bin+(HASHADDR)!check;

	//if(check)
	//	nextbin = bin;

	//check = !check;

	HASHADDR nextbin = (bin+HASHLEVELBITS-1)%HASHLEVELBITS;

	HASHADDR setbi = bin;
	HASHADDR setbsi = (bin+shiftam)%(HASHLEVELBITS);
	HASHADDR setbssi = (bin+shiftam+shiftam)%(HASHLEVELBITS);

	HASHINT previval = (ovaldown[setbi/HASHINTBITS])&(((HASHINT)1)<<(setbi%HASHINTBITS));
	HASHINT prevmval = (mval[setbi/HASHINTBITS])&(((HASHINT)1)<<(setbi%HASHINTBITS));
	HASHINT prevsval = (ovaldown[setbsi/HASHINTBITS])&(((HASHINT)1)<<(setbsi%HASHINTBITS));
	
	HASHINT previset = (oset[setbi/HASHINTBITS])&(((HASHINT)1)<<(setbi%HASHINTBITS));
	HASHINT prevmset = (mset[setbi/HASHINTBITS])&(((HASHINT)1)<<(setbi%HASHINTBITS));
	HASHINT prevsset = (oset[setbsi/HASHINTBITS])&(((HASHINT)1)<<(setbsi%HASHINTBITS));

	mset[setbi/HASHINTBITS] |= (((HASHINT)1)<<(setbi%HASHINTBITS));
	oset[setbi/HASHINTBITS] |= (((HASHINT)1)<<(setbi%HASHINTBITS));

	//if(setbi != setbsi)
	oset[setbsi/HASHINTBITS] |= (((HASHINT)1)<<(setbsi%HASHINTBITS));

	HASHINT seti0 = (oset[setbi/HASHINTBITS]>>(setbi%HASHINTBITS));
	HASHINT setm0 = (mset[setbi/HASHINTBITS]>>(setbi%HASHINTBITS));

	HASHINT seti1 = (oset[setbsi/HASHINTBITS]>>(setbsi%HASHINTBITS));
	HASHINT setm1 = (mset[setbsi/HASHINTBITS]>>(setbsi%HASHINTBITS));
	
	HASHINT seti2 = (oset[setbssi/HASHINTBITS]>>(setbssi%HASHINTBITS));
				
	for(HASHINT r=0; r<2; ++r)
	{
		for(HASHINT l=0; l<2; ++l)
		{
			for(HASHINT m=0; m<2; ++m)
			{
				HASHINT i0 = (ovaldown[setbi/HASHINTBITS]>>(setbi%HASHINTBITS));
				HASHINT s0 = (ovaldown[setbsi/HASHINTBITS]>>(setbsi%HASHINTBITS));
				HASHINT m0 = (mval[setbi/HASHINTBITS]>>(setbi%HASHINTBITS));
				//HASHINT seti0 = (oset[setbi/HASHINTBITS]>>(setbi%HASHINTBITS));
				//HASHINT setm0 = (mset[setbi/HASHINTBITS]>>(setbi%HASHINTBITS));

				HASHINT sp0 = (sparse[setbi/HASHINTBITS]>>(setbi%HASHINTBITS));

				//if(seti0&setm0&seti1)
				{
					//if( HASHCROP( ~(m0 & ~(i0 & s0)) ) != (oval[setbi/HASHINTBITS]>>(setbi%HASHINTBITS)) )
					if( HASHCROP( HASHOP3 (m0 HASHOP1 HASHOP3 (i0 HASHOP2 s0)) ) & (seti0&setm0&seti1) & sp0 != 
						(oval[setbi/HASHINTBITS]>>(setbi%HASHINTBITS)) & (seti0&setm0&seti1) & sp0 )
						goto next;
				}
				
				HASHINT i1 = (ovaldown[setbsi/HASHINTBITS]>>(setbsi%HASHINTBITS));
				HASHINT s1 = (ovaldown[setbssi/HASHINTBITS]>>(setbssi%HASHINTBITS));
				HASHINT m1 = (mval[setbsi/HASHINTBITS]>>(setbsi%HASHINTBITS));
				//HASHINT seti1 = (oset[setbsi/HASHINTBITS]>>(setbsi%HASHINTBITS));
				//HASHINT setm1 = (mset[setbsi/HASHINTBITS]>>(setbsi%HASHINTBITS));
				HASHINT sp1 = (sparse[setbsi/HASHINTBITS]>>(setbsi%HASHINTBITS));
				
				//if(seti1&setm1&seti2)
				{
					//if( HASHCROP( ~(m1 & ~(i1 & s1)) ) != (oval[setbsi/HASHINTBITS]>>(setbsi%HASHINTBITS)) )
					if( HASHCROP( HASHOP3 (m1 HASHOP1 HASHOP3 (i1 HASHOP2 s1)) ) & (seti1&setm1&seti2) & sp1 !=
						(oval[setbsi/HASHINTBITS]>>(setbsi%HASHINTBITS)) & (seti1&setm1&seti2) & sp1 )
						goto next;
				}

				Fill(oval, 
					ovaldown, 
					oset, 
					nextbin, 
					setbin, 
					mlow, 
					mval, 
					mask, 
					mset, 
					true, 
					in, 
					inmask, 
					high, 
					maskup,
					leveli, 
					sparse);

next:

				//if(!case11 && !case12 && !check)
					mval[setbi/HASHINTBITS] ^= (((HASHINT)1)<<(setbi%HASHINTBITS));
				//else
				//	break;
			}

			//if(!case8 && !case10 && !check)
				ovaldown[setbsi/HASHINTBITS] ^= (((HASHINT)1)<<(setbsi%HASHINTBITS));
			//else
			//	break;
		}

		//if(!case6 && !case7 && !check)
			ovaldown[setbi/HASHINTBITS] ^= (((HASHINT)1)<<(setbi%HASHINTBITS));
		//else
		//	break;
	}

	/*
	pass down all mset, iset, mval, ival vars down,
	setting that bin vals appropriately, and cleaning up
	for parent caller.
	*/

	//HASHADDR setbi = bin;
	//HASHADDR setbsi = (bin+level)%(HASHLEVELBITS);
	//HASHADDR previval = (ovaldown[setbi/HASHINTBITS])&(((HASHINT)1)<<(setbi%HASHINTBITS));
	//HASHADDR prevmval = (mval[setbi/HASHINTBITS])&(((HASHINT)1)<<(setbi%HASHINTBITS));
	//HASHADDR prevsval = (ovaldown[setbsi/HASHINTBITS])&(((HASHINT)1)<<(setbsi%HASHINTBITS));

	//keep set all except setbi, which is unset here
	ovaldown[setbi/HASHINTBITS] &= HASHCROP( ~(((HASHINT)1)<<(setbi%HASHINTBITS)) );
	//then toggle setbi only if it was set before
	ovaldown[setbi/HASHINTBITS] |= HASHCROP( previval );

	//if(setbsi != setbi)
	{
		ovaldown[setbsi/HASHINTBITS] &= HASHCROP( ~(((HASHINT)1)<<(setbsi%HASHINTBITS)) );
		ovaldown[setbsi/HASHINTBITS] |= HASHCROP( prevsval );
	}
	
	mval[setbi/HASHINTBITS] &= HASHCROP( ~(((HASHINT)1)<<(setbi%HASHINTBITS)) );
	mval[setbi/HASHINTBITS] |= HASHCROP( prevmval );

	///set's

	oset[setbi/HASHINTBITS] &= HASHCROP( ~(((HASHINT)1)<<(setbi%HASHINTBITS)) );
	oset[setbi/HASHINTBITS] = HASHCROP( previset );
	
	mset[setbi/HASHINTBITS] &= HASHCROP( ~(((HASHINT)1)<<(setbi%HASHINTBITS)) );
	mset[setbi/HASHINTBITS] |= HASHCROP( prevmset );

	//if(setbsi != setbi)
	{
		oset[setbsi/HASHINTBITS] &= HASHCROP( ~(((HASHINT)1)<<(setbsi%HASHINTBITS)) );
		oset[setbsi/HASHINTBITS] |= HASHCROP( prevsset );
	}
}

#if 01
bool Narrow(std::list<Mask>* a, std::list<Mask>* b)
{
	if(0)
	{
		
	std::list<Mask>::iterator ait=a->begin();
	std::list<Mask>::iterator bit=b->begin();

	for(; ait!=a->end(); ++ait)
	{
		Log("narrow a m%d", (int)ait->mask[0]);
	}
	for(; bit!=b->end(); ++bit)
	{
		Log("narrow b m%d", (int)bit->mask[0]);
	}
	}

	//bool have = false;

	if(0)
	{
		for(std::list<Mask>::iterator ait=a->begin(); ait!=a->end(); ++ait)
		{
			for(std::list<Mask>::iterator bit=b->begin(); bit!=b->end(); ++bit)
			{
				if(!Ahead2(ait->mask, bit->mask) && !Ahead2(bit->mask, ait->mask))
				{
					//have = true;
					goto af;
				}
			}
		}
	}

af:
	std::list<Mask>::iterator ait=a->begin();
	std::list<Mask>::iterator bit=b->begin();

	//Log("narrow %d %d", (int)a->size(), (int)b->size());
	
	//for(auto ait2=a->begin(); ait2!=a->end(); ait2++)
	//	Log("a %d", (int)ait2->mask);
	//for(auto bit2=b->begin(); bit2!=b->end(); bit2++)
	//	Log("b %d", (int)bit2->mask);

	bool ch = false;

	while(true)
	{
		HASHINT c[HASHLEVELBITS/HASHINTBITS];

		if(bit == b->end())
		{
			ch = ch || (ait != a->end());
			while(ait != a->end())
			{
				//Log("a rem %d", (int)ait->mask[0]);
				ait = a->erase(ait);
			}
			goto check;
			return ch;
		}
		
		if(ait == a->end())
		{
			ch = ch || (bit != b->end());
			while(bit != b->end())
			{
				//Log("b rem %d", (int)bit->mask[0]);
				bit = b->erase(bit);
			}
			goto check;
			return ch;
		}

		if(bit->mask < ait->mask)
		{
			//c = ait->mask;
			memcpy(c, ait->mask, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);

			while(bit != b->end() &&
				//bit->mask < c)
				Ahead2(bit->mask, c))
			{
				//Log("b rem %d", (int)bit->mask[0]);
				bit = b->erase(bit);
				ch = true;
			}

			if(bit != b->end() &&
				//bit->mask == c)
				!Ahead2(bit->mask, c) && !Ahead2(c, bit->mask))
				goto up;

			while(ait != a->end() &&
				//ait->mask == c)
				!Ahead2(ait->mask, c) && !Ahead2(c, ait->mask))
			{
				//Log("a rem %d", (int)ait->mask[0]);
				ait = a->erase(ait);
				ch = true;
			}
		}
		else
		{
		//	c = bit->mask;
			memcpy(c, bit->mask, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);

			while(ait != a->end() &&
			//	ait->mask < c)
				Ahead2(ait->mask, c))
			{
				//Log("a rem %d", (int)ait->mask[0]);
				ait = a->erase(ait);
				ch = true;
			}
			
			if(ait != a->end() &&
			//	ait->mask == c)
				!Ahead2(ait->mask, c) && !Ahead2(c, ait->mask))
				goto up;

			while(bit != b->end() &&
			//	bit->mask == c)
				!Ahead2(bit->mask, c) && !Ahead2(c, bit->mask))
			{
				//Log("b rem %d", (int)bit->mask[0]);
				bit = b->erase(bit);
				ch = true;
			}
		}

up:

		//Log("narrow %d", (int)c);
		
		while(bit != b->end() &&
			//bit->mask == c)
			!Ahead2(bit->mask, c) && !Ahead2(c, bit->mask))
		{
			//Log("b skip %d", (int)bit->mask[0]);
			bit++;
		}

		while(ait != a->end() &&
		//	ait->mask == c)
			!Ahead2(ait->mask, c) && !Ahead2(c, ait->mask))
		{
			//Log("a skip %d", (int)ait->mask[0]);
			ait++;
		}
	}

check:

	////if(have && (
	//	!a->size() || !b->size()))
	//	Log("!!!");

	return ch;
	//if(a->size() != b->size() &&
	//	(a->size() == 0 || b->size() == 0))
	//	Log("disc");
}
#endif

/*
pass down all mset, iset, mval, ival vars down,
setting that bin vals appropriately, and cleaning up
for parent caller.
*/

//propagate down to gotten input
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
		  std::list<Tried>* wtried)
{
//	Log("lev%d m%d mask%u to%u lowlim%d od%u", (int)level, (int)masksup->size(), (int)masksup->begin()->mask[0], (int)masksup->rbegin()->mask[0], (int)lowlim, (int)masksup->begin()->ovaldown[0]);

	if(level == (HASHADDR)-1)
	{
		std::list<Mask>::iterator* muits = new std::list<Mask>::iterator[nmc];

		for(HASHADDR mci=0; mci<nmc; ++mci)
		{
			if(!masksup[mci].size())
			{
				delete [] muits;
				return false;
			}
			muits[mci] = masksup[mci].begin();
		}

		HASHINT setm[HASHLEVELBITS/HASHINTBITS];
		memcpy(setm, muits[0]->mask, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);
nextdmc:
#if 01
		for(HASHADDR mci=0; mci<nmc; ++mci)
		{
			while(muits[mci] != masksup[mci].end() &&
				( Ahead2(muits[mci]->mask, setm) ||
				( Ahead2(muits[mci]->ovaldown, in[mci]) || Ahead2(in[mci], muits[mci]->ovaldown) ) 
				) )
			{
				++(muits[mci]);
			}	
#if 0
			for(; muits!=masksup[downmci].end() && (!Ahead2(masksup); muit++)
			{

				//Log("newupmask=%u in=%u", (int)muit->mask, (int)muit->ovaldown);

				//if (muit->ovaldown != in)
				if(Ahead2(muit->ovaldown, in[downmci]) || Ahead2(in[downmci], muit->ovaldown))
					continue;//check other mci's


				//Log("newupmask=%u in=%u", (int)muit->mask, (int)muit->ovaldown);

				//*maskup = muit->mask;
				memcpy(maskup, muit->mask, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);
				return true;
			}
#endif
			if(muits[mci] == masksup[mci].end())
			{
				delete [] muits;
				return false;
			}

			if(Ahead2(setm, muits[mci]->mask))
			{
				memcpy(setm, muits[mci]->mask, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);
				goto nextdmc;
			}
		}

		for(HASHADDR mci=0; mci<nmc; ++mci)
		{
			Mask* mp = NULL;
			
			if(muits[mci] != masksup[mci].end())
				mp = &*muits[mci];

			HASHINT o[HASHLEVELBITS/HASHINTBITS] = {0};
			HASHINT im[HASHLEVELBITS/HASHINTBITS] = {0};
			memcpy(im, in[mci], sizeof(HASHINT) * HASHLEVELBITS/HASHINTBITS);
			for(HASHADDR mli=0; mli<HASHLEVELS; ++mli)
			{
				Hash(mp?mp->mask:mval[mli], im, o, HASHLEVELBITS, (mli?(mli*2):1)%HASHLEVELBITS);
				memcpy(im, o, sizeof(HASHINT) * HASHLEVELBITS/HASHINTBITS);
				Log("==lev%d mci%d i%d m%d o%d (ri%d, rm%d)", (int)mli, (int)mci, 
					(int)im[0], 
					mp?(int)mp->mask[0]:(int)-1, //(int)mval[mli][0], 
					(int)o[0],
					mp?((int)mp->ovaldown[0]):(int)-1, mp?((int)mp->mask[0]):(int)-1);

				if(mp)
					mp = mp->maskup;
			}
			
				Log("==lev%d mci%d i%d (ri%d)", -1, (int)mci, 
					(int)im[0], 
					mp?((int)mp->ovaldown[0]):(int)-1);
		}


#if 0
		for(HASHADDR mci=0; mci<nmc; ++mci)
		{
			HASHINT o[HASHLEVELBITS/HASHINTBITS] = {0};
			HASHINT im[HASHLEVELBITS/HASHINTBITS] = {0};
			memcpy(im, mit->i, sizeof(HASHINT) * HASHLEVELBITS/HASHINTBITS);
			for(int mli=0; mli<HASHLEVELS; ++mli)
			{
				Hash(m[mli], im, o, HASHLEVELBITS, (mli?(mli*2):1)%HASHLEVELBITS);
				memcpy(im, o, sizeof(HASHINT) * HASHLEVELBITS/HASHINTBITS);
			}
			Log("gives %u->%u (correct: %u)", (int)mit->i[0], (int)o[0], (int)mit->o[0]);
		}

		//__int128 i = 123ull;

		for(SHASHADDR x=0; x<HASHLEVELS; ++x)
		{
			Log("\t mask[%d] = %u", (int)x, (int)m[x][0]);
		}
#endif
		
		memcpy(maskup, setm, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);
		delete [] muits;
		return true;
#else

		//*maskup = masksup[0].begin()->mask;
		memcpy(maskup, masksup[0].begin()->mask, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);
		return true;
#endif

		//return false;
	}

	HASHINT sparse[HASHLEVELBITS/HASHINTBITS];

#if 01
	if(level == HASHLEVELS-1)
	{
		memset(sparse, 
			(1<<0)+ 
			(1<<2)+ 
			(1<<4)+ 
			(1<<6)+ 
			(1<<8)+ 
			(1<<10)+ 
			(1<<12)+ 
			(1<<14)+ 
			(1<<16)+ 
			(1<<18)+ 
			(1<<20)+ 
			(1<<22)+ 
			(1<<24)+ 
			(1<<26)+ 
			(1<<28)+
			(1<<30),
			sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);
	}
	else
#endif
	{
		memset(sparse, -1, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);
	}

	//HASHINT m = mval[level];
	//HASHINT mset = HASHCROP( (HASHINT)-1 );
	HASHINT mset[HASHLEVELBITS/HASHINTBITS];
	HASHINT oval[HASHLEVELBITS/HASHINTBITS];
	memset(mset, 0, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);
	memset(oval, 0, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);
	//HASHINT mlow = mval[level];

	if(!lowlim)
		//mval[level] = 0;
		memset(mval[level], 0, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);

	//HASHADDR upbin=0;
	HASHADDR upbin=HASHLEVELBITS-1;

	//if(!lowlim)
	//	upbin = HASHLEVELBITS-1;

	HASHINT* zeromask = new HASHINT[nmc*HASHLEVELBITS/HASHINTBITS];
	memset(zeromask, 0, sizeof(HASHINT)*nmc*HASHLEVELBITS/HASHINTBITS);

	HASHINT* inmask = zeromask;
	//HASHINT inmask = 0;

	if(!level)
	{
		//inmask = (HASHINT)-1;
		//inmask = (HASHINT)-1;
		memset(zeromask, -1, sizeof(HASHINT)*nmc*HASHLEVELBITS/HASHINTBITS);


		//Log("zeromask[0]", (int)zeromask[0]);
		//Log("inmask[0]", (int)inmask[0]);
	}

	HASHINT inval[HASHVALS][HASHLEVELBITS/HASHINTBITS];
	memcpy(inval, in, sizeof(HASHINT)*HASHVALS*HASHLEVELBITS/HASHINTBITS);

	if(level==1)
	{
		memcpy(inval, upin, sizeof(HASHINT)*HASHVALS*HASHLEVELBITS/HASHINTBITS);
		memcpy(inmask, upinmask, sizeof(HASHINT)*HASHVALS*HASHLEVELBITS/HASHINTBITS);
	}

	HASHINT high[HASHLEVELBITS/HASHINTBITS];
	//memcpy(high, mval[level], sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);
	HASHINT lowest[HASHLEVELBITS/HASHINTBITS];
	//memcpy(lowest, mval[level], sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);
	memset(high, 0, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);
	memset(lowest, 0, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);

		high[0] = 1;
#if 0
	if(!high)
	{
		high = 1;
	}
#elif 0
	bool hz = true;

	for(int hzi=0; hzi<HASHLEVELBITS/HASHINTBITS; hzi++)
	{
		if(high[hzi])
		{
			hz = false;
			break;
		}
	}

	if(hz)
	{
		memset(high, 0, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);
		//high[HASHLEVELBITS/HASHINTBITS-1] = 1;
		high[0] = 1;
	}
#endif

	//high = (HASHINT)-1;

	std::list<Mask>* masks = new std::list<Mask>[nmc];
	//HASHINT newupmask = 0;
	HASHINT newupmask[HASHLEVELBITS/HASHINTBITS];
	//if(lowlim)
	//	memcpy(newupmask, mval[level], sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);
	//else
	memset(newupmask, 0, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);
	HASHADDR mci = 0;

	//if(masksup->size())
	//memcpy(newupmask, masksup->begin()->mask, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);

	//lowest = 0;
	//high = (HASHINT)-1;
	//mval[level] = 0;
	//memset(lowest, 0, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);
	//memset(high, -1, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);
	//memset(mval[level], 0, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);
	//lowlim = false;
	//high[HASHLEVELBITS/HASHINTBITS-1] = 1;
	high[0] = 1;

	std::list<Tried> mtried;

	//for(HASHADDR mci=0; mci<nmc; mci++)
	{

	}

#if 0
	for(std::list<Mask>::iterator muit2=masksup[0].begin();
		muit2!=masksup[0].end();
		++muit2)
			Log("muit2 m%d i%d", (int)muit2->mask[0], (int)muit2->ovaldown[0]);
#endif
	//for(; upbin<HASHLEVELBITS;)
	//while(high)
	while(mci<nmc)
	{
skipmc:

		//std::list<Mask>::iterator muit = masksup->begin();
		//std::list<Mask>::iterator start = muit;

		std::list<Mask>* mu = &masksup[mci];
		std::list<Mask>* m = &masks[mci];

		std::list<Mask>::iterator muit = mu->begin();

//nextmu:
		//std::list<Mask> masks;
		//BinTree tried;
		//std::list<Tried>* tlc = &tried[nmc*level+mci];

		while(muit != mu->end() &&
			(Ahead2(muit->mask, newupmask)))
			muit = mu->erase(muit);

//		std::list<Mask>::iterator start = muit;

		//while(muit!=mu->end() && muit->mask < upmask)
			////muit++;
			//muit = mu->erase(muit);

		while(muit!=mu->end())
		{
			//Log("muit m%d i%d", (int)muit->mask[0], (int)muit->ovaldown[0]);

			//HASHINT newupmask[HASHLEVELBITS/HASHINTBITS];
			HASHINT oval[HASHLEVELBITS/HASHINTBITS];
			//HASHINT newupmask = muit->mask;
			//HASHINT oval = muit->ovaldown;
			memcpy(newupmask, muit->mask, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);
			memcpy(oval, muit->ovaldown, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);
			//HASHINT oshift = HASHROTR(oval,1);

			if(lowlim && Ahead2(maskup, newupmask))
			//if(lowlim && Ahead2(maskup, muit->mask))
				//if(newupmask > *maskup && lowlim)
			{
				//mval[level] = 0;
				//lowest = 0;
				//high = 1;
				memset(lowest, 0, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);
				memset(high, 0, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);
				high[0] = 1;
				lowlim = false;
				memset(mval[level], 0, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);
				//memcpy(mval[level], newupmask, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);
				//memset(lowest, 0, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);
				//memset(high, 0, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);
				//high[HASHLEVELBITS/HASHINTBITS-1] = 1;
				//upbin = HASHLEVELBITS-1;
			}

			if(m->size() && muit->maskup)
			Log("lev%d m%d continue mci%d mask%u lm%d mum%d, muum%d, mui%d,, mm%d, mi%d", 
			(int)level, (int)masksup[mci].size(), 
				(int)mci, (int)newupmask[0], (int)masks[mci].size(), (int)muit->mask[0],  (int)muit->maskup->mask[0],  
				(int)muit->ovaldown[0],
				(int)m->rbegin()->mask[0], (int)m->rbegin()->ovaldown[0]);
			else if(muit->maskup)
				
			Log("lev%d m%d continue mci%d mask%u lm%d mum%d, muum%d, mui%d,, mm%d, mi%d", 
			(int)level, (int)masksup[mci].size(), 
				(int)mci, (int)newupmask[0], (int)masks[mci].size(), (int)muit->mask[0],  (int)muit->maskup->mask[0], 
				(int)muit->ovaldown[0],
				(int)-1, (int)-1);
			else
			Log("lev%d m%d continue mci%d mask%u lm%d mum%d, muum%d, mui%d,, mm%d, mi%d", 
			(int)level, (int)masksup[mci].size(), 
				(int)mci, (int)newupmask[0], (int)masks[mci].size(), (int)muit->mask[0],  (int)-1, 
				(int)muit->ovaldown[0],
				(int)-1, (int)-1);

			HASHINT ovaldown[HASHLEVELBITS/HASHINTBITS];
			HASHINT oset[HASHLEVELBITS/HASHINTBITS];
			memset(ovaldown, 0, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);
			memset(oset, 0, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);

#if 01
				//we fill even though muit->mask might > newupmask?
			//if(muit != mu->end() 
			//	//|| muit->mask > newupmask
			//	&& !Ahead2(newupmask, muit->mask)
			//	)
			Fill(oval, 
				ovaldown, 
				oset, 
				HASHLEVELBITS-1, 
				HASHLEVELBITS-1, 
				lowest, 
				mval[level], 
				m, 
				mset, 
				false, 
				inval[mci], 
				&inmask[mci*HASHLEVELBITS/HASHINTBITS], 
				high, 
				&*muit, //newupmask, 
				level,
				sparse);
#endif

			muit++;	//also up newupmask above^^^?

			if(muit == mu->end() 
				//|| muit->mask > newupmask
				|| Ahead2(newupmask, muit->mask)
				)
			{
			//	Log("btb%d", (int)m->size());
			//	Log("bt0b%d", (int)masks[0].size());
			//	Log("bt1b%d", (int)masks[1].size());
				//masks.sort(CompareMask);
				//masks.unique(UniqueMask);
#if 01
				m->sort(CompareMask);
				//for(HASHADDR mci2=0; mci2<=mci; ++mci2)
				//	masks[mci2].sort(CompareMask);
						//if(!masks[0].size())
						//	Log("!0aa0aa");
						//if(!masks[1].size())
						//	Log("!caacaa");
				for(HASHADDR mci2=0; mci2<mci; ++mci2)
				{
				//Log("11atb%d", (int)m->size());
				//Log("11at0b%d", (int)masks[0].size());
			//	Log("11at1b%d", (int)masks[1].size());
					if(Narrow(m, &masks[mci2]))
					{
						///////mci2=0;
				//		if(!masks[0].size())
				//			Log("!00");
				//		if(!masks[mci2].size())
				//			Log("!cc");
					}
				//		if(!masks[0].size())
				//			Log("!0aa2");
				//		if(!masks[1].size())
				//			Log("!caa2caa2");

						
				//Log("1atb%d", (int)m->size());
				//Log("1at0b%d", (int)masks[0].size());
				//Log("1at1b%d", (int)masks[1].size());
				}
#endif
				
			//	Log("atb%d", (int)m->size());
			//	Log("at0b%d", (int)masks[0].size());
			//	Log("at1b%d", (int)masks[1].size());

#if 0
				//check for diff'ing maskup's between channels "masks" var
				for(HASHINT mci2=1; mci2<=mci; ++mci2)
				{
					for(std::list<Mask>::iterator mit1=masks[0].begin();
						mit1!=masks[0].end();
						++mit1)
					{
						bool found = false;

						for(std::list<Mask>::iterator mit2=masks[mci2].begin();
							mit2!=masks[mci2].end();
							++mit2)
						{
							//if(!mit1->maskup || !mit2->maskup)
							{
							//	found=true;
							//	break;
							}
							//if(!Ahead2(mit1->maskup->mask, mit2->maskup->mask) &&
							//	!Ahead2(mit2->maskup->mask, mit1->maskup->mask))
							{
							//	found = true;
							//	break;
							}
							if(!Ahead2(mit1->mask, mit2->mask) &&
								!Ahead2(mit2->mask, mit1->mask))
							{
								found = true;
							Log("f! am%d bm%d", (int)mit1->mask[0], (int)mit2->mask[0]);
								goto af;
								break;
							}
						}

						if(!found)
						{
							//ErrMess("nf!","nf1");
							//Log("!nf");
						}
						else
						{
							//Log("f! am%d bm%d", );
						}
					}
				}
#endif
af:

				if(mci == nmc-1)
				{
				//Log("tb%d", (int)m->size());
				//Log("t0b%d", (int)masks[0].size());
				//Log("t1b%d", (int)masks[1].size());
#if 0
					for(HASHADDR mci2=0; mci2<nmc; ++mci2)
						masks[mci2].sort(CompareMask);
					for(HASHADDR mci2=1; mci2<nmc; ++mci2)
					{
						if(Narrow(&masks[0], &masks[mci2]))
						{
							mci2=0;
							//if(!masks[0].size())
							//	ErrMess("!0a","0a");
							//if(!masks[mci2].size())
							//	ErrMess("!ca","ca");
						}
					}
#endif
				//Log("2tb%d", (int)m->size());
				//Log("2t0b%d", (int)masks[0].size());
				//Log("2t1b%d", (int)masks[1].size());
					//m->sort(CompareMask);
					//m->unique(UniqueMask);	//only for 1 ch

					Log("nmc-1");

					//if(submasks.size() && Down(&submasks, mval, &mval[level], level-1, in, lowlim, upin, upinmask))
					if(m->size() && Down(masks, mval, mval[level], level-1, 
						in, lowlim, upin, upinmask, nmc, tried, mci, wtried))
					{
						Log("m from%u to %u", (int)masks[mci].begin()->maskup->mask[0], 
							(int)masks[mci].rbegin()->maskup->mask[0]);

						//Log("set mask[%d] = %u", (int)level, (int)mval[level]);
						//*maskup = newupmask;
						memcpy(maskup, newupmask, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);

						delete [] masks;
						delete [] zeromask;

						//Log("success");

						return true;
					}

					for(; mci!=(HASHADDR)-1; --mci)
						masks[mci].clear();

#if 01
					memcpy(mval[level], high, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);

					HASHINT low0 = 0;
					HASHADDR lowi = 0;

					while(!low0 && lowi < HASHLEVELBITS/HASHINTBITS)
					{
							low0 = HASHCROP( LOWEST0( high[lowi] ) );
							++lowi;
					}

					if(low0)
					{
						memcpy(lowest, high, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);
						high[lowi-1] |= low0;
						//muit = start;
					}
					else
					{
#endif
						if(muit == mu->end())
						{
							delete [] masks;
							delete [] zeromask;
							return false;
						}
#if 01
						memset(lowest, 0, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);
						memcpy(high, lowest, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);
						high[0] = 1;

#if 0
						bool allz = true;
						for(HASHADDR zi=0; zi<HASHLEVELBITS/HASHINTBITS; ++zi)
						{
							if(high[zi])
							{
								allz = false;
								break;
							}
						}
						if(allz)
						{
							//memset(lowest, 0, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);
							//high[HASHLEVELBITS/HASHINTBITS-1] = 1;
							high[0] = 1;
						}
#endif

						memcpy(mval[level], lowest, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);
#endif
						//memcpy(mval[leveli], muit->mask, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);
						memcpy(newupmask, muit->mask, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);
					}

					mci = 0;
					goto skipmc;
				} //if(mci == nmc-1)

				//muit = start;
				goto nextmc;
			} /* if(muit == mu->end() 
				//|| muit->mask > newupmask
				|| Ahead2(newupmask, muit->mask)
				) */

#if 0
			Fill(oval, 
				ovaldown, 
				oset, 
				HASHLEVELBITS-1, 
				HASHLEVELBITS-1, 
				mval[level], 
				mval[level], 
				m, 
				mset, 
				false, 
				inval[mci], 
				&inmask[mci*HASHLEVELBITS/HASHINTBITS], 
				high, 
				&*muit, //newupmask, 
				level);
#endif

		}	//while(muit!=mu->end())

nextmc:
		++mci;
		//mval[level] = high;
		//high <<= 1;
	}	//while(mci<nmc)

	delete [] masks;
	delete [] zeromask;
	return false;
}

void Shift(HASHINT *i, HASHINT *s, HASHADDR amt, HASHADDR nbits)
{
	HASHADDR amtB = amt/HASHINTBITS;
	HASHADDR amtb = amt%HASHINTBITS;

	for(HASHADDR bi=0; bi<nbits; bi+=HASHINTBITS)
	{
		//if amtb>0, last [8-amtb,8] bits of i[bi/8] go to first [0,amtb] bits of s[bi/8+1+amtB]
		//and first [0,8-amtb] bits of i[bi/8] go to [amtb,8] bits of s[bi/8+amtB]
		if(amtb)
		{
			s[(bi/HASHINTBITS+1+amtB)%(nbits/HASHINTBITS)] |= 
				((i[bi/HASHINTBITS]&((MAXHASH)<<(HASHINTBITS-amtb)))>>(HASHINTBITS-amtb));
			s[(bi/HASHINTBITS+amtB)%(nbits/HASHINTBITS)] |= 
				((i[bi/HASHINTBITS]&((MAXHASH)>>amtb))<<amtb);
		}
		//else if amtb=0, [0,8] bits of i[bi/8] go to [0,8] bits of s[bi/8+amtB]
		else
		{
			s[(bi/HASHINTBITS+amtB)%(nbits/HASHINTBITS)] |= (i[bi/HASHINTBITS]);
		}
	}
}

void Hash(HASHINT *m, HASHINT *i, HASHINT *o, HASHADDR nbits, HASHADDR boff)
{
	HASHINT s[HASHLEVELBITS/HASHINTBITS];
	memset(s, 0, sizeof(HASHINT)*HASHLEVELBITS/HASHINTBITS);
	Shift(i, s, boff, nbits);

	for(int bi=0; bi<nbits; bi+=HASHINTBITS)
	{
		HASHADDR Bi = bi/HASHINTBITS;
		o[Bi] = HASHCROP( HASHOP3 (m[Bi] HASHOP1 HASHOP3 (i[Bi] HASHOP2 s[Bi])) );
	}
}

#if 0 //old
HASHINT Hash(HASHINT mval[HASHLEVELS][HASHLEVELBITS/HASHINTBITS], 
			 HASHINT oval[HASHLEVELBITS/HASHINTBITS], 
			 HASHADDR level)
{
	if(level >= HASHLEVELS)
		return oval;

	//HASHINT shifto = HASHROTL(oval,1);
	HASHINT shifto[HASHLEVELBITS/HASHINTBITS] = {0};
	Shift(oval, shifto, level?1:(level*2), HASHLEVELBITS);
	const HASHINT ovalup[HASHLEVELBITS/HASHINTBITS] = {0};
	//TODO ::::
	ovalup[0] = HASHCROP( shifto[0] ^ ~(oval & mval[level]) );

	Log("level%d up%u o%u os%u", (int)level, (int)ovalup, (int)oval, (int)shifto);

	return Hash(mval, ovalup, level+1);
}
#endif

#endif
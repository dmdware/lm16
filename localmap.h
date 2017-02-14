

#ifndef LOCALMAP_H
#define LOCALMAP_H

#include "platform.h"
#include "texture.h"

//memory localizing hash map with changing hash function cipher key

#define LOWEST0(x)			((~x) & (x + 1))
#define LOWEST1(x)			(x & ((~x) + 1))

#define HASHINT		unsigned char
#define FITRESX		16
#define FITRESY		8
#define HASHLEVELBITS	(FITRESX*FITRESY*3*8)
#define HASHINTBITS		(8*sizeof(HASHINT))
#define HASHLEVELS	8

void TestHash();

#endif

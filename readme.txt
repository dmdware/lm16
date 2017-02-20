As of now I'm putting off work on this until sometime in the future.

What this is, is a binarized neural network (BNN) using NAND.

If anybody wants to have a crack at this, they're welcome.

It might work with 16-32 bit wide hashes (HASHINTBITS=HASHLEVELBITS=16-32).

Fill should be changed to use the better NAND hashing function,
instead of XOR-NAND (which requires 1/3+ more levels and is redundant,
while NAND by itself is functionally complete).

I wanted to use this to encode 640x480x3 byte video, but without
a better way to take care of the rapid increase in input-value entries
at the top or second top level (since this is pretty much brute force
trying most of the possibilities) in a case where you're mapping something
to all-0's. 

E.g., all these input values work to give this out with the mask:

out 00000000
mk 11111111
INV 0000000
in2 00000001
in3 00000010
in4 00000100
in5 00001000
in6 00001001
in7 00001010

Also, in lm15/old there are some (...13.cpp?) other versions
that try to speed up the computation with doing several mapping
"channels" together. The one in here (lm16) is taken from XOR-NAND
...14.cpp in lm15 and adapted to NAND.

The difficulty with using an array instead of a single integer is
the mask and intermediate values have to be passed by reference,
wherever they're needed (Down, Fill), and each one has to take
care to undo its change to the values when it returns.

With XOR-NAND it was working with 4 bits or more, but there were too many
possiblities with higher bit levels. With NAND it should probably work
with more, (maybe even 16-32, which would allow encoding of an RGB
sequence to work as an image format). 

 
Here's where it started http://www.gamedev.net/topic/676377-local-hash/

Then it turned out the hash function with NAND+XOR was superfluous and 
less than functionally complete so now it is NAND only.

It is a hash and a kind of binaries neural network I'm trying to 
make to associate/map inputs to outputs.

You can use any mask using an input, but given an output and 
working backwards you can only have certain masks/mvals that 
will give those outputs with certain intermediate/input values. 
But the possibilities increase as you go further down closer to 
input level. This can be alleviated by narrowing down at each 
level for masks that work for all of the required mappings from 
that point and above (same set of mvals). 

The "pyramid" way of mixing bits allows each bit to influence each other
without having to have as many levels as it is bits wide (simply shifting
by one each level with the shift bits as second input to the hash).
With 20 levels, there is a reach of 2^20 bits apart. The amount to mix
using pyramid is: level?(level*2):1, that is if level==0 then 1, otherwise 
2*level.


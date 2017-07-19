#include "DNA.h"

DNA::DNA() : 
tail(4,10)
{
	tail.setRandom();
}

shared_ptr<DNA> DNA::mutate(float rate)const
{
	shared_ptr<DNA> dna = make_shared<DNA>();
	dna->tail = tail.getRandom(rate);
	return dna;
}

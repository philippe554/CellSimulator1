#include "DNA.h"
#include "WorldSettings.h"

DNA::DNA() : 
tail(8,4),
membrane(WorldSettings::e_AmountOfParticles,1)
{
	tail.setRandom();
	membrane.setRandom();
	fitness = 0;
}

shared_ptr<DNA> DNA::mutate(float rate)const
{
	shared_ptr<DNA> dna = make_shared<DNA>();
	dna->tail = tail.getRandom(rate);
	dna->membrane = membrane.getRandom(rate);
	return dna;
}

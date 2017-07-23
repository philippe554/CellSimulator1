#pragma once
#include <memory>

class DNA;

#include "Matrix.h"

using namespace std;

class DNA
{
public:
	DNA();
	
	shared_ptr<DNA> mutate(float rate)const;

	Matrix tail;
	Matrix membrane;

	float fitness;
};
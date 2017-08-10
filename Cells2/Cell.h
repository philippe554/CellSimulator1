#pragma once

class Cell;

#include "World.h"
#include "CellFrame.h"
#include "Vector.h"
#include "Point.h"
#include "Joint.h"
#include "Line.h"
#include "DNA.h"
#include "Membrane.h"



class Cell : public CellFrame{
public:
	Cell(shared_ptr<DNA> tDna, World*tWorld, Vector tCenter, double tRadius);
	~Cell();

	bool connectCells(Cell*other);
	void disconnectCells(int );

	Membrane* getOuterMembrane()const;

	int getId()const;

	void applyForce(Vector&v);

	shared_ptr<DNA> getDNA();	
private:

	vector<shared_ptr<Point>> tail;

	
	vector<Joint*> tailJoints;

	Membrane* outerMembrane;

	shared_ptr<DNA> dna;

	Cell*connectedCells[AmountOfEdges];
	

	int id;
	double radius;

	static int idCounter;

	double tailCounter;
};



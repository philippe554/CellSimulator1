#include "Simulator.h"
#include "Color.h"
#include <ctime>
#include <cstdlib>
#include "Writer.h"
#include <random>

Simulator::Simulator(int x, int y, int xSize, int ySize) 
:View(x, y, xSize, ySize), world(WorldSettings())
{
	scale = 8;
	xOffset = 0;
	yOffset = 0;
	srand(time(NULL));
	bestDNA.push_back(make_shared<DNA>());
}

void Simulator::render(ID2D1HwndRenderTarget* RenderTarget)
{
	Cell* selectedCell = nullptr;

	for (auto& chunk : world.chunks) {
		for (int i = 0; i < world.ws.chunkSize*world.ws.chunkSize; i++)
		{
			Block*block = chunk.second->findBlock_N(i);
			drawRectrangle(RenderTarget, Vector(block->getbx()*world.ws.blockSize, block->getby()*world.ws.blockSize),
				Vector(block->getbx()*world.ws.blockSize + world.ws.blockSize, block->getby()*world.ws.blockSize + world.ws.blockSize), Color::gray());
			
			for (int i=0;i<block->getAmountOfPoints();i++)
			{
				drawCircle(RenderTarget, block->getPoint(i)->getPlace(), block->getPoint(i)->getRadius(), Color::gray());
			}
		}
		drawRectrangle(RenderTarget, Vector(chunk.first.first*world.ws.blockSize*world.ws.chunkSize, chunk.first.second*world.ws.blockSize*world.ws.chunkSize),
			Vector(chunk.first.first*world.ws.blockSize*world.ws.chunkSize + world.ws.blockSize*world.ws.chunkSize, chunk.first.second*world.ws.blockSize*world.ws.chunkSize + world.ws.blockSize*world.ws.chunkSize), Color::black());
	
		for (int i = 0; i<chunk.second->getAmountOfPoints(); i++)
		{
			drawCircle(RenderTarget, chunk.second->getPoint(i)->getPlace(), chunk.second->getPoint(i)->getRadius(), Color::gray());
		}

	}

	for (auto& chunk : world.chunks) 
	{
		for (int i = 0; i < world.ws.chunkSize*world.ws.chunkSize; i++)
		{
			Block*block = chunk.second->findBlock_N(i);
			for (int j = 0; j < block->getAmountOfLines(); j++)
			{
				drawLine(RenderTarget, block->getLine(j)->getV1(), block->getLine(j)->getV2(), Color::red());
			}

			for (int k = 0; k < block->getAmountOfCells(); k++)
			{
				Cell* cell = block->getCell(k);
				for (int j = 0; j < cell->getAmountOfPoints(); j++)
				{
					drawCircle(RenderTarget, cell->getPointPlace(j), cell->getRadius(j), Color::black());
				}
				for (int j = 0; j < cell->getAmountOfEdgeJoints(); j++)
				{
					drawLine(RenderTarget, cell->getEdgeJoint(j,true), cell->getEdgeJoint(j,false), Color::black());
				}
				for (int j = 0; j < cell->getAmountOfradiusJoints(); j++)
				{
					drawLine(RenderTarget, cell->getRadiusJoint(j, true), cell->getRadiusJoint(j, false), Color::black());
				}
				for (int j = 0; j < cell->getAmountOfSplitJoints(); j++)
				{
					drawLine(RenderTarget, cell->getSplitJoint(j, true), cell->getSplitJoint(j, false), Color::black());
				}
				for (int j = 0; j < cell->getAmountOfTailJoints(); j++)
				{
					drawLine(RenderTarget, cell->getTailJoint(j, true), cell->getTailJoint(j, false), Color::black());
				}

				if (selectedID == cell->getId())
				{
					selectedCell = cell;
					drawCircle(RenderTarget, cell->getCenter(), 15, Color::red());
				}
			}
		}
	}
	double ratio = 0;
	if (world.ws.stats_CellsCreated != 0) {
		ratio = double(world.ws.stats_PointsCreated - world.ws.stats_PointsDestroyed) / double(world.ws.stats_CellsCreated-world.ws.stats_CellsBroken);
	}
	Writer::print("Calc time: "+to_string(simulationTime),Color::black(),Writer::normal(),{0,0,400,50});
	Writer::print("World time: "+ to_string(world.getTime()), Color::black(), Writer::normal(), { 0,50,400,100 });
	Writer::print("Cells broken: " + to_string(world.ws.stats_CellsBroken), Color::black(), Writer::normal(), { 0,100,400,150 });

	Block* mouseOnBlock = world.findBlock_B(world.calcBlock((mouseX-xOffset) / scale), world.calcBlock((mouseY-yOffset) / scale));
	if (mouseOnBlock != nullptr)
	{
		Writer::print("Block: (" + to_string(mouseOnBlock->getbx()) + ","+to_string(mouseOnBlock->getby())+")", Color::black(), Writer::normal(), { 0,400,400,50 });
		/*Writer::print("Temperature: " + to_string(mouseOnBlock->getTemperature()), Color::black(), Writer::normal(), { 0,440,400,50 });
		Writer::print("Mass: " + to_string(mouseOnBlock->getMass()), Color::black(), Writer::normal(), { 0,480,400,50 });
		Writer::print("Presure: " + to_string(mouseOnBlock->getPressure()), Color::black(), Writer::normal(), { 0,520,400,50 });
		Writer::print("Volume: " + to_string(mouseOnBlock->getVolume()), Color::black(), Writer::normal(), { 0,560,400,50 });
		for(int i=0;i<WorldSettings::e_AmountOfParticles;i++)
		{
			Writer::print(to_string(i)+": M = " + to_string(mouseOnBlock->getParticle(i)) +
				", C = " + to_string(mouseOnBlock->getConcentrationPoint(i, Vector(mouseX / scale, mouseY / scale)))
				, Color::black(), Writer::normal(), { 0,float(600+i*40),600,50 });
		}*/
	}

	if (selectedCell != nullptr)
	{
		Writer::print("Cell:", Color::black(), Writer::normal(), { 0,800,400,50 });
		//Writer::print("Temperature: " + to_string(selectedCell->getTemperature()), Color::black(), Writer::normal(), { 0,840,400,50 });
		//Writer::print("Mass: " + to_string(selectedCell->getMass()), Color::black(), Writer::normal(), { 0,880,400,50 });
		//Writer::print("Presure: " + to_string(selectedCell->getPressure()), Color::black(), Writer::normal(), { 0,920,400,50 });
		//Writer::print("Volume: " + to_string(selectedCell->getVolume()), Color::black(), Writer::normal(), { 0,960,400,50 });
		//for (int i = 0; i<WorldSettings::e_AmountOfParticles; i++)
		//{
		//	Writer::print(to_string(i) + ": M = " + to_string(selectedCell->getParticle(i)) +
		//		", C = " + to_string(selectedCell->getConcentration(i))
		//		, Color::black(), Writer::normal(), { 0,float(1000 + i * 40),600,50 });
		//}
	}
}

void Simulator::drawLine(ID2D1HwndRenderTarget* RenderTarget, Vector* v1, Vector* v2, ID2D1Brush* c)
{
	RenderTarget->DrawLine({ static_cast<float>(v1->getX()*scale+xOffset) , static_cast<float>(v1->getY()*scale+yOffset) },
	{ static_cast<float>(v2->getX()*scale+xOffset), static_cast<float>(v2->getY()*scale+yOffset) }, c);
}

void Simulator::drawLine(ID2D1HwndRenderTarget* RenderTarget, Vector v1, Vector v2, ID2D1Brush* c)
{
	RenderTarget->DrawLine({ static_cast<float>(v1.getX()*scale+xOffset) , static_cast<float>(v1.getY()*scale+yOffset) },
	{ static_cast<float>(v2.getX()*scale+xOffset), static_cast<float>(v2.getY()*scale+yOffset) }, c);
}

void Simulator::drawRectrangle(ID2D1HwndRenderTarget* RenderTarget, Vector v1, Vector v2, ID2D1Brush* c)
{
	drawLine(RenderTarget, v1, Vector(v1.getX(), v2.getY()), c);
	drawLine(RenderTarget, v1, Vector(v2.getX(), v1.getY()), c);
	drawLine(RenderTarget, v2, Vector(v1.getX(), v2.getY()), c);
	drawLine(RenderTarget, v2, Vector(v2.getX(), v1.getY()), c);
}

void Simulator::fillRectrangle(ID2D1HwndRenderTarget* RenderTarget, Vector v1, Vector v2, ID2D1Brush* c)
{
	RenderTarget->FillRectangle(D2D1::RectF(v1.getX()*scale+xOffset,v1.getY()*scale+yOffset,
		v2.getX()*scale+xOffset,v2.getY()*scale+yOffset),c);
}

void Simulator::drawCircle(ID2D1HwndRenderTarget* RenderTarget, Vector v1, float radius, ID2D1Brush* c)
{
	D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(static_cast<float>(v1.getX()*scale+xOffset), static_cast<float>(v1.getY()*scale+yOffset))
		, radius * scale, radius  *scale);
	RenderTarget->DrawEllipse(ellipse, c);
}

void Simulator::update()
{
	int r1 = rand() % 30 + 1;
	int r2 = rand() % 30 + 1;

	long start = clock();
	world.jump(100, true);
	simulationTime = clock() - start;
}

void Simulator::ViewProc(App*app, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message == WM_MOUSEMOVE)
	{
		POINT pt = { LOWORD(lParam), HIWORD(lParam) };
		mouseX = pt.x - place.left;
		mouseY = pt.y - place.top;
	}
	if (message == WM_LBUTTONDOWN)
	{
		bool found = false;
		POINT pt = { LOWORD(lParam), HIWORD(lParam) };
		pt.x = (pt.x-place.left-xOffset)/scale;
		pt.y = (pt.y-place.top-yOffset)/scale;
		float smallestDistance = 10000;
		long newSelectedID;
		Cell* cellPtr = nullptr;
		for (auto& chunk : world.chunks)
		{
			for (int i = 0; i < world.ws.chunkSize*world.ws.chunkSize; i++)
			{
				Block*block = chunk.second->findBlock_N(i);
				for (int j = 0; j < block->getAmountOfCells(); j++)
				{
					float distance = Vector::getLength(block->getCell(j)->getCenter(), Vector(pt.x, pt.y));
					if(distance<smallestDistance)
					{
						if(distance<2)
						{
							found = true;
							newSelectedID = block->getCell(j)->getId();
							cellPtr = block->getCell(j);
						}
					}
				}

			}
		}
		if (!found) 
		{
			world.addCell(bestDNA[rand() % bestDNA.size()], pt.x, pt.y);
		}
		else
		{
			if (selectedID == newSelectedID) 
			{
				if (cellPtr->getStage() == 0)
				{
					cellPtr->startSplit(rand()%6);
				}
				else
				{
					cellPtr->nextStage();
				}
			}
			else
			{
				selectedID = newSelectedID;
			}
		}
	}
	if(message == WM_KEYDOWN)
	{
		if (wParam == 'Q') {
			WorldSettings ws;
			World testWorld(ws);
			for (int i = 0; i < 50; i++)
			{
				testWorld.addCell(bestDNA[rand() % bestDNA.size()]->mutate(0.1), 50, 10 + rand() % 200);
			}
			testWorld.jump(10000, true);
			bestDNA = testWorld.getDNA();

			sort(bestDNA.begin(), bestDNA.end(),
				[](const shared_ptr<DNA> & a, const shared_ptr<DNA> & b) -> bool
			{
				return a->fitness > b->fitness;
			});

			bestDNA.resize(min(bestDNA.size(),10));
		}
		if(wParam == 'P')scale *= 2;
		if(wParam == 'M')scale *= 0.5;
		if(wParam == VK_LEFT)xOffset += 10;
		if(wParam == VK_RIGHT)xOffset -= 10;
		if(wParam == VK_UP)yOffset += 10;
		if(wParam == VK_DOWN)yOffset -= 10;
	}
}

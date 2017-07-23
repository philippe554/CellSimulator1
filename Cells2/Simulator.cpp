#include "Simulator.h"
#include "Color.h"
#include <ctime>
#include <cstdlib>
#include "Writer.h"
#include <random>

Simulator::Simulator(int x, int y, int xSize, int ySize) :View(x, y, xSize, ySize)
{
	scale = 4;
	xOffset = 0;
	yOffset = 0;
	srand(time(NULL));
	bestDNA.push_back(make_shared<DNA>());
}

void Simulator::render(ID2D1HwndRenderTarget* RenderTarget)
{
	Cell* selectedCell = nullptr;

	for (auto& chunk : world.chunks) {
		for (int i = 0; i < world.ws.chunkSize*world.ws.chunkSize;i++)
		{
			Block*block = chunk.second->findBlock_N(i);
			fillRectrangle(RenderTarget, Vector(block->bx*world.ws.blockSize, block->by*world.ws.blockSize),
				Vector(block->bx*world.ws.blockSize + world.ws.blockSize, block->by*world.ws.blockSize + world.ws.blockSize),
				Color::heightMap(block->getTemperature() / 500));
		}
	}
	for (auto& chunk : world.chunks) {
		for (int i = 0; i < world.ws.chunkSize*world.ws.chunkSize; i++)
		{
			Block*block = chunk.second->findBlock_N(i);
			drawLine(RenderTarget, Vector(block->bx*world.ws.blockSize + world.ws.blockSize, block->by*world.ws.blockSize),
				Vector(block->bx*world.ws.blockSize + world.ws.blockSize, block->by*world.ws.blockSize + world.ws.blockSize), Color::black());
			drawLine(RenderTarget, Vector(block->bx*world.ws.blockSize, block->by*world.ws.blockSize + world.ws.blockSize),
				Vector(block->bx*world.ws.blockSize + world.ws.blockSize, block->by*world.ws.blockSize + world.ws.blockSize), Color::black());
			drawLine(RenderTarget,Vector(block->bx*world.ws.blockSize + 0.5*world.ws.blockSize, 
				block->by*world.ws.blockSize + 0.5*world.ws.blockSize),
				Vector(block->bx*world.ws.blockSize + 0.5*world.ws.blockSize, 
					block->by*world.ws.blockSize + 0.5*world.ws.blockSize) + block->getFlow()*10, Color::black());

		}
	}

	for (auto& chunk : world.chunks) 
	{
		for (int i = 0; i < world.ws.chunkSize*world.ws.chunkSize; i++)
		{
			Block*block = chunk.second->findBlock_N(i);
			for (auto& line : block->lines) {
				drawLine(RenderTarget, line->getV1(), line->getV2(), Color::black());
			}

			for (auto& cell : block->cells)
			{
				for (int j = 0; j < cell->amountEdges - 1; j++)
				{
					drawLine(RenderTarget, cell->getEdgePoint(j), cell->getEdgePoint(j+1), Color::black());
				}
				drawLine(RenderTarget, cell->getEdgePoint(0), cell->getEdgePoint(cell->amountEdges - 1), Color::black());

				for (int j = 0; j < cell->getAmountOfTailEdges(); j++)
				{
					drawLine(RenderTarget, cell->getTailPoint(j, 0), cell->getTailPoint(j, 1), Color::black());
				}

				for(int j=0;j<cell->getAmountOfEdgeEdges();j++)
				{
					drawLine(RenderTarget, Vector::getAverage(cell->getEdgeEdge(j)->getP1()->getPlace(), cell->getEdgeEdge(j)->getP2()->getPlace()),
						Vector::getAverage(cell->getEdgeEdge(j)->getP1()->getPlace(), cell->getEdgeEdge(j)->getP2()->getPlace())+cell->getEdgeEdge(j)->frictionForce*100, Color::black());
				}

				for (int j = 0; j<cell->getAmountOfTailEdges(); j++)
				{
					drawLine(RenderTarget, Vector::getAverage(cell->getTailEdge(j)->getP1()->getPlace(), cell->getTailEdge(j)->getP2()->getPlace()),
						Vector::getAverage(cell->getTailEdge(j)->getP1()->getPlace(), cell->getTailEdge(j)->getP2()->getPlace()) + cell->getTailEdge(j)->frictionForce*100, Color::black());
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
	if (world.stats_CellsCreated != 0) {
		ratio = double(world.stats_PointsCreated - world.stats_PointsDestroyed) / double(world.stats_CellsCreated-world.stats_CellsBroken);
	}
	Writer::print("Calc time: "+to_string(simulationTime),Color::black(),Writer::normal(),{0,0,400,50});
	Writer::print("World time: "+ to_string(world.getTime()), Color::black(), Writer::normal(), { 0,50,400,100 });
	Writer::print("Cells broken: " + to_string(world.stats_CellsBroken), Color::black(), Writer::normal(), { 0,100,400,150 });

	Block* mouseOnBlock = world.findBlock_B(world.calcBlock((mouseX-xOffset) / scale), world.calcBlock((mouseY-yOffset) / scale));
	if (mouseOnBlock != nullptr)
	{
		Writer::print("Block: (" + to_string(mouseOnBlock->bx) + ","+to_string(mouseOnBlock->by)+")", Color::black(), Writer::normal(), { 0,400,400,50 });
		Writer::print("Temperature: " + to_string(mouseOnBlock->getTemperature()), Color::black(), Writer::normal(), { 0,440,400,50 });
		Writer::print("Mass: " + to_string(mouseOnBlock->getMass()), Color::black(), Writer::normal(), { 0,480,400,50 });
		Writer::print("Presure: " + to_string(mouseOnBlock->getPressure()), Color::black(), Writer::normal(), { 0,520,400,50 });
		Writer::print("Volume: " + to_string(mouseOnBlock->getVolume()), Color::black(), Writer::normal(), { 0,560,400,50 });
		for(int i=0;i<WorldSettings::e_AmountOfParticles;i++)
		{
			Writer::print(to_string(i)+": M = " + to_string(mouseOnBlock->getParticle(i)) +
				", C = " + to_string(mouseOnBlock->getConcentrationPoint(i, Vector(mouseX / scale, mouseY / scale)))
				, Color::black(), Writer::normal(), { 0,float(600+i*40),600,50 });
		}
	}

	if (selectedCell != nullptr)
	{
		Writer::print("Cell:", Color::black(), Writer::normal(), { 0,800,400,50 });
		Writer::print("Temperature: " + to_string(selectedCell->getTemperature()), Color::black(), Writer::normal(), { 0,840,400,50 });
		Writer::print("Mass: " + to_string(selectedCell->getMass()), Color::black(), Writer::normal(), { 0,880,400,50 });
		Writer::print("Presure: " + to_string(selectedCell->getPressure()), Color::black(), Writer::normal(), { 0,920,400,50 });
		Writer::print("Volume: " + to_string(selectedCell->getVolume()), Color::black(), Writer::normal(), { 0,960,400,50 });
		for (int i = 0; i<WorldSettings::e_AmountOfParticles; i++)
		{
			Writer::print(to_string(i) + ": M = " + to_string(selectedCell->getParticle(i)) +
				", C = " + to_string(selectedCell->getConcentration(i))
				, Color::black(), Writer::normal(), { 0,float(1000 + i * 40),600,50 });
		}
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
	world.jump(5, true);
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
		for (auto& chunk : world.chunks)
		{
			for (int i = 0; i < world.ws.chunkSize*world.ws.chunkSize; i++)
			{
				Block*block = chunk.second->findBlock_N(i);
				for (auto& cell : block->cells)
				{
					float distance = Vector::getLength(cell->getCenter(), Vector(pt.x, pt.y));
					if(distance<smallestDistance)
					{
						if(distance<20/scale)
						{
							found = true;
							selectedID = cell->getId();
						}
					}
				}

			}
		}
		if (!found) 
		{
			world.addCell(bestDNA[rand() % bestDNA.size()], pt.x, pt.y);
		}
	}
	if(message == WM_KEYDOWN)
	{
		if (wParam == 'Q') {
			World testWorld;
			for (int i = 0; i < 500; i++)
			{
				testWorld.addCell(bestDNA[rand() % bestDNA.size()]->mutate(0.1), 500, 100 + rand() % 2000);
			}
			testWorld.jump(2000, true);
			bestDNA = testWorld.getDNA();

			sort(bestDNA.begin(), bestDNA.end(),
				[](const shared_ptr<DNA> & a, const shared_ptr<DNA> & b) -> bool
			{
				return a->fitness > b->fitness;
			});

			bestDNA.resize(min(bestDNA.size(),40));
		}
		if(wParam == 'P')scale *= 2;
		if(wParam == 'M')scale *= 0.5;
		if(wParam == VK_LEFT)xOffset += 10;
		if(wParam == VK_RIGHT)xOffset -= 10;
		if(wParam == VK_UP)yOffset += 10;
		if(wParam == VK_DOWN)yOffset -= 10;
	}
}

#include "Simulator.h"
#include "Color.h"
#include <ctime>
#include <cstdlib>
#include "Writer.h"
#include <random>

Simulator::Simulator(int x, int y, int xSize, int ySize) :View(x, y, xSize, ySize)
{
	scale = 3;
	srand(time(NULL));
	bestDNA.push_back(make_shared<DNA>());
}

void Simulator::render(ID2D1HwndRenderTarget* RenderTarget)
{
	Cell* selectedCell = nullptr;

	for (auto& chunk : world.chunks) {
		for (int i = 0; i < world.c_ChunkSize*world.c_ChunkSize;i++)
		{
			Block*block = chunk.second->findBlock_N(i);
			RenderTarget->FillRectangle(D2D1::RectF(block->bx*world.c_BlockSize*scale,
				block->by*world.c_BlockSize*scale,
				block->bx*world.c_BlockSize*scale + world.c_BlockSize*scale,
				block->by*world.c_BlockSize*scale + world.c_BlockSize*scale),
				Color::heightMap(block->getTemperature() / 500));
		}
	}
	for (auto& chunk : world.chunks) {
		for (int i = 0; i < world.c_ChunkSize*world.c_ChunkSize; i++)
		{
			Block*block = chunk.second->findBlock_N(i);
			RenderTarget->DrawLine({ static_cast<float>(block->bx*world.c_BlockSize*scale) ,
				static_cast<float>(block->by*world.c_BlockSize*scale + world.c_BlockSize*scale) },
				{ static_cast<float>(block->bx*world.c_BlockSize*scale + world.c_BlockSize*scale),
				static_cast<float>(block->by*world.c_BlockSize*scale + world.c_BlockSize*scale) }, Color::black());
			RenderTarget->DrawLine({ static_cast<float>(block->bx*world.c_BlockSize*scale + world.c_BlockSize*scale) ,
				static_cast<float>(block->by*world.c_BlockSize*scale) },
				{ static_cast<float>(block->bx*world.c_BlockSize*scale + world.c_BlockSize*scale),
				static_cast<float>(block->by*world.c_BlockSize*scale + world.c_BlockSize*scale) }, Color::black());
			drawLine(RenderTarget,Vector(block->bx*world.c_BlockSize*scale + 0.5*world.c_BlockSize*scale, block->by*world.c_BlockSize*scale + 0.5*world.c_BlockSize*scale),
				Vector(block->bx*world.c_BlockSize*scale + 0.5*world.c_BlockSize*scale, block->by*world.c_BlockSize*scale + 0.5*world.c_BlockSize*scale) + block->getFlow(), Color::black());

		}
	}

	for (auto& chunk : world.chunks) 
	{
		for (int i = 0; i < world.c_ChunkSize*world.c_ChunkSize; i++)
		{
			Block*block = chunk.second->findBlock_N(i);
			for (auto& line : block->lines) {
				RenderTarget->DrawLine({ static_cast<float>(line->getV1()->getX()*scale) , static_cast<float>(line->getV1()->getY()*scale) },
				{ static_cast<float>(line->getV2()->getX()*scale), static_cast<float>(line->getV2()->getY()*scale) }, Color::black());
			}

			for (auto& cell : block->cells)
			{
				for (int j = 0; j < cell->amountEdges - 1; j++)
				{
					RenderTarget->DrawLine({ static_cast<float>(cell->getEdgePoint(j)->getX()*scale) , static_cast<float>(cell->getEdgePoint(j)->getY()*scale) },
					{ static_cast<float>(cell->getEdgePoint(j + 1)->getX()*scale), static_cast<float>(cell->getEdgePoint(j + 1)->getY()*scale) }, Color::black());
					
					RenderTarget->DrawLine({ static_cast<float>(cell->getEdgePoint(j)->getX()*scale) , static_cast<float>(cell->getEdgePoint(j)->getY()*scale) },
					{ static_cast<float>(cell->getEdgePoint(j + 1)->getX()*scale), static_cast<float>(cell->getEdgePoint(j + 1)->getY()*scale) }, Color::black());

					D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(static_cast<float>(cell->getEdgePoint(j)->getX()*scale), static_cast<float>(cell->getEdgePoint(j)->getY()*scale))
						, static_cast<float>(cell->getEdgePointPtr(j)->getMass()*scale*0.1), static_cast<float>(cell->getEdgePointPtr(j)->getMass()*scale*0.1));
					RenderTarget->DrawEllipse(ellipse, Color::black());
				}
				RenderTarget->DrawLine({ static_cast<float>(cell->getEdgePoint(cell->amountEdges - 1)->getX()*scale) , static_cast<float>(cell->getEdgePoint(cell->amountEdges - 1)->getY()*scale) },
				{ static_cast<float>(cell->getEdgePoint(0)->getX()*scale), static_cast<float>(cell->getEdgePoint(0)->getY()*scale) }, Color::black());
				
				D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(static_cast<float>(cell->getEdgePoint(cell->amountEdges-1)->getX()*scale), static_cast<float>(cell->getEdgePoint(cell->amountEdges-1)->getY()*scale))
					, static_cast<float>(cell->getEdgePointPtr(cell->amountEdges-1)->getMass()*scale*0.1), static_cast<float>(cell->getEdgePointPtr(cell->amountEdges-1)->getMass()*scale*0.1));
				RenderTarget->DrawEllipse(ellipse, Color::black());

				for (int j = 0; j < cell->getAmountOfTailEdges(); j++)
				{
					RenderTarget->DrawLine({ static_cast<float>(cell->getTailPoint(j,0)->getX()*scale) , static_cast<float>(cell->getTailPoint(j,0)->getY()*scale) },
					{ static_cast<float>(cell->getTailPoint(j,1)->getX()*scale), static_cast<float>(cell->getTailPoint(j,1)->getY()*scale) }, Color::black());
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
					D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(static_cast<float>(cell->getCenter()->getX()*scale), static_cast<float>(cell->getCenter()->getY()*scale))
						, static_cast<float>(15*scale), static_cast<float>(15*scale));
					RenderTarget->DrawEllipse(ellipse, Color::red());
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

	Block* mouseOnBlock = world.findBlock_B(world.calcBlock(mouseX / scale), world.calcBlock(mouseY / scale));
	if (mouseOnBlock != nullptr)
	{
		Writer::print("Block: (" + to_string(mouseOnBlock->bx) + ","+to_string(mouseOnBlock->by)+")", Color::black(), Writer::normal(), { 0,400,400,50 });
		Writer::print("Temperature: " + to_string(mouseOnBlock->getTemperature()), Color::black(), Writer::normal(), { 0,440,400,50 });
		Writer::print("Mass: " + to_string(mouseOnBlock->getMass()), Color::black(), Writer::normal(), { 0,480,400,50 });
		Writer::print("Presure: " + to_string(mouseOnBlock->getPressure()), Color::black(), Writer::normal(), { 0,520,400,50 });
		for(int i=0;i<WorldSettings::e_AmountOfParticles;i++)
		{
			Writer::print(to_string(i)+": M = " + to_string(mouseOnBlock->getParticle(i)) +
				", C = " + to_string(mouseOnBlock->getConcentrationPoint(i, Vector(mouseX / scale, mouseY / scale)))
				, Color::black(), Writer::normal(), { 0,float(560+i*40),400,50 });
		}
	}

	if (selectedCell != nullptr)
	{
		Writer::print("Cell:", Color::black(), Writer::normal(), { 0,800,400,50 });
		Writer::print("Temperature: " + to_string(selectedCell->getTemperature()), Color::black(), Writer::normal(), { 0,840,400,50 });
		Writer::print("Mass: " + to_string(selectedCell->getMass()), Color::black(), Writer::normal(), { 0,880,400,50 });
		Writer::print("Presure: " + to_string(selectedCell->getPressure()), Color::black(), Writer::normal(), { 0,920,400,50 });
		for (int i = 0; i<WorldSettings::e_AmountOfParticles; i++)
		{
			Writer::print(to_string(i) + ": M = " + to_string(selectedCell->getParticle(i)) +
				", C = " + to_string(selectedCell->getConcentration(i))
				, Color::black(), Writer::normal(), { 0,float(960 + i * 40),400,50 });
		}
	}
}

void Simulator::drawLine(ID2D1HwndRenderTarget* RenderTarget, Vector* v1, Vector* v2, ID2D1Brush* c)
{
	RenderTarget->DrawLine({ static_cast<float>(v1->getX()*scale) , static_cast<float>(v1->getY()*scale) },
	{ static_cast<float>(v2->getX()*scale), static_cast<float>(v2->getY()*scale) }, c);
}

void Simulator::drawLine(ID2D1HwndRenderTarget* RenderTarget, Vector v1, Vector v2, ID2D1Brush* c)
{
	RenderTarget->DrawLine({ static_cast<float>(v1.getX()*scale) , static_cast<float>(v1.getY()*scale) },
	{ static_cast<float>(v2.getX()*scale), static_cast<float>(v2.getY()*scale) }, c);
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
		pt.x = (pt.x-place.left)/scale;
		pt.y = (pt.y-place.top)/scale;
		float smallestDistance = 10000;
		for (auto& chunk : world.chunks)
		{
			for (int i = 0; i < world.c_ChunkSize*world.c_ChunkSize; i++)
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
		if(wParam == 'P')
		{
			scale++;
		}
		if(wParam == 'M')
		{
			scale--;
		}
	}
}

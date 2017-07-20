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

		}
	}

	for (auto& chunk : world.chunks) {
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
	if (message == WM_LBUTTONDOWN)
	{
		POINT pt = { LOWORD(lParam), HIWORD(lParam) };
		pt.x -= place.left;
		pt.y -= place.top;
		world.addCell(bestDNA[rand() % bestDNA.size()], pt.x/scale, pt.y/scale);
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
	}
}

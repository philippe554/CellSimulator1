#pragma once
#include "Color.h"

class Simulator;

#include "View.h"
#include "World.h"

class Simulator : public View
{
public:
	Simulator(int x, int y, int xSize, int ySize);
	void render(ID2D1HwndRenderTarget* RenderTarget) override;
	void drawLine(ID2D1HwndRenderTarget* RenderTarget, Vector* v1, Vector* v2, ID2D1Brush* c);
	void drawLine(ID2D1HwndRenderTarget* RenderTarget, Vector v1, Vector v2, ID2D1Brush* c);
	void update() override;
	void ViewProc(App* app, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override;

private:
	World world;
	vector<shared_ptr<DNA>> bestDNA;

	double scale;
	long simulationTime;

	int mouseX;
	int mouseY;
	int selectedID;
};
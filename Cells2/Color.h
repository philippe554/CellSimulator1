#pragma once
#include <map>

class Color;

#include <d2d1.h>

using namespace std;

class Color
{
public:
	static void init(ID2D1HwndRenderTarget* RT);

	static ID2D1SolidColorBrush* lightGray();
	static ID2D1SolidColorBrush* gray();
	static ID2D1SolidColorBrush* black();

	static ID2D1SolidColorBrush* heightMap(double x);
private:
	static ID2D1HwndRenderTarget* RenderTarget;

	static ID2D1SolidColorBrush* lightGrayData;
	static ID2D1SolidColorBrush* grayData;
	static ID2D1SolidColorBrush* blackData;

	static map<int, ID2D1SolidColorBrush*> heightMapData;
};	

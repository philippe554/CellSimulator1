#pragma once

#include <immintrin.h>
#include <algorithm>

class Vector2DAV;

class Vector2DAVX {
public:
	Vector2DAVX();

private:
	__m256 x;
	__m256 y;
};

#pragma once

#include <math.h>
#include <ctime>
#include <algorithm>
#include <vector>
#include <iostream>

class Matrix;

class Matrix
{
public:
	Matrix(int row, int col);
	Matrix(const Matrix& other);
	~Matrix();

	int getAmountOfRows()const;
	int getAmountOfCollumns()const;
	float getElement(int i, int j)const;
	float getElementScaled(int i, int j, float min, float max)const;
	std::vector<float> getAllElements()const;
	std::vector<float> getAllElementsScaled(float min, float max)const;
	Matrix getRow(int i)const;
	Matrix getCollum(int i)const;

	void setElement(int i, int j, float e);
	void setRandom();
	Matrix getRandom(float rate) const;
	void startInput();
	Matrix& operator<<(const float e);
	void output() const;
	void outputSize() const;

	Matrix operator=(const Matrix& eq);

	Matrix operator+(const Matrix& m2)const;
	Matrix operator-(const Matrix& m2)const;
	Matrix operator-();
	void putSum(const Matrix& m2);
	void putMinus(const Matrix& m2);
	void putNegate();

	Matrix operator*(const Matrix& b)const;
	Matrix operator*(float e)const;
	void putMultiplication(float e);
	void putElementWiseMultiplication(const Matrix& other);
	Matrix getElementWiseMultiplication(const Matrix& other)const;

	Matrix getSigmoid()const;
	Matrix getSigmoidPrime()const;
	void putSigmoid();
	void putSigmoidPrime();
	Matrix getTanh()const;
	void putTanh();

	float sumElements()const;
	void putSquare();
	Matrix getSquare()const;
	float getSquareSum()const;

	Matrix getTranspose()const;

	Matrix getHorizontalStitch(const Matrix& other)const;
	Matrix getVerticalStitch(const Matrix& other)const;

private:
	float* p;
	int rows, cols, counter;
};

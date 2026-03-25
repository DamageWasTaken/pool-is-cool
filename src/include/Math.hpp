#pragma once

#include <iostream>

struct Vector2f
{
	Vector2f()
	:x(0.0f), y(0.0f)
	{}
	
	Vector2f(float p_x, float p_y)
	:x(p_x), y(p_y)
	{}

	void print()
	{
		std::cout << "{" << x << ", " << y << "}" << std::endl;
	}

	float x, y;
};

Vector2f addVector2f(Vector2f v1, Vector2f v2);

Vector2f subtractVector2f(Vector2f v1, Vector2f v2);

Vector2f scaleVector2f(Vector2f v1, float scaler);

Vector2f normalizeVector2f(Vector2f v1);

float lengthVector2f(Vector2f v1);

float dotVector2f(Vector2f v1, Vector2f v2);

float dotVector2f(Vector2f v1, Vector2f v2);

float angleBetweenVector2f(Vector2f v1, Vector2f v2);

float crossVector2f(Vector2f v1, Vector2f v2);

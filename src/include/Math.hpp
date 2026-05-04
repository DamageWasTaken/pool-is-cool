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

struct Vector2
{
	Vector2()
	:x(0), y(0)
	{}
	
	Vector2(int p_x, int p_y)
	:x(p_x), y(p_y)
	{}

	void print()
	{
		std::cout << "{" << x << ", " << y << "}" << std::endl;
	}

	int x, y;
};

struct Edge{
    Edge()
    :start(Vector2f()), end(Vector2f())
    {}

    Edge(Vector2f v1, Vector2f v2)
	:start(v1), end(v2)
	{}                           

	void print()
	{
        std::cout << "start:"; 
        start.print();
        std::cout << "end:"; 
		end.print();
	}

    bool operator==(Edge other_edge){
        return start.x == other_edge.start.x && start.y == other_edge.start.y
                    && end.x == other_edge.end.x   && end.y == other_edge.end.y;
    }

    bool operator!=(Edge other_edge){
        return !(*this == other_edge);
    }

	Vector2f start, end;
};

Vector2f rotateVector2f(Vector2f v1, float degrees);

Vector2f anticlockwiseVector2f(Vector2f v1);

Vector2f clockwiseVector2f(Vector2f v1);

Vector2f flipVector2f(Vector2f v1);

Vector2f projectVector2f(Vector2f v1, Vector2f v2);

Vector2f resizeVector2f(Vector2f v1, float size);

Vector2f addVector2f(Vector2f v1, Vector2f v2);

Vector2f subtractVector2f(Vector2f v1, Vector2f v2);

Vector2f scaleVector2f(Vector2f v1, float scaler);

Vector2f normalizeVector2f(Vector2f v1);

float lengthVector2f(Vector2f v1);

float dotVector2f(Vector2f v1, Vector2f v2);

float dotVector2f(Vector2f v1, Vector2f v2);

float angleBetweenVector2f(Vector2f v1, Vector2f v2);

float crossVector2f(Vector2f v1, Vector2f v2);

bool edgeCollision(Edge edge1, Edge edge2);

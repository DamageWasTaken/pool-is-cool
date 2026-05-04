#include <cmath>

#include "Math.hpp"

Vector2f rotateVector2f(Vector2f v1, float degrees){
    float radians = degrees * (M_PI/180.f);
    return Vector2f(v1.x*std::cosf(radians) - v1.y*std::sinf(radians), 
                    v1.x*std::sinf(radians) + v1.y*std::cosf(radians));
};

Vector2f clockwiseVector2f(Vector2f v1){
    return Vector2f(v1.y, -v1.x);
}
Vector2f anticlockwiseVector2f(Vector2f v1){
    return Vector2f(-v1.y, v1.x);
}

Vector2f flipVector2f(Vector2f v1){
    return Vector2f(-v1.x, -v1.y);
}

Vector2f projectVector2f(Vector2f v1, Vector2f v2){
    return scaleVector2f(v2, dotVector2f(v1, v2)/std::powf(lengthVector2f(v2), 2));
}

Vector2f resizeVector2f(Vector2f v1, float size){
    return scaleVector2f(normalizeVector2f(v1), size);
}

Vector2f addVector2f(Vector2f v1, Vector2f v2)
{
    Vector2f resulting_Vector2f;
    resulting_Vector2f.x = v1.x+v2.x;
    resulting_Vector2f.y = v1.y+v2.y;
    return resulting_Vector2f;
}

Vector2f subtractVector2f(Vector2f v1, Vector2f v2)
{
    Vector2f resulting_Vector2f;
    resulting_Vector2f.x = v1.x-v2.x;
    resulting_Vector2f.y = v1.y-v2.y;
    return resulting_Vector2f;
}

Vector2f scaleVector2f(Vector2f v1, float scaler)
{
    Vector2f scaled_Vector2f;
    scaled_Vector2f.x = v1.x*scaler;  
    scaled_Vector2f.y = v1.y*scaler;  
    
    return scaled_Vector2f;
}

float lengthVector2f(Vector2f v1)
{
    return sqrtf(powf(v1.x, 2.0f) + powf(v1.y, 2.0f)); 
}

Vector2f normalizeVector2f(Vector2f v1)
{
    float vector_length = lengthVector2f(v1);
    Vector2f normalized_Vector2f;
    normalized_Vector2f.x = v1.x/vector_length;
    normalized_Vector2f.y = v1.y/vector_length;
    return normalized_Vector2f;
}

float dotVector2f(Vector2f v1, Vector2f v2)
{
    return v1.x*v2.x + v1.y*v2.y;
}

float angleBetweenVector2f(Vector2f v1, Vector2f v2)
{
    float v1_length = lengthVector2f(v1);
    float v2_length = lengthVector2f(v2);

    return acosf(dotVector2f(v1, v2)/(v1_length*v2_length));
}

float crossVector2f(Vector2f v1, Vector2f v2)
{
    return v1.x * v2.y - v1.y * v2.x;
}

bool edgeCollision(Edge edge1, Edge edge2){
    Vector2f path1 = subtractVector2f(edge1.end, edge1.start);
    Vector2f path2 = subtractVector2f(edge2.end, edge2.start);
    float denom = crossVector2f(path1, path2);
    if (std::fabs(denom) < 1e-9f) return false;

    Vector2f between = subtractVector2f(edge2.start, edge1.start);
    float t = crossVector2f(between, path2) / denom;
    float u = crossVector2f(between, path1) / denom;

    return (t >= 0.f && t <= 1.f && u >= 0.f && u <= 1.f);
}

#include "Math.hpp"
#include <cmath>

Vector2f addVector2f(Vector2f v1, Vector2f v2){
  Vector2f resulting_Vector2f;
  resulting_Vector2f.x = v1.x+v2.x;
  resulting_Vector2f.y = v1.y+v2.y;
  return resulting_Vector2f;
}

Vector2f subtractVector2f(Vector2f v1, Vector2f v2){
  Vector2f resulting_Vector2f;
  resulting_Vector2f.x = v1.x-v2.x;
  resulting_Vector2f.y = v1.y-v2.y;
  return resulting_Vector2f;
}

Vector2f scaleVector2f(Vector2f v1, float scaler){
  Vector2f scaled_Vector2f;
  v1.x*scaler;  
  v1.y*scaler;  
  return scaled_Vector2f;
}

float lengthVector2f(Vector2f v1){
  return std::sqrt(std::pow(v1.x, 2), std::pow(v1, 2)); 
}

Vector2f normalizeVector2f(Vector2f v1){
  float vector_length = lengthVector2f(v1);
  Vector2f normalized_Vector2f;
  normalized_Vector2f.x = Vector2f.x/vector_length;
  normalizeVector2f.y = Vector2f.y/vector_length;
  return normalizeVector2f;
}

float dotVector2f(Vector2f v1, Vector2f v2){
  return v1.x*v2.y + v1.y*v2.y;
}

float dotVector2f(Vector2f v1, Vector2f v2){
  return v1.x*v2.y + v1.y*v2.y;
}

float angleBetweenVector2f(Vector2f v1, Vector2f v2){
  v1_length = lengthVector2f(v1);
  v2_length = lengthVector2f(v2);

  return std::acosf(dotVector2f(v1, v2)/(v1_length*v2_length));
}

float crossVector2f(Vector2f v1, Vector2f v2){
  v1_length = lengthVector2f(v1);
  v2_length = lengthVector2f(v2);

  return v1_length*v2_length*std::sinf(angleBetweenVector2f(v1, v2));
}


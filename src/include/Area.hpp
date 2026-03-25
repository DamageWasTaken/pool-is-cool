#pragma once
#include <Math.hpp>
#include <cmath>

struct Area {
    Area()
    : ul(Vector2f()), lr(Vector2f()), ll(Vector2f()), 
      ur(Vector2f()), center(Vector2f()), width(0.0f), height(0.0f)
    {
    };

    Area(Vector2f UL, Vector2f LR)
    : ul(UL), lr(LR)
    {
        width = std::fabs(lr.x-ul.x);
        height = std::fabs(lr.y-ul.y);
        ll = Vector2f(ul.x, ul.y+height);
        ur = Vector2f(lr.x, lr.y-height);
        center = Vector2f(ul.x+width/2, ul.y+height/2);
    };

    Area(Vector2f _center, float _width, float _height)
    : center(_center), width(_width), height(_height)
    {
        ul = Vector2f(center.x-width/2, center.y-height/2);
        ur = Vector2f(center.x+width/2, center.y-height/2);
        ll = Vector2f(center.x-width/2, center.y+height/2);
        lr = Vector2f(center.x+width/2, center.y+height/2);
    };

    float width, height;
    Vector2f center;
    
    Vector2f ul, ur, ll, lr;

    float getArea(){
        return height*width;
    };
};

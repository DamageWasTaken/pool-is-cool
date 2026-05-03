#pragma once
#include <vector>
#include <Math.hpp>
#include <cmath>

struct Area {
    Area()
    :points(std::vector<Vector2f>{}), edges(std::vector<Edge>{})
    {
    };

    Area(std::vector<Vector2f> _points)
    :points(_points)
    {
        for(int i = 0; i < points.size(); i++){
            edges.push_back(Edge(points[i], points[(i+1) % points.size()]));
        }
    };

    Area(std::vector<Edge> _edges)
    :edges(_edges)
    {
        for(int i = 0; i < points.size(); i++){
            points.push_back(edges[i].start);
        }
    };
    
    std::vector<Vector2f> points;
    std::vector<Edge> edges;
};

bool inArea(Area area, Vector2f pos);

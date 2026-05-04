#include <Area.hpp>
#include <bits/stdc++.h>

bool inArea(Area area, Vector2f pos){
    int crossings = 0;

    //find the right most point of area
    std::vector<float> x_cords = {};
    for (Vector2f point : area.points){
        x_cords.push_back(point.x);
    };
    Vector2f most_right = Vector2f(*std::max_element(x_cords.begin(), x_cords.end()), pos.y);

    //count crossings;
    Edge lead = {
        pos, 
        most_right 
    };

    for(Edge edge : area.edges){
        if(edgeCollision(lead, edge)) crossings++;
    }
         
    return !(crossings%2 == 0);
};

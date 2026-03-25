#pragma once
#include <Area.hpp>

class PhysicsHandler {
    puplic:
        PhysicsHandler();  
        PhysicsHandler(BallManager& _balls, Area _area);
        
        void updatePhysics(float timestep);

    private:
        Quadtree search_tree;
        BallManager balls; 
        Area area; 
};

class Quadtree {
};

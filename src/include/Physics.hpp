#pragma once

#include <vector>

#include "Area.hpp"

/*class Quadtree {
    public:
        Quadtree();
        Quadtree& getChild(int child){return Quadtree();}
    private:
        std::vector<Quadtree> children;
};*/

class PhysicsHandler {
    public:
        PhysicsHandler(BallManager& _balls, Area _area);
        
        void updatePhysics(float timestep);

    private:
        //Quadtree search_tree;
        BallManager balls; 
        Area area; 
};


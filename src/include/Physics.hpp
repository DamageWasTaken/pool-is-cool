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
        void updateVelocity(Ball& ball, float timestep);
        void updatePosition(Ball& ball, float timestep);

        bool checkWallCollision(Ball& ball, Edge edge, float timestep);
        void handleWallCollision(Ball& ball, float timestep);
        BallManager& balls; 
        Area area; 
};


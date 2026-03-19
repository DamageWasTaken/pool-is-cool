#pragma once

class PhysicsHandler {
    puplic:
        PhysicsHandler();  
        PhysicsHandler(BallManager balls, Area area);
        
        void updatePhysics(float timestep);

    private:
        Quadtree search_tree;
        BallManager balls; 
};

class Quadtree {
    puplic:
    //
    ;

    private:
}

#pragma once

#include <vector>

#include "Area.hpp"

class PhysicsHandler {
    public:
        PhysicsHandler(BallManager& _balls, Area _area);
        
        void updatePhysics(float timestep);

    private:
        void updateVelocity(Ball& ball, float timestep);
        void updatePosition(Ball& ball, float timestep);

        bool checkWallCollision(Ball& ball, Edge edge, float timestep);
        void handleWallCollision(Ball& ball, float timestep);

        void solveWallCollision(Ball& ball, Edge edge);
        void solveCornerCollision(Ball& ball, Edge edge, Edge edge);
            
        bool checkBallCollision(Ball& ball1, Ball& ball2, float timestep);
        void handleBallCollision(Ball& ball, float timestep);

        BallManager& balls; 
        Area area; 
};


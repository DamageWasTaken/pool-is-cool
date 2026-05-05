#pragma once

#include <vector>

#include "Area.hpp"
#include "Math.hpp"

class PhysicsHandler {
    public:
        PhysicsHandler(BallManager& _balls, Area _area);
        
        void updatePhysics(float timestep);

    private:
        //Quadtree search_tree;
        void updateVelocity(int ball_number, float timestep);
        void updatePosition(int ball_number, float timestep);

        bool checkWallCollision(Ball& ball, Edge edge, float timestep);
        void handleWallCollision(Ball& ball, float timestep);

        void solveWallCollision(Ball& ball, Edge edge);
        void solveCornerCollision(Ball& ball, Edge edge1, Edge edge2);
        Edge findAdjecentEdge(Edge edge, std::vector<Edge> edges);
            
        bool checkBallCollision(Ball& ball1, Ball& ball2, float timestep);
        void handleBallCollision(Ball& ball, int ball_number, float timestep);

        BallManager& s_ball_manager; 
        Area area; 
};


#include <iostream>
#include <vector>
#include <cmath>

#include "Math.hpp"
#include "Ball.hpp"
#include "Physics.hpp"
#include "Area.hpp"

#define FRICTION 0.99f

PhysicsHandler::PhysicsHandler(BallManager& _balls, Area _area) 
: balls(_balls), area(_area)
{
}

bool PhysicsHandler::checkWallCollision(Ball& ball, Edge edge, float timestep){
    Vector2f wall = subtractVector2f(edge.end, edge.start);
    Vector2f offset_direction = subtractVector2f(edge.start, ball.getPosition());
    Vector2f normal = normalizeVector2f(projectVector2f(offset_direction, clockwiseVector2f(wall)));
    Vector2f offset = scaleVector2f(normal, ball.getRadius());
    Vector2f ballStart = addVector2f(ball.getPosition(), offset);
    Vector2f ballEnd = addVector2f(addVector2f(ball.getPosition(), scaleVector2f(ball.getVelocity(), timestep)), offset);
    Vector2f ball_path = subtractVector2f(ballEnd, ballStart);
    //black magic
    float denom = crossVector2f(ball_path, wall);
    if (std::fabs(denom) < 1e-9f) return false;

    Vector2f between = subtractVector2f(edge.start, ballStart);
    float t = crossVector2f(between, wall) / denom;
    float u = crossVector2f(between, ball_path) / denom;

    return (t >= 0.f && t <= 1.f && u >= 0.f && u <= 1.f);
}

/*
Vector2f wall = subtractVector2f(edge.end, edge.start);
                Vector2f toBall = subtractVector2f(ball.getPosition(), edge.start);
                Vector2f normal = normalizeVector2f(projectVector2f(toBall, clockwiseVector2f(wall)));
                float dist = dotVector2f(toBall, normal);
                float penetration = ball.getRadius() - dist;
                if (penetration <= 0.0f) continue;
                ball.setPosition(addVector2f(ball.getPosition(), scaleVector2f(normal, penetration)));
                Vector2f vel = ball.getVelocity();
                float velDot = dotVector2f(vel, normal);
                if (velDot < 0.0f)
                    ball.setVelocity(subtractVector2f(vel, scaleVector2f(normal, 2.0f * velDot)));
                handled_edges.push_back(edge);
*/

void solveWallCollision(Ball& ball, Edge edge){
    Vector2f wall = subtractVector2f(edge.end, edge.start);
    Vector2f toBall = subtractVector2f(ball.getPosition(), edge.start);
    Vector2f normal = normalizeVector2f(projectVector2f(toBall, clockwiseVector2f(wall)));
    float dist = dotVector2f(toBall, normal);
    float penetration = ball.getRadius() - dist;
    if (penetration <= 0.0f) continue;
    ball.setPosition(addVector2f(ball.getPosition(), scaleVector2f(normal, penetration)));
    Vector2f vel = ball.getVelocity();
    float velDot = dotVector2f(vel, normal);
    if (velDot < 0.0f)
        ball.setVelocity(subtractVector2f(vel, scaleVector2f(normal, 2.0f * velDot)));
}

void solveCornerCollision(Ball& ball, Edge edge, Edge edge){

}

int getIndex(auto element; std::vector<auto> vector){
    for(int i = 0; i < vector.size(); i++){
        if(vector[i] == element){
            return i;
        }
    }
    return -1;
}

Edge findAdjecentEdge(Edge edge, std::vector<Edge> edges){
    int index = getIndex(edge, area.edges);
    Edge right = index+1 == area.edges.size() ? area.edges[0] : area.edges[index+1] ;
    Edge left = index == 0 ? area.edges[area.edges.size()-1] : area.edges[index-1] ;
    if(getIndex(right, edges) != -1){
        return right; 
    } else if(getIndex(left, edges) != -1){
        return left;
    }     
    return Edge();
}

void PhysicsHandler::handleWallCollision(Ball& ball, float timestep) {
    std::vector<Edge> colliding_edges = {};
    for (Edge edge : area.edges) {
        if (checkWallCollision(ball, edge, timestep)) {
            colliding_edges.push_back(edge);
        }
    }
    if(colliding_edges.size() == 1){
        solveWallCollision(Ball, colliding_edges[0]);
        return
    } else if (colliding_edges.size() > 1){
        while(colliding_edges.size != 0){
            for(Edge edge : colliding_edges){
                Edge adjecentEdge = findAdjecentEdge();
                if(adjecentEdge != Edge()){
                    solveCornerCollision(ball, edge, adjecentEdge);
                } else {
                    solveWallCollision(ball, edge);
                }
                break
            }
        }
    }
}

bool PhysicsHandler::checkBallCollision(Ball& ball1, Ball& ball2, float timestep){
    return lengthVector2f(subtractVector2f(ball2.getPosition(), ball1.getPosition())) <= ball1.getRadius()+ball2.getRadius();
}

void PhysicsHandler::handleBallCollision(Ball& ball, float timestep){
    for(int i = 0; i < balls.getBallAmount(); i++){
        Ball& other_ball = balls.getBall(i);
        if(&other_ball == &ball) continue; // skip self
        if(checkBallCollision(ball, other_ball, timestep)){
            Vector2f seperation = subtractVector2f(ball.getPosition(), other_ball.getPosition());
            Vector2f normal = normalizeVector2f(seperation);
            //Seperate balls
            float radiai = ball.getRadius()+other_ball.getRadius();
            float seperation_distance = (lengthVector2f(seperation)-radiai)/2;
            ball.setPosition(addVector2f(ball.getPosition(), scaleVector2f(flipVector2f(normal), seperation_distance)));
            other_ball.setPosition(addVector2f(other_ball.getPosition(), scaleVector2f(normal, seperation_distance)));
            //recalculate seperation
            seperation = subtractVector2f(ball.getPosition(), other_ball.getPosition());

            //update velocities
            Vector2f ball_pos = ball.getPosition();
            Vector2f ball_vel = ball.getVelocity();
            Vector2f other_ball_vel = other_ball.getVelocity();
            Vector2f other_ball_pos = other_ball.getPosition();

            ball.setVelocity(
                subtractVector2f(
                    ball_vel,
                    scaleVector2f(
                        seperation,
                        dotVector2f(
                            subtractVector2f(ball_vel, other_ball_vel), 
                            seperation
                        ) / std::powf(lengthVector2f(seperation), 2)
                    )
                )
            );

            other_ball.setVelocity(
                subtractVector2f(
                    other_ball_vel,
                    scaleVector2f(
                        flipVector2f(seperation),
                        dotVector2f(
                            subtractVector2f(other_ball_vel, ball_vel), 
                            flipVector2f(seperation)
                        ) / std::powf(lengthVector2f(seperation), 2)
                    )
                )
            );
        handleWallCollision(ball, timestep);
        }
    }
}

void PhysicsHandler::updateVelocity(Ball& ball, float timestep){
    ball.setVelocity(scaleVector2f(ball.getVelocity(), FRICTION));
};

void PhysicsHandler::updatePosition(Ball& ball, float timestep){
    handleBallCollision(ball, timestep);
    handleWallCollision(ball, timestep);
    handleBallCollision(ball, timestep);
    Vector2f new_position = addVector2f(ball.getPosition(), scaleVector2f(ball.getVelocity(), timestep));
    ball.setPosition(new_position);
};

void PhysicsHandler::updatePhysics(float timestep){
      for(auto it = balls.getBalls().begin(); it!=balls.getBalls().end(); it++){
        Ball& ball = it->second;
        updateVelocity(ball, timestep);
        updatePosition(ball, timestep);
    }
}

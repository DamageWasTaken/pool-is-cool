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

void PhysicsHandler::handleWallCollision(Ball& ball, float timestep) {
    Edge last_edge = Edge();
    bool handled = false;
    while (!handled) {
        for (Edge edge : area.edges) {
            if (checkWallCollision(ball, edge, timestep) && edge != last_edge) {
                Vector2f wall = subtractVector2f(edge.end, edge.start);
                Vector2f normal = normalizeVector2f(clockwiseVector2f(wall));
                Vector2f toBall = subtractVector2f(ball.getPosition(), edge.start);
                //remove from wall
                normal = flipVector2f(normal);
                float dist = dotVector2f(toBall, normal);
                float penetration = ball.getRadius() - dist;
                ball.setPosition(addVector2f(ball.getPosition(), scaleVector2f(normal, penetration*2.f)));
                Vector2f vel = ball.getVelocity();
                float velDot = dotVector2f(vel, normal);
                ball.setVelocity(subtractVector2f(vel, scaleVector2f(normal, 2.0f * velDot)));
                last_edge = edge;
                break;
            }
        }
        handled = true;
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
    for(int i = 0; i < balls.getBallAmount(); i++){
        Ball& ball = balls.getBall(i);
        updateVelocity(ball, timestep);
        updatePosition(ball, timestep);
    }
}

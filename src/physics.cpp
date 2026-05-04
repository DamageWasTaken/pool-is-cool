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
    float denom = crossVector2f(ball_path, wall);
    if (std::fabs(denom) < 1e-9f) return false;

    Vector2f between = subtractVector2f(edge.start, ballStart);
    float t = crossVector2f(between, wall) / denom;
    float u = crossVector2f(between, ball_path) / denom;

    return (t >= 0.f && t <= 1.f && u >= 0.f && u <= 1.f);
}

void PhysicsHandler::handleWallCollision(Ball& ball, float timestep) {
    for (Edge edge : area.edges) {
        if (checkWallCollision(ball, edge, timestep)) {
            Vector2f wall = subtractVector2f(edge.end, edge.start);
            Vector2f normal = normalizeVector2f(clockwiseVector2f(wall));
            Vector2f toBall = subtractVector2f(ball.getPosition(), edge.start);
            if (dotVector2f(toBall, normal) < 0.0f)
                normal = scaleVector2f(normal, -1.0f);
            float dist = dotVector2f(toBall, normal);
            float penetration = ball.getRadius() - dist;
            if (penetration > 0.0f)
                ball.setPosition(addVector2f(ball.getPosition(), scaleVector2f(normal, penetration * 2.0f)));
            Vector2f vel = ball.getVelocity();
            float velDot = dotVector2f(vel, normal);
            if (velDot < 0.0f)
                ball.setVelocity(subtractVector2f(vel, scaleVector2f(normal, 2.0f * velDot)));
        }
    }
}

bool PhysicsHandler::checkBallCollision(Ball& ball1, Ball& ball2, float timestep){
    return lengthVector2f(subtractVector2f(ball2.getPosition(), ball1.getPosition())) <= ball1.getRadius()+ball2.getRadius();
}

void PhysicsHandler::handleBallCollision(Ball& ball, float timestep){
    for(int i = 0; i < balls.getBallAmount(); i++){
        Ball& otherBall = balls.getBall(i);
        if(&otherBall == &ball) continue; // skip self
        if(checkBallCollision(ball, otherBall, timestep)){
            Vector2f normal = normalizeVector2f(subtractVector2f(ball.getPosition(), otherBall.getPosition()));
            Vector2f vel = ball.getVelocity();
            float velDot = dotVector2f(vel, normal);
            if(velDot < 0.0f)
                ball.setVelocity(subtractVector2f(vel, scaleVector2f(normal, 2.0f * velDot)));
        }
    }
}

void PhysicsHandler::updateVelocity(Ball& ball, float timestep){
    ball.setVelocity(scaleVector2f(ball.getVelocity(), FRICTION));
};

void PhysicsHandler::updatePosition(Ball& ball, float timestep){
    Vector2f new_position = addVector2f(ball.getPosition(), scaleVector2f(ball.getVelocity(), timestep));
    ball.setPosition(new_position);
    handleBallCollision(ball, timestep);
    handleWallCollision(ball, timestep);
    handleBallCollision(ball, timestep);
};

void PhysicsHandler::updatePhysics(float timestep){
    for(int i = 0; i < balls.getBallAmount(); i++){
        Ball& ball = balls.getBall(i);
        updateVelocity(ball, timestep);
        updatePosition(ball, timestep);
    }
}

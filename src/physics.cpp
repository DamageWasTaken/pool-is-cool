#include <iostream>
#include <vector>
#include <cmath>

#include "Math.hpp"
#include "Ball.hpp"
#include "Physics.hpp"
#include "Area.hpp"

PhysicsHandler::PhysicsHandler(BallManager& _balls, Area _area) 
: balls(_balls), area(_area)
{
}

bool PhysicsHandler::checkWallCollision(Ball& ball, Edge edge, float timestep){
    Vector2f d2 = subtractVector2f(edge.end, edge.start);

    Vector2f normal = normalizeVector2f(clockwiseVector2f(d2));
    Vector2f offset = scaleVector2f(normal, ball.getRadius());

    Vector2f ballStart = addVector2f(ball.getPosition(), offset);
    Vector2f ballEnd   = addVector2f(addVector2f(ball.getPosition(), scaleVector2f(ball.getVelocity(), timestep)), offset);
    Vector2f d1        = subtractVector2f(ballEnd, ballStart);

    float denom = crossVector2f(d1, d2);
    if (std::fabs(denom) < 1e-9f) return false;

    Vector2f between = subtractVector2f(edge.start, ballStart);
    float t = crossVector2f(between, d2) / denom;
    float u = crossVector2f(between, d1) / denom;

    return (t >= 0.f && t <= 1.f && u >= 0.f && u <= 1.f);
}

void PhysicsHandler::handleWallCollision(Ball& ball, float timestep) {
    for (Edge edge : area.edges) {
        if (checkWallCollision(ball, edge, timestep)) {
            Vector2f edgeDir = subtractVector2f(edge.end, edge.start);
            Vector2f normal  = normalizeVector2f(clockwiseVector2f(edgeDir));

            Vector2f vel = scaleVector2f(ball.getVelocity(), timestep);
            float dot   = dotVector2f(vel, normal);
            Vector2f reflectedVel = subtractVector2f(vel, scaleVector2f(normal, 2.0f * dot));
            ball.setVelocity(scaleVector2f(reflectedVel, 1/timestep));

            float penetration = dotVector2f(
                subtractVector2f(ball.getPosition(), edge.start),
                normal
            );
            if (penetration < ball.getRadius()) {
                Vector2f correction = scaleVector2f(normal, ball.getRadius() - penetration);
                ball.setPosition(addVector2f(ball.getPosition(), correction));
            }
        }
    }
}
    

void PhysicsHandler::updateVelocity(Ball& ball, float timestep){
    ball.setVelocity(Vector2f(500, 500)); 
};

void PhysicsHandler::updatePosition(Ball& ball, float timestep){
    handleWallCollision(ball, timestep);
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

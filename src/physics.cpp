#include <Math.hpp>
#include <Ball.hpp>
#include <Physics.hpp>

PhysicsHandler::PhysicsHandler(BallManager& _balls, Area _area) 
: balls(_balls), area(_area)
{
}

void PhysicsHandler::updatePhysics(float delta){
    for(int i = 0; i < balls.getBallAmount(); i++){
        Ball& ball = balls.getBall(i);
        Vector2f new_position = addVector2f(ball.getPosition(), scaleVector2f(ball.getVelocity(), delta));
        ball.setPosition(new_position);
    }
}

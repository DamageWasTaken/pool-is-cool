#include <Math.hpp>
#include <Balls.hpp>
#include <Physics.hpp>

PhysicsHandler::PhysicsHandler() 
{
    
}

PhysicsHandler::PhysicsHandler(BallHandler& _balls) 
: balls(_balls);
{
    
}

void PhysicsHandler::updatePhysics(float timestep){
   // 
}

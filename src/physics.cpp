#include <iostream>
#include <vector>
#include <cmath>

#include "Math.hpp"
#include "Ball.hpp"
#include "Physics.hpp"
#include "Area.hpp"

#define FRICTION 0.99f

PhysicsHandler::PhysicsHandler(BallManager& _s_ball_manager, Area _area) 
: s_ball_manager(_s_ball_manager), area(_area)
{
}

bool PhysicsHandler::checkWallCollision(Ball& ball, Edge edge, float timestep){
    // Static overlap check - catches balls already through the wall
    Vector2f wall = subtractVector2f(edge.end, edge.start);
    Vector2f toBall = subtractVector2f(ball.getPosition(), edge.start);
    float t_closest = dotVector2f(toBall, wall) / dotVector2f(wall, wall);
    t_closest = std::fmax(0.0f, std::fmin(1.0f, t_closest));
    Vector2f closest = addVector2f(edge.start, scaleVector2f(wall, t_closest));
    float dist = lengthVector2f(subtractVector2f(ball.getPosition(), closest));
    if (dist < ball.getRadius()) return true;

    // Sweep check - catches balls about to hit the wall
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

void PhysicsHandler::solveWallCollision(Ball& ball, Edge edge){
    Vector2f wall = subtractVector2f(edge.end, edge.start);
    Vector2f toBall = subtractVector2f(ball.getPosition(), edge.start);
    Vector2f normal = normalizeVector2f(projectVector2f(toBall, clockwiseVector2f(wall)));
    float dist = dotVector2f(toBall, normal);
    float penetration = ball.getRadius() - dist;
    if (penetration >= 0.0f) 
    {
        ball.setPosition(addVector2f(ball.getPosition(), scaleVector2f(normal, penetration)));
    }
    Vector2f vel = ball.getVelocity();
    float velDot = dotVector2f(vel, normal);
    if (velDot < 0.0f)
        ball.setVelocity(subtractVector2f(vel, scaleVector2f(normal, 2.0f * velDot)));
}

void PhysicsHandler::solveCornerCollision(Ball& ball, Edge edge1, Edge edge2) {
    // Find the shared vertex between the two edges
    Vector2f corner;
    if (edge1.end.x == edge2.start.x && edge1.end.y == edge2.start.y)
        corner = edge1.end;
    else if (edge1.start.x == edge2.end.x && edge1.start.y == edge2.end.y)
        corner = edge1.start;
    else
        return; // edges don't share a vertex

    Vector2f toBall = subtractVector2f(ball.getPosition(), corner);
    float dist = lengthVector2f(toBall);
    float penetration = ball.getRadius() - dist;
    if (penetration <= 0.0f) return;

    Vector2f normal = normalizeVector2f(toBall);
    ball.setPosition(addVector2f(ball.getPosition(), scaleVector2f(normal, penetration)));
    Vector2f vel = ball.getVelocity();
    float velDot = dotVector2f(vel, normal);
    if (velDot < 0.0f)
        ball.setVelocity(subtractVector2f(vel, scaleVector2f(normal, 2.0f * velDot)));
}

int getIndex(Edge edge, std::vector<Edge> vector){
    for(int i = 0; i < vector.size(); i++){
        if(vector[i] == edge){
            return i;
        }
    }
    return -1;
}

Edge PhysicsHandler::findAdjecentEdge(Edge edge, std::vector<Edge> edges){
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
        solveWallCollision(ball, colliding_edges[0]);
        return;
    } else if (colliding_edges.size() > 1){
        while(colliding_edges.size() != 0){
            for(Edge edge : colliding_edges){
                Edge adjecentEdge = findAdjecentEdge(edge, colliding_edges);
                if(adjecentEdge != Edge()){
                    solveCornerCollision(ball, edge, adjecentEdge);
                    int i1 = getIndex(edge, colliding_edges);
                    int i2 = getIndex(adjecentEdge, colliding_edges);
                    if (i1 > i2) {
                        colliding_edges.erase(colliding_edges.begin() + i1);
                        colliding_edges.erase(colliding_edges.begin() + i2);
                    } else {
                        colliding_edges.erase(colliding_edges.begin() + i2);
                        colliding_edges.erase(colliding_edges.begin() + i1);
                    }
                } else {
                    solveWallCollision(ball, edge);
                    colliding_edges.erase(colliding_edges.begin()+getIndex(edge, colliding_edges));
                }
                break;
            }
        }
    }
}

bool PhysicsHandler::checkBallCollision(Ball& ball1, Ball& ball2, float timestep){
    return lengthVector2f(subtractVector2f(ball2.getPosition(), ball1.getPosition())) <= ball1.getRadius()+ball2.getRadius();
}

void PhysicsHandler::handleBallCollision(Ball& ball, int ball_number, float timestep){
    std::unordered_map<int, Ball> balls = s_ball_manager.getBalls();
    for(auto it = balls.begin(); it!=balls.end();){
        int other_ball_number = it->first;
        if(other_ball_number == ball_number) {
            ++it;
            continue; // skip self
        };
        Ball& other_ball = s_ball_manager.getBall(other_ball_number);
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
                        ) / std::pow(lengthVector2f(seperation), 2)
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
                        ) / std::pow(lengthVector2f(seperation), 2)
                    )
                )
            );
            handleWallCollision(ball, timestep);
        }
        ++it;
    }
}

void PhysicsHandler::updateVelocity(int ball_number, float timestep){
    s_ball_manager.getBall(ball_number).setVelocity(scaleVector2f(s_ball_manager.getBall(ball_number).getVelocity(), FRICTION));
    //ball.setVelocity(scaleVector2f(ball.getVelocity(), FRICTION));
};

void PhysicsHandler::updatePosition(int ball_number, float timestep){
    Ball& ball = s_ball_manager.getBall(ball_number);
    for (int i = 0; i<10; i++){
        handleBallCollision(ball, ball_number, timestep);
        handleWallCollision(ball, timestep);
    }
    Vector2f new_position = addVector2f(ball.getPosition(), scaleVector2f(ball.getVelocity(), timestep));
    ball.setPosition(new_position);
};

void PhysicsHandler::updatePhysics(float timestep){
    std::unordered_map<int, Ball> balls = s_ball_manager.getBalls();
    for(auto it = balls.begin(); it!=balls.end();){
        int ball_number = it->first;
        updateVelocity(ball_number, timestep);
        updatePosition(ball_number, timestep);
        ++it;
    }
}

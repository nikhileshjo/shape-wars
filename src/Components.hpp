#pragma once
#include "Vec2.h"
#include <SFML/Graphics/CircleShape.hpp>


// base class for all componenets
class Components
{
    public:
        bool exists   = false;
};


// kinematics
class CTransform : public Components
{
    public:
        vec2  position;
        vec2  velocity;
        float angle;    
    
        CTransform(){}
};


class CCollision : public Components
{
    public:
        float radius;
        CCollision(){}
        CCollision(float r)
        :radius(r)
        {}
};


class CScore : public Components
{
    public:
        int score;

        CScore(){}
};

class CShape : public Components
{
    public:
        sf::CircleShape shape;
        
        CShape(){}
        CShape(int pointCount, float radius)
        {
            shape.setPointCount(pointCount);
            shape.setRadius(radius);
        }
};

class CLifeSpan : public Components
{
    public:
        float lifeSpan;
        float remaining;

        CLifeSpan(){}
        CLifeSpan(float life)
            :lifeSpan(life),
             remaining(life)
            {}
};

class CInput : public Components
{
    public:
        int up;
        int down;
        int left;
        int right;
        int shoot;
};

class CSpecialAbility : public Components
{
    public:
        CSpecialAbility(){}
};
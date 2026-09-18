#pragma once
#include <iostream>
#include <concepts>
#include <vector>
#include <cmath>

class vec2
{
    public:
        // member variables
        float x, y;

        // constructors
        vec2(){}
        vec2(float x_in, float y_in)
        :x(x_in),
        y(y_in)
        {}
        
        // functions
        void    print() const;
        vec2    operator+ (vec2 v) const;
        void    operator+= (vec2 v);
        vec2    operator- (vec2 v) const;
        void    operator-= (vec2 v);
        vec2    operator* (float scalar) const;
        void    operator*= (float scalar);
        float   mag() const;
        vec2    normalize() const;
        vec2    normalizedAngleVec(double deg) const;
};
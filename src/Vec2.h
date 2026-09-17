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
        void    print();
        vec2    operator+ (vec2 v);
        void    operator+= (vec2 v);
        vec2    operator- (vec2 v);
        void    operator-= (vec2 v);
        vec2    operator* (float scalar);
        void    operator*= (float scalar);
        float   mag();
        vec2    normalize();

};
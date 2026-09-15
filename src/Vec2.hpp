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


// function definitions
void vec2::print()
{
    std::cout << x << "i + " << y << "j" <<std::endl;
    return;
}

vec2 vec2::operator+(vec2 v)
{
    float res_x = x + v.x;
    float res_y = y + v.y;

    return vec2(res_x, res_y);

}

void vec2::operator+=(vec2 v)
{
    x += v.x;
    y += v.y;
    return;
}

vec2 vec2::operator-(vec2 v)
{
    float res_x = x - v.x;
    float res_y = y - v.y;

    return vec2(res_x, res_y);
}

void vec2::operator-=(vec2 v)
{
    x -= v.x;
    y -= v.y;
    return;
}

vec2 vec2::operator* (float scalar)
{
    float res_x = x * scalar;
    float res_y = y * scalar;

    return vec2(res_x, res_y);
}

void vec2::operator*=(float scalar)
{
    x *= scalar;
    y *= scalar;
    return;
}

float vec2::mag()
{
    float magnitude = std::sqrt((x*x) + (y*y));
    return magnitude;
}

vec2 vec2::normalize()
{
    float magnitude = this->mag();
    float res_x = x / magnitude;
    float res_y = y / magnitude;

    return vec2(res_x, res_y);
}
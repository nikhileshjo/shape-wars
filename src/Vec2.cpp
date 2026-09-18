#include "Vec2.h"


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

float vec2::mag() const
{
    float magnitude = std::sqrt((x*x) + (y*y));
    return magnitude;
}

vec2 vec2::normalize() const
{
    float magnitude = this->mag();
    if (magnitude == 0)
    {
        return vec2(0, 0);
    }
    float res_x = x / magnitude;
    float res_y = y / magnitude;

    return vec2(res_x, res_y);
}
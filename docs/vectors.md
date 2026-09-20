# Vectors
Vectors are used in this game for postion tracking. Each frame may have multiple components and to track where they need to be rendered, we use vectors.\
These vectors obey the same laws as what we use in math.\
The related file is: [/src/Vec2.hpp](../src/Vec2.hpp)
# Using vectors
You can declare an object of type `vec2` or instatiate it by passing x-axis and y-axis values. It only accepts floats, so even if you pass ints, they will be converted internally.
```
// declaration
vec2 myVec;

// instantiation
vec2 myVec(5, 6);
// or
vec2 myVec = vec2(5, 6);
```
You can perform vector operations on it:
* vector addition: Adding 2 vectors to produce a new one.
* vector subtraction: Subtracting one vector from another to produce a new vector.
* Scalar multiplication: You can multiply a scalar value to a vector to produce a new vector.
* magnitude calculation: You can get a float value which is the magnitude of vector.
* Vector normalization: You can normalize vectors to produce a unit vector.
* printing vector: you can print your vectors.\
Illustrations
```
// Assuming you have v1, v2 and v3 declared
v3 = v1 + v2;
v3 = v2 - v1;
v3 = v1 * 5;
float x = v3.mag();
vec2 myUnitVec = v3.normalize();

// printing vectors
v3.printVec();

// create a normalized vector using angle in degrees as an input
vec2 vAngle;
vAngle.normalizedAngleVec(60);  // produce a unit vector at 60 degrees
vAngle.normalizedAngleVec(200); // produce a unit vector at 200 degrees
```
Printing a vector produces outputs like this: `5i + 6j` following regular math conventions of vector representations.\
NOTE: Normalising a vector involves division by the vector's magnitude, so mathematically speaking, it is undefined, but here, it returns a vector `0i + 0j` because it fits our requrement of not doing anything to when we see a 0 magnitude vector, example if the velocity vector of an enity is 0, it means we don't move it.
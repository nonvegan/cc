#include "vec.h"

Vec2i vec2i(int x, int y)
{
    return (Vec2i) {x, y};
}

Vec2i vec2ii(int x)
{
    return vec2i(x, x);
}

Vec2i vec2if(Vec2f a)
{
    return vec2i((int) a.x, (int) a.y); 
}

Vec2i vec2i_add(Vec2i a, Vec2i b)
{
    return vec2i(a.x + b.x, a.y + b.y);
}

Vec2i vec2i_sub(Vec2i a, Vec2i b)
{
    return vec2i(a.x - b.x, a.y - b.y);
}

Vec2i vec2i_mul(Vec2i a, Vec2i b)
{
    return vec2i(a.x * b.x, a.y * b.y);
}

Vec2i vec2i_div(Vec2i a, Vec2i b)
{
    return vec2i(a.x / b.x, a.y / b.y);
}


Vec2f vec2f(float x, float y)
{
    return (Vec2f) {x, y};
}

Vec2f vec2ff(float x)
{
    return vec2f(x, x);
}

Vec2f vec2fi(Vec2i a)
{
    return vec2f((float) a.x, (float) a.y); 
}

Vec2f vec2f_add(Vec2f a, Vec2f b)
{
    return vec2f(a.x + b.x, a.y + b.y);
}

Vec2f vec2f_sub(Vec2f a, Vec2f b)
{
    return vec2f(a.x - b.x, a.y - b.y);
}

Vec2f vec2f_mul(Vec2f a, Vec2f b)
{
    return vec2f(a.x * b.x, a.y * b.y);
}

Vec2f vec2f_div(Vec2f a, Vec2f b)
{
    return vec2f(a.x / b.x, a.y / b.y);
}

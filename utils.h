#ifndef UTILS
#define UTILS

#include <cmath>

struct Vector2 {
    float x;
    float y;

    Vector2(float x=0, float y=0) : x(x), y(y) {}

    Vector2& operator=(const Vector2& a) {
        x=a.x;
        y=a.y;
        return *this;
    }

    Vector2& operator=(float a) {
        x = a;
        y = a;
        return *this;
    }

    Vector2& operator+=(const Vector2& a) {
        x = x + a.x;
        y = y + a.y;
        return *this;
    }

    Vector2& operator-=(const Vector2& a) {
        x = x - a.x;
        y = y - a.y;
        return *this;
    }

    Vector2 operator+(const Vector2& a) const {
        return Vector2(x+a.x, y+a.y);
    }

    Vector2 operator+(float a) const {
        return Vector2(x + a, y + a);
    }

    Vector2 operator-(const Vector2& a) const {
        return Vector2(x-a.x, y-a.y);
    }

    Vector2 operator-(float a) const {
        return Vector2(x - a, y - a);
    }

    Vector2 operator*(const Vector2& a) const {
        return Vector2(x*a.x, y*a.y);
    }

    Vector2 operator*(float a) const {
        return Vector2(x * a, y * a);
    }

    Vector2 operator/(const Vector2& a) const {
        return Vector2(x/a.x, y/a.y);
    }

    Vector2 operator/(float a) const {
        return Vector2(x / a, y / a);
    }

    bool operator==(const Vector2& a) const {
        return (x == a.x && y == a.y);
    }

    float magnitude() const {
        return std::sqrt(x*x + y*y);
    }

    Vector2 normalized() const {
        float mag = magnitude(); 
        return Vector2(x/mag, y/mag);
    }

    Vector2 perp(bool flip) const {
        if (flip)
            return Vector2(y, -x);
        else
            return Vector2(-y, x);
    }

    float sq_magnitude() const {
        return x*x + y*y;
    }

};

namespace RNG {
    int Int(int min, int max);
    float Float(float min, float max);
    Vector2 ToVector(Vector2 vec);
}

struct transform {
    Vector2 pos = Vector2();
    Vector2 scale = Vector2();
    double theta;
};

struct bullet {
    transform pos = transform();
    Vector2 speed = Vector2();
};


#endif

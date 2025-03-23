#ifndef UTILS
#define UTILS

namespace RNG {
    int Int(int min, int max);
    float Float(float min, float max);
}

struct transform {
    float x;
    float y;
    double theta;
    float scaleX;
    float scaleY;
};

struct bullet {
    float x;
    float y;
    float speedX;
    float speedY;
};

#endif

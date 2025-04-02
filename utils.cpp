#include "utils.h"
#include <random>

namespace RNG {

int Int(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(min, max);

    return distr(gen);
}

float Float(float min, float max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> distr(min, max);

    return (float)distr(gen);
}

Vector2 ToVector(Vector2 vec) {
    return Vector2(Float(0, vec.x), Float(0, vec.y));  
}

}

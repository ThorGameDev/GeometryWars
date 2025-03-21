#ifndef UTILS
#define UTILS

namespace RNG {
    int Int(int min, int max);
    double Double(double min, double max);
}

struct transform {
    double x;
    double y;
    double theta;
    double scaleX;
    double scaleY;
};

#endif

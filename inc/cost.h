#ifndef STEREO_MATCHING_COST_H
#define STEREO_MATCHING_COST_H

#include <opencv2/opencv.hpp>
#include "config.h"

class CostVolume {
private:
    int n, m;
    double*** c;
public:
    void Compute(int n, int m, int** imgL, int** imgR);
    double** Get(int row);
    ~CostVolume();
};

#endif //STEREO_MATCHING_COST_H
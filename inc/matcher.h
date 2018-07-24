#ifndef STEREO_MATCHING_MATCHER_H
#define STEREO_MATCHING_MATCHER_H

#include <opencv2/opencv.hpp>
#include "config.h"

class Matcher {
private:
    int n, m;
    int **arrL, **arrR;
    int **ansL, **ansR;
    int **opt;
    double** dp;

    void ComputeRow(int n, int row, double** cost);
    void FillDisparity(int** arr);
public:
    void ComputeDispMap(const cv::Mat &imgL, const cv::Mat &imgR, cv::Mat &outputL, cv::Mat &outputR);
};

#endif //STEREO_MATCHING_MATCHER_H
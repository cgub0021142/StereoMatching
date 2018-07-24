#ifndef STEREO_MATCHING_MATCHER_H
#define STEREO_MATCHING_MATCHER_H

#include <opencv2/opencv.hpp>
#include "config.h"

class Matcher {
public:
    void ComputeDispMap(const cv::Mat& imgL, const cv::Mat& imgR, cv::Mat& ansL, cv::Mat& ansR);
};

#endif //STEREO_MATCHING_MATCHER_H

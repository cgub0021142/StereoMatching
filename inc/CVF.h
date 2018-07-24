#ifndef STEREO_MATCHING_CVF_H
#define STEREO_MATCHING_CVF_H

#include <opencv2/opencv.hpp>
#include "config.h"

void FilterCV(const cv::Mat& img, cv::Mat costVolume[]);

#endif //STEREO_MATCHING_CVF_H

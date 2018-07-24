#ifndef STEREO_MATCHING_CVC_H
#define STEREO_MATCHING_CVC_H

#include <opencv2/opencv.hpp>
#include "config.h"

cv::Mat BuildCV_L(const cv::Mat& imgL, const cv::Mat& imgR, const cv::Mat& gradL, const cv::Mat& gradR, const int d);
cv::Mat BuildCV_R(const cv::Mat& imgL, const cv::Mat& imgR, const cv::Mat& gradL, const cv::Mat& gradR, const int d);

#endif //STEREO_MATCHING_CVC_H

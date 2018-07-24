#include "../inc/CVC.h"

static float magicCost(float* lC, float* rC, float* lG, float* rG) {
    float clrDiff = fabs(lC[0] - rC[0]) + fabs(lC[1] - rC[1]) + fabs(lC[2] - rC[2]);
    float grdDiff = fabs(*lG - *rG);
    return ALPHA * clrDiff + (1 - ALPHA) * grdDiff;
}

static float magicCost(float* lC, float* lG) {
    float clrDiff = fabs(lC[0] - BETA) + fabs(lC[1] - BETA) + fabs(lC[2] - BETA);
    float grdDiff = fabs(*lG - BETA);
    return ALPHA * clrDiff + (1 - ALPHA) * grdDiff;
}

cv::Mat BuildCV_L(const cv::Mat& imgL, const cv::Mat& imgR, const cv::Mat& gradL, const cv::Mat& gradR, const int d) {
    int n = imgL.rows;
    int m = imgL.cols;
    cv::Mat costVolume(n, m, CV_32FC1);

    for (int i = 0; i < n; ++i) {
        float* lData = (float*) imgL.ptr<float>(i);
        float* rData = (float*) imgR.ptr<float>(i);
        float* lGData = (float*) gradL.ptr<float>(i);
        float* rGData = (float*) gradR.ptr<float>(i);
        float* cost = (float*) costVolume.ptr<float>(i);
        for (int j = d; j < m; ++j) {
            float* lC = lData + 3 * j;
            float* rC = rData + 3 * (j - d);
            float* lG = lGData + j;
            float* rG = rGData + j - d;
            costVolume.at<float>(i, j) = magicCost(lC, rC, lG, rG);
        }
        for (int j = 0; j < d; ++j) {
            float* lC = lData + 3 * j;
            float* lG = lGData + j;
            costVolume.at<float>(i, j) = magicCost(lC, lG);
        }
    }
    return costVolume;
}

cv::Mat BuildCV_R(const cv::Mat& imgL, const cv::Mat& imgR, const cv::Mat& gradL, const cv::Mat& gradR, const int d) {
    int n = imgL.rows;
    int m = imgL.cols;
    cv::Mat costVolume(n, m, CV_32FC1);

    for (int i = 0; i < n; ++i) {
        float* lData = (float*) imgL.ptr<float>(i);
        float* rData = (float*) imgR.ptr<float>(i);
        float* lGData = (float*) gradL.ptr<float>(i);
        float* rGData = (float*) gradR.ptr<float>(i);
        float* cost = (float*) costVolume.ptr<float>(i);
        for (int j = 0; j + d < m; ++j) {
            float* lC = lData + 3 * j;
            float* rC = rData + 3 * (j + d);
            float* lG = lGData + j;
            float* rG = rGData + j + d;
            cost[j] = magicCost(lC, rC, lG, rG);
        }
        for (int j = m - d; j < m; ++j) {
            float* lC = lData + 3 * j;
            float* lG = lGData + j;
            cost[j] = magicCost(lC, lG);
        }
    }
    return costVolume;
}
#include "../inc/matcher.h"
#include "../inc/CVC.h"
#include "../inc/CVF.h"
#include "../inc/jointWMF.h"

static cv::Mat getGradient(const cv::Mat &img) {
    cv::Mat ans;
    cv::cvtColor(img, ans, cv::COLOR_RGB2GRAY);
    cv::Sobel(ans, ans, CV_32F, 1, 0, 1);
    return ans;
}

static cv::Mat dispSelect(int n, int m, cv::Mat costVolume[]) {
    cv::Mat ans(n, m, CV_8UC1);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            float minCost = 1e18;
            int pos = 0;
            for (int d = 0; d < MAX_DISPARITY; ++d) {
                float cost = costVolume[d].at<float>(i, j);
                if (minCost > cost) {
                    minCost = cost;
                    pos = d;
                }
            }
            ans.at<uchar>(i, j) = uchar(pos * 4);
        }
    }
    return ans;
}

static void postProcess(const cv::Mat &imgL, const cv::Mat &imgR, cv::Mat &ansL, cv::Mat &ansR) {
    cv::Mat lImg_8UC3, rImg_8UC3;
    imgL.convertTo(lImg_8UC3, CV_8UC3, 255);
    imgR.convertTo(rImg_8UC3, CV_8UC3, 255);

    ansL = JointWMF::filter(ansL, lImg_8UC3, MED_SZ / 2);
    ansR = JointWMF::filter(ansR, rImg_8UC3, MED_SZ / 2);

    ansL.convertTo(ansL, CV_8U);
    ansR.convertTo(ansR, CV_8U);
}

void Matcher::ComputeDispMap(const cv::Mat &oriL, const cv::Mat &oriR, cv::Mat &ansL, cv::Mat &ansR) {
    cv::Mat imgL = oriL.clone();
    cv::Mat imgR = oriR.clone();

    if ((imgL.type() & CV_MAT_DEPTH_MASK) != CV_32F) {
        imgL.convertTo(imgL, CV_32F, 1 / 255.0f);
        imgR.convertTo(imgR, CV_32F, 1 / 255.0f);
    }

    cv::Mat gradL = getGradient(imgL);
    cv::Mat gradR = getGradient(imgR);

    cv::Mat costVolumeL[MAX_DISPARITY];
    cv::Mat costVolumeR[MAX_DISPARITY];
    for (int d = 0; d < MAX_DISPARITY; ++d) {
        costVolumeL[d] = BuildCV_L(imgL, imgR, gradL, gradR, d);
        costVolumeR[d] = BuildCV_R(imgR, imgL, gradR, gradL, d);
    }

    FilterCV(imgL, costVolumeL);
    FilterCV(imgR, costVolumeR);

    int n = imgL.rows;
    int m = imgL.cols;
    ansL = dispSelect(n, m, costVolumeL);
    ansR = dispSelect(n, m, costVolumeR);

    postProcess(imgL, imgR, ansL, ansR);
}
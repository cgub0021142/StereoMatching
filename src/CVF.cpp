#include "../inc/CVF.h"
#include "../inc/fastGIF.h"

void FilterCV(const cv::Mat& img, cv::Mat costVolume[]) {
    FastGuidedFilter fastGuidedFilter(img, GIF_R_WIN, GIF_EPS, SUBSAMPLE_RATE);
    for (int d = 0; d < MAX_DISPARITY; ++d) {
        costVolume[d] = fastGuidedFilter.filter(costVolume[d]);
    }
}
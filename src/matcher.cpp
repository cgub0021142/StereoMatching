#include "../inc/matcher.h"
#include "../inc/cost.h"
#include <bits/stdc++.h>
using namespace std;

static cv::Mat arr2Mat(int n, int m, int** arr) {
    cv::Mat ans(n, m, CV_8UC1);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            ans.at<uchar>(i, j) = uchar(arr[i + 1][j + 1] * 4);
        }
    }
    return ans;
}

void Matcher::ComputeRow(int n, int row, double** cost) {
    for (int i = 1; i <= n; ++i) {
        for (int j = max(1, i - MAX_DISPARITY + 1); j <= i; ++j) {
            double cost1 = dp[i - 1][j - 1] + cost[i][i - j];
            double cost2 = dp[i - 1][j] + OCCLUSION_COST;
            double cost3 = dp[i][j - 1] + OCCLUSION_COST;
            dp[i][j] = INF;
            if (dp[i][j] > cost1) {
                dp[i][j] = cost1;
                opt[i][j] = 1;
            }
            if (dp[i][j] > cost2) {
                dp[i][j] = cost2;
                opt[i][j] = 2;
            }
            if (dp[i][j] > cost3) {
                dp[i][j] = cost3;
                opt[i][j] = 3;
            }
        }
    }

    for (int i = n, j = n; i >= 1 && j >= 1; ) {
        assert(i >= j);
        switch (opt[i][j]) {
            case 1: ansL[row][i] = ansR[row][j] = uchar(i - j); i--; j--; break;
            case 2: i--; break;
            case 3: j--; break;
            default: break;
        }
    }

    // Reset DP
    for (int i = 1; i <= n; ++i) {
        for (int j = max(1, i - MAX_DISPARITY + 1); j <= i; ++j) {
            dp[i][j] = INF;
            opt[i][j] = -1;
        }
    }
}

void Matcher::FillDisparity(int** arr) {
    int* L = new int[m + 2];
    int* R = new int[m + 2];
    for (int i = 1; i <= n; ++i) {
        L[0] = 0;
        R[m + 1] = m + 1;
        for (int j = 1; j <= m; ++j) {
            L[j] = (arr[i][j] == -1) ? L[j - 1] : j;
        }
        for (int j = m; j >= 1; --j) {
            R[j] = (arr[i][j] == -1) ? R[j + 1] : j;
        }
        for (int j = 1; j <= m; ++j) {
            if (arr[i][j] != -1) {
                continue;
            }
            if (L[j] != -1 && R[j] != -1) {
                arr[i][j] = min(arr[i][L[j]], arr[i][R[j]]);
            } else if (L[j] != -1) {
                arr[i][j] = arr[i][L[j]];
            } else if (R[j] != -1) {
                arr[i][j] = arr[i][R[j]];
            } else {
                arr[i][j] = 0;
            }
        }
    }
}

void Matcher::ComputeDispMap(const cv::Mat &imgL, const cv::Mat &imgR, cv::Mat &outputL, cv::Mat &outputR) {
    cv::Mat grayL, grayR;
    cv::cvtColor(imgL, grayL, cv::COLOR_RGB2GRAY);
    cv::cvtColor(imgR, grayR, cv::COLOR_RGB2GRAY);

    n = imgL.rows;
    m = imgL.cols;
    arrL = new int*[n + 1];
    arrR = new int*[n + 1];
    ansL = new int*[n + 1];
    ansR = new int*[n + 1];
    for (int i = 1; i <= n; ++i) {
        arrL[i] = new int[m + 1];
        arrR[i] = new int[m + 1];
        ansL[i] = new int[m + 1];
        ansR[i] = new int[m + 1];
        for (int j = 1; j <= m; ++j) {
            arrL[i][j] = grayL.at<uchar>(i - 1, j - 1);
            arrR[i][j] = grayR.at<uchar>(i - 1, j - 1);
            ansL[i][j] = -1;
            ansR[i][j] = -1;
        }
    }

    // Init DP
    dp = new double*[m + 1];
    opt = new int*[m + 1];
    for (int i = 0; i <= m; ++i) {
        dp[i] = new double[m + 1];
        opt[i] = new int[m + 1];
        for (int j = 0; j <= m; ++j) {
            dp[i][j] = INF;
            opt[i][j] = -1;
        }
    }
    dp[0][0] = 0;

    CostVolume costVolume;
    costVolume.Compute(n, m, arrL, arrR);
    for (int i = 1; i <= n; ++i) {
        ComputeRow(m, i, costVolume.Get(i));
    }

    FillDisparity(ansL);
    FillDisparity(ansR);

    outputL = arr2Mat(n, m, ansL);
    outputR = arr2Mat(n, m, ansR);

    // Free memory
    for (int i = 1; i <= n; ++i) {
        delete[] arrL[i];
        delete[] arrR[i];
        delete[] ansL[i];
        delete[] ansR[i];
    }
    delete[] arrL;
    delete[] arrR;
    delete[] ansL;
    delete[] ansR;

    for (int i = 0; i <= m; ++i) {
        delete[] dp[i];
        delete[] opt[i];
    }
    delete[] dp;
    delete[] opt;
}
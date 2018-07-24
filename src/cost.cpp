#include "../inc/cost.h"
#include <cmath>
#include <iostream>
#include <omp.h>
using namespace std;

static const int MAXV = 10000;
static double e[MAXV];
static double f[MAXV];

static double Weight(int** img, int x1, int y1, int x2, int y2) {
    int c = abs(img[x1][y1] - img[x2][y2]);
    int g = (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
    return e[c] * f[g];
}

void CostVolume::Compute(int n, int m, int** imgL, int** imgR) {
    for (int i = 0; i < MAXV; ++i) {
        e[i] = exp(-i / GC);
        f[i] = exp(-sqrt(i) / GG);
    }

    this->n = n;
    this->m = m;

    c = new double **[n + 1];
    for (int i = 1; i <= n; ++i) {
        c[i] = new double *[m + 1];
        for (int j = 1; j <= m; ++j) {
            c[i][j] = new double[MAX_DISPARITY];
        }
    }

    // Initial values
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            for (int d = 0; d < MAX_DISPARITY && d <= j; ++d) {
                c[i][j][d] = abs(imgL[i][j] - imgR[i][j - d]);
            }
            for (int d = j + 1; d < MAX_DISPARITY; ++d) {
                c[i][j][d] = 0;
            }
        }
    }

    // Horizontal filtering
#pragma omp parallel for
    for (int dx = 0; dx < N_THREAD; ++dx) {
        int l = dx * THREAD_LENGTH;
        int r = min(l + THREAD_LENGTH, MAX_DISPARITY);
        double **tmp = new double *[n + 1];
        for (int i = 1; i <= n; ++i) {
            tmp[i] = new double[m + 1];
            for (int j = 1; j <= m; ++j) {
                tmp[i][j] = 0;
            }
        }
        for (int d = l; d < r; ++d) {
            for (int i = 1; i <= n; ++i) {
                for (int j = 1 + d; j <= m; ++j) {
                    double sumWeight = 0;
                    tmp[i][j] = 0;
                    for (int x = -WINDOW_SIZE; x <= WINDOW_SIZE; ++x) {
                        if (j - d + x < 1 || j + x > m) {
                            continue;
                        }
                        double w1 = Weight(imgL, i, j, i, j + x);
                        double w2 = Weight(imgR, i, j - d, i, j - d + x);
                        double weight = w1 * w2;
                        sumWeight += weight;
                        tmp[i][j] += weight * c[i][j + x][d];
                    }
                    tmp[i][j] /= sumWeight;
                }
            }
            for (int i = 1; i <= n; ++i) {
                for (int j = 1 + d; j <= m; ++j) {
                    c[i][j][d] = tmp[i][j];
                }
            }
        }
        for (int i = 1; i <= n; ++i) {
            delete[] tmp[i];
        }
        delete[] tmp;
    }


    // Vertical filtering
#pragma omp parallel for
    for (int dx = 0; dx < N_THREAD; ++dx) {
        int l = dx * THREAD_LENGTH;
        int r = min(l + THREAD_LENGTH, MAX_DISPARITY);
        double **tmp = new double *[n + 1];
        for (int i = 1; i <= n; ++i) {
            tmp[i] = new double[m + 1];
            for (int j = 1; j <= m; ++j) {
                tmp[i][j] = 0;
            }
        }
        for (int d = l; d < r; ++d) {
            for (int i = 1; i <= n; ++i) {
                for (int j = 1 + d; j <= m; ++j) {
                    double sumWeight = 0;
                    tmp[i][j] = 0;
                    for (int x = -WINDOW_SIZE; x <= WINDOW_SIZE; ++x) {
                        if (i + x < 1 || i + x > n) {
                            continue;
                        }
                        double w1 = Weight(imgL, i, j, i + x, j);
                        double w2 = Weight(imgR, i, j - d, i + x, j - d);
                        double weight = w1 * w2;
                        sumWeight += weight;
                        tmp[i][j] += weight * c[i + x][j][d];
                    }
                    tmp[i][j] /= sumWeight;
                }
            }
            for (int i = 1; i <= n; ++i) {
                for (int j = 1 + d; j <= m; ++j) {
                    c[i][j][d] = tmp[i][j];
                }
            }
        }
        for (int i = 1; i <= n; ++i) {
            delete[] tmp[i];
        }
        delete[] tmp;
    }
}

CostVolume::~CostVolume() {
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            delete[] c[i][j];
        }
        delete[] c[i];
    }
    delete[] c;
}

double** CostVolume::Get(int row) {
    return c[row];
}
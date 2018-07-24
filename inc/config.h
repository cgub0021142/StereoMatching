#ifndef STEREO_MATCHING_CONFIG_H
#define STEREO_MATCHING_CONFIG_H

const int MAX_DISPARITY = 64;
const int WINDOW_SIZE = 8;

const int N_THREAD = 4;
const int THREAD_LENGTH = MAX_DISPARITY / N_THREAD;

const double INF = 1e15;
const double OCCLUSION_COST = 9;
const double GC = 14;
const double GG = 10;

#endif //STEREO_MATCHING_CONFIG_H
#ifndef STEREO_MATCHING_CONFIG_H
#define STEREO_MATCHING_CONFIG_H

const int MAX_DISPARITY = 64;

const float ALPHA = 0.9;
const float BETA = 1.0;

const int GIF_R_WIN = 8;
const float GIF_EPS = 0.0001f;
const int SUBSAMPLE_RATE = 4;

const int MED_SZ = 19;

const int N_THREAD = 4;
const int THREAD_LENGTH = (MAX_DISPARITY + N_THREAD - 1) / N_THREAD;

#endif //STEREO_MATCHING_CONFIG_H

#include <iostream>
#include "inc/matcher.h"
using namespace std;

int main() {
    freopen("input.txt", "r", stdin);

    string pathL, pathR;
    getline(cin, pathL);
    getline(cin, pathR);

    cv::Mat imgL = cv::imread(pathL, cv::IMREAD_COLOR);
    cv::Mat imgR = cv::imread(pathR, cv::IMREAD_COLOR);

    Matcher matcher;
    cv::Mat ansL, ansR;
    matcher.ComputeDispMap(imgL, imgR, ansL, ansR);

    cv::namedWindow("Left", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("Right", cv::WINDOW_AUTOSIZE);
    cv::imshow("Left", ansL);
    cv::imshow("Right", ansR);
    while (cv::waitKey(0) != 27) {};
    cv::destroyAllWindows();

    return 0;
}
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include "model.h"

int main() {

    cv::Mat test = cv::imread("/home/wangxc/Code/NIQSV+/resource/Fehn_interpolated_Newspaper_bh_4_to_6_IV_136.bmp");

    model new_model(0.5, 3, 8, 4, 0.5, 200, 1);
    float score = new_model.test(test);

    std::cout << "Score: " << score << std::endl;

    return 0;
}
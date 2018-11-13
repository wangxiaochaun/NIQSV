#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include "model.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    cv::Mat test = cv::imread("/home/wangxc/Code/NIQSV+/resource/synthesized_image.bmp");

    model new_model(0.5, 3, 8, 4);
    new_model.test(test);

    return 0;
}
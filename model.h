//
// Created by wangxc on 18-11-13.
//

#ifndef NIQSV_MODEL_H
#define NIQSV_MODEL_H

#include <iostream>
#include <opencv2/opencv.hpp>

class model {
public:
    model(double k_c, int open_size, int close_size, int diameter);
    double test(const cv::Mat);
private:
    double k_c;                    // weighting between D_Y and D_Cb, D_Cr
    int open_size, close_size, diameter;     // size for morphological operation.
    double niqsv(const cv::Mat input);
    double zrate(const cv::Mat input);
};


#endif //NIQSV_MODEL_H

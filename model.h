//
// Created by wangxc on 18-11-13.
//

#ifndef NIQSV_MODEL_H
#define NIQSV_MODEL_H

#include <iostream>
#include <opencv2/opencv.hpp>

class model {
public:
    model(){};
    model(float k_c, int open_size, int close_size, int diameter, float significant_level,
            float k_z, float C);
    /**
     * @brief: give initialization of all trainable parameters.
     * @param: k_c: weighted sum of distortions across Y, Cb, Cr channels. Large k_c reduces
     * effect of Y.
     * @param: open_size: size of structure element for opening operation.
     * @param: close_size: size of structure element for closing operation.
     * @param: diameter: size of structure element for edge erosion and dilation.
     * @param: significant_level: percent of mean value of AHG. Large significant level will
     * keep more SW regions.
     * @param: k_z: controlling the effect of holes, large k_z will reduce the effect of holes.
     * @param: C: a constant used to adjust the difference between the images with geometric
     * distortion and without.
     * @return
     */
    float test(const cv::Mat input);
private:
    float k_c;                    // weighting between D_Y and D_Cb, D_Cr
    int open_size, close_size, diameter;     // size for morphological operation.
    float significant_level;
    float k_z;                    // weighting the holes effect
    float C;
    float niqsv(const cv::Mat input);
    float zrate(const cv::Mat input);
    float stretching(const cv::Mat input);
};


#endif //NIQSV_MODEL_H

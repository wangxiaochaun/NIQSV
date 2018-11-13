//
// Created by wangxc on 18-11-13.
//

#include "model.h"
#include <vector>

model::model(double k_c, int open_size, int close_size, int diameter)
{
    this->k_c = k_c;
    this->open_size = open_size;
    this->close_size = close_size;
    this->diameter = diameter;

}

double model::test(const cv::Mat input)
{
    double result = niqsv(input);

    std::cout << "NIQSV score: " << result << std::endl;

    return -1;
}

double model::niqsv(const cv::Mat input)
{
    cv::Mat temp = input.clone();
    cv::cvtColor(temp, temp, CV_BGR2YCrCb);
    temp.convertTo(temp, CV_64FC3);         // Disabled -- lead to very large negative score.

    std::vector<cv::Mat> temp_channels;
    cv::split(temp, temp_channels);
    auto temp_iter = temp_channels.begin();

    //-----------------------------------
    // A1: Opening Y, Cb, Cr -> Closing Y, Cb, Cr,
    // -> Difference Dx -> Integration with k_c -> Overall Difference
    cv::Mat dst = temp.clone();
    std::vector<cv::Mat> dst_channels;
    cv::split(dst, dst_channels);
    auto dst_iter = dst_channels.begin();

    cv::Mat open_element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(open_size, open_size));
    cv::Mat close_element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(close_size, close_size));

    for (; temp_iter != temp_channels.end(); temp_iter++, dst_iter++)
    {
        cv::morphologyEx(*temp_iter, *dst_iter, cv::MORPH_OPEN, open_element);
        cv::morphologyEx(*dst_iter, *dst_iter, cv::MORPH_CLOSE, close_element);
        cv::absdiff(*temp_iter, *dst_iter, *dst_iter);
    }

    cv::Mat D = cv::Mat(temp.rows, temp.cols, CV_64F, cv::Scalar(0.0f));

    D = (1 - k_c) * dst_channels[0] + 0.5 * k_c * (dst_channels[1] + dst_channels[2]);
    cv::imshow("combination", D);
    //-----------------------------------

    cv::Mat edge_element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(diameter, diameter));
    cv::Mat edge = cv::Mat(temp.rows, temp.cols, CV_64F, cv::Scalar(0.0f));

    cv::Mat dilation_Y, erosion_Y;
    cv::morphologyEx(temp_channels[0], dilation_Y, cv::MORPH_DILATE, edge_element);
    cv::morphologyEx(temp_channels[0], erosion_Y, cv::MORPH_ERODE, edge_element);
    edge = dilation_Y - erosion_Y;
    cv::Mat b = edge / 255;
    //cv::imshow("edge", b);


    double MSE = 0.0f;
    double denominator = 0.0f;

    for (int i = 0; i < temp.rows; i++)
    {
        for (int j = 0; j < temp.cols; j++)
        {
            MSE += b.at<double>(i, j) * sqrt(D.at<double>(i, j));
            denominator += b.at<double>(i, j);
        }
    }

    MSE /= denominator;

    double NIQSV = 10 * log10(255 * 255 / MSE);

    //cv::waitKey(0);

    return NIQSV;
}

double model::zrate(const cv::Mat input)
{


    return -1;
}
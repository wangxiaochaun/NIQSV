//
// Created by wangxc on 18-11-13.
//

#include "model.h"
#include <vector>
#include <numeric>

model::model(float k_c, int open_size, int close_size, int diameter, float significant_level,
        float k_z, float C)
{
    this->k_c = k_c;
    this->open_size = open_size;
    this->close_size = close_size;
    this->diameter = diameter;
    this->significant_level = significant_level;
    this->k_z = k_z;
    this->C = C;

}

float model::test(const cv::Mat input)
{
    float result = niqsv(input);

    std::cout << "NIQSV score: " << result << std::endl;

    float hole_rate = zrate(input);

    std::cout << "zrate: " << hole_rate << std::endl;

    float detection_stretching = stretching(input);

    std::cout << "detection_stretching: " << detection_stretching << std::endl;

    float score = result / (detection_stretching * (1 + k_z * hole_rate) + C);

    return score;
}

float model::niqsv(const cv::Mat input)
{
    cv::Mat temp = input.clone();
    cv::cvtColor(temp, temp, CV_BGR2YCrCb);
    temp.convertTo(temp, CV_32FC3, 1, 0);         // Disabled -- lead to very large negative score.

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

    cv::Mat D = cv::Mat(temp.rows, temp.cols, CV_32F, cv::Scalar(0.0f));

    D = (1 - k_c) * dst_channels[0] + 0.5 * k_c * (dst_channels[1] + dst_channels[2]);

    //-----------------------------------

    cv::Mat edge_element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(diameter, diameter));
    cv::Mat edge = cv::Mat(temp.rows, temp.cols, CV_32F, cv::Scalar(0.0f));

    cv::Mat dilation_Y, erosion_Y;
    cv::morphologyEx(temp_channels[0], dilation_Y, cv::MORPH_DILATE, edge_element);
    cv::morphologyEx(temp_channels[0], erosion_Y, cv::MORPH_ERODE, edge_element);
    edge = dilation_Y - erosion_Y;
    cv::Mat b = edge / 255;


    float MSE = 0.0f;
    float denominator = 0.0f;

    for (int i = 0; i < temp.rows; i++)
    {
        for (int j = 0; j < temp.cols; j++)
        {
            MSE += b.at<float>(i, j) * sqrt(D.at<float>(i, j));
            denominator += b.at<float>(i, j);
        }
    }

    MSE /= denominator;

    float NIQSV = 10 * log10(255 * 255 / MSE);

    //cv::waitKey(0);

    return NIQSV;
}

float model::zrate(const cv::Mat input)
{
    cv::Mat temp = input.clone();
    cv::cvtColor(temp, temp, CV_BGR2YCrCb);
    std::vector<cv::Mat> channels;
    cv::split(temp, channels);

    cv::Mat luminance = channels[0];

    int num_of_holes = 0;

    for (int i = 0; i < luminance.rows; i++)
    {
        for (int j = 0; j < luminance.cols; j++)
        {
            if ((int)luminance.at<uchar>(i, j) == 0)
            {
                num_of_holes++;
            }
        }
    }

    return (num_of_holes * 1.0f / (luminance.rows * luminance.cols));
}

float model::stretching(const cv::Mat input)
{
    // Sobel gradient
    cv::Mat temp = input.clone();
    cv::cvtColor(temp, temp, CV_BGR2YCrCb);
    temp.convertTo(temp, CV_32F, 1, 0);
    std::vector<cv::Mat> channels;
    cv::split(temp, channels);
    cv::Mat luminance = channels[0];

    cv::Mat grad_ver, grad_hor;
    cv::Mat abs_grad_ver, abs_grad_hor;

    // horizontal gradient
    cv::Sobel(luminance, grad_hor, CV_32F, 1, 0, 3, 1, 1, cv::BORDER_DEFAULT);
    cv::convertScaleAbs(grad_hor, abs_grad_hor);
    cv::imshow("horizontal direction Sobel", abs_grad_hor);

    // std::cout << "temp: " << temp.at<cv::Vec3f>(100, 100) << std::endl;
    // std::cout << "luminance: " << luminance.at<float>(100, 100) << std::endl;
    // std::cout << "grad_ver: " << grad_ver.at<float>(100, 100) << std::endl;
    // std::cout << "grad_ver: " << abs_grad_ver.at<float>(100, 100) << std::endl;

    // vertical gradient
    cv::Sobel(luminance, grad_ver, CV_32F, 0, 1, 3, 1, 1, cv::BORDER_DEFAULT);
    cv::convertScaleAbs(grad_ver, abs_grad_ver);
    cv::imshow("vertical direction Sobel", abs_grad_ver);

    std::vector<float> AHG(luminance.cols);
    std::vector<float> AVG(luminance.cols);

    // AHG/AVG
    for (int i = 0; i < luminance.cols; i++)
    {
        float temp_hor = 0;
        float temp_ver = 0;

        for (int j = 0; j < luminance.rows; j++)
        {
            temp_hor += cv::abs(grad_hor.at<float>(j, i));
            temp_ver += cv::abs(grad_ver.at<float>(j, i));
        }

        AHG[i] = temp_hor / luminance.rows;
        AVG[i] = temp_ver / luminance.rows;
    }

    float sum = (float)std::accumulate(std::begin(AHG), std::end(AHG), 0.0);
    float mean = sum / AHG.size();
    float threshold = significant_level * mean;

    // Determine the width of the stretching region

    std::vector<float> AHG_front(AHG.begin(), AHG.begin() + luminance.cols * 0.1);
    std::vector<float> AHG_back(AHG.end() - luminance.cols * 0.1, AHG.end());

    int SW_front = 0;

    for (auto item : AHG_front)
    {
        if (item < threshold)
        {
            SW_front++;
        }
    }

    int SW_back = 0;

    for (auto item : AHG_back)
    {
        if (item < mean)
        {
            SW_back++;
        }
    }

    int SW = SW_front + SW_back;

    // Stretching rate

    std::vector<float> AVG_front_stre(AVG.begin(), AVG.begin() + SW_front);
    std::vector<float> AVG_front_adjc(AVG.begin() + SW_front, AVG.begin() + SW_front * 2);
    std::vector<float> AVG_back_stre(AVG.end() - SW_back, AVG.end());
    std::vector<float> AVG_back_adjc(AVG.end() - SW_back * 2, AVG.end() - SW_back);

    float sum_stre = (float)std::accumulate(std::begin(AVG_front_stre), std::end(AVG_front_stre), 0.0);
    sum_stre += (float)std::accumulate(std::begin(AVG_back_stre), std::end(AVG_back_stre), 0.0);
    float mean_stre = sum_stre / (AVG_front_stre.size() + AVG_back_stre.size());

    float sum_adjc = (float)std::accumulate(std::begin(AVG_front_adjc), std::end(AVG_front_adjc), 0.0);
    sum_adjc += (float)std::accumulate(std::begin(AVG_back_adjc), std::end(AVG_back_adjc), 0.0);
    float mean_adjc = sum_adjc / (AVG_front_adjc.size() + AVG_back_adjc.size());

    float SR = (mean_adjc - mean_stre) / mean_adjc;

    float S_index = (log10(SW + 1) + 1) * (SR + 1);

    return S_index;
}
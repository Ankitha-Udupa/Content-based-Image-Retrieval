//
//  matchingMethods.hpp
//  Project2
//
//  Created by Ankitha Udupa on 3/15/22.
//

#ifndef matchingMethods_hpp
#define matchingMethods_hpp

#include <stdio.h>
#include "opencv2/opencv.hpp"

std::vector<float> baselineFeatures(cv::Mat img);

std::vector<float> histogramFeatures(cv::Mat img);

std::vector<float> histogram3DFeatures(cv::Mat img);

std::vector<float> textureFeatures(cv::Mat img);

std::vector<float> histogramFeaturesHSV(cv::Mat img);

#endif /* matchingMethods_hpp */

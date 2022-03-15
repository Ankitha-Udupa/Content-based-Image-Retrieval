//
//  distanceMetric.hpp
//  Project2
//
//  Created by Ankitha Udupa on 2/17/22.
//

#ifndef distanceMetric_hpp
#define distanceMetric_hpp

#include <stdio.h>
#include <vector>
#include "opencv2/opencv.hpp"

float ssd(std::vector<float> &src, std::vector<float> &dst);

float intersection(std::vector<float> &hist1, std::vector<float> &hist2);

#endif /* distanceMetric_hpp */

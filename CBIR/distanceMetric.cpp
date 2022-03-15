//
//  distanceMetric.cpp
//  Project2
//
//  Created by Ankitha Udupa on 2/17/22.
//

#include "distanceMetric.hpp"
#include "opencv2/opencv.hpp"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <dirent.h>
#include <math.h>

/*
 Calculates the sum squared distance for two images
 returns the distance.
 */
float ssd(std::vector<float> &src, std::vector<float> &dst){
    float total=0.0;
    
    for(int i=0; i<src.size();i++){
        total+= pow(src[i]-dst[i],2);
    }
    
    total=total/243;
    
    return total;
}

/*
 Calculates the histogram intersection for two images
 returns the distance.
 */
float intersection(std::vector<float> &hist1, std::vector<float> &hist2)
{
    float sum = 0;
    float distance=0;
    
    for (int i = 0; i < hist1.size(); i++)
    {
        sum += (hist1[i] < hist2[i]) ? hist1[i] : hist2[i]; //similarity
    }
        
    distance= 1-sum;
                
    return distance;
}


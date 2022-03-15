//
//  matchingMethods.cpp
//  Project2
//
//  Created by Ankitha Udupa on 3/15/22.
//

#include "matchingMethods.hpp"
#include "opencv2/opencv.hpp"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <dirent.h>
#include <math.h>

/*
  Computes baseline matching
 */
std::vector<float> baselineFeatures(cv::Mat img){
    // Crop image
    cv::Mat cropped_image = img(cv::Range(img.size().height/2-4,img.size().height/2+5), cv::Range(img.size().width/2-4,img.size().width/2+5));

    //Convert Mat to vector type
    std::vector<float> array;
    array.assign(cropped_image.begin<float>(), cropped_image.end<float>());
    
    return array;
}

/*
 Normalises 2d histogram
 */
void normalizeHist(cv::Mat &hist, cv::Mat &histNorm, int num_Of_Bins, int num_of_Pixels)
{
    for (int i = 0; i < num_Of_Bins; i++)
    {
        for(int j=0; j< num_Of_Bins; j++){
            histNorm.at<float>(i,j)= ((float)hist.at<int>(i,j))/(float)num_of_Pixels;
        }
    }
}

/*
 Normaliszes 3D histogram
 */
void normalizeHist3(cv::Mat &hist, cv::Mat &histNorm, int num_Of_Bins, int num_of_Pixels)
{
    for (int i = 0; i < num_Of_Bins; i++)
    {
        for(int j=0; j< num_Of_Bins; j++)
        {
            for(int k=0; k<num_Of_Bins;k++)
            {
            histNorm.at<float>(i,j,k)=((float)hist.at<int>(i,j,k))/(float)num_of_Pixels;
            }
        }
    }
}

/*
 Creates 2d r-g histogram for baseline matching
 */
std::vector<float> histogramFeatures(cv::Mat img){
    cv::Mat hist2d;
    const int Hsize=16;
    int dim[2]={Hsize,Hsize};
    
    //creates a 2D histogram 16x16 consisting of signed int type
    //all initialised to zero
    hist2d = cv::Mat::zeros(2,dim, CV_32S);
    
    for(int i=0; i<img.rows;i++){
        for(int j=0;j<img.cols;j++){
            cv::Vec3b color = img.at<cv::Vec3b>(i,j); //bgr
            //calculating chromaticity
            float r = ((float)color[2])/(color[0]+color[1]+color[2]+1);
            float g = ((float)color[1])/(color[0]+color[1]+color[2]+1);
            
            int rix = Hsize * r;
            int gix = Hsize * g;
        
            hist2d.at<int>(rix,gix)+=1;
            
        }
    }
    
    //normalize histogram
    cv::Mat hist2d_norm;
    hist2d_norm = cv::Mat::zeros(2,dim, CV_32F);
    normalizeHist(hist2d, hist2d_norm, Hsize, img.rows*img.cols);
    
    //Convert Mat to vector type
    std::vector<float> array;
    //mat_to_vector(hist2d_norm, array);
    
    array.assign(hist2d_norm.begin<float>(), hist2d_norm.end<float>());
    
    return array;
}

/*
 converts BGR to HSV image.
 */
cv::Mat rgb_hsv(cv::Mat &img){
    
    cv::Mat hsv(img.size(),CV_32F);

    for(int i=0; i<img.rows;i++){
        for(int j=0; j<img.cols;j++){
            cv::Vec3b color = img.at<cv::Vec3b>(i,j); //BGR
            
            float h=-1;
            float s=-1;
            float v=-1;
            
            float b=((float)color[0])/255;
            float g=((float)color[1])/255;
            float r=((float)color[2])/255;
            
            double cmax = std::max(r, std::max(g, b)); // max of r,g,b
            double cmin = std::min (r, std::min(g, b)); //min of r,g,b
            float diff = cmax - cmin;
            
            if (cmax == cmin)
                h = 0;
            
            else if (cmax == r)
                h = fmod((60 * ((g - b) / diff) + 360),360);
            
            else if (cmax == g)
                h =fmod((60 * ((b - r) / diff) + 120),360);
            
            // if cmax equal b then compute h
            else if (cmax == b)
                h = fmod((60 * ((r - g) / diff) + 240),360);
            
            // if cmax equal zero
            if (cmax == 0)
                s = 0;
            else
                s = (diff / cmax) * 100;
            
            v = cmax * 100;
            
            //Assigns values to HSV mat
            hsv.at<cv::Vec3b>(i,j)[0]=h;
            hsv.at<cv::Vec3b>(i,j)[1]=s;
            hsv.at<cv::Vec3b>(i,j)[2]=v;
        }
    }
    return hsv;
}

/*
 Creates 3d r-g histogram for HSV image matching
 */
std::vector<float> histogramFeaturesHSV(cv::Mat img){
    cv::Mat hist3d;
    const int Hsize=32;
    int dim[3]={Hsize,Hsize,Hsize};
    
    //creates a 2D histogram 16x16 consisting of signed int type
    //all initialised to zero
    hist3d = cv::Mat::zeros(3,dim, CV_32S);
    
    cv::Mat newImage = rgb_hsv(img);
        
    for(int i=0; i<newImage.rows;i++){
        for(int j=0;j<newImage.cols;j++){
            cv::Vec3b color = newImage.at<cv::Vec3b>(i,j); //bgr
            //calculating chromaticity
            float h = ((float)color[0])/(180);
            float s = ((float)color[1])/(256);
            float v = ((float)color[2])/(256);
                        
            int hix = Hsize * h;
            int six = Hsize * s;
            int vix = Hsize * v;
        
            hist3d.at<int>(hix,six,vix)+=1;
            
        }
    }
    
    
    //normalize histogram
    cv::Mat hist3d_norm;
    hist3d_norm = cv::Mat::zeros(3,dim, CV_32F);
    normalizeHist3(hist3d, hist3d_norm, Hsize, newImage.rows*newImage.cols);
    
    //Convert Mat to vector type
    std::vector<float> array;
    
    array.assign(hist3d_norm.begin<float>(), hist3d_norm.end<float>());

    return array;
}


/*
 Computes vertical edges
 */
int sobelX3x3( cv::Mat &src, cv::Mat &dst){
    
    cv::Vec3s result={0,0,0};
    
    for(int i=1; i<src.rows-1; i++){
        for(int j=0; j<src.cols; j++){
            for(int c=0; c<3; c++){
                result[c] = src.at<cv::Vec3b>(i-1, j)[c]*1+
                src.at<cv::Vec3b>(i, j)[c]*2+
                src.at<cv::Vec3b>(i+1, j)[c]*1;
                
                result[c]/=4;
                dst.at<cv::Vec3s>(i, j)[c]=result[c];
            }
        }
    }
    
    cv::Mat inter(src.size(),CV_16SC3);
    
    for(int i=0; i<src.rows; i++){
        for(int j=1; j<src.cols-1; j++){
            for(int c=0; c<3; c++){
                inter.at<cv::Vec3s>(i,j)[c] = dst.at<cv::Vec3s>(i, j-1)[c]*-1
                +dst.at<cv::Vec3s>(i, j)[c]*0+
                dst.at<cv::Vec3s>(i, j+1)[c]*1;
            }
        }
    }
    
    dst=inter;
    
    return 0;
}


/*
 Computes horizontal edges
 */
int sobelY3x3( cv::Mat &src, cv::Mat &dst ){
        
    cv::Vec3s result={0,0,0};
    
    for(int i=1; i<src.rows-1; i++){
        for(int j=0; j<src.cols; j++){
            for(int c=0; c<3; c++){
                result[c] = src.at<cv::Vec3b>(i-1, j)[c]*-1+
                src.at<cv::Vec3b>(i, j)[c]*0+
                src.at<cv::Vec3b>(i+1, j)[c]*1;
                
                dst.at<cv::Vec3s>(i, j)[c]=result[c];
            }
        }
    }
    
    cv::Mat inter(src.size(),CV_16SC3);
    
    for(int i=0; i<src.rows; i++){
        for(int j=1; j<src.cols-1; j++){
            for(int c=0; c<3; c++){
                inter.at<cv::Vec3s>(i,j)[c] = dst.at<cv::Vec3s>(i, j-1)[c]*1/4
                +dst.at<cv::Vec3s>(i, j)[c]*2/4+
                dst.at<cv::Vec3s>(i, j+1)[c]*1/4;
                
            }
        }
    }
    
    dst=inter;
    
    
    return 0;
}

/*
 Computes magnitude from sobelX and sobelY
 */
int magnitude( cv::Mat &sx, cv::Mat &sy, cv::Mat &dst ){
    

    cv::Mat mag(sx.size(),CV_16SC3);;
    
    for(int i=0; i<sx.rows;i++){
        for(int j=0; j<sx.cols;j++){
            for(int c=0; c<3;c++){
                mag.at<cv::Vec3s>(i, j)[c] = sqrt(pow(sx.at<cv::Vec3s>(i,j)[c] ,2)+pow(sy.at<cv::Vec3s>(i,j)[c] ,2));
            }
        }
    }
    
    cv::convertScaleAbs(mag,dst);
    
    return 0;
}


/*
 Computes 2D histogram to detect texture using gradient magnitude.
 */
std::vector<float> textureFeatures(cv::Mat img){
    cv::Mat hist2d;
    const int Hsize=16;
    int dim[2]={Hsize,Hsize};
    
    //creates a 2D histogram 16x16 consisting of signed int type
    //all initialised to zero
    hist2d = cv::Mat::zeros(2,dim, CV_32S);
    
    cv::Mat sobelx(img.size(),CV_16SC3);
    sobelX3x3(img, sobelx);
    
    cv::Mat sobely(img.size(),CV_16SC3);
    sobelY3x3(img, sobely);
    
    cv::Mat magImg;
    magnitude( sobelx, sobely, magImg );
    
    for(int i=0; i<magImg.rows;i++){
        for(int j=0;j<magImg.cols;j++){
            cv::Vec3b color = magImg.at<cv::Vec3b>(i,j); //bgr
            //calculating chromaticity
            float r = ((float)color[2])/(color[0]+color[1]+color[2]+1);
            float g = ((float)color[1])/(color[0]+color[1]+color[2]+1);
            
            int rix = Hsize * r;
            int gix = Hsize * g;
        
            hist2d.at<int>(rix,gix)+=1;
            
        }
    }
    
    //normalize histogram
    cv::Mat hist2d_norm;
    hist2d_norm = cv::Mat::zeros(2,dim, CV_32F);
    normalizeHist(hist2d, hist2d_norm, Hsize, magImg.rows*magImg.cols);
    
    //Convert Mat to vector type
    std::vector<float> array;
    
    array.assign(hist2d_norm.begin<float>(), hist2d_norm.end<float>());
    
    return array;
}

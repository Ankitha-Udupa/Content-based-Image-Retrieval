//
//  main.cpp
//  Project2
//
//  Created by Ankitha Udupa on 2/17/22.
//

#include "main.hpp"

#include "opencv2/opencv.hpp"
#include "distanceMetric.hpp"
#include "csv_util.hpp"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <dirent.h>
#include <vector>
#include "matchingMethods.hpp"

using namespace std;

/**
 Computes Content Based Image Matching
 **/

int main(int argc, char *argv[]) {
    //Command Line Inputs
    const char *TARGET_IMG = argv[1];
    const char *DIRECTORY_PATH = argv[2];
    const char *FEATURE_TYPE=argv[3];
    const char *MATCHING_METHOD=argv[4];
    const int num_Of_Images=atoi(argv[5]);
    const char *CSV_PATH=argv[6];

    //Get the target image path
    char TARGET_IMAGE_PATH[100];
    std::strcpy(TARGET_IMAGE_PATH,DIRECTORY_PATH);
    std::strcat(TARGET_IMAGE_PATH, TARGET_IMG);

    //read target image
    cv::Mat img= cv::imread(TARGET_IMAGE_PATH);

    //*******Common*******//
    char filename[256]="";
    std::vector<char *> filenames;
    std::vector<std::vector<float>> data;
    vector<float> finalDistance;

    /**TASK 1 **/
    if(0==strcmp(FEATURE_TYPE,"baseline")){
        
        //get target features
        std::vector<float> targetFeatures=baselineFeatures(img);
        
        //Get the csv file for database image features
        strcat(filename, CSV_PATH);

        //read image features from the csv file
        read_image_data_csv(filename, filenames, data, 0 );

        //Compute distances for each image in database with target image
        if(0==strcmp(MATCHING_METHOD,"ssd")){
            for(int i=0;i<data.size();i++){
                float dist=ssd(targetFeatures, data[i]);
                finalDistance.insert(finalDistance.end(),dist);
            }
        }
    }

    /**TASK 2 **/
    if(0==strcmp(FEATURE_TYPE,"histogram")){
        //compute target image features
        std::vector<float> targetFeatures=histogramFeatures(img);
    
        //Get the csv file for database image features
        strcat(filename, CSV_PATH);


        //read image features from the csv file
        read_image_data_csv(filename, filenames, data, 0 );
        
        //Compute distances for each image in database with target image
        if(0==strcmp(MATCHING_METHOD,"intersection")){
            for(int i=0;i<data.size();i++){
                float dist=intersection(targetFeatures, data[i]);
                finalDistance.insert(finalDistance.end(),dist);
            }
        }
    }
    
    /**TASK 3**/
    if(0==strcmp(FEATURE_TYPE,"multihistogram")){
        
        //Cropping top half of target image
        cv::Mat targetImgTop = img(cv::Range(0,img.size().height/2), cv::Range(0,img.size().width));
        
        //Cropping bottom half of target image
        cv::Mat targetImgBot = img(cv::Range(img.size().height/2,img.size().height), cv::Range(0, img.size().width));

        //Features for top half of target image
        std::vector<float> targetFeatTop=histogramFeatures(targetImgTop);
        
        //Features for bottom half of target image
        std::vector<float> targetFeatBot=histogramFeatures(targetImgBot);
    
        //Get the csv file for database image features
        strcat(filename, CSV_PATH);
        
        //reading database image features
        read_image_data_csv(filename, filenames, data, 0 );

        //Compute distances for each image in database with target image
        if(0==strcmp(MATCHING_METHOD,"intersection")){
            for(int i=0;i<data.size();i++){
                float distTop=intersection(targetFeatTop, data[i]);
                float distBot=intersection(targetFeatBot, data[i]);
                float dist=0.25*distBot+0.75*distTop;
                finalDistance.insert(finalDistance.end(),dist);
            }
        }
    }

    //**TASK 4**//
    if(0==strcmp(FEATURE_TYPE,"texture")){

        //Features for rgb val of target image
        std::vector<float> targetFeatColor=histogramFeatures(img);
        
        //Features for texture of target image
        std::vector<float> targetFeatTexture=textureFeatures(img);
    
        //Get the csv file for database image features
        char filenameTex[256]="";
        std::vector<char *> filenamesTex;
        std::vector<std::vector<float>> dataTex;
        
        //Read color features
        strcat(filename, CSV_PATH);
        read_image_data_csv(filename, filenames, data, 0 );

        //Read texture features
        strcat(filenameTex, CSV_PATH);
        read_image_data_csv(filenameTex, filenamesTex, dataTex, 0 );
    
        
        if(0==strcmp(MATCHING_METHOD,"intersection")){
            for(int i=0;i<data.size();i++){
                float distColor=intersection(targetFeatColor, data[i]);
                float distTex=intersection(targetFeatTexture, dataTex[i]);
                float dist=0.5*distColor+0.5*distTex;
                finalDistance.insert(finalDistance.end(),dist);
            }
        }
    }

    if(0==strcmp(FEATURE_TYPE,"hsv")){

        //Features HSV image
        std::vector<float> targetFeatures=histogramFeaturesHSV(img);

        //Read color features
        strcat(filename, CSV_PATH);
        read_image_data_csv(filename, filenames, data, 0 );

        //Compute distances for each image in database with target image
        if(0==strcmp(MATCHING_METHOD,"intersection")){
            for(int i=0;i<data.size();i++){
                float dist=intersection(targetFeatures, data[i]);
                finalDistance.insert(finalDistance.end(),dist);
            }
        }
    }
    
    if(0==strcmp(FEATURE_TYPE,"extension")){

        //Features HSV image
        //cv::Mat targetCropped = img(cv::Range(img.size().height/4,img.size().height*3/4), cv::Range(img.size().width/4, img.size().width*3/4));
        
        //Features for rgb target image
        std::vector<float> targetFeatColor=histogramFeatures(img);
        
        //Features for texture target image
        std::vector<float> targetFeatTexture=textureFeatures(img);
    
        //Get the csv file for database image features
        char filenameTex[256]="";
        std::vector<char *> filenamesTex;
        std::vector<std::vector<float>> dataTex;
        
        //Read color features
        strcat(filename, CSV_PATH);
        read_image_data_csv(filename, filenames, data, 0 );

        //Read texture features
        strcat(filenameTex, CSV_PATH);
        read_image_data_csv(filenameTex, filenamesTex, dataTex, 0 );
    
        //Compute distances for each image in database with target image
        if(0==strcmp(MATCHING_METHOD,"intersection")){
            for(int i=0;i<data.size();i++){
                float distColor=intersection(targetFeatColor, data[i]);
                float distTex=intersection(targetFeatTexture, dataTex[i]);
                float dist=0.7*distColor+0.3*distTex;
                finalDistance.insert(finalDistance.end(),dist);
            }
        }
    }

    //sort distance metric array and filenames
    for(int i=0; i<data.size()-1; i++){
        for(int j=i+1; j<data.size();j++){
            if(finalDistance[j]<finalDistance[i]){
                std::swap(finalDistance[i], finalDistance[j]);
                std::swap(filenames[i], filenames[j]);
            }
        }
    }

    //print the top n matching images
    for(int i=0; i<num_Of_Images+1; i++){
        std::cout<<filenames[i]<<" file \n";
        
    }

    return 0;
}


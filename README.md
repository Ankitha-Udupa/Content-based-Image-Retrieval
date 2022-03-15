# Content-based-Image-Retrieval

## About
The purpose of this project is to learn how to manipulate and analyze images at a pixel level to do image matching, or pattern recognition. I have explored baseline matching, histogram-based matching, multi-histogram-based matching, color and texture-based matching, HSV based matching and with sum squared distance and histogram intersection distance metrics. 

## List of features:

The software could be used to:
1. Match images by using rgb values from 9x9 centre
2. Match images by using 2D chromaticity histograms and histogram intersection.
3. Match images by using 2D chromaticity histograms and texture histograms along with histogram intersection.
4. Match images by using 2D chromaticity histograms with sectioned target image.
5. Match images by using HSV histograms and histogram intersection.

## How To Run:
   
   Run the main.cpp file with the following command argumets.
   1. target filename for T e.g pic1.jpg
   2. A directory of images as the database B.
   3. he feature type: baseline, histogram, multihistogram, texture, hsv, extension.
   4. matching method: ssd, intersection.
   5. number of images N to return. 
   6. Path to csv feature file.
      
## Operating system and IDE
   I have used MacOS operating system and Xcode IDE to implement the project.

## Baseline Matching
In this task we first compute the feature vector from a 9x9 square in the centre of the image. The feature vector of the target image is then compared to every other image in the database using sum squared distance metric. This task simply involves comparing the pixel values of two images in order to check for similarity between the image. 

**Target Image**

![pic 1016](https://user-images.githubusercontent.com/44782426/158473440-153a55be-cba7-4e96-b82e-b0be722b022d.jpg)

**Top Two Matches**

![pic 0986](https://user-images.githubusercontent.com/44782426/158473513-ce1bd9ce-09ec-4ed8-8ab4-deccf0aecdaa.jpg)

![pic 0233](https://user-images.githubusercontent.com/44782426/158473541-bbc2b49a-ffbe-4ca5-ad0f-0db674fd03b0.jpg)

## Histogram Matching
We compute a 2D rg histogram as the feature vector. We compute the r-g chromaticity for each pixel and assigned them to 16 bins to create a histogram. We then normalize the histogram. Histogram intersection is used to calculate the similarity between the two histograms. The distance metric is 1-similarity achieved. This task shows us how leveraging the spatially invariant attribute of histograms leads to good results for image matching.

**Target Image**

![pic 0164](https://user-images.githubusercontent.com/44782426/158473852-50c74417-b0eb-4c39-b472-e7dcf808ad26.jpg)

**Top Two Matches**

![pic 0080](https://user-images.githubusercontent.com/44782426/158473933-a4be5a75-0ff3-495e-837f-de4e1c5a6e7b.jpg)

![pic 1032](https://user-images.githubusercontent.com/44782426/158473956-ecf331e2-b147-4aad-b6fa-128e8563213b.jpg)

## Texture & Color
We compute a 2, 2D rg histogram as the feature vector. First 2D histogram is a rg chromaticity histogram and the second histogram is a texture histogram. 
Both the texture and r-g chromaticity histograms 16 bins. The texture histogram is created by running the image through sobelX and sobelY and calculating the magnitude. This gives us an idea about the horizontal and vertical edges in the image as the texture. Two distances are created for each histogram type using histogram intersection distance metric. The two distances are then weighted equally ( 50% each) to compute the best matching image. This task shows us how we can boost image matching by using texture in the image as an additional feature to color.

**Target Image**

![pic 0535](https://user-images.githubusercontent.com/44782426/158474186-1e622f38-faff-4e56-a427-6ffa2bc87166.jpg)

**Top Two Matches**

![pic 0004](https://user-images.githubusercontent.com/44782426/158474233-81238964-a8f8-427d-8334-db61da500731.jpg)

![pic 0011](https://user-images.githubusercontent.com/44782426/158474258-241c8034-7bc7-41d6-912a-095e1174b0d6.jpg)

# Custom Design
I wanted to explore the color spectrum and its effects on feature matching. I have written a method to convert each pixel at RGB channels to HSV values and used these vvalues instead of the BGR values to compute a 3d histogram.
This 3D histogram consists of 32 bins. I wanted to explore HSV values instead of BGR values as they are more sensitive to light as compared to BGR. This is why HSV is often used for image matching.

**Target Image**

![pic 0747](https://user-images.githubusercontent.com/44782426/158474488-741f726b-a849-41e1-ba7e-0f03649d7cdb.jpg)

**Top Two Matches**

![pic 0723](https://user-images.githubusercontent.com/44782426/158474607-341b80ae-7864-4428-86b9-105395891ae5.jpg)

![pic 0746](https://user-images.githubusercontent.com/44782426/158474609-5e592089-2412-404d-bef6-0d6167feadd1.jpg)

**Target Image**

![pic 0013](https://user-images.githubusercontent.com/44782426/158474676-e000443b-1cb8-4332-b194-9c4a27402ac3.jpg)


**Top Two Matches**

![pic 0014](https://user-images.githubusercontent.com/44782426/158474755-1587d6de-2fad-40ad-8256-c56e60b494f8.jpg)

![pic 0017](https://user-images.githubusercontent.com/44782426/158474756-6b4fa2fb-3cc0-4297-a0e2-828417225ea8.jpg)

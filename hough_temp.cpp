
//
//  live webcam.cpp
#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <stdio.h>
#include <iostream>
#include <memory>


std::shared_ptr<cv::Mat> pFrameIn{new cv::Mat};
std::shared_ptr<cv::Mat> pFrameGray{new cv::Mat};
std::shared_ptr<cv::Mat> pCanny{new cv::Mat};
std::shared_ptr<cv::Mat> pBlur{new cv::Mat};
std::shared_ptr<cv::Mat> pOut{new cv::Mat};
std::shared_ptr<cv::Mat> pBilat{new cv::Mat};

int sliderGauss;
const int sliderGauss_Max = 10;

std::vector<cv::Vec4i> lines;

void gaussTrackbar(int, void*)
{
     ///Apply Gaussian Blur
     cv::GaussianBlur(*pFrameGray, *pBlur, cv::Size(sliderGauss,sliderGauss),0,0);
     cv::imshow("Gaussian Blur", *pBlur);
}



int main(void){

    cv::VideoCapture stream(0);
    sliderGauss = 1;

    if (!stream.isOpened()){
    std::cout << "Cannot Open Camera" << std::endl;
    }


    while(1){
        stream.read(*pFrameIn);

	cv::namedWindow("Filter Sliders", 1);

	//Convert from RGB to Gray
        cvtColor(*pFrameIn, *pFrameGray, cv::COLOR_RGB2GRAY);

        char TrackbarName[50];
        sprintf(TrackbarName, "sliderGauss x %d", sliderGauss_Max);

	cv::createTrackbar(TrackbarName, "Kernel Size", &sliderGauss, sliderGauss_Max, gaussTrackbar);

        gaussTrackbar(sliderGauss, 0);
        


       

        // Applying Bilateral filter on the Image
        //cv::bilateralFilter(*pFrameGray, *pBilat, 9, 80, 80);

        /// Canny detector
        //cv::Canny(*pBilat, *pCanny, 50,50*3, 3);

        //Hough Transform
        //cv::HoughLinesP(*pCanny, lines, 1, CV_PI/180, 80,30,10);

        //for( size_t i = 0; i < lines.size(); i++ )
        //{
        //    cv::Vec4i l = lines[i];
        //    cv::line( *pOut, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0,0,255), 3, CV_AA);
        //}


        //cv::imshow("Canny Camera", *pCanny);
        //cv::imshow("Hough Lines", *pOut);

        if(cv::waitKey(30) >= 0)
            break;
    }
    return 0;
}

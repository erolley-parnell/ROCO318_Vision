
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

std::vector<cv::Vec4i> lines;


int main(void){

    cv::VideoCapture stream(0);

    if (!stream.isOpened()){
    std::cout << "Cannot Open Camera" << std::endl;
    }


    while(1){
        stream.read(*pFrameIn);

        //Convert from RGB to Gray
        cvtColor(*pFrameIn, *pFrameGray, cv::COLOR_RGB2GRAY);


        ///Apply Gaussian Blur
        cv::GaussianBlur(*pFrameGray, *pBlur, cv::Size(5,5),0,0);

        /// Canny detector
        cv::Canny(*pBlur, *pCanny, 50,50*3, 3);

        //Hough Transform
        cv::HoughLinesP(*pCanny, lines, 1, CV_PI/180, 80,30,10);

        for( size_t i = 0; i < lines.size(); i++ )
        {
            cv::Vec4i l = lines[i];
            cv::line( *pOut, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0,0,255), 3, CV_AA);
        }


        cv::imshow("Canny Camera", *pCanny);
        cv::imshow("Hough Lines", *pOut);

        if(cv::waitKey(30) >= 0)
            break;
    }
    return 0;
}

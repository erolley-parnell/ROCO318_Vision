
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
std::shared_ptr<cv::Mat> pDetectedEdges{new cv::Mat};


int main(void){

    cv::VideoCapture stream(0);

    if (!stream.isOpened()){
    std::cout << "Cannot Open Camera" << std::endl;
    }


    while(1){
        stream.read(*pFrameIn);
        cvtColor(*pFrameIn, *pFrameGray, cv::COLOR_RGB2GRAY);

        /// Canny detector
        cv::Canny(*pFrameGray, *pDetectedEdges, 10, 350);


        cv::imshow("Canny Camera", *pDetectedEdges);
        if(cv::waitKey(30) >= 0)
            break;
    }
    return 0;
}

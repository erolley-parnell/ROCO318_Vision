//
//  live webcam.cpp
#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <stdio.h>
#include <iostream>

int main(void){

    cv::VideoCapture stream(0);

    if (!stream.isOpened()){
    std::cout << "Cannot Open Camera" << std::endl;
    }


    while(1){
        cv::Mat Frame;
        stream.read(Frame);
        cv::imshow("Camera", Frame);
        if(cv::waitKey(30) >= 0)
            break;
    }
    return 0;
}

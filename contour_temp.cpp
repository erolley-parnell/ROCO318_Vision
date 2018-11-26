#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
using namespace cv;

const String window_capture_name = "Source RGB Video";
const String window_detection_name = "Object Detection";
const String window_gauss_name = "Gaussian Blur";
const String window_canny_name = "Canny Edge Detection";
const String window_contour_name = "Contour Detection";

//-------HSV Variables --------//
const int max_value_H = 360/2;
const int max_value = 255;
int low_H = 160, low_S = 0, low_V = 160;
int high_H = max_value_H, high_S = max_value, high_V = max_value;


//-------Gaussian Blur variables ------//
int gauss_blur = 9, max_gauss = 9;

// -------Canny Edge Detection variables ---- //
int canny_low = 65, canny_edge_thresh = 1, canny_ratio = 3, canny_kernel_size = 3;
int const canny_max_low_threshold = 100;

// --------Contour detection variables -----//
int contour_points = 4;
int const contour_max_points = 20;
std::vector <std::vector<Point> > contours;
std::vector <Vec4i> hierarchy;
RNG rng(12345);


// ------- Trackbars -------//
static void on_low_H_thresh_trackbar(int, void *)
{
    low_H = min(high_H-1, low_H);
    setTrackbarPos("Low H", window_detection_name, low_H);
}
static void on_high_H_thresh_trackbar(int, void *)
{
    high_H = max(high_H, low_H+1);
    setTrackbarPos("High H", window_detection_name, high_H);
}
static void on_low_S_thresh_trackbar(int, void *)
{
    low_S = min(high_S-1, low_S);
    setTrackbarPos("Low S", window_detection_name, low_S);
}
static void on_high_S_thresh_trackbar(int, void *)
{
    high_S = max(high_S, low_S+1);
    setTrackbarPos("High S", window_detection_name, high_S);
}
static void on_low_V_thresh_trackbar(int, void *)
{
    low_V = min(high_V-1, low_V);
    setTrackbarPos("Low V", window_detection_name, low_V);
}
static void on_high_V_thresh_trackbar(int, void *)
{
    high_V = max(high_V, low_V+1);
    setTrackbarPos("High V", window_detection_name, high_V);
}
static void gauss_blur_trackbar(int, void*)
{
     if (gauss_blur % 2 == 0)
     {
       gauss_blur = gauss_blur + 1;
     }
     setTrackbarPos("Blur", window_gauss_name, gauss_blur);
}
static void canny_trackbar(int, void*)
{
    setTrackbarPos("Canny: Minimum Threshold", window_canny_name, canny_low);
}
static void contour_points_trackbar(int, void*)
{
    setTrackbarPos("Points", window_contour_name, contour_points);
}

std::vector<Point> contoursConvexHull( std::vector<std::vector<Point> > contours )
{
    std::vector<Point> result;
    std::vector<Point> pts;
    for ( size_t i = 0; i< contours.size(); i++)
        for ( size_t j = 0; j< contours[i].size(); j++)
            pts.push_back(contours[i][j]);
    convexHull( pts, result );
    return result;
}


int main(int argc, char* argv[])
{
    VideoCapture cap(argc > 1 ? atoi(argv[1]) : 0);
    namedWindow(window_capture_name);
    namedWindow(window_detection_name);
    namedWindow(window_gauss_name);
    namedWindow(window_canny_name);
    namedWindow(window_contour_name);

    // Trackbars to set thresholds for HSV values
    createTrackbar("Low H", window_detection_name, &low_H, max_value_H, on_low_H_thresh_trackbar);
    createTrackbar("High H", window_detection_name, &high_H, max_value_H, on_high_H_thresh_trackbar);
    createTrackbar("Low S", window_detection_name, &low_S, max_value, on_low_S_thresh_trackbar);
    createTrackbar("High S", window_detection_name, &high_S, max_value, on_high_S_thresh_trackbar);
    createTrackbar("Low V", window_detection_name, &low_V, max_value, on_low_V_thresh_trackbar);
    createTrackbar("High V", window_detection_name, &high_V, max_value, on_high_V_thresh_trackbar);
    createTrackbar("Blur", window_gauss_name, &gauss_blur, max_gauss, gauss_blur_trackbar);
    createTrackbar("Canny: Minimum Threshold", window_canny_name, &canny_low, canny_max_low_threshold, canny_trackbar);
    createTrackbar("Points", window_contour_name, &contour_points, contour_max_points, contour_points_trackbar);

    Mat frame, frame_HSV, frame_threshold, frame_blur, frame_edges, frame_contours;
    while (true) {
        cap >> frame;
        if(frame.empty())
        {
            break;
        }
        //Apply a Gaussian Blur to the frame
        GaussianBlur(frame, frame_blur, Size(gauss_blur,gauss_blur),0,0);
        // Convert from BGR to HSV colorspace
        cvtColor(frame_blur, frame_HSV, COLOR_BGR2HSV);
        // Detect the object based on HSV Range Values
        inRange(frame_HSV, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), frame_threshold);

        //Erode and then dilate (close)

        // Applying Canny edge Detect on the Image
        Canny(frame_threshold, frame_edges, canny_low, canny_low*canny_ratio, canny_kernel_size);

        findContours(frame_edges, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

        Mat drawing = Mat::zeros(frame_edges.size(), CV_8UC3);

        for (int i = 0; i< contours.size(); i++)
        {
            Scalar color = Scalar( 255,255,255);
            drawContours( drawing, contours, i, color, 1 );
        }

        std::vector <Point> ConvexHullPoints =  contoursConvexHull(contours);

        polylines( drawing, ConvexHullPoints, true, Scalar(0,0,255), 2 );
        imshow("Contours", drawing);
        // Show the frames
        imshow(window_capture_name, frame);
        imshow(window_gauss_name, frame_blur);
        imshow(window_detection_name, frame_threshold);
        imshow(window_canny_name, frame_edges);
        char key = (char) waitKey(30);
        if (key == 'q' || key == 27)
        {
            break;
        }
    }
    return 0;
}

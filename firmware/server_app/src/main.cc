/**
 * @file
 * 
 * @brief This is the main entry point to the ARC Server application.
 * 
 * @author Eddie Sasagawa
 * @date November 4, 2018
 * 
 */

#include <stdio.h>

#include <opencv2/opencv.hpp>
#include <zmq.hpp>

// using namespace cv;

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("usage: DEPRECATED, DO NOT USE ... DisplayImage.out <Image_Path>\n");
        return -1;
    }

    cv::Mat image;
    image = cv::imread(argv[1], 1);

    if (!image.data) {
        printf("No image data \n");
        return -1;
    }

    cv::namedWindow("Display Image", cv::WINDOW_NORMAL);
    cv::imshow("Display Image", image);

    cv::waitKey(0);
    return 0;
}
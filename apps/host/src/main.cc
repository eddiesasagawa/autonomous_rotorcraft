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
#include <iostream>
#include <string>

#include <ncurses.h>
#include <opencv2/opencv.hpp>
#include <zmq.hpp>

#include "host2arc_interface.hh"
#include "user_interface.hh"

// using namespace cv;

int main(int argc, char** argv) {
    if (argc != 4) {
        printf("usage: arc_host <MY_IP_ADDR> <MY_PORT> <ARC_IP_ADDR> <ARC_PORT>\n");
        return -1;
    }

    /* ARC Connection */
    arc::host::Host2ArcInterface& h2ai = arc::host::Host2ArcInterface::GetInstance();
    h2ai.Connect(argv[1], argv[2], argv[3], argv[4]);

    /* Open CV stuff */
    cv::Mat image;

    /* User interface */
    arc::host::UserInterface user_if;
    user_if.AddCallback('w', h2ai.CommandMoreThrust);

    bool quit_now = false;

    /** MAIN LOOP **/
    while (!quit_now) {
        user_if.ProcessInput(&quit_now);
    }

    // image = cv::imread(argv[1], 1);

    // if (!image.data) {
    //     printf("No image data \n");
    //     return -1;
    // }

    // cv::namedWindow("Display Image", cv::WINDOW_NORMAL);
    // cv::imshow("Display Image", image);

    // cv::waitKey(0);
    return 0;
}
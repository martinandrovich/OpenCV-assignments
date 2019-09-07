#include <iostream>
#include <string>

#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

enum class rotdir
{
    cw,
    ccw
};

enum class flipdir
{
    flipx,
    flipy
};

void rotate(cv::Mat& img_target, rotdir dir)
{

    // create a copy of target image with same type
    // and post-rotation dimensions
    cv::Mat img_rotated = cv::Mat(img_target.cols, img_target.rows, img_target.type());

    // iterate each pixel of target image and move into new
    // transpose algorithm
    for (auto x = 0; x < img_target.rows; x++)
    {
        for (auto y = 0; y < img_target.cols; y++)
        {
            if (dir == rotdir::cw)
            {
                // rotate clock-wise
                img_rotated.at<cv::Vec3b>(cv::Point2i(img_rotated.rows - 1 - y,x)) = img_target.at<cv::Vec3b>(cv::Point2i(x,y));

            }
            
            if (dir == rotdir::ccw)
            {
                // rotate counter clock-wise
                img_rotated.at<cv::Vec3b>(cv::Point2i(x, img_rotated.rows - 1 - y)) = img_target.at<cv::Vec3b>(cv::Point2i(y, x));

            }
            
            // flip along x-axis
            //img_rotated.at<cv::Vec3b>(cv::Point2i(x, img_rotated.rows - 1 - y)) = img_target.at<cv::Vec3b>(cv::Point2i(x,y));

            // flip along y-axis
            //img_rotated.at<cv::Vec3b>(cv::Point2i(img_rotated.rows - 1 - y,x)) = img_target.at<cv::Vec3b>(cv::Point2i(y,x));
        }
    }

    img_target = img_rotated;
}

void callback_color_picker(int e, int x, int y, int s, void* data)
{
    if (e == 1) // left mouse down
    {
        cv::Vec3b p = ((cv::Mat*)data)->at<cv::Vec3b>(y, x);
        std::cout << int(p[0]) << " " << int(p[1]) << " " << int(p[2]) << std::endl;
    }
}

void color_picker(std::string filename, cv::ColorConversionCodes cs = cv::COLOR_BGR2HLS)
{

    // https://answers.opencv.org/question/112807/opencv-color-ranges-for-hsv/

    static cv::Mat img_pick;
    
    // load image
    img_pick = cv::imread(filename);

    // check if image has been loaded
    if (img_pick.empty())
    {
        std::cerr << "Could not load the specified image.";
        return;
    }

    // bind color picker callback
    cv::namedWindow("Color Picker");
    cv::setMouseCallback("Color Picker", &callback_color_picker, &img_pick);

    // convert to desired color space
    cv::cvtColor(img_pick, img_pick, cs);
    
    // display image
    cv::imshow("Color Picker", img_pick);
    cv::waitKey();
}

cv::Mat segment_image(cv::Mat img)
{
    // create blank image (white) for the segmented image
    cv::Mat img_seg = cv::Mat(img.rows, img.cols, img.type(), cv::Scalar(255, 255, 255));

    // convert target image from BGR to HSL
    cv::cvtColor(img, img_seg, cv::COLOR_BGR2HLS);

    // show original image
    cv::namedWindow("Original Image");
    cv::imshow("Original Image", img);
    cv::waitKey();

    // segment the red color
    ;

    // show segmented image
    cv::namedWindow("Segmented Image");
    cv::imshow("Segmented Image", img_seg);
    cv::waitKey();
}

void astronaut_rotate_demo(rotdir dir)
{
    // load image
    cv::Mat img = cv::imread("assets/Astronaut1.jpg");

    // check if image has been loaded
    if (img.empty())
    {
        std::cerr << "Could not load the specified image.";
        return;
    }

    // display original image
    cv::namedWindow("Original Image");
    cv::imshow("Original Image", img);

    // rotate CCW and display image
    rotate(img, dir);
    cv::namedWindow("Rotated Image");
    cv::imshow("Rotated Image", img);

    // wait for input
    cv::waitKey();

    // close all windows
    cv::destroyAllWindows();
}

void babyfood_segment_demo()
{
    // load image
    cv::Mat img = cv::imread("assets/BabyFood/BabyFood-Sample1.JPG");

    // check if image has been loaded
    if (img.empty())
    {
        std::cerr << "Could not load the specified image.";
        return;
    }

    segment_image(img);
}

int main()
{
    // log information
    std::cout << "Hello world!\n";

    // rotation demo (CCW)
    astronaut_rotate_demo(rotdir::ccw);

    // color picker demo
    color_picker("assets/BabyFood/BabyFood-Sample1.JPG", cv::COLOR_BGR2HLS);
    cv::destroyAllWindows();

    // reg segmentation demo
    babyfood_segment_demo();

    return 0;
}
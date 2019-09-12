#include <algorithm>
#include <iostream>
#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"

// structs
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

// forward declarations
void rotate(cv::Mat &img_target, rotdir dir);
void flip(cv::Mat &img_target, flipdir dir);
void callback_color_picker(int e, int x, int y, int s, void *data);
void color_picker(std::string filename, cv::ColorConversionCodes cs = cv::COLOR_BGR2HLS);
cv::Mat segment_image(cv::Mat &img);
void histogram(std::string filename);

void astronaut_rotate_demo(rotdir dir);
void babyfood_segment_demo();

void rotate(cv::Mat &img_target, rotdir dir)
{
	// create a copy of target image with same type
	// and post-rotation dimensions
	cv::Mat img_rotated =
		cv::Mat(img_target.cols, img_target.rows, img_target.type());

	// iterate each pixel of target image and move into new
	// transpose algorithm
	for (auto x = 0; x < img_target.rows; x++)
	{
		for (auto y = 0; y < img_target.cols; y++)
		{
			if (dir == rotdir::cw)
			{
				// rotate clock-wise
				img_rotated.at<cv::Vec3b>(cv::Point2i(img_rotated.rows - 1 - y, x)) =
					img_target.at<cv::Vec3b>(cv::Point2i(x, y));
			}

			if (dir == rotdir::ccw)
			{
				// rotate counter clock-wise
				img_rotated.at<cv::Vec3b>(cv::Point2i(x, img_rotated.rows - 1 - y)) =
					img_target.at<cv::Vec3b>(cv::Point2i(y, x));
			}
		}
	}

	img_target = img_rotated;
}

void flip(cv::Mat &img_target, flipdir dir)
{
	// create a copy of target image with same type
	cv::Mat img_rotated = img_target.clone();

	// iterate each pixel of target image and move into new
	// flip algorithm
	for (auto x = 0; x < img_target.rows; x++)
	{
		for (auto y = 0; y < img_target.cols; y++)
		{
			if (dir == flipdir::flipx)
			{
				// flip along x-axis
				img_rotated.at<cv::Vec3b>(cv::Point2i(x, img_rotated.rows - 1 - y)) =
					img_target.at<cv::Vec3b>(cv::Point2i(x, y));
			}

			if (dir == flipdir::flipy)
			{
				// flip along y-axis
				img_rotated.at<cv::Vec3b>(cv::Point2i(img_rotated.rows - 1 - y, x)) =
					img_target.at<cv::Vec3b>(cv::Point2i(y, x));
			}
		}
	}

	img_target = img_rotated;
}

void callback_color_picker(int e, int x, int y, int s, void *data)
{
	if (e == 1) // left mouse down
	{
		cv::Vec3b p = ((cv::Mat *)data)->at<cv::Vec3b>(y, x);
		auto H = int(p[0]);
		auto S = int(p[1]);
		auto L = int(p[2]);
		auto LS = (float)L / S;

		std::cout << H << " " << S << " " << L << " | L/S: " << LS << "\n";
	}
}

void color_picker(std::string filename, cv::ColorConversionCodes cs)
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

void histogram(std::string filename)
{
	// load image
	cv::Mat img = cv::imread(filename);

	// check if image has been loaded
	if (img.empty())
	{
		std::cerr << "Could not load the specified image.";
		return;
	}

	// split image into channels
	std::vector<cv::Mat> img_channels;
	cv::split(img, img_channels);

	// histogram settings
	int hist_size = 256;			   // number of bins
	float range[] = {0, 256};		   // range
	const float *hist_range = {range}; // -
	bool uniform = true;			   // -
	bool accumulate = false;		   // -

	cv::Mat b_hist, g_hist, r_hist;

	// compute histogram
	cv::calcHist(&img_channels[0], 1, 0, cv::Mat(), b_hist, 1, &hist_size,
				 &hist_range, uniform, accumulate);
	cv::calcHist(&img_channels[1], 1, 0, cv::Mat(), g_hist, 1, &hist_size,
				 &hist_range, uniform, accumulate);
	cv::calcHist(&img_channels[2], 1, 0, cv::Mat(), r_hist, 1, &hist_size,
				 &hist_range, uniform, accumulate);

	// draw histogiram
	int hist_w = 512;
	int hist_h = 400;
	int bin_w = cvRound((double)hist_w / hist_size);

	cv::Mat img_hist(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));

	/// Normalize the result to [ 0, img_hist.rows ]
	normalize(b_hist, b_hist, 0, img_hist.rows, cv::NORM_MINMAX, -1, cv::Mat());
	normalize(g_hist, g_hist, 0, img_hist.rows, cv::NORM_MINMAX, -1, cv::Mat());
	normalize(r_hist, r_hist, 0, img_hist.rows, cv::NORM_MINMAX, -1, cv::Mat());

	/// Draw for each channel
	for (int i = 1; i < hist_size; i++)
	{
		cv::line(
			img_hist,
			cv::Point(bin_w * (i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
			cv::Point(bin_w * (i), hist_h - cvRound(b_hist.at<float>(i))),
			cv::Scalar(255, 0, 0), 2, 8, 0);

		cv::line(
			img_hist,
			cv::Point(bin_w * (i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
			cv::Point(bin_w * (i), hist_h - cvRound(g_hist.at<float>(i))),
			cv::Scalar(0, 255, 0), 2, 8, 0);

		cv::line(
			img_hist,
			cv::Point(bin_w * (i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
			cv::Point(bin_w * (i), hist_h - cvRound(r_hist.at<float>(i))),
			cv::Scalar(0, 0, 255), 2, 8, 0);
	}

	// display histogram
	cv::namedWindow("Histogram", CV_WINDOW_AUTOSIZE);
	cv::imshow("Histogram", img_hist);
	cv::waitKey();
}

cv::Mat segment_image(cv::Mat &img)
{
	// create blank image (white) for the segmented image
	cv::Mat img_hsl;
	cv::Mat img_seg = cv::Mat(img.rows, img.cols, img.type(), cv::Scalar(255, 255, 255));

	// convert target image from BGR to HSL
	cv::cvtColor(img, img_hsl, cv::COLOR_BGR2HLS);

	// show original image
	cv::namedWindow("Original Image");
	cv::moveWindow("Original Image", 20, 20);
	cv::imshow("Original Image", img);
	cv::waitKey();

	// variables
	uchar H, S, L;
	float LS;
	bool has_spoon = false;
	int red_count = 0;

	// segment the spoon by color descriptor (boolean)
	for (auto x = 0; x < img_hsl.rows; x++)
	{
		for (auto y = 0; y < img_hsl.cols; y++)
		{
			H 	= img_hsl.at<cv::Vec3b>(cv::Point(y, x))[0];
			S 	= img_hsl.at<cv::Vec3b>(cv::Point(y, x))[1];
			L 	= img_hsl.at<cv::Vec3b>(cv::Point(y, x))[2];
			LS 	= (float)L/S;

			if ((L >= 64) && (S >= 55) && (LS > 1.5f && LS < 5.0f) && (H >= 172 || H <= 4))
			{
				img_seg.at<cv::Vec3b>(cv::Point(y, x)) = img_hsl.at<cv::Vec3b>(cv::Point(y, x));
				red_count++;
			}
		}
	}

	// log number of matching pixels
	std::cout << "Found " << red_count << " matching pixels.\n";

	// check ranges
	if (red_count > 4079 && red_count < 11952)
	{
		std::cout << "Found a single spoon!\n";
		has_spoon = true;
	}
	else
	{
		std::cout << "Mistake!\n";
		has_spoon = false;
	}

	// print text on original image
	cv::putText(
		img,
		(has_spoon) ? "status: correct" : "status: incorrect",
		cv::Point(20, 40),
		cv::FONT_HERSHEY_DUPLEX,
		0.80,
		CV_RGB(255, 255, 255),
		1
	);

	cv::imshow("Original Image", img);

	// show segmented image
	cv::namedWindow("Segmented Image");
	cv::moveWindow("Segmented Image", 20 + img.cols + 20, 20);
	cv::imshow("Segmented Image", img_seg);

	// bind color picker callback
	cv::namedWindow("Segmented Image");
	cv::setMouseCallback("Segmented Image", &callback_color_picker, &img_seg);

	// wait for user input
	cv::waitKey();

	return img_seg;
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
	cv::Mat img;

	// iterate sample images
	for (int i = 0; i < 3; i++)
	{
		auto filename = "assets/BabyFood/BabyFood-Sample" + std::to_string(i) + ".JPG";
		img = cv::imread(filename);
		segment_image(img);
		cv::destroyAllWindows();
	}

	// iterate test images
	for (int i = 1; i < 19; i++)
	{
		auto filename = "assets/BabyFood/BabyFood-Test" + std::to_string(i) + ".JPG";
		img = cv::imread(filename);
		segment_image(img);
		cv::destroyAllWindows();
	}

}

int main()
{
	// log information
	std::cout << "RCA-VIS asgmt-01\n";

	// rotation demo (CCW)
	//astronaut_rotate_demo(rotdir::ccw);

	// histogram demo
	histogram("assets/BabyFood/BabyFood-Sample1.JPG");
	cv::destroyAllWindows();

	// color picker demo
	color_picker("assets/BabyFood/BabyFood-Sample2.JPG", cv::COLOR_BGR2HLS);
	cv::destroyAllWindows();

	// red spoon segmentation demo
	babyfood_segment_demo();
	cv::destroyAllWindows();

	return 0;
}
#include "hist.h"

namespace plt = matplotlibcpp;

namespace hist
{
	enum class channel
	{
		GRAY				= 0,
		BGR_B, RGB_R, HLS_H	= 0,
		BGR_G, RGB_G, HLS_L	= 1,
		BGR_R, RGB_B, HLS_S	= 2,
	};

	std::vector<int>
	get_hist_vec_1c(const cv::Mat& img, channel ch);

	cv::Mat
	get_3dhist(const cv::Mat& img);

	void
	plot_1c_histogram(const cv::Mat& img);

	void
	plot_3c_histogram(const cv::Mat& img, cv::ColorConversionCodes cs, bool norm);
}

/* -- private method definitions: ---------------------------------------------------------------------------- */

std::vector<int>
hist::get_hist_vec_1c(const cv::Mat& img, channel ch)
{
	constexpr auto max_intensity = 256;

	// check that img has the requested channel
	assert((img.channels() - 1) >= (unsigned)ch);

	// initialize histogram to value 0
	std::vector<int> hist_vec(max_intensity, 0);

	// iterate image and compute hist_vec
	for (int row = 0; row < img.rows; row++)
	{
		for (int col = 0; col < img.cols; col++)
		{
			// get pixel value (intesnity) at desired channel
			uchar val;
			if (img.channels() > 1)
				val = img.at<cv::Vec3b>(row, col)[(unsigned)ch];
			else
				val = img.at<uchar>(row, col);

			// incrment count of found intensity
			hist_vec[val]++;
		}
	}

	// return vector
	return std::move(hist_vec);
}

cv::Mat
hist::get_3dhist(const cv::Mat& img)
{
	cv::Mat hist_3d;
	
	// setup computation
	// int channel_numbers[] = { 0, 1, 2 };
	// int* number_bins = new int[img.channels()];

	// for (uint ch = 0; ch < img.channels(); ch++)
	// 	number_bins[ch] = 4;

	// float ch_range[] = {0.f, 255.f};
	// const float* channel_ranges[] = { ch_range, ch_range, ch_range };

	float ch_range[] = { 0, 256 };
	const float* etendu[] = { ch_range, ch_range,ch_range };
	int hist_bins = 32;
	int hist_size[] = { hist_bins, hist_bins , hist_bins  };
	int channels[] = { 2, 1, 0 };

	// compute histogram
	cv::calcHist(&img, 1, channels, cv::Mat(), hist_3d, 3, hist_size, etendu, true, false);

	//cv::calcHist(&img, 1, channel_numbers, cv::Mat(), hist_3d, img.channels(), number_bins, channel_ranges);

	return hist_3d;
}

void
hist::plot_1c_histogram(const cv::Mat& img)
{
	constexpr auto max_intensity = 256;

	// convert to grayscale
	cv::Mat img_gray;

	if (img.channels() != 1)
		cv::cvtColor(img, img_gray, cv::COLOR_BGR2GRAY);
	else
		img_gray = img.clone();

	assert(img_gray.channels() == 1);

	// initialize histogram to value 0
	std::vector<int> hist_gray(max_intensity, 0);

	// iterate image and compute hist_gray
	for (int row = 0; row < img_gray.rows; row++)
	{
		for (int col = 0; col < img_gray.cols; col++)
		{
			// get pixel value (intesnity)
			auto val = img_gray.at<uchar>(row, col);

			// incrment count of found intensity
			hist_gray[val]++;
		}
	}

	// plot
	plt::plot(hist_gray);
	plt::title("1D Histogram");
	plt::legend();
	plt::show();
}

void
hist::plot_3c_histogram(const cv::Mat& img, cv::ColorConversionCodes cs, bool norm)
{
	constexpr auto max_intensity = 256;
	constexpr auto norm_val = 100;

	// image must have 3 channels
	assert(img.channels() == 3);

	// convert to desired color space
	cv::Mat img_hist;
	cv::cvtColor(img, img_hist, cs);

	// initialize histograms to value 0
	std::vector<int> hist_c1(max_intensity, 0);
	std::vector<int> hist_c2(max_intensity, 0);
	std::vector<int> hist_c3(max_intensity, 0);

	// iterate image and compute hist_gray
	for (int row = 0; row < img_hist.rows; row++)
	{
		for (int col = 0; col < img_hist.cols; col++)
		{
			// get pixel value (intesnity)
			auto val = img_hist.at<cv::Vec3b>(row, col);

			// incrment count of found intensity
			hist_c1[val[0]]++;
			hist_c2[val[1]]++;
			hist_c3[val[2]]++;
		}
	}

	// normalize data
	if (norm)
	{
		normalize(hist_c1, hist_c1, 0, norm_val, cv::NORM_MINMAX, -1, cv::Mat());
		normalize(hist_c2, hist_c2, 0, norm_val, cv::NORM_MINMAX, -1, cv::Mat());
		normalize(hist_c3, hist_c3, 0, norm_val, cv::NORM_MINMAX, -1, cv::Mat());
	}

	// plot labels
	struct plot_label
	{
		std::string name;
		std::string format;
	} c1, c2, c3;

	// select appropriate labels and format for plot
	switch (cs)
	{
		case cv::ColorConversionCodes::COLOR_BGR2HLS:
		case cv::ColorConversionCodes::COLOR_BGR2HLS_FULL:

			c1 = {"Hue", "r"};
			c2 = {"Lum", "#adadad"};
			c3 = {"Sat", "k"};

			break;
	
		default:
			
			c1 = {"Red", "r"};
			c2 = {"Green", "g"};
			c3 = {"Blue", "b"};

			break;
	}

	// plot using matplotlib
	plt::named_plot(c1.name, hist_c1, c1.format);
	plt::named_plot(c2.name, hist_c2, c2.format);
	plt::named_plot(c3.name, hist_c3, c3.format);
	plt::title("3D Histogram");
	plt::legend();
	plt::show();
}

/* -- public method definitions: ----------------------------------------------------------------------------- */

void
hist::plot_histogram(const cv::Mat& img, hist::type hist_type, cv::ColorConversionCodes cs, bool normalize)
{
	// plot histogram according to the passed type
	if (hist_type == type::hist1C) { hist::plot_1c_histogram(img); return; }
	if (hist_type == type::hist3C) { hist::plot_3c_histogram(img, cs, normalize); return; }
}

void
hist::smooth_1d(const cv::Mat& img)
{
	// image must be 1D
	assert(img.channels() == 1);

	// perform local average

}

void
hist::equalize(cv::Mat& img)
{
	constexpr auto max_intensity = 256;

	// select appropriate target channel
	channel trgt_ch = channel::HLS_L;

	// convert to HSL if img is not grayscale
	if (img.channels() > 1)
		cv::cvtColor(img, img, cv::COLOR_BGR2HLS);
	else
		trgt_ch = channel::GRAY;

	// get luminance histogram
	auto hist_vec = hist::get_hist_vec_1c(img, trgt_ch);

	// create LUT
	std::vector<int> lut_vec(hist_vec.size(), 0);

	// equalization algorithm
	int tot_pixels = img.rows * img.cols;
	int num_pixels = 0;
	int out_gray = 0;

	for (int in_gray = 0; in_gray < max_intensity; in_gray++)
	{
		num_pixels = num_pixels + hist_vec[in_gray];

		auto out_gray_new = (num_pixels * max_intensity) / (float)(tot_pixels + 1);
		lut_vec[in_gray] = ((out_gray + out_gray_new + 1) / 2.f);
		
		out_gray = out_gray_new;
	}

	// aplly LUT

	for (uint row = 0; row < img.rows; row++)
	{
		for (uint col = 0; col < img.cols; col++)
		{	
			if (trgt_ch == channel::GRAY)
			{
				auto gray_val = img.at<uchar>(row, col);
				img.at<uchar>(row, col) = lut_vec[gray_val];
			}
			else
			{
				auto ch_idx = (unsigned)trgt_ch;
				auto lum_val = img.at<cv::Vec3b>(row, col)[ch_idx];
				img.at<cv::Vec3b>(row, col)[ch_idx] = lut_vec[lum_val];
			}
		}
	}

	// convert back if not grayscale
	if (img.channels() > 1)
		cv::cvtColor(img, img, cv::COLOR_HLS2BGR);
	
}

void
hist::compare(const cv::Mat& img1, const cv::Mat& img2, int method)
{
	// ensure color image
	assert(img1.channels() == 3 && img2.channels() == 3);

	// calculate 3D histograms
	auto hist1 = hist::get_3dhist(img1);
	auto hist2 = hist::get_3dhist(img2);

	// normalize histograms
	normalize(hist1, hist1, 100.0/(img1.total()), 0, cv::NORM_MINMAX, -1, cv::Mat());

	// calculate matching score
	auto matching_score = cv::compareHist(hist1, hist2, method);

	// print score
	std::cout << matching_score << std::endl;
}
#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <random>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <opencv2/plot.hpp>

#define WITHOUT_NUMPY
#pragma GCC diagnostic ignored "-Wregister"
#include "../lib/matplotlibcpp.h"

namespace hist
{
	enum class type
	{
		hist1C,
		hist3C
	};

	void
	plot_histogram(const cv::Mat& img, hist::type hist_type, cv::ColorConversionCodes cs = cv::COLOR_BGR2RGB, bool normalize = true);

	void
	smooth_1d(const cv::Mat& img);

	void
	equalize(cv::Mat& img);

	void
	compare(const cv::Mat& img1, const cv::Mat& img2, int method = CV_COMP_CORREL);

	template <class ... Args> void
	compare(const cv::Mat& img_org, Args ... imgs)
	{
		const auto list_imgs = {imgs...};
		int counter = 1;

		std::cout << "performing multiple matches:" << std::endl;

		for (auto& img : list_imgs)
		{
			std::cout << " img " << counter++ << ": ";
			hist::compare(img_org, img);
		}
	}

	void
	back_proj(const cv::Mat& img_sample, const cv::Mat& img, int bin_size = 8);

	template <typename T> float
	k_means_dist(uchar val1, uchar val2)
	{
		return sqrt(pow(abs(val2 - val1), 2));
	}

	template <typename T> float
	k_means_dist(cv::Vec3b val1, cv::Vec3b val2)
	{
		return sqrt( pow(abs(val2[0] - val1[1]), 2) +
						pow(abs(val2[1] - val1[1]), 2) +
						pow(abs(val2[2] - val1[2]), 2));
	}

	template <typename T> void
	k_means_clust(const cv::Mat& img, uint k, uint maxiter = 30)
	{

		// template restriction: must be either Vec3b or uchar
		static_assert(std::is_same<T, cv::Vec3b>::value || std::is_same<T, uchar>::value,
			"Wrong type; use cv::Vec3b or uchar.");

		// assert that call with correct type pairs
		assert((std::is_same<T, uchar>::value && img.channels() == 1) ||
			(std::is_same<T, cv::Vec3b>::value && img.channels() == 3));

		// data structures
		struct point_t; struct cluster_t;

		struct point_t
		{
			T val;
			cluster_t* cluster = nullptr;
		};

		struct cluster_t
		{
			cluster_t(T init_point) : mean(init_point) {}
			T mean;
			std::vector<point_t> pts_vec;
		};

		// lambdas

		auto recalc_mean = [](cluster_t& c) {
			
			int sum_x = 0, sum_y = 0, sum_z = 0;
			float mean_x, mean_y, mean_z;

			// calculate sum(s)
			for (auto& p : c.pts_vec)
			{
				if (std::is_same<T, cv::Vec3b>::value)
				{
					sum_x += (int)(*(cv::Vec3b*)(&p.val))[0];
					sum_y += (int)(*(cv::Vec3b*)(&p.val))[1];
					sum_z += (int)(*(cv::Vec3b*)(&p.val))[2];
				}
				else
				{
					sum_x += (int)(*((uchar*)(&p.val)));
				}
			}

			// calculate mean(s)
			mean_x = (float)sum_x / c.pts_vec.size();
			mean_y = (float)sum_y / c.pts_vec.size();
			mean_z = (float)sum_z / c.pts_vec.size();

			// apply new mean(s)
			if (std::is_same<T, cv::Vec3b>::value)
			{
				(*(cv::Vec3b*)(&c.mean))[0] = (int)mean_x;
				(*(cv::Vec3b*)(&c.mean))[1] = (int)mean_y;
				(*(cv::Vec3b*)(&c.mean))[2] = (int)mean_z;
			}
			else
			{
				c.mean = (int)mean_x;
			}
		};

		// random number generator (uniform)
		std::random_device rd;  // used to get seed for rng engine
    	std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
		std::uniform_int_distribution<> rng_rows(0, img.rows);
		std::uniform_int_distribution<> rng_cols(0, img.cols);

		// initialize vec of clusters with random data points from img
		std::vector<cluster_t> cluster_vec;

		for (uint i = 0; i < k; i++)
			cluster_vec.emplace_back(img.at<T>(rng_rows(gen), rng_cols(gen)));

		// initalize matrix of points from img
		auto points = std::vector<std::vector<point_t>>(img.rows, std::vector<point_t>(img.cols));

		for (int row = 0, col = 0; row < img.rows; row += (col = (col + 1) % img.cols) ? 0 : 1)
			points[row][col].val = img.at<T>(row, col);

		int num_asgmts = 0, iter = 0;

		// k-means algorhithm

		do
		{	
			// reset number of assignments
			num_asgmts = 0;

			// clear points in clusters
			for (auto& c : cluster_vec)
				c.pts_vec.clear();

			// iterate pixels of image (use points matrix)
			for (int row = 0, col = 0; row < img.rows; row += (col = (col + 1) % img.cols) ? 0 : 1)
			{
				auto&      pt              = points[row][col];
				cluster_t* nearest_cluster = nullptr;
				float      shortest_dist   = -1;

				// find nearest cluster
				for (auto& c : cluster_vec)
				{				
					if (auto dist = hist::k_means_dist<T>(pt.val, c.mean); shortest_dist > dist || shortest_dist == -1)
					{
						nearest_cluster = &c;
						shortest_dist = dist;
					}
				}

				// assign point to nearest cluster if not allready assigned to it
				if (pt.cluster == nullptr || pt.cluster != nearest_cluster)
					{ pt.cluster = nearest_cluster; num_asgmts++; }

				// add point to nearest cluster
				nearest_cluster->pts_vec.push_back(pt);
			}

			// recalculate means of clusters
			for (auto& c : cluster_vec)
				recalc_mean(c);

			std::cout << "[" << ++iter << "]: " << num_asgmts << std::endl;

		} while (num_asgmts);

		// convert matrix of points to img
		auto img_kmean = cv::Mat_<T>(img.rows, img.cols);

		for (int row = 0, col = 0; row < img.rows; row += (col = (col + 1) % img.cols) ? 0 : 1)
			img_kmean[row][col] = points[row][col].cluster->mean;

		// show output
		cv::imshow("img k-mean", img_kmean);
		cv::waitKey();
	}
}

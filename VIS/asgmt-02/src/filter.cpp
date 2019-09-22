#include "filter.h"

namespace filter
{
	uint64_t
	pow2sum_pixels(const cv::Mat& img);

	float
	dispersion(const cv::Mat& mask, float mask_avg);

	std::vector<float>
	make_gauss_vec(float mu, float sigma, int G = 256);
}

/* -- private method definitions: ---------------------------------------------------------------------------- */

uint64_t
filter::pow2sum_pixels(const cv::Mat& img)
{
	float sum = 0.f;

	for (auto row = 0; row < img.rows; row++)
	{
		for (auto col = 0; col < img.cols; col++)
		{
			auto v = img.at<cv::Vec3b>(row, col);
			sum += pow(v[0], 2) + pow(v[1], 2) + pow(v[2], 2);
		}
	}

	return (uint64_t)sum;
}

float
filter::dispersion(const cv::Mat& mask, float mask_avg)
{
	// dispersion is the average of the squared difference
	// of each point in the mask minus the mask_avg

	// number of pixels
	float num_pixels = mask.rows * mask.cols;
	float sq_diff_sum, dispersion;

	// sum of squared difference
	for (auto row = 0; row < mask.rows; row++)
	{
		for (auto col = 0; col < mask.cols; col++)
		{	
			// sum of channels at current pixel
			auto sum_ch = cv::sum(mask.at<cv::Vec3i>(row, col))[0];

			// compute squared difference of current pixel and mask average
			sq_diff_sum += pow(sum_ch - mask_avg, 2);
		}
	}

	// average the summed square diff
	dispersion = (1.f / num_pixels) * sq_diff_sum;

	return dispersion;
}

std::vector<float>
filter::make_gauss_vec(float mu, float sigma, int G)
{
	constexpr auto PI = 3.1415;

	auto min_k = -(G - 1);
	auto max_k =  (G - 1);

	// p(k)
	auto gauss = [&](int k){
		return exp(-pow(k,2) / (2 * pow(sigma,2))) / (sigma * sqrt(2 * PI));
	};

	// accumulated p values
	std::vector<float> p_cum;

	// generate vector for range [-max_k ; max_k] = [-G - 1 ; G + 1]
	// p_cum(k) = p_cum(k - 1) + p(k)
	// p_cum(-G - 1) = p(-G - 1)
	for (int i = 0, k = min_k; k < max_k; i++, k++)
	{
		auto val = (k == min_k) ? gauss(min_k) : p_cum[i - 1] + gauss(k);
		p_cum.push_back(val);
	}

	return std::move(p_cum);
}

/* -- public method definitions: ----------------------------------------------------------------------------- */

void
filter::playground()
{
	// matrix creation
	cv::Mat ones = cv::Mat::ones(8, 8, CV_8U);

	// 3D matrix creation
	cv::Mat mat_3d = cv::Mat(cv::Size(3, 3), CV_8UC3, cv::Scalar(1, 1, 1));

	// scalar multiplications
	ones = ones * 2;

	// mask
	// format: col, row, width, height
	auto mask = ones(cv::Rect(5,1,3,2));

	// submatrix test
	std::cout << "is mask sub of ones : " << std::boolalpha << mask.isSubmatrix() << std::endl;

	// edit area
	ones(cv::Rect(2, 2, 4, 4)) = 0;

	// print
	std::cout << "\n\nones:\n\n" << ones << std::endl;
	std::cout << "\n\nmask:\n\n" << mask << std::endl;

	// parallel traverse
	mask.forEach<uchar>([](auto& pxl, auto pos) -> void {
		auto row = pos[0]; auto col = pos[1];
		std::cout << "col: " << col << " | row: " << row << " | val: " << (int)pxl << std::endl;
	});

	// matrices
	auto rect1  = cv::Mat::ones(3,3, CV_8U);
	auto rect2  = cv::Mat::ones(3,3, CV_8U) * 2;

	// dot product
	auto dot    = rect1.dot(rect2);

	// matrix element wise multiplication
	auto rect3  = rect1.mul(rect2);

	std::cout
		<< "\n\nrect1:\n\n" 	<< rect1
		<< "\n\nrect2:\n\n" 	<< rect2
		<< "\n\ndot:\n\n" 		<< dot
		<< "\n\nrect3:\n\n" 	<< rect3
		<< std::endl;

	// for more: operations on arrays
	// https://docs.opencv.org/2.4/modules/core/doc/operations_on_arrays.html
}

float
filter::snr(const cv::Mat& img_org, const cv::Mat& img_noisy)
{	
	//cv::Mat img_noise = img_noisy - img_org;
	cv::Mat img_noise;
	cv::absdiff(img_noisy, img_org, img_noise);

	auto sum_signal	= filter::pow2sum_pixels(img_org);
	auto sum_noise	= filter::pow2sum_pixels(img_noise);
	auto snr 		= sum_signal / (float)sum_noise;

	return (float)snr;
}

float
filter::psnr(const cv::Mat& img1, const cv::Mat& img2)
{
	// https://stackoverflow.com/questions/29428308/snr-of-an-image-in-c-using-opencv

	cv::Mat s1;
	cv::absdiff(img1, img2, s1);					// |img1 - img2|
	s1.convertTo(s1, CV_32F); 						// cannot make a square on 8 bits
	s1 = s1.mul(s1);								// |img1 - img2|^2	
	cv::Scalar s = cv::sum(s1);						// sum elements per channel	
	double sse = s.val[0] + s.val[1] + s.val[2]; 	// sum channels	
	
	if( sse <= 1e-10)								// for small values return zero
		return 0;
	else
	{
		double mse = sse /(double)(img1.channels() * img1.total());
		double psnr = 10.0*log10((255*255)/mse);
		return psnr;
	}
}

void
filter::snr_test(float noise_pct)
{
	cv::Mat img = cv::imread("../assets/lena.png", 0);
	auto img_noise = cv::Mat(img.size(), CV_8U);
	auto sigma = unsigned(255 * (noise_pct / 100.f));

	cv::Scalar mu(0);
	cv::Scalar stddev(sigma);

	randn(img_noise,mu, stddev);

	cv::Mat img_noisy = img + img_noise;
	
	cv::imshow("img", img);
	cv::imshow("noise", img_noise);
	cv::imshow("noisy img", img_noisy);

	// log information
	std::cout
		<< "target PCT: " << noise_pct << "% | "
		<< "target SNR: " << (1 / noise_pct) * 100 << " | "
		<< "result SNR: " 	<< filter::snr(img, img_noisy) << " | "
		<< "result PSNR: "	<< filter::psnr(img, img_noisy)
		<< std::endl;

	cv::waitKey();
	cv::destroyAllWindows();
}

void
filter::sap_noise(cv::Mat& img, float snr)
{
	constexpr auto threshold = 10;

	auto img_noisy = img.clone();
	auto noise_pct = 100.f / snr;
	auto num_noise_pts = (unsigned)((img.rows * img.cols * img.channels()) * (noise_pct/100.f));

	for (int i = 0; i < num_noise_pts; i++)
	{
		// pick a random row, column and channel
		auto row = rand() % img.rows;
		auto col = rand() % img.cols;
		auto ch  = rand() % img.channels();

		// apply to clone using ptr
		auto pxl = img_noisy.ptr<uchar>(row) + (col * img.channels()) + ch;
		
		// if below certain threshold
		// ensure the opposite color
		if (*pxl > (255 - threshold) || *pxl < (0 + threshold))
		{
			*pxl = (*pxl > 127) ? 0 : 255;
		}
		else
		{
			*pxl = (rand() % 2) ? 0 : 255;
		}
	}

	// log information
	std::cout
		<< "target PCT: "	<< noise_pct << "% | "
		<< "target SNR: "	<< snr << " | "
		<< "result SNR: " 	<< filter::snr(img, img_noisy) << " | "
		<< "result PSNR: "	<< filter::psnr(img, img_noisy)
		<< std::endl;

	// apply
	img.release();
	img = std::move(img_noisy);
}

void
filter::gauss_noise(cv::Mat& img, float snr)
{
	constexpr auto G = 256;

	auto noise_pct = 100.f / snr;
	auto sigma = unsigned(255 * (noise_pct / 100.f));

	auto p_cum = filter::make_gauss_vec(0, sigma, G);

	for (auto row = 0; row < img.rows; row++)
	{
		for (auto col = 0; col < img.cols; col++)
		{

			for (auto k = 0; k < 3; k++)
			{

				// generate random float [0;1]
				float rng = (float)rand() / (float)RAND_MAX;

				// argmin_k(rand() - p_cum[k])
				// find the index k of p_cum[k] whose value is closest to the rand() float
				auto argmin = std::lower_bound(p_cum.begin(), p_cum.end(), rng) - p_cum.begin() - G;

				// f*(i,j) = g(i,j) + argmin_k(rand() - p_cum[k])
				int new_val = (int)img.at<cv::Vec3b>(cv::Point(row,col))[k] + argmin;

				// f'(i,j) = truncate the f*(i,j) val
				if (new_val < 0)		new_val = 0;			// f*(i,j) < 0
				if (new_val > G - 1)	new_val = G - 1;		// f*(i,j) > G - 1
										new_val = new_val;		// otherwise

				// apply new value
				img.at<cv::Vec3b>(cv::Point(row,col))[k] = new_val;

			}
		}
	}
	
	return;
}

void
filter::convolve(cv::Mat& img, cv::Mat1i mask, float weight)
{
	// check that kernel (mask) has square, odd-numbered size
	if (mask.cols % 2 != 1 || mask.cols != mask.rows)
	{
		std::cerr << "Mask of size " << mask.size() << "is not an odd-numberer and/or square; please supply such." << std::endl;
		return;
	}

	// store original type
	auto org_type = img.type();

	// determine size
	auto size = mask.cols | mask.rows;

	// clone image
	img.convertTo(img, CV_32SC3);
	cv::Mat img_clone = img.clone();

	// calculate offset
	int offset = (size - 1) / 2;

	// create 3D mask from 1D mask
	// each channel has same mask; simply done for faster multiplciation
	cv::Mat mask_3d;
	std::vector<cv::Mat> masks = { mask, mask, mask };
	cv::merge(masks, mask_3d);	

	// traverse pixels of image in parallel
	img.forEach<cv::Point3i>([&](auto& pxl, const int* pos) -> void {
		
		// defs
		auto row = pos[0];
		auto col = pos[1];

		// bounds checking
		if (row - offset < 0 || row + offset >= img.rows) return;
		if (col - offset < 0 || col + offset >= img.cols) return;

		// create kernel (grid)
		auto kernel = img(cv::Rect(col - offset, row - offset, size, size));

		// convolve kernel with mask and multiply with a weight
		// must be done for each channel

		// i.e. traverse kernel (per channel), multiply each pixel with its
		// corresponding position in the mask matrix and sum it up all up

		// convolution: sum of elementwise multiplication of 3D mask * kernel
		// averaged by multiplication of weight
		auto res = cv::sum(kernel.mul(mask_3d)) * weight;

		// apply to cloned image channel-wise
		for (auto k = 0; k < 3; k++)
			img_clone.at<cv::Vec3i>(row, col)[k] = (int)res[k];

	});

	// overwrite image with new and convert back
	img.release();
	img_clone.convertTo(img, org_type);
}

void
filter::box_filter(cv::Mat& img, size_t size)
{
	// generate mask
	auto avg_mask = cv::Mat::ones(size, size, CV_32SC1);

	// compute weight
	float weight = 1 / cv::sum(avg_mask)[0];

	// run convolution with local avg mask
	filter::convolve(img, avg_mask, weight);
}

void
filter::gauss_filter(cv::Mat& img, float sigma)
{

	// generate gauss distribution
	int gauss_data[] =	{ 1, 2, 1, 
						  2, 4, 2,
						  1, 2, 1 };

	// generate gauss mask matrix
	auto gauss_mask = cv::Mat(3, 3, CV_32SC1, gauss_data);

	// compute weight
	float weight = 1 / cv::sum(gauss_mask)[0];

	// run convolution with gauss mask
	filter::convolve(img, gauss_mask, weight);
}

void
filter::rot_mask(cv::Mat& img)
{
	// store original type
	auto org_type = img.type();

	// clone image
	img.convertTo(img, CV_32SC3);
	cv::Mat img_clone = img.clone();

	// uniform mask
	auto  mask = cv::Mat::ones(3, 3, CV_32SC1);

	// create 3D mask from 1D mask
	// each channel has same mask; simply done for faster multiplciation
	cv::Mat mask_3d;
	std::vector<cv::Mat> masks = { mask, mask, mask };
	cv::merge(masks, mask_3d);

	// compute sizes and offset
	// 3x3 kernel -> 5x5 region
	int kernel_size		= 3;
	int region_size		= (kernel_size * 2) - 1;
	int kernel_offset	= (kernel_size - 1) / 2;
	int region_offset	= (region_size - 1) / 2;

	// traverse pixels of image
	for (auto row = 0; row < img.rows; row++)
	{
		for (auto col = 0; col < img.cols; col++)
		{

			// bounds checking for whole region
			if (row - region_offset < 0 || row + region_offset >= img.rows) continue;
			if (col - region_offset < 0 || col + region_offset >= img.cols) continue;

			// define region
			auto region = img(cv::Rect(col - region_offset, row - region_offset, region_size, region_size));

			// iterate 9 possible rotation masks
			// starting with rot mask in bottom-right corner of region

			// map of dispersions and the assotiated local averages
			std::map<float, cv::Vec4d> map_disp;

			// calculate dispersions for each of the 9 possible rotation masks
			for (auto region_row = 0; region_row < kernel_size; region_row++)
			{
				for (auto region_col = 0; region_col < kernel_size; region_col++)
				{
					// select rotation mask from region
					auto rotmask = region(cv::Rect(region_col, region_row, kernel_size, kernel_size));
					float num_pixels = rotmask.rows * rotmask.cols;

					// calculate mask average for each channel and sum it
					auto vec_avg  = cv::sum(rotmask.mul(mask_3d)) * (1.f / num_pixels);
					auto mask_avg = vec_avg[0] + vec_avg[1] + vec_avg[2]; 
					
					// calculate dispersion
					auto disp = filter::dispersion(rotmask, mask_avg);

					// add to map
					map_disp[disp] = vec_avg;	
				}
			}

			// select the best local average with the minimum dispersion
			// will be first element due to ordered map
			auto best_avg = map_disp.begin()->second;

			// apply to cloned image channel-wise
			for (auto k = 0; k < 3; k++)
				img_clone.at<cv::Vec3i>(row, col)[k] = (int)best_avg[k];

		}
	}

	// overwrite image with new and convert back
	img.release();
	img_clone.convertTo(img, org_type);
}

void
filter::median_filter(cv::Mat& img, size_t size)
{
	// store original type
	auto org_type = img.type();

	// clone image
	img.convertTo(img, CV_32SC3);
	cv::Mat img_clone = img.clone();

	// calculate offset
	int offset = (size - 1) / 2;

	// traverse pixels of image
	for (auto row = 0; row < img.rows; row++)
	{
		for (auto col = 0; col < img.cols; col++)
		{

			// bounds checking for whole region
			if (row - offset < 0 || row + offset >= img.rows) continue;
			if (col - offset < 0 || col + offset >= img.cols) continue;

			// define kernel
			cv::Mat kernel = img(cv::Rect(col - offset, row - offset, size, size));

			// split kernel into channels
			std::vector<cv::Mat> kernel_ch;
			cv::split(kernel, kernel_ch);

			// compute median per channel
			for (auto k = 0; k < kernel_ch.size(); k++)
			{
				// convert 1D matix to vector
				std::vector<int> kernel_vec;
				kernel_vec.assign((int*)kernel_ch[k].data, (int*)kernel_ch[k].data + kernel_ch[k].total());

				// sort vector
				std::sort(kernel_vec.begin(), kernel_vec.end());

				auto vecsize = kernel_vec.size();

				// access middle element (median)
				//auto median = kernel_vec[kernel_vec.size() / 2];
				auto median = (vecsize % 2) ? (kernel_vec[vecsize / 2]) : ((kernel_vec[vecsize / 2 - 1] + kernel_vec[vecsize / 2]) / 2) ;

				// apply median to channel
				img_clone.at<cv::Vec3i>(row, col)[k] = median;
			}				
		}
	}

	// overwrite image with new and convert back
	img.release();
	img_clone.convertTo(img, org_type);
}

void
filter::mediangray_filter(cv::Mat& img, size_t size)
{
	// store original type
	auto org_type = img.type();

	// clone image
	img.convertTo(img, CV_32SC3);
	cv::Mat img_clone = img.clone();

	// calculate offset
	int offset = (size - 1) / 2;

	// traverse pixels of image
	for (auto row = 0; row < img.rows; row++)
	{
		for (auto col = 0; col < img.cols; col++)
		{

			// bounds checking for whole region
			if (row - offset < 0 || row + offset >= img.rows) continue;
			if (col - offset < 0 || col + offset >= img.cols) continue;

			// define kernel
			cv::Mat kernel = img(cv::Rect(col - offset, row - offset, size, size));
			cv::Mat kernel_1C;

			// sum up channels into a single matrix
			// transform 3D matrix into 1D matrix where each channel was summed
			cv::transform(kernel, kernel_1C, cv::Matx<int,1, 3>(1,1,1));

			// convert 1D matix to vector
			std::vector<int> kernel_vec;
			kernel_vec.assign((int*)kernel_1C.data, (int*)kernel_1C.data + kernel_1C.total());

			// sort vector
			std::sort(kernel_vec.begin(), kernel_vec.end());

			// access middle element (median)
			auto median = kernel_vec[kernel_vec.size() / 2];

			// average the median for channels
			auto avg_median = median * (1.f / img.channels());

			// apply to cloned image channel-wise
			for (auto k = 0; k < img.channels(); k++)
				img_clone.at<cv::Vec3i>(row, col)[k] = avg_median;

		}
	}

	// overwrite image with new and convert back
	img.release();
	img_clone.convertTo(img, org_type);
}

void
filter::rot_mask2(cv::Mat& img)
{
	// store original type
	auto org_type = img.type();

	// clone image
	img.convertTo(img, CV_32SC3);
	cv::Mat img_clone = img.clone();

	// uniform mask
	auto  mask = cv::Mat::ones(3, 3, CV_32SC1);

	// compute sizes and offset
	// 3x3 kernel -> 5x5 region
	int kernel_size		= 3;
	int region_size		= (kernel_size * 2) - 1;
	int kernel_offset	= (kernel_size - 1) / 2;
	int region_offset	= (region_size - 1) / 2;

	// traverse pixels of image
	for (auto row = 0; row < img.rows; row++)
	{
		for (auto col = 0; col < img.cols; col++)
		{

			// bounds checking for whole region
			if (row - region_offset < 0 || row + region_offset >= img.rows) continue;
			if (col - region_offset < 0 || col + region_offset >= img.cols) continue;

			// define region
			auto region = img(cv::Rect(col - region_offset, row - region_offset, region_size, region_size));

			// iterate 9 possible rotation masks
			// starting with rot mask in bottom-right corner of region

			// map of dispersions and the assotiated local averages
			std::vector<std::map<float, double>> vecmap_disp(region.channels());

			// calculate dispersions for each of the 9 possible rotation masks
			for (auto region_row = 0; region_row < kernel_size; region_row++)
			{
				for (auto region_col = 0; region_col < kernel_size; region_col++)
				{
					// vector for channels
					std::vector<cv::Mat> rotmask_ch;
					
					// select rotation mask from region
					auto rotmask = region(cv::Rect(region_col, region_row, kernel_size, kernel_size));
					float num_pixels = rotmask.rows * rotmask.cols;

					// split rotmask into channels
					cv::split(rotmask, rotmask_ch);

					// calculate dispersion per channel
					for (auto k = 0; k < rotmask_ch.size(); k++)
					{	
						// calculate channel average
						auto mask_avg  = (cv::sum(rotmask_ch[k]) * (1.f / num_pixels))[0];

						// calculate channel dispersion
						auto disp = filter::dispersion(rotmask_ch[k], mask_avg);

						// add to vector of maps
						vecmap_disp[k][disp] = mask_avg;
					}
				}
			}

			// select the best local average with the minimum dispersion per channel
			for (auto k = 0; k < vecmap_disp.size(); k++)
			{
				auto best_avg = vecmap_disp[k].begin()->second;
				img_clone.at<cv::Vec3i>(row, col)[k] = (int)best_avg;
			}
				
		}
	}

	// overwrite image with new and convert back
	img.release();
	img_clone.convertTo(img, org_type);
}
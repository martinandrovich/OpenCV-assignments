void kernel(cv::Mat & img, int size)
{

  cv::Mat kernel = cv::Mat::ones( cv::Size(size,size), CV_32F )/ (float)(size*size);

  cv::Mat copy[3];

  cv::split(img, copy);

  cv::Mat & img_copy = copy[2];

  cv::imshow("red image 3", img_copy);

  cv::Mat res;

  cv::filter2D(img_copy, res, CV_32F, kernel);

  cv::imshow("red_image4", res);

}

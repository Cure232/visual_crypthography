#ifndef IMAGE_SPLITTER_H
#define IMAGE_SPLITTER_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;

Mat generate_random_matrix(const Size& size);
vector<Mat> split_image(const Mat& image, const int& n, const int& k);
Mat restore_image(const vector<Mat>& parts, const int& k);

#endif
#include "main.h"

Mat generate_random_matrix(const Size& size) {
    Mat random_matrix = Mat::zeros(size, CV_8U);
    randu(random_matrix, Scalar(0), Scalar(256));
    return random_matrix;
}

vector<Mat> split_image(const Mat& image, const int& n, const int& k) {
    Mat bw_image; //black and white image
    if (image.channels() > 1) {
        cvtColor(image, bw_image, COLOR_BGR2GRAY);
    }
    else {
        bw_image = image;
    }

    vector<Mat> parts(n);
    for (int i = 0; i < n - 1; i++) {
        //first n-1 parts are random
        parts[i] = generate_random_matrix(bw_image.size());
    }

    Mat last_part = bw_image.clone();
    //last part is xor reflection of other parts
    for (int row = 0; row < bw_image.rows; row++) {
        for (int col = 0; col < bw_image.cols; col++) {
            uchar value = 0;
            for (int p = 0; p < n - 1; p++) {
                value ^= parts[p].at<uchar>(row, col);
            }
            last_part.at<uchar>(row, col) ^= value;
        }
    }
    parts[n-1] = last_part;

    return parts;
}

Mat restore_image(const vector<Mat>& parts, const int& k) {
    Mat restored_image = Mat::zeros(parts[0].size(), CV_8U);
    for (int i = 0; i < k; i++) {
        for (int row = 0; row < parts[i].rows; row++) {
            for (int col = 0; col < parts[i].cols; col++) {
                restored_image.at<uchar>(row, col) ^= parts[i].at<uchar>(row, col);
            }
        }
    }
    return restored_image;
}

int main() {
    Mat image = imread("cat.jpg");
    if (image.empty()) {
        cout << "Couldn`t read image" << endl;
        return -1;
    }

    int n = 3;
    int k = 3;
    vector<Mat> parts = split_image(image, n, k);
    for (int i = 0; i < n; i++) {
        imwrite("part" + to_string(i + 1) + ".jpg", parts[i]);
    }

    Mat restored_image = restore_image(parts, k);
    imwrite("restored_image.jpg", restored_image);

    imshow("original_image", image);
    imshow("restored_image", restored_image);
    waitKey(0);
    return 0;
}

#include <opencv2/opencv.hpp>
#include<opencv2/xfeatures2d/nonfree.hpp>
//#include<opencv2/legacy/legacy.hpp>
#include <opencv2/calib3d.hpp>
#include<vector>
#include<iostream>
#include<sstream>
#include<string>
#include<time.h> 
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/imgproc/types_c.h>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/features2d.hpp>

using namespace std;
using namespace cv;
Mat Stitched(Mat img1, Mat img2) {
	Mat g1(img1, Rect(0, 0, img1.cols, img1.rows));
	Mat g2(img2, Rect(0, 0, img2.cols, img2.rows));
	cvtColor(g1, g1, CV_BGR2GRAY);
	cvtColor(g2, g2, CV_BGR2GRAY);
	int kp_number{ 800 };
	//SiftFeatureDetector siftdet;
	//cv::Ptr<cv::xfeatures2d::SIFT> sift = cv::xfeatures2d::SIFT::create();
	//cv::Ptr<SiftFeatureDetector> siftdtc = SiftFeatureDetector::create(kp_number);
	cv::Ptr<cv::xfeatures2d::SiftFeatureDetector>siftdtc = cv::xfeatures2d::SiftFeatureDetector::create(kp_number);
	vector<KeyPoint>kp1, kp2;
	//SIFT sift;
	//SiftDescriptorExtractor extractor;
	Mat descriptor1, descriptor2;
	FlannBasedMatcher matcher;
	vector<DMatch> matches, goodmatches;
	/*进行特征点提取*/
	siftdtc->detect(g1, kp1);
	siftdtc->detect(g2, kp2);
	/* 进行特征向量提取 */
	//cv::Ptr<SiftDescriptorExtractor> extractor = SiftDescriptorExtractor::create();
	cv::Ptr<cv::xfeatures2d::SiftDescriptorExtractor>extractor = cv::xfeatures2d::SiftDescriptorExtractor::create();
	//sift->detectAndCompute(g1, cv::Mat(),kp1, descriptor1);
	//sift->detectAndCompute(g2, cv::Mat(),kp2, descriptor2);
	extractor->compute(g1,kp1,descriptor1);
	extractor->compute(g2,kp2,descriptor2);
	/* 进行特征向量临近匹配 */
	matcher.match(descriptor1, descriptor2, matches);
	Mat  firstmatches;
	/*画出第一次匹配的结果*/
	drawMatches(img1, kp1, img2, kp2,
		matches, firstmatches, Scalar::all(-1), Scalar::all(-1),
		vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	imshow("first_matches", firstmatches);
	imwrite("first_matches.jpg",firstmatches);
	//waitKey();
	/* 下面计算向量距离的最大值与最小值 */
	double max_dist = 0; double min_dist = 1000;
	for (int i = 0; i < descriptor1.rows; i++) {
		if (matches[i].distance > max_dist) {
			max_dist = matches[i].distance;
		}
		if (matches[i].distance < min_dist) {
			min_dist = matches[i].distance;
		}
	}
	cout << "The max distance is: " << max_dist << endl;
	cout << "The min distance is: " << min_dist << endl;
	for (int i = 0; i < descriptor1.rows; i++) {
		if (matches[i].distance < 2 * min_dist) {
			goodmatches.push_back(matches[i]);
		}
	}
	Mat img_matches;
	/*第二次筛选后的结果*/
	drawMatches(img1, kp1, img2, kp2,
		goodmatches, img_matches, Scalar::all(-1), Scalar::all(-1),
		vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	imshow("good_matches", img_matches);
	imwrite("good_matches.jpg",img_matches);
	//waitKey();
	vector<Point2f> keypoints1, keypoints2;
	for (int i = 0; i < goodmatches.size(); i++) {
		keypoints1.push_back(kp1[goodmatches[i].queryIdx].pt);
		keypoints2.push_back(kp2[goodmatches[i].trainIdx].pt);
	}
	/*计算单应矩阵*/
	Mat H = findHomography(keypoints1, keypoints2, cv::RANSAC);
	Mat stitchedImage;
	int mRows = img2.rows;
	if (img1.rows> img2.rows)
	{
		mRows = img1.rows;
	}
	/*判断图像在左边还是在右边*/
	int propimg1 = 0, propimg2 = 0;
	for (int i = 0; i < goodmatches.size(); i++) {
		if (kp1[goodmatches[i].queryIdx].pt.x > img1.cols / 2) {
			propimg1++;
		}
		if (kp2[goodmatches[i].trainIdx].pt.x > img2.cols / 2) {
			propimg2++;
		}
	}
	bool flag = false;
	Mat imgright;
	Mat imgleft;
	if ((propimg1 / (goodmatches.size() + 0.0)) > (propimg2 / (goodmatches.size() + 0.0))) {
		imgleft = img1.clone();
		flag = true;
	}
	else {
		imgleft = img2.clone();
		flag = false;
	}
	if (flag) {
		imgright = img2.clone();
		flag = false;
	}
	else {
		imgright = img1.clone();
	}
	/*把上边求得的右边的图像经过矩阵H转换到stitchedImage中对应的位置*/
	warpPerspective(imgright, stitchedImage, H, Size(img2.cols + img1.cols, mRows));
	/*把左边的图像放进来*/
	Mat half(stitchedImage, Rect(0, 0, imgleft.cols, imgleft.rows));
	imgleft.copyTo(half);
	return stitchedImage;
}
int main() {
	Mat img1 = imread("1.jpg");
	Mat  stitchedImage;
	int n;
	
	cout << "Dataset2" << endl;
	cout << "请输入想拼接的图片数量（大于1小于18）" << endl;
	cin >> n;
	cout << "输入成功，开始计时" << endl;
	
	clock_t start,finish;  
    	double totaltime;
    	start=clock();  
	resize(img1, img1, Size(img1.cols / 4, img1.rows / 4));
	for (int k = 2; k <= n; k++) {
		stringstream stream;
		string str;
		stream << k;
		stream >> str;
		string filename = str + ".jpg";
		//string filename = str + ".png";
		cout << "正在拼接......." << filename << endl;
		Mat img = imread(filename);
		//Mat img = imread("6.jpg");
		resize(img, img, Size(img.cols / 4, img.rows / 4));
		stitchedImage = Stitched(img1, img);
		img1 = stitchedImage;
	}
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "拼接成功" << endl;
	cout << "拼接花费总时间为：" << totaltime << "秒！" << endl;
	imshow("ResultImage", stitchedImage);
	imwrite("ResultImage.jpg", stitchedImage);
	waitKey();
	return 0;
}

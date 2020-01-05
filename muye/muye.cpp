#include <opencv2/highgui/highgui.hpp>
//#include "opencv2/nonfree/nonfree.hpp"
#include <opencv2/xfeatures2d/nonfree.hpp>
//#include "opencv2/legacy/legacy.hpp" 
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/imgproc/types_c.h>
#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/calib3d.hpp>
 
using namespace cv;
using namespace std;
 
//计算原始图像点位在经过矩阵变换后在目标图像上对应位置
Point2f getTransformPoint(const Point2f originalPoint,const Mat &transformMaxtri);
 
//int main(int argc,char *argv[])
int main() 
{  
	//Mat image01=imread(argv[1]);  
	//Mat image02=imread(argv[2]);
	Mat image1=imread("1.jpg");
	Mat image2=imread("2.jpg");
	//resize(image1, image1, Size(image1.cols / 4, image1.rows / 4));
	//resize(image2, image2, Size(image2.cols / 4, image2.rows / 4));
	cv::resize(image1,image1,cv::Size(1600,1200), (0, 0), (0, 0), cv::INTER_AREA);
        cv::resize(image2,image2,cv::Size(1600,1200), (0, 0), (0, 0), cv::INTER_AREA);

	//imshow("拼接图像1",image01);
	//imshow("拼接图像2",image02);
	//waitKey(5000);
	//return 0;
 
	//灰度图转换
	//Mat image1,image2;  
	//cvtColor(image01,image1,CV_RGB2GRAY);
	//cvtColor(image02,image2,CV_RGB2GRAY);
 	//imshow("1",image1);
	//waitKey(5000);
	//return 0;
	//提取特征点
	//SiftFeatureDetector siftDetector(800);  // 海塞矩阵阈值
	//cv::xfeatures2d::SiftFeatureDetector siftDetector;
	//cout<<"1"<<endl;
	//cv::Ptr<cv::xfeatures2d::SIFT> sift = cv::xfeatures2d::SIFT::create();
	cv::Ptr<cv::xfeatures2d::SiftFeatureDetector>siftdtc = cv::xfeatures2d::SiftFeatureDetector::create(800);
	vector<KeyPoint> keyPoint1,keyPoint2;  
	//siftDetector.detect(image1,keyPoint1);
	//cout<<"2"<<endl;
	//siftDetector.detect(image2,keyPoint2);
	siftdtc->detect(image1,keyPoint1);
	siftdtc->detect(image2,keyPoint2);

		//return 0;
	//特征点描述，为下边的特征点匹配做准备
	//cv::xfeatures2d::SiftDescriptorExtractor siftDescriptor;

	Mat imageDesc1,imageDesc2;
	//cv::xfeatures2d::SiftDescriptorExtractor::compute(image1,keyPoint1,imageDesc1);
	//cv::Feature2D::compute(image1,keyPoint1,imageDesc1);
	cv::Ptr<cv::xfeatures2d::SiftDescriptorExtractor>extractor = cv::xfeatures2d::SiftDescriptorExtractor::create();
	extractor->compute(image1, keyPoint1, imageDesc1);
	extractor->compute(image2, keyPoint2, imageDesc2);
	//siftDescriptor.compute(image1,keyPoint1,imageDesc1);
	//siftDescriptor.compute(image2,keyPoint2,imageDesc2);
	//sift->descriptorSize().computer(image1,keyPoint1,imageDesc1);
	//sift->descriptorSize().computer(image2,keyPoint2,imageDesc2);
	//sift->descriptor_extractor->compute(image1,keypoint1,imageDesc1);
	//sift->descriptor_extractor->compute(image2,keypoint2,imageDesc2); 	
 	//sift->descriptor->compute(image1,keypoint1,imageDesc1);
	//获得匹配特征点，并提取最优配对  	
	cv::Mat keyPointImage1;
	cv::Mat keyPointImage2;
	drawKeypoints(image1, keyPoint1, keyPointImage1, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	drawKeypoints(image2, keyPoint2, keyPointImage2, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	//显示窗口
	cv::namedWindow("KeyPoints of image1");
	cv::namedWindow("KeyPoints of image2");

	

	//显示特征点
	cv::imshow("KeyPoints of image1", keyPointImage1);
	cv::imshow("KeyPoints of image2", keyPointImage2);

	
	
 	//imshow("1",image1);
	//waitKey(5000);

	FlannBasedMatcher matcher;
	vector<DMatch> matchePoints,goodmatches;  
	Mat  firstmatches;
	matcher.match(imageDesc1,imageDesc2,matchePoints,Mat());
	drawMatches(image1, keyPoint1, image2, keyPoint2, matchePoints, firstmatches, Scalar::all(-1), Scalar::all(-1),vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	imshow("first_matches", firstmatches);
        imwrite("first_matches.jpg",firstmatches);


	double max_dist = 0; double min_dist = 1000;
         for (int i = 0; i < imageDesc1.rows; i++) {
                 if (matchePoints[i].distance > max_dist) {
                         max_dist = matchePoints[i].distance;
                 }
                 if (matchePoints[i].distance < min_dist) {
                         min_dist = matchePoints[i].distance;
                 }
         }
         cout << "The max distance is: " << max_dist << endl;
         cout << "The min distance is: " << min_dist << endl;
         for (int i = 0; i < imageDesc1.rows; i++) {
                 if (matchePoints[i].distance < 2 * min_dist) {
                         goodmatches.push_back(matchePoints[i]);
                 }
         }
         Mat img_matches;
         /*第二次筛选后的结果*/
         drawMatches(image1, keyPoint1, image2, keyPoint2,
                 goodmatches, img_matches, Scalar::all(-1), Scalar::all(-1),
                 vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
         imshow("good_matches", img_matches);
         imwrite("good_matches.jpg",img_matches);  //108行-130行这一段只是显示good_matches，并未实际使用

	sort(matchePoints.begin(),matchePoints.end()); //特征点排序	
	//sort(goodmatches.begin(),goodmatches.end());
	//获取排在前N个的最优匹配特征点
	vector<Point2f> imagePoints1,imagePoints2;
	for(int i=0;i<10;i++)
	{		
		imagePoints1.push_back(keyPoint1[matchePoints[i].queryIdx].pt);		
		imagePoints2.push_back(keyPoint2[matchePoints[i].trainIdx].pt);
		//imagePoints1.push_back(keyPoint1[goodmatches[i].queryIdx].pt);
		//imagePoints2.push_back(keyPoint2[goodmatches[i].queryIdx].pt);		
	}
 
	//获取图像1到图像2的投影映射矩阵，尺寸为3*3
	//Mat homo=findHomography(imagePoints1,imagePoints2,CV_RANSAC);	
	Mat homo=findHomography(imagePoints1,imagePoints2,cv::RANSAC);	
	Mat adjustMat=(Mat_<double>(3,3)<<1.0,0,image1.cols,0,1.0,0,0,0,1.0);
	Mat adjustHomo=adjustMat*homo;
 
	//获取最强配对点在原始图像和矩阵变换后图像上的对应位置，用于图像拼接点的定位
	Point2f originalLinkPoint,targetLinkPoint,basedImagePoint;
	originalLinkPoint=keyPoint1[matchePoints[0].queryIdx].pt;
	targetLinkPoint=getTransformPoint(originalLinkPoint,adjustHomo);
	basedImagePoint=keyPoint2[matchePoints[0].trainIdx].pt;
 
	//图像配准
	Mat imageTransform1;
	warpPerspective(image1,imageTransform1,adjustMat*homo,Size(image2.cols+image1.cols+110,image2.rows));
 
	//在最强匹配点左侧的重叠区域进行累加，是衔接稳定过渡，消除突变
	Mat image1Overlap,image2Overlap; //图1和图2的重叠部分	
	image1Overlap=imageTransform1(Rect(Point(targetLinkPoint.x-basedImagePoint.x,0),Point(targetLinkPoint.x,image2.rows)));
	image2Overlap=image2(Rect(0,0,image1Overlap.cols,image1Overlap.rows));
	Mat image1ROICopy=image1Overlap.clone();  //复制一份图1的重叠部分
	for(int i=0;i<image1Overlap.rows;i++)
	{
		for(int j=0;j<image1Overlap.cols;j++)
		{
			double weight;
			weight=(double)j/image1Overlap.cols;  //随距离改变而改变的叠加系数
			image1Overlap.at<Vec3b>(i,j)[0]=(1-weight)*image1ROICopy.at<Vec3b>(i,j)[0]+weight*image2Overlap.at<Vec3b>(i,j)[0];
			image1Overlap.at<Vec3b>(i,j)[1]=(1-weight)*image1ROICopy.at<Vec3b>(i,j)[1]+weight*image2Overlap.at<Vec3b>(i,j)[1];
			image1Overlap.at<Vec3b>(i,j)[2]=(1-weight)*image1ROICopy.at<Vec3b>(i,j)[2]+weight*image2Overlap.at<Vec3b>(i,j)[2];
		}
	}
	Mat ROIMat=image2(Rect(Point(image1Overlap.cols,0),Point(image2.cols,image2.rows)));	 //图2中不重合的部分
	ROIMat.copyTo(Mat(imageTransform1,Rect(targetLinkPoint.x,0, ROIMat.cols,image2.rows))); //不重合的部分直接衔接上去
	namedWindow("拼接结果",0);
	imshow("拼接结果",imageTransform1);	
	imwrite("拼接结果.jpg",imageTransform1);
	waitKey();  
	return 0;  
}
 
//计算原始图像点位在经过矩阵变换后在目标图像上对应位置
Point2f getTransformPoint(const Point2f originalPoint,const Mat &transformMaxtri)
{
	Mat originelP,targetP;
	originelP=(Mat_<double>(3,1)<<originalPoint.x,originalPoint.y,1.0);
	targetP=transformMaxtri*originelP;
	float x=targetP.at<double>(0,0)/targetP.at<double>(2,0);
	float y=targetP.at<double>(1,0)/targetP.at<double>(2,0);
	return Point2f(x,y);
}

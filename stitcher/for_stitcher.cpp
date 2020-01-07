#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/stitching.hpp>
//#include "windows.h"
#include<time.h>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>


using namespace std;
using namespace cv;

//bool try_use_gpu = false;
vector<Mat> imgs;
string result_name = "dst1.jpg";
/*尝试修改成c++代码失败
Mat gray,ret,thresh,cnts,hierarchy;
status = cv::copyMakeBorder(status, 10, 10, 10, 10, cv::BORDER_CONSTANT, (0, 0, 0));
cv::cvtColor(status, gray, CV_BGR2GRAY);
ret, thresh = cv::threshold(gray, 0, 255, cv::THRESH_BINARY);

cnts, hierarchy = cv::findContours(thresh.copy(), cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
cnt = max(cnts, key=cv::contourArea);

//mask = np.zeros(thresh.shape, dtype="uint8")
vector<Point2f> mask;
x, y, w, h = cv::boundingRect(cnt);
cv::rectangle(mask, (x, y), (x + w, y + h), 255, -1);

minRect = mask.clone();
sub = mask.copy();

// 开始while循环，直到sub中不再有前景像素
while cv::countNonZero(sub) > 0
    {
    minRect = cv2.erode(minRect, None);
    sub = cv2.subtract(minRect, thresh);
    }
cnts, hierarchy = cv::findContours(minRect.copy(), cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
cnt = max(cnts, key=cv::contourArea);
x, y, w, h = cv::boundingRect(cnt);

//使用边界框坐标提取最终的全景图
stitched = stitched[y:y + h, x:x + w];

cv::imwrite("final.jpg", stitched);
*/
    //cout << "Time: "<<t1 - t0 << endl;

/*使用python去除拼接后的黑边，参考知乎：https://zhuanlan.zhihu.com/p/83225676
stitched = cv2.copyMakeBorder(stitched, 10, 10, 10, 10, cv2.BORDER_CONSTANT, (0, 0, 0))
gray = cv2.cvtColor(stitched, cv2.COLOR_BGR2GRAY)
ret, thresh = cv2.threshold(gray, 0, 255, cv2.THRESH_BINARY)

cnts, hierarchy = cv2.findContours(thresh.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
cnt = max(cnts, key=cv2.contourArea)

mask = np.zeros(thresh.shape, dtype="uint8")
x, y, w, h = cv2.boundingRect(cnt)
cv2.rectangle(mask, (x, y), (x + w, y + h), 255, -1)

minRect = mask.copy()
sub = mask.copy()

# 开始while循环，直到sub中不再有前景像素
while cv2.countNonZero(sub) > 0:
    minRect = cv2.erode(minRect, None)
    sub = cv2.subtract(minRect, thresh)

cnts, hierarchy = cv2.findContours(minRect.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
cnt = max(cnts, key=cv2.contourArea)
x, y, w, h = cv2.boundingRect(cnt)

# 使用边界框坐标提取最终的全景图
stitched = stitched[y:y + h, x:x + w]

cv2.imwrite('final.jpg', stitched)
*/

/*************************************************
 * author : wyj
 * date : 2016-10-31
 * **********************************************/ 

const int black = 10;


/**************************************************
 * remove the black border
 * ***********************************************/ 
class imageRemove{
public:
	imageRemove(string imageName,string saveImageName);
	string getRawImageName();
	void setSaveImageName(string a);
	int removeblack();
private:
	string saveImageName;
	string imageName;
};

imageRemove::imageRemove(string imageName, string saveImageName) 
{	
	this->imageName = imageName;
	this->saveImageName = saveImageName;
}

string imageRemove::getRawImageName()
{
	return imageName;
}

void imageRemove::setSaveImageName(string a)
{	
	this->saveImageName = a;
}

// remove black border

int imageRemove::removeblack()
{
	Mat inputImg = imread(imageName);
	if(!inputImg.data)
	{
		cerr<<"read image error:\t"<<imageName;
		return 1;
	}
	if(!inputImg.isContinuous())
	{
		return 2;
	}
	Mat tmpMat = inputImg.clone();
	cvtColor(tmpMat, tmpMat, CV_RGB2GRAY);
	for(int i = 0; i < 2; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			cout<<(int)tmpMat.at<uchar>(i,j)<<" ";
		}
		cout<<endl;
	}

	int left = 0;
	int right = tmpMat.cols;
	int up = 0;
	int down = tmpMat.rows;
	// 
	int num = 0;
	//int range = tmpMat.cols / 5;
	for(int i = 0; i < tmpMat.rows; i++)
	{
		num = 0;
		for(int j = 0; j < tmpMat.cols; j++)
		{
			if((int)tmpMat.at<uchar>(i,j) >= black)
			{
				//cout<<"(i,j)="<<tmpMat<<endl;
				num++;
			}
		}
		if(num >=0.65* tmpMat.cols)
		{
			up = i;
			break;
		}
	}
	for(int i = tmpMat.rows-1; i >= 0; i--)
	{
		num  = 0;
		for(int j = 0; j < tmpMat.cols; j++)
		{
			if((int)tmpMat.at<uchar>(i,j) >= black)
			{
				num ++;
			}
		}
		if(num >=0.85* tmpMat.cols)
		{
			down = i;
			break;
		}
	}
	//range = tmpMat.rows / 5;
	for(int i = 0; i < tmpMat.cols; i++)
	{
		num = 0;
		for(int j = 0; j < tmpMat.rows; j++)
		{
			if((int)tmpMat.at<uchar>(j,i) >= black)
			{
				num++;
			}
		}
		if(num >=0.8* tmpMat.rows)
		{
			left = i;
			break;
		}
	}
	for(int i = tmpMat.cols-1; i >= 0; i--)
	{
		num = 0;
		for(int j = 0; j < tmpMat.rows; j++)
		{
			if((int)tmpMat.at<uchar>(j,i) >= black)
			{
				num++;
			}
		}
		if(num >=0.8* tmpMat.rows)
		{
			right = i;
			break;
		}
	}
	cout<<up<<" "<<down<<" "<<left<<" "<<right<<endl;
	if(up > down || right < left)
	{
		cerr<<"image is total black:\t"<<imageName;
		return 3;
	}
	Rect rect(left, up, right - left, down - up);
	Mat imageSave = inputImg(rect);
	imshow(saveImageName,imageSave);
	imwrite(saveImageName, imageSave);
	waitKey();
	return 0;
}

// 

void usage()
{
	cout<<"*******************************************"<<endl;
	cout<<"./removeblack inputImage outPutImage"<<endl;
}

int main(int argc, char** argv)
{
	//Mat  stitchedImage;
	int n;

        cout << "请输入想拼接的图片数量（大于1小于10）" << endl;
        cin >> n;
        cout << "输入成功，开始计时" << endl;

        clock_t start,finish;
        double totaltime;
        start=clock();

        for (int k = 1; k <= n; k++) {
                stringstream stream;
                string str;
                stream << k;
                stream >> str;
                string filename = str + ".jpg";
                Mat img = imread(filename);
		if (img.empty())
		{
        		cout << "Can't read image" << endl;
        		return -1;
    		}

		imgs.push_back(img.clone());
        }
	
	
    Ptr<Stitcher> stitcher = Stitcher::create(Stitcher::PANORAMA);
    // 使用stitch函数进行拼接
    Mat pano;
    Stitcher::Status status = stitcher->stitch(imgs, pano);
    if (status != Stitcher::OK)
    {
        cout << "Can't stitch images, error code = " << int(status) << endl;
        return -1;
    }
    finish = clock();
    totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
    cout << "拼接成功" << endl;
    cout << "拼接花费总时间为：" << totaltime << "秒！" << endl;

    imwrite(result_name, pano);
    Mat pano2 = pano.clone();
    // 显示源图像，和结果图像
    imshow("全景图像", pano);

    waitKey();
    /*if(argc != 3)
	{
		cerr<<"parameter error";
		usage();
		return 1;
	}*/
	string inputFileName = (result_name);
	string outputFileName = ("out_pic.jpg");
	imageRemove  iR(inputFileName, outputFileName);
	iR.removeblack();

    return 0;
}


/*************************************************
 * author : wyj
 * date : 2016-10-31
 * **********************************************/ 
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <string>
#include <opencv2/imgproc/types_c.h>


using namespace std;
using namespace cv;

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
		//for(int j = 0; j < tmpMat.cols; j+=range)
		for(int j = 0; j < tmpMat.cols; j++)
		{
			//if((int)tmpMat.at<uchar>(i,j) <= black)
			if((int)tmpMat.at<uchar>(i,j) >= black)
			{
				num++;
			}
		}
		//if(num < 5)
		if(num >=0.8* tmpMat.cols)
		{
			up = i;
			break;
		}
	}
	for(int i = tmpMat.rows-1; i >= 0; i--)
	{
		num  = 0;
		//for(int j = 0; j < tmpMat.cols; j+=range)
		for(int j = 0; j < tmpMat.cols; j++)
		{
			//if((int)tmpMat.at<uchar>(i,j) <= black)
			if((int)tmpMat.at<uchar>(i,j) >= black)
			{
				num ++;
			}
		}
		//if(num < 5)
		if(num >=0.8* tmpMat.cols)
		{
			down = i;
			break;
		}
	}
	//range = tmpMat.rows / 5;
	for(int i = 0; i < tmpMat.cols; i++)
	{
		num = 0;
		//for(int j = 0; j < tmpMat.rows; j+=range)
		for(int j = 0; j < tmpMat.rows; j++)
		{
			//if((int)tmpMat.at<uchar>(j,i) <= black)
			if((int)tmpMat.at<uchar>(j,i) >= black)
			{
				num++;
			}
		}
		//if(num < 5)
		if(num >= 0.8*tmpMat.rows)
		{
			left = i;
			break;
		}
	}
	for(int i = tmpMat.cols -1; i >= 0; i--)
	{
		num = 0;
		//for(int j = 0; j < tmpMat.rows; j += range)
		for(int j = 0; j < tmpMat.rows; j++)
		{
			//if((int)tmpMat.at<uchar>(j,i) <= black)
			if((int)tmpMat.at<uchar>(j,i) >= black)
			{
				num++;
			}
		}
		//if(num < 5)
		if(num >= 0.8*tmpMat.rows)
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
	imwrite(saveImageName, imageSave);
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
	if(argc != 3)
	{
		cerr<<"parameter error";
		usage();
		return 1;
	}
	string inputFileName = (string)(argv[1]);
	string outputFileName = (string)(argv[2]);
	imageRemove  iR(inputFileName, outputFileName);
	iR.removeblack();
	return 0;
}



	




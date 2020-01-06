#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/stitching.hpp>
//#include "windows.h"

using namespace std;
using namespace cv;

//bool try_use_gpu = false;
vector<Mat> imgs;
string result_name = "dst1.jpg";
int main(int argc, char * argv[])
{
    Mat img1 = imread("1.jpg");
    Mat img2 = imread("2.jpg");
    Mat img3 = imread("3.jpg");
    Mat img4 = imread("4.jpg");
    //imshow("p1", img1);
    //imshow("p2", img2);

    //long t0 = GetTickCount();

    if (img1.empty() || img2.empty())
    {
        cout << "Can't read image" << endl;
        return -1;
    }
    imgs.push_back(img1);
    imgs.push_back(img2);
    imgs.push_back(img3);
    imgs.push_back(img4);

    //Stitcher stitcher = Stitcher::createDefault(try_use_gpu);
    Ptr<Stitcher> stitcher = Stitcher::create(Stitcher::PANORAMA);
    // 使用stitch函数进行拼接
    Mat pano;
    Stitcher::Status status = stitcher->stitch(imgs, pano);
    if (status != Stitcher::OK)
    {
        cout << "Can't stitch images, error code = " << int(status) << endl;
        return -1;
    }
    //long t1 = GetTickCount();
    imwrite(result_name, pano);
    Mat pano2 = pano.clone();
    // 显示源图像，和结果图像
    imshow("全景图像", pano);

    //cout << "Time: "<<t1 - t0 << endl;

    if (waitKey() == 27)
        return 0;
}


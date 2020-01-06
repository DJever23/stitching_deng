#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/stitching.hpp>
//#include "windows.h"
#include<time.h>

using namespace std;
using namespace cv;

//bool try_use_gpu = false;
vector<Mat> imgs;
string result_name = "dst1.jpg";
int main()
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

    //cout << "Time: "<<t1 - t0 << endl;

    waitKey();
    return 0;
}


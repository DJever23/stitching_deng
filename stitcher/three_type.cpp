#include <iostream>  
#include <fstream>  
#include "opencv2/highgui/highgui.hpp"  
#include "opencv2/stitching.hpp" 
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/features2d.hpp>
 
   
using namespace std;  
using namespace cv;  
   
//bool try_use_gpu = false;  
vector<Mat> imgs;  
string result_name = "result.jpg";  
   
int parseCmdArgs(int argc, char** argv)
{  //输入的图片全部填充到容器imgs中,并将输入的图片显示出来。
    for (int i = 1; i < argc-1; ++i)
    {  
        Mat img = imread(argv[i]);  
        if (img.empty()){  
            cout << "Can't read image '" << argv[i] << "'\n";  
            return -1;  
        }  
        imgs.push_back(img);  
   
        imshow(argv[i],img); 
	waitKey(500); 
    }  
    return 0;  
}  
   
int main(int argc, char* argv[])
{  
    int retval = parseCmdArgs(argc, argv);  
    if (retval) return -1;  
   
    Mat pano;        
    //Stitcher stitcher = Stitcher::createDefault(try_use_gpu);  //创建一个stitcher对象。
    Ptr<Stitcher> stitcher = Stitcher::create(Stitcher::PANORAMA);
    // 使用stitch函数进行拼接
    //Mat pano;
    //Stitcher::Status status = stitcher->stitch(imgs, pano);
   
    if(argv[4][0] == '1')
    {  //1:平面拼接
        PlaneWarper* cw = new PlaneWarper();  
        stitcher->setWarper(cw);  
    }
    else if(argv[4][0] == '2')
    {//2:柱面 拼接
        SphericalWarper* cw = new SphericalWarper();      
        stitcher->setWarper(cw);  
    }
    else if(argv[4][0] == '3')
    {//3:立体画面拼接
        StereographicWarper *cw = new cv::StereographicWarper();      
        stitcher->setWarper(cw);  
    }  
   
    //使用Surf算法来寻找特征点,支持Surf和Orb两种方式
    //detail::SurfFeaturesFinder *featureFinder = new detail::SurfFeaturesFinder();  
    //stitcher->setFeaturesFinder(featureFinder);  
    Ptr<Feature2D> finder = xfeatures2d::SURF::create();
    stitcher->setFeaturesFinder(finder); 
    /*匹配给定的图像和估计相机的旋转*/  
    //Stitcher::Status status = stitcher.estimateTransform(imgs);  //另一种方式来实现拼接
    Stitcher::Status status = stitcher->estimateTransform(imgs);
    if (status != Stitcher::OK)  
    {  
        cout << "Can't stitch images, error code = " << int(status) << endl;  
        return -1;  
    }  
   
    /*生成全景图像*/  
    status = stitcher->composePanorama(pano);  
    if (status != Stitcher::OK)  
    {  
        cout << "Can't stitch images, error code = " << int(status) << endl;  
        return -1;  
    }  
   
    imwrite(result_name, pano);  
    imshow("show", pano);  
    cv::waitKey(0);  
    return 0;  
}  

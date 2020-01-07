1.运行source_stitcher.cpp编译的文件时要加上参数
./source_stitcher "1.jpg" "2.jpg" "3.jpg"

2.运行three_type.cpp编译的文件时要加上参数，此文件只能拼3张图
./threetype "1.jpg" "2.jpg" "3.jpg" 1 
1表示投影到平面
2表示投影到柱面
3表示投影到立体面

3.for_stitcher.cpp
使用for循环读取图片，可以实现多张图拼接且不用输入参数，需要在当前目录下有图片

4.stitcher.cpp
未使用for循环的代码

5.stitching.cpp
opencv源码里的文件

6.source_stitcher.cpp和for_stitcher.cpp可以实现多张图片的全景拼接，并有拼接完成后的裁剪功能（需要针对每一张图片做修改，不能通用）

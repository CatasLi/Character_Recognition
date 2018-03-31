#include <sstream>
#include <string>
#include <iostream>
#include <highgui.h>
#include <opencv\cv.h>
using namespace cv;
//该函数的目的是，将图片中的标志牌读取出来，
//并返回一个二值化后 在正方形中的标志牌
Mat getSignInSquare(char* fileName)
{
	extern Mat find_blue(Mat src);//找到蓝色的区域
	extern Mat find_circles(Mat src);//找到圆形区域
	extern Mat get_sign(Mat A,Mat B);//得到标志牌（放在正方形中）
	//char fileName[20];
	//printf("请输入文件的名称:\n");
	//scanf("%s", fileName);
	//Mat src = frame;
      Mat src = imread(fileName);
	//彩色图像转化成灰度图
	Mat src_gray;
	cvtColor(src, src_gray, COLOR_BGR2GRAY);

	Mat blue_image = find_blue(src);
	Mat circle_image=find_circles(blue_image);
	Mat sign = get_sign(src_gray,circle_image);
	//Mat sign_gray;
	//cvtColor(sign,sign_gray, CV_BGR2GRAY);
	imwrite("sign.png",sign);
	//return sign_gray;
	return sign;
}
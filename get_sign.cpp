#include <sstream>
#include <string>
#include <iostream>
#include <opencv\highgui.h>
#include <opencv\cv.h>

using namespace cv;
const int kvalue = 15;//双边滤波邻域大小

const int min_radius=10;
const int max_radius = 100;
int point_x=0,point_y=0,r=0;

void setWhite(IplImage * res)
{
	 unsigned char * res_data;
	 res_data= (unsigned char*)(res->imageData);
	for(int j=0;j<res->height;j++)
	{
		for(int i=0;i<res->width;i++)
		{
			res_data[res->widthStep*j+i] =255;
		}
	}
}

Mat find_circles(Mat src_color)
{
	//声明一个三通道图像，像素值全为0，用来将霍夫变换检测出的圆画在上面
	Mat dst(src_color.size(), src_color.type());
	dst = Scalar::all(0);
	Mat src_gray = src_color;
	Mat bf;//对灰度图像进行双边滤波
	bilateralFilter(src_gray, bf, kvalue, kvalue*2, kvalue/2);
	//imshow("灰度双边滤波处理", bf);
	//imwrite("src_bf.png", bf);

	vector<Vec3f> circles;//声明一个向量，保存检测出的圆的圆心坐标和半径
	
	HoughCircles(bf, circles, CV_HOUGH_GRADIENT, 1.5, 20, 130, 38, min_radius, max_radius);//霍夫变换检测圆
	if(circles.size()>1){
		printf("检测出多个圆，注意调节圆的半径大小\n");
		exit(-1);
	}
	if(circles.size()==0){
		printf("未检测到圆，注意调节检测圆的半径大小\n");
		exit(-1);
	}
	for(size_t i = 0; i < circles.size(); i++)//把霍夫变换检测出的圆画出来
	{
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		circle( dst, center, radius*6/7, CV_RGB( 255,255,255 ),-1, 8, 0 );
		point_x = circles[i][0];
		point_y = circles[i][1];
		r = radius*6/7;
	}
	return dst;
}
Mat get_sign(Mat src_gray,Mat white_circle)
{
	Mat binary_src;
	int bound = 60;
	//将灰度图二值化，注意二值化的阈值可以调节
	threshold(src_gray, binary_src,bound, 255, CV_THRESH_BINARY);
	IplImage gray_img(binary_src);
	IplImage whiteC_blackR(white_circle);
	IplImage * res = cvCreateImage(cvGetSize(&gray_img), 8, 1);
	setWhite(res);//res全白
	cvAnd(&gray_img, &gray_img, res, &whiteC_blackR);
	//对res提取roi区域
	

	CvRect  roi1=cvRect(point_x-r,point_y-r, r*2, r*2); 
	cvSetImageROI(res, roi1);
	IplImage *img2 = cvCreateImage(cvGetSize(res),
                               8,
                               1);
	cvCopy(res, img2, NULL);
	cvResetImageROI(res);

	Mat sign_image(img2);
	return sign_image;
}
#include <sstream>
#include <string>
#include <iostream>
#include <opencv\highgui.h>
#include <opencv\cv.h>

using namespace cv;
const int kvalue = 15;//˫���˲������С

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
	//����һ����ͨ��ͼ������ֵȫΪ0������������任������Բ��������
	Mat dst(src_color.size(), src_color.type());
	dst = Scalar::all(0);
	Mat src_gray = src_color;
	Mat bf;//�ԻҶ�ͼ�����˫���˲�
	bilateralFilter(src_gray, bf, kvalue, kvalue*2, kvalue/2);
	//imshow("�Ҷ�˫���˲�����", bf);
	//imwrite("src_bf.png", bf);

	vector<Vec3f> circles;//����һ�����������������Բ��Բ������Ͱ뾶
	
	HoughCircles(bf, circles, CV_HOUGH_GRADIENT, 1.5, 20, 130, 38, min_radius, max_radius);//����任���Բ
	if(circles.size()>1){
		printf("�������Բ��ע�����Բ�İ뾶��С\n");
		exit(-1);
	}
	if(circles.size()==0){
		printf("δ��⵽Բ��ע����ڼ��Բ�İ뾶��С\n");
		exit(-1);
	}
	for(size_t i = 0; i < circles.size(); i++)//�ѻ���任������Բ������
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
	//���Ҷ�ͼ��ֵ����ע���ֵ������ֵ���Ե���
	threshold(src_gray, binary_src,bound, 255, CV_THRESH_BINARY);
	IplImage gray_img(binary_src);
	IplImage whiteC_blackR(white_circle);
	IplImage * res = cvCreateImage(cvGetSize(&gray_img), 8, 1);
	setWhite(res);//resȫ��
	cvAnd(&gray_img, &gray_img, res, &whiteC_blackR);
	//��res��ȡroi����
	

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
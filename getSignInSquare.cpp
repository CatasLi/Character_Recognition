#include <sstream>
#include <string>
#include <iostream>
#include <highgui.h>
#include <opencv\cv.h>
using namespace cv;
//�ú�����Ŀ���ǣ���ͼƬ�еı�־�ƶ�ȡ������
//������һ����ֵ���� ���������еı�־��
Mat getSignInSquare(char* fileName)
{
	extern Mat find_blue(Mat src);//�ҵ���ɫ������
	extern Mat find_circles(Mat src);//�ҵ�Բ������
	extern Mat get_sign(Mat A,Mat B);//�õ���־�ƣ������������У�
	//char fileName[20];
	//printf("�������ļ�������:\n");
	//scanf("%s", fileName);
	//Mat src = frame;
      Mat src = imread(fileName);
	//��ɫͼ��ת���ɻҶ�ͼ
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
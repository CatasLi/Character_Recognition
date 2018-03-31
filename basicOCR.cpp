/*
 *  basicOCR.c
 *  
 *
 *  Created by damiles on 18/11/08.
 *  Copyright 2008 Damiles. GPL License
 *
 */
#ifdef _CH_
#pragma package <opencv>
#endif

#ifndef _EiC
#include "cv.h"
#include "highgui.h"
#include "ml.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#endif

#include "preprocessing.h"
#include "basicOCR.h"

basicOCR::basicOCR()//���캯��
{

	//initial
	sprintf(file_path , "OCR/");
	//train_samples = 50;//ѵ���������ܹ�100����50��ѵ����50������
	train_samples = 50;
	classes= 10;//��ʱʶ��ʮ������

	size=128;//


	trainData = cvCreateMat(train_samples*classes, size*size, CV_32FC1);//ѵ�����ݵľ���
	trainClasses = cvCreateMat(train_samples*classes, 1, CV_32FC1);

	//Get data (get images and process it)
	getData();

	//train	
	train();
	//Test	
	test();

	//printf(" ------------------------------------------------------------------------\n");
	//printf("|\tʶ����\t|\t ���Ծ���\t|\t  ׼ȷ��\t|\n");
	//printf(" ------------------------------------------------------------------------\n");


}

void printResult(int num)
{
	printf("��⵽��ʾ�� ��   ");
	switch(num){
	case 0:
		printf("ֱ��\n");
		break;
	case 1:
		printf("ͣ��\n");
		break;
	case 2:
		printf("��ת\n");
		break;
	case 3:
		printf("��ת\n");
		break;
	default:
		printf("%d\n",num);
		break;
	}
	printf(" ------------------------------------------------------------------------\n");
}

void basicOCR::getData()
{
	IplImage* src_image;
	IplImage prs_image;
	CvMat row,data;
	char file[255];
	int i,j;
	for(i =0; i<classes; i++)
	{
		for( j = 0; j< train_samples; j++)
		{
			
			//����pbm��ʽͼ����Ϊѵ��
			if(j<10)
				sprintf(file,"%s%d/%d0%d.pbm",file_path, i, i , j);
			else
				sprintf(file,"%s%d/%d%d.pbm",file_path, i, i , j);
			src_image = cvLoadImage(file,0);
			if(!src_image)
			{
				printf("Error: Cant load image %s\n", file);
				//exit(-1);
			}
			//process file
			prs_image = preprocessing(src_image, size, size);
			
			//Set class label
			cvGetRow(trainClasses, &row, i*train_samples + j);
			cvSet(&row, cvRealScalar(i));
			//Set data 
			cvGetRow(trainData, &row, i*train_samples + j);

			IplImage* img = cvCreateImage( cvSize( size, size ), IPL_DEPTH_32F, 1 );
			//convert 8 bits image to 32 float image
			cvConvertScale(&prs_image, img, 0.0039215, 0);

			cvGetSubRect(img, &data, cvRect(0,0, size,size));
			
			CvMat row_header, *row1;
			//convert data matrix sizexsize to vecor
			row1 = cvReshape( &data, &row_header, 0, 1 );
			cvCopy(row1, &row, NULL);
		}
	}
} 

void basicOCR::train()
{
	knn=new CvKNearest( trainData, trainClasses, 0, false, K );
}

float basicOCR::classify(IplImage* img, int showResult)//�ڶ���������Ҫ���������ǲ���ѵ������������дʶ��
{
	IplImage prs_image;
	CvMat data;
	CvMat* nearest=cvCreateMat(1,K,CV_32FC1);
	float result;
	//���������ͼ��
	prs_image = preprocessing(img, size, size);
	
	//Set data 
	IplImage* img32 = cvCreateImage( cvSize( size, size ), IPL_DEPTH_32F, 1 );
	cvConvertScale(&prs_image, img32, 0.0039215, 0);
	cvGetSubRect(img32, &data, cvRect(0,0, size,size));
	CvMat row_header, *row1;
	row1 = cvReshape( &data, &row_header, 0, 1 );

	result=knn->find_nearest(row1,K,0,0,nearest,0);
	
	int accuracy=0;
	for(int i=0;i<K;i++)
	{
		if( (nearest->data.fl[i]) == result)
                    accuracy++;
	}
	float pre=100*((float)accuracy/(float)K);
	if(showResult==1)
	{
		//printf("|\t    %.0f    \t| \t    %.2f%%  \t| \t %d of %d \t| \n",result,pre,accuracy,K);
		//printf(" ------------------------------------------------------------------------\n");
		if(accuracy>=5){//��ȷ�ʸ���50%ʱ�������
		printResult(result);
		}
		//cvNamedWindow("care");
		//cvShowImage("care",&prs_image);
		//cvSaveImage("care.png",&prs_image);
		//cvWaitKey(0);
		//cvDestroyWindow("care");
		//prs_image
	}

	return result;

}

void basicOCR::test()
{/*
	IplImage* src_image;
	IplImage prs_image;
	CvMat row,data;
	char file[255];
	int i,j;
	int error=0;
	int testCount=0;
	for(i =0; i<classes; i++)
	{
		for( j = 50; j< 50+train_samples; j++)//��ʮ����������������һ�´�����
		{
			
			sprintf(file,"%s%d/%d%d.pbm",file_path, i, i , j);
			src_image = cvLoadImage(file,0);
			if(!src_image)
			{
				printf("Error: Cant load image %s\n", file);
				exit(-1);
			}
			//process file
			prs_image = preprocessing(src_image, size, size);
			float r=classify(&prs_image,0);
			if((int)r!=i)
				error++;
			
			testCount++;
		}
	}
	float totalerror=100*(float)error/(float)testCount;
	printf("����ϵͳ��ʶ��: %.2f%%\n", totalerror);
	*/
}



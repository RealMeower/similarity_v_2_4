#include "stdafx.h"
#include "ContoursCompare.h"


void calc_contours( char *winname,
				   CvSeq* first_contour,
				   CvMemStorage* storage, 
				   IplImage* img_src, 
				   IplImage* img_con
				   )//计算轮廓
{
	IplImage* img_edge = cvCreateImage( cvGetSize(img_src), 8, 0 );
	cvSmooth( img_src, img_edge, CV_GAUSSIAN, 3, 3, 0, 0 );
	cvNot( img_src, img_edge );
	cvCanny(img_src, img_edge, 100, 300, 3);

	cvNamedWindow( winname, 0 );
	cvResizeWindow(winname,200,200);
	first_contour = NULL;
	int Nc = cvFindContours(img_edge,
		storage,
		&first_contour,
		sizeof(CvContour),
		CV_RETR_EXTERNAL
		);

	cvSetZero(img_con);
	cvDrawContours(
		img_con,
		first_contour,
		cvScalarAll(255), 
		cvScalarAll(255),
		1,        //  values of max_level
		2,
		8
		);
	cvShowImage( winname, img_con );
	cvReleaseImage( &img_edge );
}

int gesContourCompFunc(const void* _a, const void* _b, void* userdata)
{
	int retval;
	double s1, s2;
	CvContour* a = (CvContour*)_a;
	CvContour* b = (CvContour*)_b;

	s1 = fabs(cvContourArea(a));
	s2 = fabs(cvContourArea(b));
	//s1 = a->rect.height * a->rect.width;
	//s2 = b->rect.height * b->rect.width;

	if(s1 < s2)
	{
		retval = 1;
	}
	else if(s1 == s2)
	{
		retval = 0;
	}
	else
	{
		retval = -1;
	}

	return retval;
}

//src:BGR dst:
void gesFindContours(IplImage* src, IplImage* dst, CvSeq** templateContour, CvMemStorage* templateStorage, int flag)
{
	int count;//轮廓数
	IplImage* gray;
	CvMemStorage* first_sto;
	CvMemStorage* all_sto;
	CvSeq* first_cont;
	CvSeq* all_cont;
	CvSeq* cur_cont;

	//初始化动态内存
	first_sto = cvCreateMemStorage(0);
	first_cont = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint), first_sto);
	all_sto = cvCreateMemStorage(0);
	all_cont = cvCreateSeq(0, sizeof(CvSeq), sizeof(CvSeq), all_sto);

	//创建源图像对应的灰度图像
	gray = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	cvCvtColor(src, gray, CV_BGR2GRAY);

	//灰度图像边缘检测
	IplImage* img_edge = cvCreateImage( cvGetSize(src), 8, 0 );
	//cvSmooth( gray, img_edge, CV_GAUSSIAN, 3, 3, 0, 0 );
	cvNot( gray, img_edge );
	cvCanny(gray, img_edge, 100, 300, 3);

	//得到图像的外层轮廓
	count = cvFindContours(img_edge, first_sto, &first_cont, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	//如果没有检测到轮廓则返回
	if(first_sto == NULL)
	{
		return;
	}

	//将所有的轮廓都放到first_cont中
	for(;first_cont != 0;first_cont = first_cont->h_next)
	{
		if(((CvContour* )first_cont)->rect.height * ((CvContour* )first_cont)->rect.width >= 625)
			cvSeqPush(all_cont, first_cont);
	}

	//对轮廓按照面积进行排序
	cvSeqSort(all_cont, gesContourCompFunc, 0);

	//在dst中画出轮廓
	cvZero(dst);
	for(int i = 0;i < min(all_cont->total, 3);i++)///////////////////////次数待改
	{
		cur_cont = (CvSeq* )cvGetSeqElem(all_cont, i);
		if(flag != 0 && i == 0)
		{
			*templateContour = cvCloneSeq(cur_cont, templateStorage);
		}

		CvScalar color = CV_RGB(rand()&255, rand()&255, rand()&255);
		cvDrawContours(dst, (CvSeq* )cur_cont, color, color, -1, 1, 8);
	}

	//判断原点位置以确定是否需要反转图像
	if(src->origin == 1)
	{
		cvFlip(dst);
	}

	//释放内存
	cvReleaseMemStorage(&first_sto);
	cvReleaseMemStorage(&all_sto);
	cvReleaseImage(&gray);
	cvReleaseImage(&img_edge);
}

void gesMatchContoursTemplate(IplImage* src, IplImage* dst, CvSeq** templateContour)
{
	CvSeq* contour;
	CvMemStorage* storage;

	//初始化动态内存
	storage = cvCreateMemStorage(0);
	contour = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint), storage);

	//得到轮廓并进行匹配
	gesFindContours(src, dst, &contour, storage, 1);
	if(contour->total != 0)//如果得到的轮廓不为空
	{
		double result = cvMatchShapes((CvContour* )contour, (CvContour* )(*templateContour), CV_CONTOURS_MATCH_I3);
		printf("%.2f\n", result);/////////////////////////////////////////////
	}

	//释放内存
	cvReleaseMemStorage(&storage);
}

//模版匹配法的完整实现
int gesMatchContoursTemplate2(IplImage* src, IplImage* dst, CvSeq* templateContour)
{
	CvSeq* contour;
	CvSeq* cur_cont;
	CvMemStorage* storage;
	double minValue, tempValue;
	int i, minIndex;

	//初始化动态内存
	storage = cvCreateMemStorage(0);
	contour = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint), storage);

	//得到轮廓并进行匹配
	minIndex = -1;
	gesFindContours(src, dst, &contour, storage, 1);
	if(contour->total != 0)//如果得到的轮廓不为空
	{
		if(templateContour->total != 0)
		{
			cur_cont = (CvSeq* )cvGetSeqElem(templateContour, 0);
			minValue = cvMatchShapes((CvContour* )contour, (CvContour* )cur_cont, CV_CONTOURS_MATCH_I3);
			minIndex = 0;
			printf("0:%.2f\n", minValue);
		}

		for(i = 1;i < templateContour->total;i++)
		{
			cur_cont = (CvSeq* )cvGetSeqElem(templateContour, i);
			tempValue = cvMatchShapes((CvContour* )contour, (CvContour* )cur_cont, CV_CONTOURS_MATCH_I3);
			if(tempValue < minValue)
			{
				minValue = tempValue;
				minIndex = i;
			}
			printf("%d:%.2f\n", i, tempValue);
		}

		if(minValue >= 0.3)
		{
			minIndex = -1;
		}
	}

	//打印匹配结果
	printf("the result is %d\n", minIndex);

	//释放内存
	cvReleaseMemStorage(&storage);

	return minIndex;
}

//找出轮廓最大的5个极大值点
void gesFindContourMaxs(CvSeq* contour)
{
	int i;
	CvScalar center;//重心位置
	CvPoint* p;
	CvMat max;//存储5个极大值的数组
	double initMax[] = {-1, -1, -1, -1, -1};//初始极大值设置为-1
	double minValue, maxValue;//5个极大值中的最大值与最小值
	CvPoint minLoc;//最小值的位置
	double preDistance = 0;
	bool isCandidate = false;//是否是候选的极大值点

	//初始化重心位置
	center = cvScalarAll(0);

	//初始化极大值矩阵
	max = cvMat(1, 5, CV_64FC1, initMax);

	//首先求出轮廓的重心
	for(i = 0;i < contour->total;i++)
	{
		p = (CvPoint* )cvGetSeqElem(contour, i);
		center.val[0] += p->x;
		center.val[1] += p->y;
	}
	center.val[0] /= contour->total;
	center.val[1] /= contour->total;

	//遍历轮廓,找出所有的极大值点
	for(i = 0;i < contour->total;i++)
	{
		p = (CvPoint* )cvGetSeqElem(contour, i);
		double distance = sqrt(pow(center.val[0] - p->x, 2) + pow(center.val[1] - p->y, 2));

		if(distance > preDistance)
		{
			isCandidate = true;
		}
		else if(distance < preDistance && isCandidate == true)
		{
			cvMinMaxLoc(&max, &minValue, &maxValue, &minLoc);

			if(distance > minValue)
			{
				cvmSet(&max, minLoc.y, minLoc.x, distance);
			}
			isCandidate = false;
		}
		else
		{
			isCandidate = false;
		}

		preDistance = distance;
	}

	//打印5个极大值
	printf("%.2f %.2f %.2f %.2f %.2f\n", cvmGet(&max, 0, 0), cvmGet(&max, 0, 1), cvmGet(&max, 0, 2), cvmGet(&max, 0, 3), cvmGet(&max, 0, 4));
}

//计算轮廓的pair-wise几何直方图
CvHistogram* gesCalcContoursPGH(CvSeq* contour)
{
	CvHistogram* hist;//成对几何直方图
	CvContour* tempCont;

	//得到成对几何直方图第二个维度上的范围
	tempCont = (CvContour* )contour;
	cvBoundingRect(tempCont, 1);

	int sizes[2] = {60, 200};
	float ranges[2][2] = {{0,PI}, {0,200}};
	float** rangesPtr = new float* [2];
	rangesPtr[0] = ranges[0];
	rangesPtr[1] = ranges[1];

	//初始化几何直方图
	hist = cvCreateHist(2, sizes, CV_HIST_ARRAY, rangesPtr, 1);

	//计算轮廓的成对几何直方图
	cvCalcPGH(contour, hist);

	return hist;
}

//对轮廓的pair-wise几何直方图进行匹配
double gesMatchContoursPGH(CvSeq* contour, CvHistogram* templateHist)
{
	CvHistogram* hist;

	//得到轮廓的成对几何直方图
	hist = gesCalcContoursPGH(contour);

	//归一化直方图
	cvNormalizeHist(templateHist, 1);
	cvNormalizeHist(hist, 1);

	//直方图匹配
	double result = cvCompareHist(hist, templateHist, CV_COMP_BHATTACHARYYA);
	//printf("result:%.2f\n", result);

	//释放内存
	cvReleaseHist(&hist);
	return result;
}
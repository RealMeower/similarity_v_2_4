#include "stdafx.h"
#include "ColorCompare.h"

int hist_size = 64;
float range_0[]={0,256};
float* ranges[] = { range_0 };

char win_name_r[] = "R chanel";
char win_name_g[] = "G chanel";
char win_name_b[] = "B chanel";

void cal_rgb_hist(IplImage *src_image, RGB_HIST *rgb_hist)
{
	rgb_hist->Hist_R = cvCreateHist(1, &hist_size, CV_HIST_ARRAY, ranges, 1);
	rgb_hist->Hist_G = cvCreateHist(1, &hist_size, CV_HIST_ARRAY, ranges, 1);
	rgb_hist->Hist_B = cvCreateHist(1, &hist_size, CV_HIST_ARRAY, ranges, 1);

	IplImage *src_image_r,*src_image_g, *src_image_b;  
	src_image_r = cvCreateImage(cvGetSize(src_image), 8, 1);
	src_image_g = cvCreateImage(cvGetSize(src_image), 8, 1);
	src_image_b = cvCreateImage(cvGetSize(src_image), 8, 1);
	cvSplit(src_image, src_image_r, src_image_g, src_image_b, NULL);
	cvCalcHist( &src_image_r, rgb_hist->Hist_R, 0, NULL );
	cvCalcHist( &src_image_g, rgb_hist->Hist_G, 0, NULL );
	cvCalcHist( &src_image_b, rgb_hist->Hist_B, 0, NULL );
	cvReleaseImage(&src_image_r);
	cvReleaseImage(&src_image_g);
	cvReleaseImage(&src_image_b);
}

void draw_1D_curves_histogram(CvHistogram *hist, IplImage *hist_image, CvScalar color)
{
	int i, bin_w;
	float max_value = 0;
	CvPoint Org, Dst;

	cvGetMinMaxHistValue( hist, 0, &max_value, 0, 0 );
	cvScale( hist->bins, hist->bins, ((double)hist_image->height)/max_value, 0 );
	bin_w = cvRound((double)hist_image->width/hist_size);
	Org = cvPoint(0, hist_image->height);
	for( i = 0; i < hist_size; i++ )
	{
		Dst = cvPoint((i+1)*bin_w, hist_image->height - cvRound(cvGetReal1D(hist->bins,i)));
		cvLine(hist_image, Org, Dst, color, 1, 8, 0 );
		Org = Dst;
	}
}


void draw_rgb_histogram(RGB_HIST *rgb_hist, 
						IplImage *hist_imageR, 
						IplImage *hist_imageG, 
						IplImage *hist_imageB,
						CvScalar color
						)
{
	draw_1D_curves_histogram(rgb_hist->Hist_R, hist_imageR, color);
	draw_1D_curves_histogram(rgb_hist->Hist_G, hist_imageG, color);
	draw_1D_curves_histogram(rgb_hist->Hist_B, hist_imageB, color);
}

void show_rgb_histogram(RGB_HIST *rgb_hist, 
						IplImage *hist_imageR, 
						IplImage *hist_imageG, 
						IplImage *hist_imageB,
						CvScalar color
						)
{
	draw_rgb_histogram(rgb_hist, hist_imageR, hist_imageG, hist_imageB, color);

	cvShowImage( win_name_r, hist_imageR );
	cvShowImage( win_name_g, hist_imageG );
	cvShowImage( win_name_b, hist_imageB );
}

double rgb_total_hist_compare(RGB_HIST *rgbHist1, RGB_HIST *rgbHist2)
{
	double distR;
	double distG;
	double distB;
	double dist;
	distR = cvCompareHist(rgbHist1->Hist_R, rgbHist2->Hist_R, CV_COMP_BHATTACHARYYA);
	distG = cvCompareHist(rgbHist1->Hist_G, rgbHist2->Hist_G, CV_COMP_BHATTACHARYYA);
	distB = cvCompareHist(rgbHist1->Hist_B, rgbHist2->Hist_B, CV_COMP_BHATTACHARYYA);
	dist = (distR + distG + distB)/3;
	return dist;
}

void cal_1D_h_hist(IplImage* src, CvHistogram* hist)
{
	IplImage* hsv = cvCreateImage( cvGetSize(src), 8, 3 ); 
	cvCvtColor( src, hsv, CV_BGR2HSV );

	IplImage* h_plane  = cvCreateImage( cvGetSize(src), 8, 1 );
	IplImage* s_plane  = cvCreateImage( cvGetSize(src), 8, 1 );
	IplImage* v_plane  = cvCreateImage( cvGetSize(src), 8, 1 );

	cvCvtPixToPlane( hsv, h_plane, s_plane, v_plane, 0 );
	cvCalcHist( &h_plane, hist, 0, NULL );
}

void draw_1D_rect_histogram(CvHistogram *hist, IplImage *hist_image, CvScalar color)
{
	int i, bin_w;
	float max_value = 0;

	cvGetMinMaxHistValue( hist, 0, &max_value, 0, 0 );
	cvScale( hist->bins, hist->bins, ((double)hist_image->height)/max_value, 0 );
	bin_w = cvRound((double)hist_image->width/hist_size);

	for( i = 0; i < hist_size; i++ )
		cvRectangle( hist_image, cvPoint(i*bin_w, hist_image->height),
					cvPoint((i+1)*bin_w, hist_image->height - cvRound(cvGetReal1D(hist->bins,i))),
					cvScalarAll(0), -1, 8, 0 
					);
}

void draw_1D_h_histogram(CvHistogram *hist, IplImage *hist_image)
{
	int i, bin_w;
	float max_value = 0;

	cvGetMinMaxHistValue( hist, 0, &max_value, 0, 0 );
	cvScale( hist->bins, hist->bins, ((double)hist_image->height)/max_value, 0 );
	bin_w = cvRound((double)hist_image->width/hist_size);

	for( i = 0; i < hist_size; i++ )
		cvRectangle( hist_image, cvPoint(i*bin_w, hist_image->height),
		cvPoint((i+1)*bin_w, hist_image->height - cvRound(cvGetReal1D(hist->bins,i))),
		cvScalarAll(0), -1, 8, 0 
		);
}

void cal_2D_hsv_hist(IplImage* src, CvHistogram* hist)
{
	IplImage* hsv = cvCreateImage( cvGetSize(src), 8, 3 ); 
	cvCvtColor( src, hsv, CV_BGR2HSV );

	IplImage* h_plane  = cvCreateImage( cvGetSize(src), 8, 1 );
	IplImage* s_plane  = cvCreateImage( cvGetSize(src), 8, 1 );
	IplImage* v_plane  = cvCreateImage( cvGetSize(src), 8, 1 );
	IplImage* planes[] = { h_plane, s_plane };
	cvCvtPixToPlane( hsv, h_plane, s_plane, v_plane, 0 );

	cvCalcHist( planes, hist, 0, 0 );
}

void draw_2D_h_s_hist(IplImage* hist_img, CvHistogram *hist, int h_bins, int s_bins, int scale)
{
	float max_value = 0;
	cvGetMinMaxHistValue( hist, 0, &max_value, 0, 0 ); 
	for( int h = 0; h < h_bins; h++ ) {
		for( int s = 0; s < s_bins; s++ ) {
			float bin_val = cvQueryHistValue_2D( hist, h, s );
			int intensity = cvRound( bin_val * 255 / max_value );
			cvRectangle( 
				hist_img, 
				cvPoint( h*scale, s*scale ),
				cvPoint( (h+1)*scale - 1, (s+1)*scale - 1),
				CV_RGB(intensity,intensity,intensity), 
				CV_FILLED
				);
		}
	}
}

double rgb_subset_compare_hist(IplImage *image1,IplImage *image2, int row_num, int col_num )
{
	RGB_HIST *rgbHist1; RGB_HIST *rgbHist2;

	CvRect rect;
	int row_height = (image1->height)/row_num;
	int col_width = (image1->width)/col_num;
	double sub_dist;
	double temp = 0;

	rgbHist1 = (RGB_HIST *)malloc(sizeof(CvHistogram*)*3);
	rgbHist2 = (RGB_HIST *)malloc(sizeof(CvHistogram*)*3);

	CvMat *pMat1 = cvCreateMatHeader(row_height, col_width, CV_8UC1);  
	CvMat *pMat2 = cvCreateMatHeader(row_height, col_width, CV_8UC1);  

	IplImage *pSubImg1 = cvCreateImageHeader(cvSize(col_width, row_height), 8, 3);
	IplImage *pSubImg2 = cvCreateImageHeader(cvSize(col_width, row_height), 8, 3); 

	int i, j;
	for (i=0; i<row_num; i++)
		for (j=0; j<col_num; j++)
		{
			rect = cvRect(col_width*j, row_height*i, col_width-1, row_height-1);
			cvGetSubRect(image1, pMat1, rect);
			cvGetImage(pMat1, pSubImg1); 
			cvGetSubRect(image2, pMat2, rect); 
			cvGetImage(pMat2, pSubImg2); 
			cal_rgb_hist(pSubImg1, rgbHist1);
			cal_rgb_hist(pSubImg2, rgbHist2);
			sub_dist = rgb_total_hist_compare(rgbHist1, rgbHist2);
			//printf("第%d行%d列图像块三通道直方图对比结果为:%f\n", i, j, sub_dist);
			temp += sub_dist;
		}
		temp = temp/(row_num*col_num);

		for(i=1; i<col_num; i++)
		{
			cvLine(image1, cvPoint(col_width*i,0), cvPoint( col_width*i,image1->height), CV_RGB(0,0,255), 1, 8, 0);
			cvLine(image2, cvPoint(col_width*i,0), cvPoint( col_width*i,image2->height), CV_RGB(0,0,255), 1, 8, 0);
		}

		for(i=1; i<row_num; i++)
		{
			cvLine(image1, cvPoint(0,row_height*i), cvPoint(image1->width, row_height*i), CV_RGB(0,0,255), 1, 8, 0);
			cvLine(image2, cvPoint(0,row_height*i), cvPoint(image2->width, row_height*i), CV_RGB(0,0,255), 1, 8, 0);
		}

		free(rgbHist1);
		free(rgbHist2);
		return temp;
}

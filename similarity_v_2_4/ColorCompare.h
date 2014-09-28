#ifndef __COLORCOMPARE_H__
#define __COLORCOMPARE_H__

#include "cv.h"
#include "highgui.h"

extern int hist_size;
extern float range_0[];
extern float* ranges[];

extern char win_name_r[];
extern char win_name_g[];
extern char win_name_b[];

typedef struct rgb_hist
{
	CvHistogram *Hist_R;
	CvHistogram *Hist_G;
	CvHistogram *Hist_B;
} 
RGB_HIST;

void draw_1D_curves_histogram(CvHistogram *hist, IplImage *hist_image, CvScalar color);
void draw_1D_rect_histogram(CvHistogram *hist, IplImage *hist_image, CvScalar color);
void draw_rgb_histogram(RGB_HIST *rgb_hist, 
						IplImage *hist_imageR, 
						IplImage *hist_imageG, 
						IplImage *hist_imageB,
						CvScalar color
						);
void show_rgb_histogram(RGB_HIST *rgb_hist, 
						IplImage *hist_imageR, 
						IplImage *hist_imageG, 
						IplImage *hist_imageB,
						CvScalar color
						);
void cal_rgb_hist(IplImage *src_image, RGB_HIST *rgb_hist);
double rgb_total_hist_compare(RGB_HIST *rgbHist1, RGB_HIST *rgbHist2);
void cal_1D_h_hist(IplImage* src, CvHistogram* hist);
void draw_1D_h_histogram(CvHistogram *hist, IplImage *hist_image);

void cal_2D_hsv_hist(IplImage* src, CvHistogram* hist);
void cal_2D_hsv_hist(IplImage* src, CvHistogram* hist);
void draw_2D_h_s_hist(IplImage* hist_img, CvHistogram *hist, int h_bins, int s_bins, int scale);

#endif
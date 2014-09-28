#ifndef __CONTOURSCOMPARE_H__
#define __CONTOURSCOMPARE_H__

#include "cv.h"
#include "highgui.h"
#define PI 3.14159f
void calc_contours( char *winname,
				   CvSeq* first_contour,
				   CvMemStorage* storage, 
				   IplImage* img_src, 
				   IplImage* img_con
				   );

int gesContourCompFunc(const void* _a, const void* _b, void* userdata);
void gesFindContours(IplImage* src, IplImage* dst, CvSeq** templateContour, CvMemStorage* templateStorage, int flag);
void gesMatchContoursTemplate(IplImage* src, IplImage* dst, CvSeq** templateContour);
int gesMatchContoursTemplate2(IplImage* src, IplImage* dst, CvSeq* templateContour);
void gesFindContourMaxs(CvSeq* contour);
CvHistogram* gesCalcContoursPGH(CvSeq* contour);
double gesMatchContoursPGH(CvSeq* contour, CvHistogram* templateHist);
double rgb_subset_compare_hist(IplImage *image1,IplImage *image2, int row_num, int col_num );

#endif